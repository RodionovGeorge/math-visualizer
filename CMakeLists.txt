cmake_minimum_required(VERSION 3.1.0)

project(math_visualizer)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
set(CMAKE_PREFIX_PATH /usr/include/qt/)
set(CMAKE_DEBUG_POSTFIX d)
find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
add_executable(math_visualizer
	main.cpp
	widget.cpp
    algorithms.cpp
    screen.cpp
)
set_target_properties(math_visualizer PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})
target_link_libraries(math_visualizer Qt5::Core Qt5::Widgets)
