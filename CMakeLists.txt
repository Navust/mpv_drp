cmake_minimum_required(VERSION 3.30)
project(mpv_drp C)

set(CMAKE_C_STANDARD 11)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME mpv
        GIT_REPOSITORY https://github.com/mpv-player/mpv.git
        VERSION 0.39.0
        DOWNLOAD_ONLY YES
)

add_library(mpv_drp SHARED mpv_drp.c)

if (mpv_ADDED)
    target_include_directories(mpv_drp PUBLIC ${mpv_SOURCE_DIR})
    target_compile_definitions(mpv_drp PUBLIC "MPV_CPLUGIN_DYNAMIC_SYM")
endif()

# TODO: Link precompiled Discord Game SDK library
# https://stackoverflow.com/a/17127764
