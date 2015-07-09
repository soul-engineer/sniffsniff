/* 
 * File:   worker.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 09:28 PM
 */

#include "worker.h"
#include "dispatcher.h"
#include "frame.h"
#include "frame_pool.h"
#include <boost/lockfree/spsc_queue.hpp>
#include <glog/logging.h>
#include <memory>


/******************************************************************************/
/*                             Worker_impl                                    */
/******************************************************************************/

namespace
{
    const size_t TASKS_COUNT {frame_pool::POOL_SIZE + CAPACITY_CORRECTION()};
}


struct worker_impl
{
    // Frame pool
    frame_pool pool_;
    // Tasks queue
    boost::lockfree::spsc_queue<
            frame*,
            boost::lockfree::capacity<TASKS_COUNT> > tasks_;
};


/******************************************************************************/
/*                                 worker                                     */
/******************************************************************************/

worker::worker(const dispatcher& pool)
: pool_(pool)
{
    try
    {
        impl_.reset(new worker_impl);
    }
    catch (const std::bad_alloc& e)
    {
        LOG(ERROR) << "Not enough memory to initiate a worker";
        throw;
    }
    catch (...)
    {
        LOG(ERROR) << "Unexpected exception while initiating a worker";
        throw;
    }
    
    LOG_IF(WARNING, !impl_->tasks_.is_lock_free()) <<
            "Frame ring is not lock free";
}

worker::worker(worker&& orig)
: pool_(orig.pool_), impl_(orig.impl_.release())
{
    // do nothing
}

worker::~worker()
{
    // dtor
}

void worker::operator()()
{
    while (pool_.active())
    {
        frame* fr {};
        auto deleter = [this](frame* fr)
        {
            impl_->pool_.release(fr);
        };
        
        // Check if there is a frame to process
        if (impl_->tasks_.pop(fr))
        {
            std::unique_ptr<frame, decltype(deleter)> guard(fr, deleter);
            parse_frame(*fr);
        }
        
        // TODO: Reduce CPU consuming of this busy loop
    }
}

bool worker::add_frame(const uint8_t* data, size_t size)
{
    DCHECK(size <= frame::MAX_SIZE) << "Frame is too big: " << size << "B";
    
    // Acquire a buffer from the pool
    auto ptr = impl_->pool_.acquire();
    if (nullptr == ptr)
    {
        return false;
    }
    
    // Copy given frame in the internal buffer ...
    ptr->set(data, size);
    // ... and place this buffer to the task queue
    auto success = impl_->tasks_.push(ptr);
    
    if (!success)
    {
        LOG(ERROR) << "UB: Couldn't place a frame to the task queue";
        impl_->pool_.release(ptr);
        return false;
    }
    
    return true;
}

void worker::parse_frame(const frame& fr) const
{
    // TODO: no actual processing yet
    LOG(ERROR) << "Packet length: " << fr.size();
}