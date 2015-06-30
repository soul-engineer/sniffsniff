/* 
 * File:   worker.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 09:28 PM
 */

#include "worker.h"
#include "worker_pool.h"
#include "frame.h"
#include <boost/lockfree/spsc_queue.hpp>
#include <glog/logging.h>


namespace
{
    const size_t RING_SIZE {10};
}


struct worker_impl
{
    // Every worker has its own queue to store a frame
    boost::lockfree::spsc_queue<
            frame,
            boost::lockfree::capacity<RING_SIZE> > ring;
};


worker::worker(const worker_pool& pool)
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
    
    LOG_IF(WARNING, !impl_->ring.is_lock_free()) <<
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
        
    }
}

bool worker::try_push(const uint8_t* data, size_t size)
{
    DCHECK(size <= frame::MAX_SIZE) << "Frame is too big " << size << "B";
    //return impl_->ring.push(frame(data, size));
}