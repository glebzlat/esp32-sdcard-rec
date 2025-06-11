cmake_minimum_required(VERSION 3.16)

include($ENV{ADF_PATH}/CMakeLists.txt)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

list(APPEND EXTRA_COMPONENT_DIRS
    "$ENV{ADF_PATH}/examples/protocols/components/av_stream"
)

idf_build_set_property(MINIMAL_BUILD ON)

project(sd_card_sound)
