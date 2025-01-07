cmake_minimum_required(VERSION 3.16)

project(helloworld VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)
qt_standard_project_setup()

qt_add_executable(helloworld
    circular_window.cpp
)

target_link_libraries(helloworld PRIVATE Qt6::Widgets Qt6::Multimedia Qt6::MultimediaWidgets)
# target_link_options(helloworld PRIVATE LINKER:-sectcreate,__TEXT,__info_plist,${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
set_target_properties(helloworld
        PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.app.in")
set_target_properties(helloworld PROPERTIES MACOSX_BUNDLE TRUE)
#set_target_properties(helloworld PROPERTIES
#    WIN32_EXECUTABLE ON
#    MACOSX_BUNDLE ON
#)
