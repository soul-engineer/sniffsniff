/* 
 * File:   worker_pool.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 08:55 PM
 */

#include "worker_pool.h"
#include "config.h"
#include "worker.h"
#include <vector>
#include <thread>
#include <string>
#include <glog/logging.h>
#include <algorithm>
#include <atomic>


struct worker_pool_impl
{
    std::vector<std::thread> threads_;
    std::atomic<bool> running_ {true};
};


worker_pool::worker_pool(const config& cfg)
{
    try
    {
        // Create an implementation
        impl_.reset(new worker_pool_impl);
        impl_->threads_.reserve(cfg.get_thread_count());
    }
    catch (const std::bad_alloc& e)
    {
        LOG(ERROR) << "Can't allocate memory for " << cfg.get_thread_count() <<
                " workers.";
        throw std::runtime_error(
                std::string("Can't allocate memory for worker_pool: ") +
                std::string(e.what()));
    }
    catch (...)
    {
        LOG(ERROR) << "Unknown exception while starting workers " ;
        throw;
    }
    
    LOG_IF(WARNING, !impl_->running_.is_lock_free()) <<
            "atomic<bool> is not lockfree on this system";
    
    try
    {
        // Run requested workers
        for (auto i = 0u; i < cfg.get_thread_count(); ++i)
        {
            impl_->threads_.emplace_back(worker(*this));
            impl_->threads_.back().detach();
        }
    }
    catch (...)
    {
        // Unexpected exception - we should stop all existing workers
        LOG(ERROR) << "Can't start workers - exiting";
        stop_threads();
        std::for_each(impl_->threads_.begin(), impl_->threads_.end(),
                [](std::thread& thread)
                {
                    if (thread.joinable())
                    {
                        thread.join();
                    }
                });
        throw;    
    }
}

worker_pool::~worker_pool()
{
    // dtor
}

void worker_pool::stop_threads()
{
    impl_->running_.store(false, std::memory_order_relaxed);
}

bool worker_pool::active() const
{
    return impl_->running_.load(std::memory_order_relaxed);
}
