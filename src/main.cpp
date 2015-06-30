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


int main(int argc, char** argv)
{
    try
    {
        google::InitGoogleLogging(argv[0]);

        // TODO: this code is for quick test only
        // Parse program options
        config cfg;
        cfg.init(argc, argv);
        
        dispatcher disp(cfg);
        provider prov(disp);
        prov.open_iface(cfg);
        prov.run();
        
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

