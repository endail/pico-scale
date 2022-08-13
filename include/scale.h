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

#ifndef _SCALE_H_4B64A868_05B1_4F4C_99CF_E75ED9BAED50
#define _SCALE_H_4B64A868_05B1_4F4C_99CF_E75ED9BAED50

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "pico/time.h"
#include "../extern/hx711-pico-c/include/hx711.h"
#include "mass.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    strategy_type_samples = 0,
    strategy_type_time
} strategy_type_t;

typedef enum {
    read_type_median = 0,
    read_type_average
} read_type_t;

typedef struct {
    strategy_type_t strat;
    read_type_t read;
    size_t samples;
    uint timeout; //us
} scale_options_t;

static const scale_options_t SCALE_DEFAULT_OPTIONS = {
    .strat = strategy_type_samples,
    .read = read_type_median,
    .samples = 3, //3 samples
    .timeout = 1000000 //1 second
};

typedef struct {

    mass_unit_t unit;
    int32_t ref_unit;
    int32_t offset;

    hx711_t* _hx;

} scale_t;

/**
 * @brief Initialise the scale with a hx711_t
 * 
 * @param sc Pointer to scale_t
 * @param hx Pointer to hx711_t
 * @param unit The mass_unit_t to output mass_t's in
 * @param ref_unit The reference unit to use (see: calibration)
 * @param offset The offset from 0 (see: calibration)
 */
void scale_init(
    scale_t* const sc,
    hx711_t* const hx,
    const mass_unit_t unit,
    const int32_t ref_unit,
    const int32_t offset);

/**
 * @brief Adjusts a raw value to a normalised value according to the scale's
 * reference unit and offset. Returns true if the operation succeeded.
 * 
 * @param sc 
 * @param raw 
 * @param normalised 
 * @return true 
 * @return false 
 */
bool scale_normalise(
    const scale_t* const sc,
    const double* const raw,
    double* const normalised);

/**
 * @brief Fills arr with len number of samples from the scale. Returns true
 * if the operation succeeded.
 * 
 * @param sc 
 * @param arr 
 * @param len 
 * @return true 
 * @return false 
 */
bool scale_get_values_samples(
    scale_t* const sc,
    int32_t** const arr,
    const size_t len);

/**
 * @brief Fills arr with as many number of samples as possible up to the timeout.
 * Returns true if the operation succeeded.
 * 
 * @param sc 
 * @param arr 
 * @param len Will be to the number of samples obtained
 * @param timeout Microseconds
 * @return true 
 * @return false 
 */
bool scale_get_values_timeout(
    scale_t* const sc,
    int32_t** const arr,
    size_t* const len,
    const uint timeout);

/**
 * @brief Obtains a value from the scale according to the given options. Returns
 * true if the operation succeeded.
 * 
 * @param sc 
 * @param val 
 * @param opt 
 * @return true 
 * @return false 
 */
bool scale_read(
    scale_t* const sc,
    double* const val,
    const scale_options_t* const opt);

/**
 * @brief Zeros the scale (tare) by adjusting its offset from 0 according to
 * the given options. Returns true if the operation succeeded.
 * 
 * @param sc 
 * @param opt 
 * @return true 
 * @return false 
 */
bool scale_zero(
    scale_t* const sc,
    const scale_options_t* const opt);

/**
 * @brief Obtains a weight from the scale according to the given options. Returns
 * true if the operation succeeded.
 * 
 * @param sc 
 * @param m 
 * @param opt 
 * @return true 
 * @return false 
 */
bool scale_weight(
    scale_t* const sc,
    mass_t* const m,
    const scale_options_t* const opt);

#ifdef __cplusplus
}
#endif

#endif
