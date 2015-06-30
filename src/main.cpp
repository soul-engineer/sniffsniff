/* 
 * File:   main.cpp
 * Author: Artem.Nazyrov
 *
 * Created on June 29, 2015, 03:31 PM
 */

#include <glog/logging.h>
#include "config.h"
#include "provider.h"
#include "dispatcher.h"
#include <stdexcept>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <csignal>


namespace
{
    const auto SLEEP_MS = 1;
    std::atomic<bool> running_ {true};

    void sig_handler(int signo)
    {
        switch (signo)
        {
            case SIGINT:
            case SIGTERM:
                running_.store(false, std::memory_order_relaxed);
                break;
            default:;
        }
    }
}


int main(int argc, char** argv)
{
    try
    {
        google::InitGoogleLogging(argv[0]);

        // Parse program options
        config cfg;
        cfg.init(argc, argv);
        
        // Configure signals handling
        LOG_IF(FATAL, signal(SIGINT, sig_handler) == SIG_ERR) <<
                "Can't configure signal handling";
        
        // Create dispatcher
        dispatcher disp(cfg);
        
        // Configure frame capturer...
        provider prov(disp);
        prov.open_iface(cfg);
        // ...and run it in a separate thread
        std::thread prov_thread(&provider::run, &prov);
        
        // Main cycle
        while (running_.load(std::memory_order_relaxed))
        {
            // TODO: Examine worker results and log it
            std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MS));
        }

        // Wait for provider to stop
        prov.stop();
        prov_thread.join();
        
        LOG(INFO) << "Program finished successfully";
        
        return 0;
    }
    catch (const options_error& e)
    {
        std::cout << e.what() << std::endl;
        return 0;
    }
    catch (const std::runtime_error& e)
    {
        LOG(ERROR) << "Can't open interface: " << e.what();
    }
    catch (...)
    {
        LOG(ERROR) << "Unknown exception";
    }
    
    return -1;
}

