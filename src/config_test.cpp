/* 
 * File:   config_test.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 9:51 PM
 */

#include <catch.hpp>
#include "config.h"
#include <string>
#include <vector>
#include <glog/logging.h>
#include <sstream>


namespace
{
    const char* const TAGS = "[CONFIG]";
    
    // This class parses a string to an array of options
    class argv
    {
        static const char DEL {' '};
        std::vector<char*> vr_raw_;
        std::vector<std::string> vr_;
        
    public:
        explicit argv(const std::string& str)
        {
            size_t start {}, pos {}, len {str.length()};

            do
            {
                pos = str.find(DEL, start);
                if (pos == str.npos)
                {
                    // Delimeter not found
                    pos = len;
                }
                
                auto arg = str.substr(start, pos - start);
                if (!arg.empty())
                {
                    vr_.push_back(arg);
                    vr_raw_.push_back(new char[arg.length() + 1]);
                    std::copy(arg.c_str(), arg.c_str() + arg.length() + 1,
                            vr_raw_.back());
                }

                start = pos + 1;
            } while (pos != len);
        }
        
        ~argv()
        {
            std::for_each(vr_raw_.begin(), vr_raw_.end(), [](char* str)
            {
                delete[] str;
            });
        }
        
        const std::vector<std::string>& get() const
        {
            return vr_;
        }
        
        char** raw()
        {
            return vr_raw_.empty() ? nullptr : &vr_raw_.front();
        }
    };
}

TEST_CASE( "argv class: raw array should be equal to vector", TAGS)
{
    auto arg = argv("a b");
    auto& vr = arg.get();
    auto raw = arg.raw();
    
    for (auto i = 0u; i < vr.size(); ++i)
    {
        REQUIRE( vr[i] == std::string(raw[i]));
    }
}

TEST_CASE( "Test argv class: regular", TAGS )
{
    auto arg = argv("a b");
    auto vr = arg.get();
    REQUIRE( 2 == vr.size());
    REQUIRE( vr[0] == "a" );
    REQUIRE( vr[1] == "b" );
}

TEST_CASE( "Test argv class: two spaces", TAGS )
{
    auto arg = argv("a  b");
    auto vr = arg.get();
    REQUIRE( 2 == vr.size());
    REQUIRE( vr[0] == "a" );
    REQUIRE( vr[1] == "b" );
}

TEST_CASE( "Test argv class: space at the begin", TAGS )
{
    auto arg = argv(" a b");
    auto vr = arg.get();
    REQUIRE( 2 == vr.size());
    REQUIRE( vr[0] == "a" );
    REQUIRE( vr[1] == "b" );
}

TEST_CASE( "Test argv class: space in the end", TAGS )
{
    auto arg = argv("a b ");
    auto vr = arg.get();
    REQUIRE( 2 == vr.size());
    REQUIRE( vr[0] == "a" );
    REQUIRE( vr[1] == "b" );
}

TEST_CASE( "Test argv class: no spaces", TAGS )
{
    auto arg = argv("ab");
    auto vr = arg.get();
    REQUIRE( 1 == vr.size());
    REQUIRE( vr.front() == "ab" );
}

TEST_CASE( "Test if_name getter and setter", TAGS )
{
    const std::string IF_NAME {"eth0"};
    config cfg;
    cfg.set_if_name(IF_NAME.c_str());
    REQUIRE( IF_NAME == cfg.get_if_name() );
}

TEST_CASE( "Test thread_count getter and setter", TAGS )
{
    const auto COUNT = 123;
    config cfg;
    cfg.set_thread_count(COUNT);
    REQUIRE( COUNT == cfg.get_thread_count() );
}

TEST_CASE( "Test promisc getter and setter", TAGS )
{
    config cfg;
    bool promisc = cfg.get_promisc();
    cfg.set_promisc(!promisc);
    REQUIRE( !promisc == cfg.get_promisc() );
}

TEST_CASE( "All options are given", TAGS )
{
    auto arg = argv("path -Ieth0 -j10 --no-promisc");
    config cfg;
    REQUIRE_NOTHROW(cfg.init(arg.get().size(), arg.raw()));
    
    REQUIRE( "eth0" == cfg.get_if_name() );
    REQUIRE( 10 == cfg.get_thread_count() );
    REQUIRE_FALSE( cfg.get_promisc() );
}

TEST_CASE( "All options are given with spaces between", TAGS )
{
    auto arg = argv("path -I eth0 -j 10 --no-promisc");
    config cfg;
    REQUIRE_NOTHROW(cfg.init(arg.get().size(), arg.raw()));
    
    REQUIRE( "eth0" == cfg.get_if_name() );
    REQUIRE( 10 == cfg.get_thread_count() );
    REQUIRE_FALSE( cfg.get_promisc() );
}

TEST_CASE( "Do not set promiscuous mode", TAGS )
{
    auto arg = argv("path -Ieth0 -j10");
    config cfg;
    REQUIRE_NOTHROW(cfg.init(arg.get().size(), arg.raw()));
    
    REQUIRE( "eth0" == cfg.get_if_name() );
    REQUIRE( 10 == cfg.get_thread_count() );
}

TEST_CASE( "Set only interface name", TAGS )
{
    auto arg = argv("path -Ieth0");
    config cfg;
    REQUIRE_NOTHROW(cfg.init(arg.get().size(), arg.raw()));
    
    REQUIRE( "eth0" == cfg.get_if_name() );
}

TEST_CASE( "Interface name is an mandatory argument", TAGS )
{
    auto arg = argv("path");
    config cfg;
    REQUIRE_THROWS_AS(cfg.init(arg.get().size(), arg.raw()), options_error);
}

TEST_CASE( "Invalid option name", TAGS )
{
    auto arg = argv("path -Ieth0 -b");
    config cfg;
    REQUIRE_THROWS_AS(cfg.init(arg.get().size(), arg.raw()), options_error);
}