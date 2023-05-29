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

#ifndef HX711_SCALE_ADAPTOR_H_E0320524_681B_4A8E_85FC_EADBCD788510
#define HX711_SCALE_ADAPTOR_H_E0320524_681B_4A8E_85FC_EADBCD788510

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include "scale_adaptor.h"
#include "../extern/hx711-pico-c/include/common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    hx711_t* _hx;
    scale_adaptor_t _sa;
} hx711_scale_adaptor_t;

bool hx711_scale_adaptor_init(
    hx711_scale_adaptor_t* const hxa,
    hx711_t* const hx);

scale_adaptor_t* hx711_scale_adaptor_get_base(
    hx711_scale_adaptor_t* const hxa);

bool hx711_scale_adaptor_get_value_timeout(
    scale_adaptor_t* const sa,
    int32_t* const value,
    const uint timeout);

bool hx711_scale_adaptor_get_value(
    scale_adaptor_t* const sa,
    int32_t* const value);

#ifdef __cplusplus
}
#endif

#endif
