#ifndef LED_TEXT_SCROLLER_H
#define LED_TEXT_SCROLLER_H

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/bind/bind.hpp>
#include <boost/thread.hpp>

#include <usb.h>

#include <memory>
#include <string>
#include <vector>

typedef std::shared_ptr<boost::asio::serial_port> serial_port_ptr;

class LedTextScroller : public std::enable_shared_from_this<LedTextScroller>
{
public:
    LedTextScroller();
    virtual ~LedTextScroller();

    char end_of_line_char() const;
    void end_of_line_char(const char& c);

    virtual bool start(const char* port_name, int baud_rate = 9600);
    virtual void stop();

    int write_some(const std::string& buf);
    int write_some(const char* buf, const int& size);

protected:
    virtual void async_read_some();
    virtual void on_receive(const boost::system::error_code& ec, size_t bytes_transferred);

private:
    LedTextScroller(const LedTextScroller& p);
    LedTextScroller& operator=(const LedTextScroller& p);

protected:
    boost::asio::io_service io_service_;
    serial_port_ptr         port_;
    boost::asio::streambuf  buffer_;

    char end_of_line_char_ = '\n';
};

#endif