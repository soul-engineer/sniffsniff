/* 
 * File:   frame.h
 * Author: Artem.Nazyrov
 *
 * Created on July 8, 2015, 7:29 PM
 */

#ifndef FRAME_H
#define	FRAME_H

#include <array>
#include <cinttypes>


class frame
{
public:
    // Maximum frame size in bytes
    static const size_t MAX_SIZE {65535};
    using elem_t = uint8_t;

    frame() = default;
    frame(const frame& orig) = delete;
    frame& operator=(const frame& orig) = delete;
    frame(frame&& orig) = delete;
    
    /**
     * Copies given data to the internal buffer
     * @param data A pointer to an ethernet frame
     * @param size Frame length in bytes
     */
    void set(const elem_t* data, size_t size) noexcept;
    const elem_t* get() const noexcept
    { return data_.data(); }
    size_t size() const noexcept
    { return size_; }
    
protected:
    std::array<elem_t, MAX_SIZE> data_;
    // Actual frame length in bytes
    size_t size_{};
};

#endif	/* FRAME_H */

