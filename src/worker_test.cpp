/* 
 * File:   worker_test.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 09:28 PM
 */

#include <catch.hpp>
#include "worker.h"
#include "dispatcher.h"
#include "config.h"
#include "frame_pool.h"

namespace
{
    const char* const TAGS {"[WORKER]"};
}


TEST_CASE( "Check maximum frame count in worker", TAGS )
{
    const uint8_t arr[] {1,2,3};
    config cfg;
    // Disable internal workers
    cfg.set_thread_count(0);
    dispatcher pool(cfg);
    
    worker wrk(pool);
    
    for (auto i = 0u; i < frame_pool::POOL_SIZE; ++i)
    {
        REQUIRE( wrk.add_frame(arr, sizeof(arr)) );
    }
    
    REQUIRE( !wrk.add_frame(arr, sizeof(arr)) );
}