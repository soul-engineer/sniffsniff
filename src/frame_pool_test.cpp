/* 
 * File:   frame_pool_test.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 8, 2015, 10:33 PM
 */

#include <catch.hpp>
#include "frame_pool.h"
#include <glog/logging.h>
#include <vector>

namespace
{
    const char* const TAGS {"[FRAME_POOL]"};
}


SCENARIO( "Working with frame_pool (single thread)", TAGS )
{
    GIVEN( "frame_pool" )
    {
        frame_pool pool;
        WHEN( "All frames are taken" )
        {
            std::vector<frame*> buf;
            buf.reserve(frame_pool::POOL_SIZE);
            
            for (auto i = 0u; i < frame_pool::POOL_SIZE; ++i)
            {
                auto ptr = pool.acquire();
                CAPTURE( i );
                REQUIRE( nullptr != ptr );
                buf.push_back(ptr);
            }
            
            THEN( "pool is empty" )
            {
                REQUIRE( nullptr == pool.acquire() );
            }
            
            THEN( "All items could be returned back" )
            {
                std::for_each(buf.begin(), buf.end(),
                        [&pool](frame* fr)
                        {
                            pool.release(fr);
                        });
            }
        }
    }
}
