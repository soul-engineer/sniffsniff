/* 
 * File:   config.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on June 30, 2015, 9:01 PM
 */

#include "config.h"
#include <glog/logging.h>
#include <thread>
#include <boost/program_options.hpp>
#include <sstream>

#define PREFIX "Configuration: "


namespace
{
    // Default value for promisc_ parameter
    const auto DEF_PROMISC = true;
    
    // Helper to simplify program_options configuration
    // It represents an option with its short name, long name and description
    struct option
    {
        const std::string short_, long_, desc_, full_;
        
        option(const std::string& short_name, const std::string& long_name,
                const std::string& desc = "")
        :   short_{short_name}, long_{long_name}, desc_{desc},
            full_{short_.empty() ? long_ : long_ + std::string(",") + short_}
        {
            DCHECK(!long_.empty() || !short_.empty()) <<
                    "At least one option name is required";
        }
        
        const std::string index() const
        {
            return long_.empty() ? short_ : long_;
        }
    };
    
    // Program options
    const option IFACE_PARAM {"I", "", "Interface name"};
    const option THREADS_PARAM {"j", "", "Working threads count"};
    const option NOPROMISC_PARAM {"", "no-promisc",
            "Do not enable promiscuous mode"};
    const option HELP_PARAM {"h", "help", "Print this message"};
}


/******************************************************************************/
/*                           config implementation                            */
/******************************************************************************/

config::config()
: if_name_{}, promisc_{DEF_PROMISC},
  thread_count_{std::thread::hardware_concurrency()}
{
}

void config::set_if_name(const char* if_name)
{
    CHECK(nullptr != if_name) << "if_name couldn't be nullptr";
    if_name_.assign(if_name);
}

void config::init(int argc, char** argv)
{
    using namespace boost::program_options;
    
    try 
    { 
        // Build options description
        options_description desc("sniffsniff options"); 
        desc.add_options()
            (IFACE_PARAM.full_.c_str(), value(&if_name_)->required(),
                IFACE_PARAM.desc_.c_str())
            (THREADS_PARAM.full_.c_str(),
                value(&thread_count_)->default_value(thread_count_),
                THREADS_PARAM.desc_.c_str())
            (NOPROMISC_PARAM.full_.c_str(), NOPROMISC_PARAM.desc_.c_str())
            (HELP_PARAM.full_.c_str(), HELP_PARAM.desc_.c_str());
                
        // Parse options
        variables_map vm; 
        store(parse_command_line(argc, argv, desc),  vm);
        
        // "help" message requested or empty arguments array
        if (argc == 1 || vm.count(HELP_PARAM.index())) 
        {
            // Convert description to string
            std::stringstream ss;
            ss << desc;
            throw options_error(ss.str());
        } 

        // Check mandatory options
        notify(vm);
        
        // Interface name is an mandatory argument
        LOG(INFO) << PREFIX << IFACE_PARAM.desc_ << " is " << get_if_name();
        
        // Working threads count
        LOG(INFO) << PREFIX << THREADS_PARAM.desc_ << " is " << 
                get_thread_count();
        LOG_IF(INFO, 0 == get_thread_count()) << "Parsing is disabled when " <<
                THREADS_PARAM.index() << " is 0";
        
        // Promiscuous mode switcher
        if (vm.count(NOPROMISC_PARAM.index()))
        {
            set_promisc(false);
            LOG(INFO) << PREFIX << "Promiscuous mode disabled";
        }
        else
        {
            LOG(INFO) << PREFIX << "Promiscuous mode enabled";
        }
    } 
    catch(const error& e) 
    { 
          throw options_error(e.what());
    }
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
                REQUIRE( cfg.get_if_name().empty() );
            }
            
            THEN( "default promisc" )
            {
                REQUIRE( cfg.get_promisc() == DEF_PROMISC );
            }
            
            THEN( "default thread count" )
            {
                REQUIRE( cfg.get_thread_count() ==
                        std::thread::hardware_concurrency() );
            }
        }
    }
}

TEST_CASE( "Option construction", TAGS )
{
    const std::string sh {"a"};
    const std::string l {"b"};
    option opt(sh, l);
    
    REQUIRE( sh == opt.short_ );
    REQUIRE( l == opt.long_ );
    REQUIRE( (l + std::string(",") + sh) == opt.full_ );
}

TEST_CASE( "Option index - only short part", TAGS )
{
    option opt("a", "");
    REQUIRE( !opt.index().empty() );
}

TEST_CASE( "Option index - only long part", TAGS )
{
    option opt("", "a");
    REQUIRE( !opt.index().empty() );
}

TEST_CASE( "Option index - should return long part", TAGS )
{
    const std::string sh {"a"};
    const std::string l {"b"};
    option opt(sh, l);
    REQUIRE( l == opt.index() );
}

TEST_CASE( "Option description", TAGS )
{
    const std::string sh {"a"};
    const std::string l {"b"};
    const std::string d {"d"};
    option opt(sh, l, d);
    REQUIRE( d == opt.desc_ );
}

#undef PREFIX

#endif