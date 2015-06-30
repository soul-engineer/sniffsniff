/* 
 * File:   frame_pool.h
 * Author: Artem.Nazyrov
 * 
 * Created on July 8, 2015, 10:33 PM
 */

#ifndef FRAME_POOL_H
#define	FRAME_POOL_H

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/version.hpp>
#include <array>
#include "frame.h"


// Error in boost 1.53: https://svn.boost.org/trac/boost/ticket/8135
// When using compile time capacity with boost lockfree queue
// there is no additional dummy node to provide queue functionality:
// so one needs to correct fixed queue size manually
constexpr int CAPACITY_CORRECTION()
{
    #if BOOST_VERSION == 105300
        return 1;
    #else
        return 0;
    #endif
}


class frame_pool
{
public:
    // Fixed item count in a pool
    static const size_t POOL_SIZE {100};
    
    frame_pool();
    frame_pool(const frame_pool& orig) = delete;
    frame_pool& operator=(const frame_pool& orig) = delete;
    frame_pool(frame_pool&& orig) = delete;
    ~frame_pool();
    
    frame* acquire() noexcept;
    void release(frame* fr) noexcept;

protected:
    // Frames data
    std::array< frame, POOL_SIZE > data_;
    // Ring queue to manage data pointers
    boost::lockfree::spsc_queue<
            frame*,
            boost::lockfree::capacity<POOL_SIZE + CAPACITY_CORRECTION()>> ring_;
};

#endif	/* FRAME_POOL_H */

