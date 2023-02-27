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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pico/stdio.h"
#include "../include/hx711_scale_adaptor.h"
#include "../include/scale.h"

int main(void) {

    stdio_init_all();

    const mass_unit_t unit = mass_g;
    const int32_t refUnit = 432;
    const int32_t offset = -367539;

    hx711_t hx;
    hx711_config_t hxcfg;
    hx711_scale_adaptor_t hxsa;

    scale_t sc;
    scale_options_t opt = SCALE_DEFAULT_OPTIONS;

    char buff[MASS_TO_STRING_BUFF_SIZE];

    hx711_get_default_config(&hxcfg);
    hxcfg.clock_pin = 14;
    hxcfg.data_pin = 15;

    hx711_init(&hx, &hxcfg);
    hx711_power_up(&hx, hx711_gain_128);
    hx711_wait_settle(hx711_rate_80);

    hx711_scale_adaptor_init(
        &hxsa,
        &hx);

    scale_init(
        &sc,
        hx711_scale_adaptor_get_base(&hxsa),
        unit,
        refUnit,
        offset);

    //6. spend 10 seconds obtaining as many samples as
    //possible to zero (aka. tare) the scale
    opt.strat = strategy_type_time;
    opt.timeout = 10000000;

    if(scale_zero(&sc, &opt)) {
        printf("Scale zeroed successfully\n");
    }
    else {
        printf("Scale failed to zero\n");
    }

    mass_t mass;
    mass_t max;
    mass_t min;

    //7. change to spending 250 milliseconds obtaining
    //as many samples as possible
    opt.timeout = 250000;

    mass_init(&max, mass_g, 0);
    mass_init(&min, mass_g, 0);

    for(;;) {

        memset(buff, 0, MASS_TO_STRING_BUFF_SIZE);

        //8. obtain a mass from the scale
        if(scale_weight(&sc, &mass, &opt)) {

            //9. check if the newly obtained mass
            //is less than the existing minimum mass
            if(mass_lt(&mass, &min)) {
                min = mass;
            }

            //10. check if the newly obtained mass
            //is greater than the existing maximum mass
            if(mass_gt(&mass, &max)) {
                max = mass;
            }

            //11. display the newly obtained mass...
            mass_to_string(&mass, buff);
            printf("%s", buff);

            //...the current minimum mass...
            mass_to_string(&min, buff);
            printf(" min: %s", buff);

            //...and the current maximum mass
            mass_to_string(&max, buff);
            printf(" max: %s\n", buff);

        }
        else {
            printf("Failed to read weight\n");
        }

    }

    return EXIT_SUCCESS;

}
