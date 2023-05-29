// MIT License
// 
// Copyright (c) 2022 Daniel Robertson
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

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../extern/hx711-pico-c/include/common.h"
#include "../include/hx711_scale_adaptor.h"

bool hx711_scale_adaptor_init(
    hx711_scale_adaptor_t* const hxa,
    hx711_t* const hx) {

        assert(hxa != NULL);
        assert(hx != NULL);

        hxa->_hx = hx;

        scale_adaptor_init(&hxa->_sa, hxa);
        hxa->_sa.get_value = hx711_scale_adaptor_get_value;
        hxa->_sa.get_value_timeout = hx711_scale_adaptor_get_value_timeout;

        return true;

}

scale_adaptor_t* hx711_scale_adaptor_get_base(
    hx711_scale_adaptor_t* const hxa) {
        assert(hxa != NULL);
        return &hxa->_sa;
}

bool hx711_scale_adaptor_get_value_timeout(
    scale_adaptor_t* const sa,
    int32_t* const value,
    const uint timeout) {

        assert(sa != NULL);
        assert(value != NULL);

        hx711_scale_adaptor_t* const hxa = scale_adaptor_get_data(sa);
        return hx711_get_value_timeout(hxa->_hx, value, timeout);

}

bool hx711_scale_adaptor_get_value(
    scale_adaptor_t* const sa,
    int32_t* const value) {

        assert(sa != NULL);
        assert(value != NULL);

        hx711_scale_adaptor_t* const hxa = scale_adaptor_get_data(sa);
        *value = hx711_get_value(hxa->_hx);

        return true;

}
