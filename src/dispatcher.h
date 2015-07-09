/* 
 * File:   dispatcher.h
 * Author: Artem.Nazyrov
 *
 * Created on July 7, 2015, 08:55 PM
 */

#ifndef DISPATCHER_H
#define	DISPATCHER_H

#include <memory>

class config;
struct dispatcher_impl;

class dispatcher
{
public:
    dispatcher(const config& cfg);
    dispatcher(const dispatcher& orig) = delete;
    dispatcher(dispatcher&& orig) = delete;
    dispatcher& operator=(const dispatcher& orig) = delete;
    ~dispatcher();
    
    bool active() const noexcept;
    void dispatch_frame(const uint8_t* data, size_t len) noexcept;
    
protected:
    std::unique_ptr<dispatcher_impl> impl_;
};

#endif	/* DISPATCHER_H */

