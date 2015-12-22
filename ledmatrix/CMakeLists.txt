# Copyright 2015 sean93.park@gmail.com
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.8)

project(ledmatrix)

# options
set(FLAGS_COMMON
    "-march=armv6"
    "-mno-thumb-interwork"
    "-Wall"
    "-std=gnu++11"
    "-pthread"
    )

foreach(FLAG ${FLAGS_COMMON})
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FLAG}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}")
endforeach()

# name
set(PROGRAM_NAME "ledmatrix")

# path
set(ROOT_PATH "${CMAKE_SOURCE_DIR}")
set(INCLUDE_PATH "${ROOT_PATH}/include")
set(SOURCE_PATH "${ROOT_PATH}/source")

# files
set(C_SOURCE_FILES
    "${SOURCE_PATH}/ledmatrix.cpp"
    "${SOURCE_PATH}/rpi_i2c.cpp"
    "${SOURCE_PATH}/font_5x7.cpp"
    )

# program
add_executable(
    ${PROGRAM_NAME}
    ${C_SOURCE_FILES}
    )

# include
include_directories(
    ${PROGRAMNAME}
    PUBLIC
      ${INCLUDE_PATH}
      ${SOURCE_PATH}
    )

# linker
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lrt")
