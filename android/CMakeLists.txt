cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME android)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Bluetooth)

qt_standard_project_setup()

qt_add_executable(app${PROJECT_NAME}
    main.cpp
    android/AndroidManifest.xml
)

qt_add_qml_module(app${PROJECT_NAME}
    URI ${PROJECT_NAME}
    VERSION 1.0
    QML_FILES
    Main.qml
    SOURCES
    BluetoothManager.h
    BluetoothManager.cpp
)

target_link_libraries(app${PROJECT_NAME}
    PRIVATE
    Qt6::Quick
    Qt6::Bluetooth
)

include(GNUInstallDirs)
install(TARGETS app${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
