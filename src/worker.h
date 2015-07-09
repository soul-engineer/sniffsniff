/* 
 * File:   worker.h
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 09:28 PM
 */

#ifndef WORKER_H
#define	WORKER_H

#include <memory>

class worker_pool;
struct worker_impl;


class worker
{
public:
    worker(const worker_pool& pool);
    worker(const worker& orig) = delete;
    worker& operator=(const worker& orig) = delete;
    worker(worker&& orig);
    ~worker();
    void operator()();
    
    /**
     * Adds given frame to task queue
     * @param data A pointer to an ethernet frame
     * @param size Frame length in bytes
     * @return  true if task was added successfully,
     * false if task queue is already full
     */
    bool add_frame(const uint8_t* data, size_t size);
    
protected:
    const worker_pool& pool_;
    std::unique_ptr<worker_impl> impl_;
};

#endif	/* WORKER_H */

