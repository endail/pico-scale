# MIT License
# 
# Copyright (c) 2022 Daniel Robertson
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 3.12)

# ===================== INCLUDE PICO SDK =====================
# ================= MUST BE ABOVE PROJECT()! =================
include(pico_sdk_import.cmake)
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()
# ============================================================

project(pico-scale
        VERSION 0.0.1
        DESCRIPTION "Example use of hx711-pico-c library as a scale application"
        HOMEPAGE_URL "https://github.com/endail/pico-scale"
        LANGUAGES C CXX ASM
        )

# ====================== INIT PICO SDK =======================
# ================= MUST BE BELOW PROJECT()! =================
pico_sdk_init()
# ============================================================

# include hx711 lib
add_subdirectory(extern/hx711-pico-c)

add_compile_options(
        -Wall
        -Wextra
        -Werror
        -Wfatal-errors
        -Wfloat-equal
        -Wunreachable-code
        -Wno-unused-function
        -Wno-maybe-uninitialized
        -Wno-ignored-qualifiers
        -Wno-stringop-overflow          # rp2040_usb.c:61:3
        -Wno-array-bounds               # rp2040_usb.c:61:3
        )

add_executable(main
        main.c
        src/mass.c
        src/scale.c
        src/util.c
        )

target_link_libraries(main
        hx711-pico-c
        pico_divider
        pico_double
        pico_stdlib
        pico_stdio
        )

pico_add_extra_outputs(main)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 1)
