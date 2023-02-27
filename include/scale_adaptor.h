// MIT License
// 
// Copyright (c) 2023 Daniel Robertson
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef SCALE_ADAPTOR_H_49094126_20AB_4B6A_B881_97D7D0DDD91E
#define SCALE_ADAPTOR_H_49094126_20AB_4B6A_B881_97D7D0DDD91E

#include <stdbool.h>
#include <stdint.h>
#include "pico/types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scale_adaptor {

    void* _data;

    bool (*get_value)(
        struct scale_adaptor* const sa,
        int32_t* const value);

    bool (*get_value_timeout)(
        struct scale_adaptor* const sa,
        int32_t* const value,
        const uint timeout);

} scale_adaptor_t;

void scale_adaptor_init(
    scale_adaptor_t* const sa,
    void* data);

void* scale_adaptor_get_data(
    scale_adaptor_t* const sa);

#ifdef __cplusplus
}
#endif

#endif
