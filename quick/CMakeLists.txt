cmake_minimum_required(VERSION 3.5)
project(ads_example_quick VERSION ${VERSION_SHORT})
find_package(Qt5 5.5 COMPONENTS Core Gui Widgets REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(quick WIN32
    main.cpp
    FacChart.cpp
    MainWindow.cpp
)
target_include_directories(quick PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../src")
target_link_libraries(quick PRIVATE qtadvanceddocking)
target_link_libraries(quick PUBLIC Qt5::quick Qt5::Core Qt5::Gui Qt5::Widgets)
set_target_properties(quick PROPERTIES
    AUTOMOC ON
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
    VERSION ${VERSION_SHORT}
    EXPORT_NAME "Qt Advanced Docking System quick Example"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${ads_PlatformDir}/bin"
)
