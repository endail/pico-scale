# pico-scale

A scale API for a Raspberry Pi Pico (RP2040) using the [hx711-pico-c library](https://github.com/endail/hx711-pico-c).

![resources/hx711_serialout.gif](resources/hx711_serialout.gif)

The .gif above illustrates the [current example code](main.c) obtaining data from a HX711 operating at 80 samples per second. Each line shows the current weight calculated from all samples obtained within 250 milliseconds, along with the minimum and maximum weights of the scale since boot. I applied pressure to the load cell to show the change in weight.

## Clone

### Method 1

If you want to use this repository as-is with the example code, clone the respository and initialise the `hx711-pico-c` submodule.

```console
git clone https://github.com/endail/pico-scale
git submodule update --init
```

Then `#include` as follows:

```c
#include "include/scale.h"
#include "extern/hx711-pico-c/include/hx711_noblock.pio.h"
```

### Method 2

Alternatively, if you want to use the scale functionality as an API in your own project, add `pico-scale` as a submodule and then initialise it.

```console
git submodule add https://github.com/endail/pico-scale extern/
git submodle update --init --recursive
```

Then, from your own code, `#include` the relevant files as follows and initialise the hx711:

```c
#include "extern/pico-scale/include/scale.h"
#include "extern/pico-scale/extern/hx711-pico-c/include/hx711_noblock.pio.h"
```

See the explanation [here](https://github.com/endail/hx711-pico-c#custom-pio-programs) for why you need to manually include the PIO program.

## Documentation

[https://endail.github.io/pico-scale](https://endail.github.io/pico-scale/)

## Initialise the HX711

You will always need to initialise the HX711 before using it as a scale. See [here](https://github.com/endail/hx711-pico-c#how-to-use) for how to do that.

## How to Use the HX711 as a Scale

```c
scale_t sc;

// the values obtained when calibrating the scale
// if you don't know them, read the following section How to Calibrate
mass_unit_t scaleUnit = mass_g;
int32_t refUnit = -432;
int32_t offset = -367539;

scale_init(
    &sc,
    &hx, // pass a pointer to the hx711_t
    scaleUnit,
    refUnit,
    offset);

// 3. Set options for how the scale will read and interpret values

// SCALE_DEFAULT_OPTIONS will give some default settings which you
// do not have to use
scale_options_t opt = SCALE_DEFAULT_OPTIONS;

// scale_options_t has the following options
//
// opt.strat, which defines how the scale will collect data. By default,
// data is collected according to the number of samples. So opt.strat
// is set to strategy_type_samples. opt.samples defines how many samples
// to obtain. You can also set opt.strat to read_type_time which will
// collect as many samples as possible within the timeout period. The
// timeout period is defined by opt.timeout and is given in microseconds
// (us). For example, 1 second is equal to 1,000,000 us.
//
// opt.read, which defines how the scale will interpret data. By default,
// data is interpreted according to the median value. So opt.read is set
// to read_type_median. You can also set opt.read to read_type_average
// which will calculate the average value.
//
// Example:
//
// opt.strat = strategy_type_time;
// opt.read = read_type_average;
// opt.timeout = 250000;
//
// These options mean... collect as many samples as possible within 250ms
// and then use the average of all those samples.

// 4. Zero the scale (OPTIONAL) (aka. tare)

if(scale_zero(&sc, &opt)) {
    printf("Scale zeroed successfully\n");
}
else {
    printf("Scale failed to zero\n");
}

// 5. Obtain the weight
mass_t mass;

if(scale_weight(&sc, &mass, &opt)) {

    // mass will contain the weight on the scale obtanined and interpreted
    // according to the given options and be in the unit defined by the
    // mass_unit_t 'scaleUnit' variable above
    //
    // you can now:

    // get the weight as a numeric value according to the mass_unit_t
    double val;
    mass_get_value(&mass, &val);

    // convert the mass to a string
    char buff[MASS_TO_STRING_BUFF_SIZE];
    mass_to_string(&mass, buff);
    printf("%s\n", buff);

    // or do other operations (see: mass.h file)

}
else {
    printf("Failed to read weight\n");
}
```

## How to Calibrate

1. Modify [the calibration program](calibration.c#L68-L75) and change the clock and data pins to those connected to the HX711. Also change the rate at which the HX711 operates if needed.

2. Build.

3. Copy `calibration.uf2` in the build directory to the Raspberry Pi Pico.

4. Open a serial connection to the Pico at a baud rate of 115200 and follow the prompts.

## FAQ

Q: __"Which mass units are supported?"__

A: The following `mass_unit_t`s are available.

- `mass_ug`: micrograms
- `mass_mg`: milligrams
- `mass_g`: grams
- `mass_kg`: kilograms
- `mass_ton`: metric tons
- `mass_imp_ton`: imperial tons
- `mass_us_ton`: US tons
- `mass_st`: stones
- `mass_lb`: pounds
- `mass_oz`: ounces

Q: __"How do I get the weight in pounds/ounces/tons/etc...?"__

A: You can either: set the `scale_t` or change the `mass_t`.

```c
// 1. setting the scale_t

// when you initialise the scale
scale_init(
    &sc,
    &hx,
    mass_imp_ton, //change the scaleUnit to your chosen mass_unit_t
    refUnit,
    offset);

// or, if you've already initialised the scale
sc.unit = mass_imp_ton;


// 2. change the mass_t

// if, for whatever reason, you are initialising a mass_t
mass_init(
    &m,
    mass_lb, //your desired mass_unit_t
    val);

// or, if you've already initialised a mass_t
m.unit = mass_lb;
```

Q: __"How do I perform math on the weights?"__

A: You can either: get the underlying value and operate on that, or use the in-built functions to operate on two `mass_t` structs.

```c
// 1. get the underlying value
double val;
mass_get_value(&m, &val);

if(val >= 10.5) {
    //do something if val is greater than or equal to 10.5
}

//2. built-in functions
if(mass_gteq(&m1, &m2)) {
    //do something if m1 is greater than m2
}
```

The advantage of using the built-in functions is that the `mass_t` structs can be of different units. So you can check if, for example, `m1` is greater than or equal to `m2`, even if `m1` is is in pounds and `m2` is in kilograms. The conversion is taken care of.

Q: __"Which math functions are available?"__

A:

- `mass_add(mass_t* lhs, mass_t* rhs, mass_t* res)`: add `lhs` and `rhs` and store result in `res`
- `mass_sub(mass_t* lhs, mass_t* rhs, mass_t* res)`: subtract `rhs` from `lhs` and store result in `res`
- `mass_mul(mass_t* lhs, mass_t* rhs, mass_t* res)`: multiply `lhs` and `rhs` and store result in `res`
- `mass_div(mass_t* lhs, mass_t* rhs, mass_t* res)`: divide `lhs` by `rhs` and store result in `res`, returns false if `rhs` is 0
- `mass_addeq(mass_t* self, mass_t* rhs)`: add `rhs` to `self`
- `mass_subeq(mass_t* self, mass_t* rhs)`: subtract `rhs` from `self`
- `mass_muleq(mass_t* self, mass_t* rhs)`: multiply `self` by `rhs`
- `mass_diveq(mass_t* self, mass_t* rhs)`: divide `self` by `rhs`, returns false if `rhs` is 0
- `mass_eq(mass_t* lhs, mass_t* rhs)`: return true if `lhs` equals `rhs`
- `mass_neq(mass_t* lhs, mass_t* rhs)`: return true if `lhs` does not equal `rhs`
- `mass_lt(mass_t* lhs, mass_t* rhs)`: return true if `lhs` is less than `rhs`
- `mass_gt(mass_t* lhs, mass_t* rhs)`: return true if `lhs` is greater than `rhs`
- `mass_lteq(mass_t* lhs, mass_t* rhs)`: return true if `lhs` is less than or equal to `rhs`
- `mass_gteq(mass_t* lhs, mass_t* rhs)`: return true if `lhs` is greater than or equal to `rhs`
