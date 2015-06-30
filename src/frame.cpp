/* 
 * File:   frame.cpp
 * Author: Artem.Nazyrov
 *
 * Created on July 8, 2015, 7:29 PM
 */

#include "frame.h"
#include <algorithm>
#include <glog/logging.h>

void frame::set(const frame::elem_t* data, size_t size)
{
    DCHECK(size <= MAX_SIZE) << "Too much data to copy";
    std::copy(data, data + size, data_.begin());
    size_ = size;
}
