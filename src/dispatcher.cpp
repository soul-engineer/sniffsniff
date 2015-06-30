/* 
 * File:   dispatcher.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on June 30, 2015, 10:25 AM
 */

#include "dispatcher.h"
#include <glog/logging.h>


dispatcher::dispatcher()
{
    // ctor
}

void dispatcher::dispatch_frame(const uint8_t* data, size_t len) noexcept
{
    DCHECK(nullptr != data) << "data pointer is null";
    LOG(ERROR) << "Packet length" << len;
}

