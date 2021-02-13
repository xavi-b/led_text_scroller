set(MODULE_NAME led_text_scroller_example)

message("Compiling \"${MODULE_NAME}\"")

add_executable(${MODULE_NAME} "")

set_target_properties(${MODULE_NAME} PROPERTIES SOVERSION "1.0.0")

# dependencies
find_package(PkgConfig REQUIRED)
pkg_search_module(LIBUSB REQUIRED libusb-1.0)

target_include_directories(${MODULE_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/lib/include
        ${PROJECT_SOURCE_DIR}/third-party/libusbp/include
)

target_link_libraries(${MODULE_NAME}
    PUBLIC
        led_text_scroller
        usbp
        usb-1.0
)


# sources
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${MODULE_NAME}
    PUBLIC
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)