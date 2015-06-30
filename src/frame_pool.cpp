/* 
 * File:   frame_pool.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 8, 2015, 10:33 PM
 */

#include "frame_pool.h"
#include <glog/logging.h>


frame_pool::frame_pool()
{
    // Fill ring queue with pointers to frames from data array
    std::for_each(data_.begin(), data_.end(),
            [this](frame& fr)
            {
                this->ring_.push(&fr);
            });
}

frame_pool::~frame_pool()
{
    // TODO: Check if all items are returned to the pool
}

frame* frame_pool::acquire() noexcept
{
    frame* fr {};
    ring_.pop(fr);
    
    return fr;
}

void frame_pool::release(frame* fr) noexcept
{
    DCHECK(nullptr != fr);
    const bool res{ring_.push(fr)};
    DCHECK(true == res) << "Could not return item to the pool";
    (void)res;
}

