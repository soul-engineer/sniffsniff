/* 
 * File:   dispatcher.h
 * Author: Artem.Nazyrov
 *
 * Created on June 30, 2015, 10:25 AM
 */

#ifndef DISPATCHER_H
#define	DISPATCHER_H

#include <cinttypes>
#include <cstddef>


class dispatcher
{
public:
    dispatcher();
    dispatcher(const dispatcher& orig) = delete;
    dispatcher(dispatcher&& orig) = delete;
    dispatcher& operator=(const dispatcher& orig) = delete;
    ~dispatcher() = default;
    
    void dispatch_frame(const uint8_t* data, size_t len) noexcept;
    
private:

};

#endif	/* DISPATCHER_H */

