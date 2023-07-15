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
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "pico/time.h"
#include "../include/scale.h"
#include "../include/scale_adaptor.h"
#include "../include/util.h"

void scale_options_get_default(
    scale_options_t* const opt) {
        assert(opt != NULL);
        *opt = SCALE_DEFAULT_OPTIONS;
}

void scale_init(
    scale_t* const sc,
    scale_adaptor_t* const adaptor,
    const mass_unit_t unit,
    const int32_t ref_unit,
    const int32_t offset) {

        assert(sc != NULL);
        assert(adaptor != NULL);
        assert(ref_unit != 0);

        sc->_adaptor = adaptor;
        sc->unit = unit;
        sc->ref_unit = ref_unit;
        sc->offset = offset;

}

bool scale_normalise(
    const scale_t* const sc,
    const double* const raw,
    double* const normalised) {

        assert(sc != NULL);
        assert(raw != NULL);
        assert(normalised != NULL);

        //protect against div / 0
        if(sc->ref_unit == 0) {
            return false;
        }

        *normalised = (*raw - sc->offset) / sc->ref_unit;
        return true;

}

bool scale_get_values_samples(
    scale_t* const sc,
    int32_t* const arr,
    const size_t len) {

        assert(sc != NULL);
        assert(sc->_adaptor != NULL);
        assert(arr != NULL);

        for(size_t i = 0; i < len; ++i) {
            if(!sc->_adaptor->get_value(sc->_adaptor, &(arr[i]))) {
                return false;
            }
        }

        return true;

}

bool scale_get_values_timeout(
    scale_t* const sc,
    int32_t* const arr,
    const size_t arrlen,
    size_t* const len,
    const uint timeout) {

        assert(sc != NULL);
        assert(sc->_adaptor != NULL);
        assert(arr != NULL);
        assert(arrlen > 0);
        assert(len != NULL);

        //the absolute end time for seeking values (now + timeout)
        const absolute_time_t end = make_timeout_time_us(timeout);
        int32_t val; //temporary value from the adaptor
        int64_t diff; //difference in us from current time to end; updated

        //reset len to 0
        *len = 0;

        for(;;) {

            if(*len >= arrlen) {
                //no more room left
                break;
            }

            //update the time diff between now and the end
            diff = absolute_time_diff_us(get_absolute_time(), end);

            if(diff <= 0) {
                //timeout reached
                break;
            }

            if(sc->_adaptor->get_value_timeout(sc->_adaptor, &val, (uint)diff)) {

                //new value available, so increase the counter
                //this is the actual number of values obtained
                ++(*len);

                //store the value in the array
                arr[(*len) - 1] = val;

            }
            else {
                /* the last get_value_timeout call might fail because there is little time
                   left until the timeout is reached, so fail only if no values were read at all. */
                break;
            }

        }

        return *len > 0;

}

bool scale_read(
    scale_t* const sc,
    double* const val,
    const scale_options_t* const opt) {

        assert(sc != NULL);
        assert(val != NULL);
        assert(opt != NULL);

        size_t len;
        bool ok = false; //assume error

        switch(opt->strat) {
            case strategy_type_time:
                ok = scale_get_values_timeout(
                    sc,
                    opt->buffer,
                    opt->bufflen,
                    &len,
                    opt->timeout);
                break;

            case strategy_type_samples:
            default:
                assert(opt->bufflen >= opt->samples);
                len = opt->samples;
                ok = scale_get_values_samples(
                    sc,
                    opt->buffer,
                    len);
                break;
        }

        //exit early if fail
        if(!ok) {
            return false;
        }

        switch(opt->read) {
            case read_type_average:
                util_average(opt->buffer, len, val);
                break;

            case read_type_median:
            default:
                util_median(opt->buffer, len, val);
                break;
        }

        return true;

}

bool scale_zero(
    scale_t* const sc,
    const scale_options_t* const opt) {

        assert(sc != NULL);
        assert(opt != NULL);

        double val;
        const int32_t refBackup = sc->ref_unit;
        bool ok = false;

        sc->ref_unit = 1;

        if((ok = scale_read(sc, &val, opt))) {
            //only change the offset if the read
            //succeeded
            sc->offset = (int32_t)round(val);
        }

        sc->ref_unit = refBackup;

        return ok;

}

bool scale_weight(
    scale_t* const sc,
    mass_t* const m,
    const scale_options_t* const opt) {

        assert(sc != NULL);
        assert(m != NULL);
        assert(opt != NULL);

        double val;

        //if the read fails, return false
        if(!scale_read(sc, &val, opt)) {
            return false;
        }

        //if normalising the value fails, return false
        if(!scale_normalise(sc, &val, &val)) {
            return false;
        }

        mass_init(m, sc->unit, val);

        return true;

}