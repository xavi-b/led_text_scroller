#include <libusbp.hpp>
#include <iostream>
#include <iomanip>
#include <libusb-1.0/libusb.h>
#include <led_text_scroller.h>

std::string serial_number_or_default(const libusbp::device& device, const std::string& def)
{
    try
    {
        return device.get_serial_number();
    }
    catch (const libusbp::error& error)
    {
        if (error.has_code(LIBUSBP_ERROR_NO_SERIAL_NUMBER))
        {
            return def;
        }
        throw;
    }
}

bool try_print_port_name(const libusbp::device& device, uint8_t interface_number, bool composite)
{
    std::string port_name;
    try
    {
        libusbp::serial_port port(device, interface_number, composite);
        port_name = port.get_name();
    }
    catch (const libusbp::error& error)
    {
        return false;
    }
    std::cout << "  " << port_name << std::endl;
    return true;
}

void try_print_product_name(libusbp::device& device)
{
    libusb_context* context = NULL;

    int rc = 0;
    if ((rc = libusb_init(&context)) != 0)
    {
        std::cerr << libusb_strerror(rc) << std::endl;
        return;
    }

    libusb_device_handle* dev_handle =
        libusb_open_device_with_vid_pid(context, device.get_vendor_id(), device.get_product_id());
    if (dev_handle)
    {
        std::cerr << "dev_handle" << std::endl;
        uint8_t data[256];

        int size = 0;

        size = libusb_get_string_descriptor_ascii(dev_handle, device.get_vendor_id(), data, 256);
        if (size > 0)
        {
            std::cout << "  " << std::string((char*)data, size) << std::endl;
        }

        size = libusb_get_string_descriptor_ascii(dev_handle, device.get_product_id(), data, 256);
        if (size > 0)
        {
            std::cout << "  " << std::string((char*)data, size) << std::endl;
        }

        libusb_close(dev_handle);
    }
    else
    {
        std::cerr << "libusb_open_device_with_vid_pid error" << std::endl;
    }

    libusb_exit(context);
}

void print_device(libusbp::device& device)
{
    bool        success       = false;
    uint16_t    vendor_id     = device.get_vendor_id();
    uint16_t    product_id    = device.get_product_id();
    uint16_t    revision      = device.get_revision();
    std::string serial_number = serial_number_or_default(device, "-");
    std::string os_id         = device.get_os_id();

    // Note: The serial number might have spaces in it, so it should be the last
    // field to avoid confusing programs that are looking for a field after the
    // serial number.

    std::ios::fmtflags flags(std::cout.flags());
    std::cout << std::hex << std::setfill('0') << std::right << std::setw(4) << vendor_id << ':' << std::setw(4)
              << product_id << ' ' << std::setfill(' ') << std::setw(2) << (revision >> 8) << '.' << std::setfill('0')
              << std::setw(2) << (revision & 0xFF) << ' ' << os_id << ' ' << std::setfill(' ') << std::left
              << serial_number << std::endl;
    std::cout.flags(flags);

    try_print_product_name(device);

    // First, assume the device is composite and try the first 16 interfaces.
    // Most devices don't have more interfaces than that.  Trying all 255 possible
    // interfaces slows the program down noticeably.  This issue could be fixed if
    // we added better serial port enumeration support to libusbp.
    for (uint32_t i = 0; i < 16; i++)
    {
        success = try_print_port_name(device, i, true) || success;
    }

    // Try to find a port assuming the device is non-composite.  Only do so if
    // no ports were found earlier, to help avoid printing the same port twice.
    if (!success)
    {
        try_print_port_name(device, 0, false);
    }
}

int main_with_exceptions()
{
    std::vector<libusbp::device> list = libusbp::list_connected_devices();
    for (auto it = list.begin(); it != list.end(); ++it)
    {
        print_device(*it);
    }

    std::shared_ptr<LedTextScroller> c = std::make_shared<LedTextScroller>();

    std::string port_name;
    std::string msg;

    // std::cout << "Select port name (ex: /dev/ttyACM0): ";
    // std::getline(std::cin, port_name);
    // c->start(port_name.c_str());
    c->start("/dev/ttyACM0");

    while (true)
    {
        std::cout << "Enter message: ";
        std::getline(std::cin, msg);
        c->write_some(msg);
    }

    return 0;
}

int main(int argc, char** argv)
{
    // Suppress unused parameter warnings.
    (void)argc;
    (void)argv;

    try
    {
        return main_with_exceptions();
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << std::endl;
    }
    return 1;
}