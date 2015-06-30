/* 
 * File:   config.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 9:01 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <string>


class config
{
public:
    config();
    config(const config& orig) = delete;
    config(config&& orig) = delete;
    config& operator=(const config& orig) = delete;
    ~config() = default;
    
    void init(int argc, char** argv);
    
    const std::string& get_if_name() const noexcept
    { return if_name_; }
    void set_if_name(const char* if_name);
    
    bool get_promisc() const noexcept
    { return promisc_; }
    void set_promisc(bool promisc) noexcept
    { promisc_ = promisc; }
    
    uint32_t get_thread_count() const noexcept
    { return thread_count_; }
    void set_thread_count(uint32_t thread_count) noexcept
    { thread_count_ = thread_count; }

protected:
    std::string if_name_;
    bool promisc_;
    uint32_t thread_count_;
};

class options_error: public std::exception
{
    std::string str_;
    
public:
    options_error(const std::string& str) noexcept
    : str_{str}
    {}
    virtual const char* what() const noexcept
    { return str_.c_str(); }
};

#endif	/* CONFIG_H */

