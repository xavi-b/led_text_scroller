#include "led_text_scroller.h"

#include <iostream>

LedTextScroller::LedTextScroller(){
#ifdef DEBUG
// spdlog::set_level(spdlog::level::debug);
#endif
}

LedTextScroller::~LedTextScroller()
{
    stop();
}

char LedTextScroller::end_of_line_char() const
{
    return this->end_of_line_char_;
}

void LedTextScroller::end_of_line_char(const char& c)
{
    this->end_of_line_char_ = c;
}

bool LedTextScroller::start(const char* port_name, int baud_rate)
{
    // spdlog::debug(__PRETTY_FUNCTION__);
    boost::system::error_code ec;

    if (port_)
    {
        // spdlog::error("Port is already opened...");
        return false;
    }

    port_ = std::make_shared<boost::asio::serial_port>(io_service_);
    port_->open(port_name, ec);
    if (ec)
    {
        // spdlog::error("port_->open() failed...com_port_name=" + std::string(port_name) + ", e=" + ec.message());
        return false;
    }

    // option settings...
    port_->set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    port_->set_option(boost::asio::serial_port_base::character_size(8));
    port_->set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    port_->set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    port_->set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    async_read_some();

    return true;
}

void LedTextScroller::stop()
{
    if (port_)
    {
        port_->cancel();
        port_->close();
        port_.reset();
    }
    io_service_.stop();
    io_service_.reset();
}

int LedTextScroller::write_some(const std::string& buf)
{
    return this->write_some(buf.c_str(), buf.size());
}

int LedTextScroller::write_some(const char* buf, const int& size)
{
    boost::system::error_code ec;

    if (!port_)
        return -1;
    if (size == 0)
        return 0;

    return port_->write_some(boost::asio::buffer(buf, size), ec);
}

void LedTextScroller::async_read_some()
{
    if (!port_ || !port_->is_open())
        return;

    boost::asio::async_read_until(*port_,
                                  buffer_,
                                  this->end_of_line_char(),
                                  boost::bind(&LedTextScroller::on_receive,
                                              this->shared_from_this(),
                                              boost::asio::placeholders::error,
                                              boost::asio::placeholders::bytes_transferred));
}

void LedTextScroller::on_receive(const boost::system::error_code& ec, size_t bytes_transferred)
{
    // spdlog::debug(__PRETTY_FUNCTION__);
    if (!port_ || !port_->is_open())
        return;
    if (ec)
    {
        this->async_read_some();
        return;
    }

    auto str = std::string((char*)buffer_.data().data(), bytes_transferred);

    // spdlog::info("LedTextScroller::on_receive() : " + str);

    buffer_.consume(bytes_transferred);

    async_read_some();
}