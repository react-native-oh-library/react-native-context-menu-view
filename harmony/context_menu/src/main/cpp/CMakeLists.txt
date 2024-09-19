cmake_minimum_required(VERSION 3.13)
set(CMAKE_VERBOSE_MAKEFILE on)

file(GLOB rnoh_context_menu_SRC CONFIGURE_DEPENDS *.cpp)
add_library(rnoh_context_menu SHARED ${rnoh_context_menu_SRC})
target_include_directories(rnoh_context_menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(rnoh_context_menu PUBLIC rnoh)

