/* 
 * File:   provider.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 10:16 AM
 */

#ifndef PROVIDER_H
#define	PROVIDER_H

#include <atomic>

// Forward declarations
class dispatcher;
class config;

class provider
{
public:
    provider(dispatcher& disp) noexcept;
    provider(const provider& orig) = delete;
    provider(provider&& orig) = delete;
    provider& operator=(const provider& orig) = delete;
    ~provider() noexcept;
    
    void open_iface(const config& cfg);
    void run() noexcept;
    void stop() noexcept;

protected:
    dispatcher& dispatcher_;
    std::atomic_flag running_ {ATOMIC_FLAG_INIT};
    void* dev_handler_;
};

#endif	/* PROVIDER_H */

