/* 
 * File:   worker_pool.h
 * Author: Artem.Nazyrov
 *
 * Created on July 7, 2015, 08:55 PM
 */

#ifndef WORKER_POOL_H
#define	WORKER_POOL_H

#include <memory>

class config;
struct worker_pool_impl;

class worker_pool
{
public:
    worker_pool(const config& cfg);
    worker_pool(const worker_pool& orig) = delete;
    worker_pool(worker_pool&& orig) = delete;
    worker_pool& operator=(const worker_pool& orig) = delete;
    ~worker_pool();
    
    void stop_threads();
    bool active() const;
    
private:
    std::unique_ptr<worker_pool_impl> impl_;
};

#endif	/* WORKER_POOL_H */

