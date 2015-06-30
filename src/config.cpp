/* 
 * File:   config.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on June 30, 2015, 9:01 PM
 */

#include "config.h"
#include <glog/logging.h>


namespace
{
    const auto DEF_IF_NAME = "enp0s3";
    const auto DEF_PROMISC = true;
}

config::config()
: if_name_ {DEF_IF_NAME}, promisc_{DEF_PROMISC}
{
}

void config::set_if_name(const char* if_name)
{
    CHECK(nullptr != if_name) << "if_name couldn't be nullptr";
    if_name_.assign(if_name);
}


/******************************************************************************/
/*                               Unit tests                                   */
/******************************************************************************/
#ifdef _BUILT_WITH_CATCH

#include <catch.hpp>

namespace
{
    const char* const TAGS {"[CONFIG]"};
}

SCENARIO( "config construction", TAGS )
{
    GIVEN( "Default constructed config object" )
    {
        config cfg;
        WHEN( "check its properties" )
        {
            THEN( "default if_name" )
            {
                REQUIRE( cfg.get_if_name() == std::string(DEF_IF_NAME) );
            }
            
            THEN( "default promisc" )
            {
                REQUIRE( cfg.get_promisc() == DEF_PROMISC );
            }
        }
    }
}

#endif