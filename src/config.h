/* 
 * File:   config.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 9:01 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <string>


// TODO: This class should wrap boost::program_options
class config
{
public:
    config();
    config(const config& orig) = delete;
    config(config&& orig) = delete;
    config& operator=(const config& orig) = delete;
    ~config() = default;
    
    const std::string& get_if_name() const noexcept
    { return if_name_; }
    void set_if_name(const char* if_name);
    
    bool get_promisc() const noexcept
    { return promisc_; }
    
    uint8_t get_thread_count() const noexcept
    { return thread_count_; }

protected:
    std::string if_name_;
    bool promisc_;
    uint32_t thread_count_;
};

#endif	/* CONFIG_H */

