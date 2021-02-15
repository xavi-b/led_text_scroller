set(MODULE_NAME led_text_scroller)

message("Compiling \"${MODULE_NAME}\"")

add_library(${MODULE_NAME} SHARED "")

set_target_properties(${MODULE_NAME} PROPERTIES SOVERSION "1.0.0")

# dependencies
find_package(Boost COMPONENTS system thread REQUIRED)

target_link_libraries(${MODULE_NAME}
    PUBLIC
        ${Boost_LIBRARIES}

)

# sources
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(${MODULE_NAME}
    PUBLIC
        external
        "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

# install
install(
    TARGETS ${MODULE_NAME}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)