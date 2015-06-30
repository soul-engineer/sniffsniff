/* 
 * File:   provider.cpp
 * Author: Artem.Nazyrov
 * 
 * Created on June 30, 2015, 10:16 AM
 */

#include "provider.h"
#include "dispatcher.h"
#include "config.h"
#include "frame.h"
#include <pcap/pcap.h>
#include <glog/logging.h>
#include <stdexcept>
#include <cinttypes>
#include <array>


namespace
{
    // Packet read timeout in milliseconds
    const int READ_TIMEOUT {1};
    
    // pcap_next_ex() result codes
    enum RES_CODES
    {
        // The packet was read without problems
        SUCCESS = 1,
        // The timeout has expired
        TIMEOUT = 0,
        // For pcaps only: there are no more packets to read from the pcap file
        PCAP_EOF = -2,
        // Other error
        ERROR = -1
    };
}


provider::provider(dispatcher& disp) noexcept
: dispatcher_(disp)
{
    // ctor
}

provider::~provider() noexcept
{
    // Close device handler and deallocate all resources
    if (nullptr != dev_handler_)
    {
        pcap_close(static_cast<pcap_t*>(dev_handler_));
        dev_handler_ = nullptr;
    }
}

void provider::open_iface(const config& cfg)
{
    // Initialize libpcap
    std::array<char, PCAP_ERRBUF_SIZE> error_buffer;
    error_buffer.fill(0);
    
    dev_handler_ = pcap_open_live(
            cfg.get_if_name().c_str(),
            frame::MAX_SIZE,
            cfg.get_promisc(),
            READ_TIMEOUT,
            error_buffer.data());
    
    if (nullptr == dev_handler_)
    {
        LOG(ERROR) << "Can't bind to the interface \"" << cfg.get_if_name() <<
                "\". Error message is " << error_buffer.data();
        throw std::runtime_error(error_buffer.data());
    }
    
    // Set up flag to activate run cycle
    running_.test_and_set();
}

void provider::run() noexcept
{
    CHECK(nullptr != dev_handler_) << "Call open_iface() first";
    
    pcap_pkthdr*  pkt_header {nullptr}; 
    const u_char*  pkt_data {nullptr};
    auto handler = static_cast<pcap_t* const>(dev_handler_);
    
    while (running_.test_and_set(std::memory_order_relaxed))
    {
        // Read a frame
        const int res_code {pcap_next_ex(handler, &pkt_header, &pkt_data)};
        
        switch (res_code)
        {
            case SUCCESS:
                DCHECK(nullptr != pkt_header) << "pkt_header pointer is a null";
                DCHECK(nullptr != pkt_data) << "pkt_data pointer is a null";
                DCHECK(pkt_header->len <= pkt_header->caplen) <<
                        "Packet lengths mismatch: " << pkt_header->len <<
                        " > " << pkt_header->caplen;
                static_assert(sizeof(u_char) == sizeof(uint8_t), "Check types");                
                
                dispatcher_.dispatch_frame(
                        static_cast<const uint8_t*>(pkt_data),
                        pkt_header->len);
                break;
                
            case TIMEOUT:
                // TODO: don't forget to choose right timeout value
                break;
                
            case PCAP_EOF:
                stop();
                break;
            
            case ERROR:
                PLOG(ERROR) << "pcap_next_ex() error: " << pcap_geterr(handler);
            default:
                LOG(ERROR) << "Unknown pcap_next_ex() res_code: " << res_code;
        }
    }
}

void provider::stop() noexcept
{
    running_.clear();
}
