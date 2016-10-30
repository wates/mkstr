cmake_minimum_required(VERSION 2.8)

project(mkstr)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "-Wno-unused-parameter -fno-strict-aliasing -O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11 ${CMAKE_CXX_FLAGS}")
    add_definitions(-DGCC)
elseif(MSVC)
    set(CMAKE_C_FLAGS "/W4 /wd4819 /wd4100")
    set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
    add_definitions(-DWINDOWS)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
endif()

include_directories("freetype/include" "zlib/include")

add_subdirectory(freetype)
add_subdirectory(zlib)

add_executable(mkstr
	bincc.h
	font_ttf.cc
    mkstr.cc
	png.cc
	png.h
    )

target_link_libraries(mkstr freetype zlib)
