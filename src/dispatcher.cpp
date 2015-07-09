/* 
 * File:   dispatcher.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 08:55 PM
 */

#include "dispatcher.h"
#include "config.h"
#include "worker.h"
#include <vector>
#include <thread>
#include <string>
#include <glog/logging.h>
#include <algorithm>
#include <atomic>


struct dispatcher_impl
{
    // Workers
    std::vector<worker> workers_;
    // Working threads
    std::vector<std::thread> threads_;
    // Flag to stop threads work
    std::atomic<bool> running_ {true};
    // Next worker id to give to it a task
    size_t next_worker_id {};
};


dispatcher::dispatcher(const config& cfg)
{
    try
    {
        impl_.reset(new dispatcher_impl);
        impl_->workers_.reserve(cfg.get_thread_count());
        
        // Create workers
        for (auto i = 0u; i < cfg.get_thread_count(); ++i)
        {
            impl_->workers_.emplace_back(worker(*this));
        }
        
        impl_->threads_.reserve(cfg.get_thread_count());
    }
    catch (const std::bad_alloc& e)
    {
        LOG(ERROR) << "Can't allocate memory for " << cfg.get_thread_count() <<
                " workers.";
        throw std::runtime_error(
                std::string("Can't allocate memory for dispatcher: ") +
                std::string(e.what()));
    }
    catch (...)
    {
        LOG(ERROR) << "Unknown exception while creating workers " ;
        throw;
    }
    
    LOG_IF(WARNING, !impl_->running_.is_lock_free()) <<
            "atomic<bool> is not lockfree on this system";
    
    try
    {
        // Start requested workers
        std::for_each(impl_->workers_.begin(), impl_->workers_.end(),
                [this](worker& wrk)
                {
                    impl_->threads_.emplace_back(std::ref(wrk));
                });
    }
    catch (...)
    {
        // Unexpected exception while starting workers
        // All running workers will be stopped in destructor
        LOG(ERROR) << "Can't start workers - exiting";
        throw;    
    }
}

dispatcher::~dispatcher()
{
   // Send a signal to threads to stop
    impl_->running_.store(false, std::memory_order_relaxed);
    // Wait for it
    std::for_each(impl_->threads_.begin(), impl_->threads_.end(),
            [](std::thread& thread)
            {
                if (thread.joinable())
                {
                    thread.join();
                }
            });
}

bool dispatcher::active() const noexcept
{
    return impl_->running_.load(std::memory_order_relaxed);
}

namespace
{
    inline bool next(size_t& id, size_t max)
    {
        if (++id == max)
        {
            id = 0;
            return false;
        }
        
        return true;
    }
}

void dispatcher::dispatch_frame(const uint8_t* data, size_t len) noexcept
{
    DCHECK(nullptr != data) << "data pointer is null";
    
    if (impl_->workers_.empty())
    {
        return;
    }
    
    bool overflow {};
    bool done {};
    
    do
    {   // Round-robin scheduling
        done = impl_->workers_[impl_->next_worker_id].add_frame(data, len);
        overflow = next(impl_->next_worker_id, impl_->workers_.size());
    }
    while (!done && !overflow);
    
    LOG_IF(WARNING, !done) << "All workers are busy, frame is missed";
}


/******************************************************************************/
/*                               Unit tests                                   */
/******************************************************************************/
#ifdef _BUILT_WITH_CATCH

#include <catch.hpp>

namespace
{
    const char* const TAGS {"[DISPATCHER]"};
}

TEST_CASE( "check next()", TAGS )
{
    const size_t MAX {3};
    
    for (auto i = 0u; i< MAX-1; ++i)
    {
        size_t res {i};
        CAPTURE(res);
        CAPTURE(MAX);
        REQUIRE( next(res, MAX) );
        REQUIRE( res == i + 1 );
    }
    
    size_t res {MAX-1};
    CAPTURE(res);
    CAPTURE(MAX);
    REQUIRE( !next(res, MAX) );
    REQUIRE( 0 == res );
}

#endif