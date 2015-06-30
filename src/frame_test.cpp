/* 
 * File:   frame_test.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on July 08, 2015, 10:15 PM
 */

#include <catch.hpp>
#include "frame.h"
#include <cstring>

namespace
{
    const char* const TAGS {"[FRAME]"};
}


TEST_CASE( "Frame is empty by default", TAGS )
{
    REQUIRE( 0 == frame().size() );
}

TEST_CASE( "Frame should copy given data properly", TAGS )
{
    const uint8_t data[] {1,2,3,4,5};
    frame fr;
    fr.set(data, sizeof(data));
    REQUIRE( sizeof(data) == fr.size() );
    REQUIRE( 0 == memcmp(data, fr.get(), fr.size()) );
}