/* 
 * File:   config_test.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 9:51 PM
 */

#include <catch.hpp>
#include "config.h"
#include <string>

namespace
{
    const char* const TAGS = "[CONFIG]";
}

TEST_CASE( "Test if_name getter and setter" )
{
    const std::string IF_NAME {"eth0"};
    config cfg;
    cfg.set_if_name(IF_NAME.c_str());
    REQUIRE( IF_NAME == cfg.get_if_name() );
}

TEST_CASE( "Test thread_count getter and setter" )
{
    const auto COUNT = 123;
    config cfg;
    cfg.set_thread_count(COUNT);
    REQUIRE( COUNT == cfg.get_thread_count() );
}