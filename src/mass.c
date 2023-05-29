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
#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include "pico/types.h"
#include "../include/mass.h"

const char* const mass_unit_to_string(const mass_unit_t u) {
    return MASS_NAMES[(uint)u];
}

const double* const mass_unit_to_ratio(const mass_unit_t u) {
    return &MASS_RATIOS[(uint)u];
}

void mass_convert(
    const double* const fromAmount,
    double* const toAmount,
    const mass_unit_t fromUnit,
    const mass_unit_t toUnit) {

        assert(fromAmount != NULL);
        assert(toAmount != NULL);

        if(fromUnit == toUnit) {
            *toAmount = *fromAmount;
        }
        else if(toUnit == mass_ug) {
            *toAmount = *fromAmount * *mass_unit_to_ratio(fromUnit);
        }
        else if(fromUnit == mass_ug) {
            *toAmount = *fromAmount / *mass_unit_to_ratio(toUnit);
        }
        else {
            mass_convert(fromAmount, toAmount, toUnit, mass_ug);
        }

}

void mass_init(
    mass_t* const m,
    const mass_unit_t unit,
    const double val) {

        assert(m != NULL);

        mass_convert(&val, &m->ug, unit, mass_ug);
        m->unit = unit;

}

void mass_get_value(
    const mass_t* const m,
    double* const val) {

        assert(m != NULL);
        assert(val != NULL);

        mass_convert(&m->ug, val, mass_ug, m->unit);

}

void mass_add(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res) {

        assert(lhs != NULL);
        assert(rhs != NULL);
        assert(res != NULL);

        res->ug = lhs->ug + rhs->ug;
        res->unit = lhs->unit;

}

void mass_sub(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res) {

        assert(lhs != NULL);
        assert(rhs != NULL);
        assert(res != NULL);

        res->ug = lhs->ug - rhs->ug;
        res->unit = lhs->unit;

}

void mass_mul(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res) {

        assert(lhs != NULL);
        assert(rhs != NULL);
        assert(res != NULL);

        res->ug = lhs->ug * rhs->ug;
        res->unit = lhs->unit;

}

bool mass_div(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res) {

        assert(lhs != NULL);
        assert(rhs != NULL);
        assert(res != NULL);

        //if ~0; protect against div / 0
        if(fabs(rhs->ug) < DBL_EPSILON) {
            return false;
        }

        res->ug = lhs->ug / rhs->ug;
        res->unit = lhs->unit;
        return true;

}

void mass_addeq(
    mass_t* const self,
    const mass_t* const rhs) {
        mass_add(self, rhs, self);
}

void mass_subeq(
    mass_t* const self,
    const mass_t* const rhs) {
        mass_sub(self, rhs, self);
}

void mass_muleq(
    mass_t* const self,
    const mass_t* const rhs) {
        mass_mul(self, rhs, self);
}

bool mass_diveq(
    mass_t* const self,
    const mass_t* const rhs) {
        return mass_div(self, rhs, self);
}

bool mass_eq(
    const mass_t* const lhs,
    const mass_t* const rhs) {

        assert(lhs != NULL);
        assert(rhs != NULL);

        //if ~==; if approx ~0
        return fabs(lhs->ug - rhs->ug) < DBL_EPSILON;

}

bool mass_neq(
    const mass_t* const lhs,
    const mass_t* const rhs) {

        assert(lhs != NULL);
        assert(rhs != NULL);

        return !mass_eq(lhs, rhs);

}

bool mass_lt(
    const mass_t* const lhs,
    const mass_t* const rhs) {

        assert(lhs != NULL);
        assert(rhs != NULL);

        return lhs->ug < rhs->ug;

}

bool mass_gt(
    const mass_t* const lhs,
    const mass_t* const rhs) {

        assert(lhs != NULL);
        assert(rhs != NULL);

        return mass_lt(rhs, lhs);

}

bool mass_lteq(
    const mass_t* const lhs,
    const mass_t* const rhs) {

        assert(lhs != NULL);
        assert(rhs != NULL);

        return !mass_gt(lhs, rhs);

}

bool mass_gteq(
    const mass_t* const lhs,
    const mass_t* const rhs) {

        assert(lhs != NULL);
        assert(rhs != NULL);

        return !mass_lt(lhs, rhs);

}


int mass_to_string(
    const mass_t* const m,
    char* const buff) {

        assert(m != NULL);
        assert(buff != NULL);

        double n; //value
        mass_get_value(m, &n);
        double i; //int part; discard
        const double f = fabs(modf(n, &i)); //frac part
        uint d = 0; //decimal count

        //if less than the epsilon, then it's ~0
        if(f >= DBL_EPSILON) { 
            d = (uint)fmax(0, ceil(1 - log10(f)));
        }

        return snprintf(
            buff,
            MASS_TO_STRING_BUFF_SIZE,
            "%01.*f %s", //format
            d, //how many decimals
            n, //the value
            mass_unit_to_string(m->unit)); //suffix (ie. "kg")

}
