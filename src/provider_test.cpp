/* 
 * File:   provider_test.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on June 30, 2015, 01:12 PM
 */

#include <catch.hpp>
#include "provider.h"
#include "dispatcher.h"
#include "config.h"
#include <stdexcept>


namespace
{
    const char* const TAGS {"[PROVIDER]"};
}


TEST_CASE( "Wrong device name should cause an exception", TAGS )
{
    const auto FAKE_IF = "42: The answer to life, the universe and everything";
    dispatcher disp;
    config cfg;
    cfg.set_if_name(FAKE_IF);
    provider prov(disp);
            
    REQUIRE_THROWS_AS( prov.open_iface(cfg), std::runtime_error );
}
