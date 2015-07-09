/* 
 * File:   worker.h
 * Author: Artem.Nazyrov
 * 
 * Created on July 7, 2015, 09:28 PM
 */

#ifndef WORKER_H
#define	WORKER_H

#include <memory>

class dispatcher;
struct worker_impl;
class frame;


class worker
{
public:
    worker(const dispatcher& pool);
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
    const dispatcher& pool_;
    std::unique_ptr<worker_impl> impl_;
    
    void parse_frame(const frame& fr) const;
};

#endif	/* WORKER_H */

