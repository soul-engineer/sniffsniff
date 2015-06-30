/* 
 * File:   config.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 9:01 PM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#include <string>


class config {
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

protected:
    std::string if_name_;
    bool promisc_;
};

#endif	/* CONFIG_H */

