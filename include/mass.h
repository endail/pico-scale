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

#ifndef MASS_H_62063E22_421F_4578_BAD8_46AF5C66C4CF
#define MASS_H_62063E22_421F_4578_BAD8_46AF5C66C4CF

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

static const double MASS_RATIOS[] = {
    1.0,
    1000.0,
    1000000.0,
    1000000000.0,
    1000000000000.0,
    1016046908800.0,
    907184740000.0,
    6350293180.0,
    453592370.0,
    28349523.125
};

static const char* const MASS_NAMES[] = {
    "μg",
    "mg",
    "g",
    "kg",
    "ton",
    "ton (IMP)",
    "ton (US)",
    "st",
    "lb",
    "oz"
};

static const size_t MASS_TO_STRING_BUFF_SIZE = 64;

/**
 * Enum values are mapped to MASS_RATIOS and MASS_NAMES
 */
typedef enum {
    mass_ug = 0,
    mass_mg,
    mass_g,
    mass_kg,
    mass_ton,
    mass_imp_ton,
    mass_us_ton,
    mass_st,
    mass_lb,
    mass_oz
} mass_unit_t;

/**
 * @brief Returns a pointer to char with the textual representation of the unit
 * 
 * @param u 
 * @return const char* const 
 */
const char* const mass_unit_to_string(const mass_unit_t u);

/**
 * @brief Returns a pointer to a double with the number of micrograms per unit
 * 
 * @param u 
 * @return const double* const 
 */
const double* const mass_unit_to_ratio(const mass_unit_t u);

typedef struct {
    double ug;
    mass_unit_t unit;
} mass_t;

/**
 * @brief Converts a floating point value from one unit to another
 * 
 * @param fromAmount 
 * @param toAmount 
 * @param fromUnit 
 * @param toUnit 
 */
void mass_convert(
    const double* const fromAmount,
    double* const toAmount,
    const mass_unit_t fromUnit,
    const mass_unit_t toUnit);

/**
 * @brief Initialises a mass_t with the given unit and value
 * 
 * @param m 
 * @param unit 
 * @param val 
 */
void mass_init(
    mass_t* const m,
    const mass_unit_t unit,
    const double val);

/**
 * @brief Sets val to the value representing the mass_t according to its unt
 * 
 * @param m 
 * @param val 
 */
void mass_get_value(
    const mass_t* const m,
    double* const val);

/**
 * @brief Add lhs to rhs and store result in res
 * 
 * @param lhs 
 * @param rhs 
 * @param res 
 */
void mass_add(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res);

/**
 * @brief Substract rhs from lhs and store result in res
 * 
 * @param lhs 
 * @param rhs 
 * @param res 
 */
void mass_sub(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res);

/**
 * @brief Multiply lhs by rhs and store result in res
 * 
 * @param lhs 
 * @param rhs 
 * @param res 
 */
void mass_mul(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res);

/**
 * @brief Divide lhs by rhs and store result in res, returns false if rhs is 0
 * 
 * @param lhs 
 * @param rhs 
 * @param res 
 * @return true 
 * @return false 
 */
bool mass_div(
    const mass_t* const lhs,
    const mass_t* const rhs,
    mass_t* const res);

/**
 * @brief Add rhs to self
 * 
 * @param self 
 * @param rhs 
 */
void mass_addeq(
    mass_t* const self,
    const mass_t* const rhs);

/**
 * @brief Subtract rhs from self
 * 
 * @param self 
 * @param rhs 
 */
void mass_subeq(
    mass_t* const self,
    const mass_t* const rhs);

/**
 * @brief Multiply self by rhs
 * 
 * @param self 
 * @param rhs 
 */
void mass_muleq(
    mass_t* const self,
    const mass_t* const rhs);

/**
 * @brief Divide self by rhs, returns false if rhs is 0
 * 
 * @param self 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_diveq(
    mass_t* const self,
    const mass_t* const rhs);

/**
 * @brief Returns true if lhs equals rhs
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_eq(
    const mass_t* const lhs,
    const mass_t* const rhs);

/**
 * @brief Returns true if lhs does not equal rhs
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_neq(
    const mass_t* const lhs,
    const mass_t* const rhs);

/**
 * @brief Returns true if lhs is less than rhs
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_lt(
    const mass_t* const lhs,
    const mass_t* const rhs);

/**
 * @brief Returns true if lhs is greater than rhs
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_gt(
    const mass_t* const lhs,
    const mass_t* const rhs);

/**
 * @brief Returns true if lhs is less than or equal to rhs
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_lteq(
    const mass_t* const lhs,
    const mass_t* const rhs);

/**
 * @brief Returns true if lhs is greater than or equal to rhs
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool mass_gteq(
    const mass_t* const lhs,
    const mass_t* const rhs);

/**
 * @brief Fills buff with the string representation of the mass_t. eg. "32.4762 mg"
 * 
 * @param m 
 * @param buff Must be at least MASS_TO_STRING_BUFF_SIZE in length
 * @return int Returns result of snprintf
 */
int mass_to_string(
    const mass_t* const m,
    char* const buff);

#ifdef __cplusplus
}
#endif

#endif
