/*
 * Copyright (c) Meta Platforms, Inc. and affiliates.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *	 http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zephyr.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <logging/log.h>
#include "cmsis_os.h"
#include "libutil.h"
#include "hal_gpio.h"
#include "plat_gpio.h"
#include "plat_isr.h"
#include "plat_class.h"
LOG_MODULE_REGISTER(plat_gpio);

#define gpio_name_to_num(x) #x,
char *opa_gpio_name[] = {
	OPA_name_gpioA OPA_name_gpioB OPA_name_gpioC OPA_name_gpioD OPA_name_gpioE OPA_name_gpioF
		OPA_name_gpioG OPA_name_gpioH OPA_name_gpioI OPA_name_gpioJ OPA_name_gpioK
			OPA_name_gpioL OPA_name_gpioM OPA_name_gpioN OPA_name_gpioO OPA_name_gpioP
				OPA_name_gpioQ OPA_name_gpioR OPA_name_gpioS OPA_name_gpioT
					OPA_name_gpioU
};

char *opb_gpio_name[] = {
	OPB_name_gpioA OPB_name_gpioB OPB_name_gpioC OPB_name_gpioD OPB_name_gpioE OPB_name_gpioF
		OPB_name_gpioG OPB_name_gpioH OPB_name_gpioI OPB_name_gpioJ OPB_name_gpioK
			OPB_name_gpioL OPB_name_gpioM OPB_name_gpioN OPB_name_gpioO OPB_name_gpioP
				OPB_name_gpioQ OPB_name_gpioR OPB_name_gpioS OPB_name_gpioT
					OPB_name_gpioU
};

#undef gpio_name_to_num

// clang-format off

/* chip,
 * number,
 * is_init,is_latch,
 * direction,
 * status,
 * property,
 * int_type,
 * int_cb,
*/

GPIO_CFG plat_gpio_a_cfg[] = {
	/** Group A: 00-07 **/
	{ CHIP_GPIO, 0, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 1, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 2, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 3, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 4, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 5, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 6, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 7, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group B: 08-15 **/
	{ CHIP_GPIO, 8, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 9, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 10, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 11, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 12, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 13, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 14, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 15, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group C: 16-23 **/
	{ CHIP_GPIO, 16, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 17, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 18, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 19, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 20, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 21, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 22, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 23, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group D: 24-31 **/
	{ CHIP_GPIO, 24, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 25, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 26, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 27, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 28, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 29, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 30, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 31, DISABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group E: 32-39 **/
	{ CHIP_GPIO, 32, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 33, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 34, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 35, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 36, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 37, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 38, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 39, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group F: 40-47 **/
	{ CHIP_GPIO, 40, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 41, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 42, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 43, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 44, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 45, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 46, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 47, DISABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group G: 48-55 **/
	{ CHIP_GPIO, 48, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 49, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 50, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 51, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 52, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 53, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 54, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 55, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group H: 56-63 **/
	{ CHIP_GPIO, 56, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 57, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 58, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 59, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 60, DISABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 61, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 62, DISABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 63, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },

	/** Group I: 64-71 **/
	{ CHIP_GPIO, 64, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 65, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 66, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 67, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 68, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 69, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 70, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 71, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },

	/** Group J: 72-79 **/
	{ CHIP_GPIO, 72, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 73, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 74, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 75, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 76, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 77, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 78, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 79, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group K: 80-87 **/
	{ CHIP_GPIO, 80, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 81, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 82, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 83, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 84, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 85, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 86, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 87, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group L: 88-95 **/
	{ CHIP_GPIO, 88, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 89, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 90, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 91, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 92, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 93, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 94, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 95, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },

	/** Group M: 96-103 **/
	{ CHIP_GPIO, 96, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 97, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 98, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 99, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 100, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 101, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 102, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 103, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },

	/** Group N: 104-111 **/
	{ CHIP_GPIO, 104, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 105, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 106, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 107, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 108, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 109, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 110, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 111, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group O: 112-119 **/
	{ CHIP_GPIO, 112, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 113, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 114, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 115, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 116, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 117, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 118, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 119, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group P: 120-127 **/
	{ CHIP_GPIO, 120, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 121, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 122, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 123, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 124, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 125, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 126, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 127, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group Q: 128-135 **/
	{ CHIP_GPIO, 128, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_EDGE_BOTH, ISR_FM_EXP_MAIN_PWR_EN },
	{ CHIP_GPIO, 129, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 130, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 131, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 132, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 133, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 134, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 135, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group R: 136-143 **/
	{ CHIP_GPIO, 136, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 137, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 138, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 139, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 140, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 141, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 142, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 143, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group S: 144-151 **/
	{ CHIP_GPIO, 144, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 145, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 146, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 147, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 148, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 149, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 150, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 151, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group T: 152-159 **/
	{ CHIP_GPIO, 152, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 153, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 154, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 155, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 156, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 157, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 158, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 159, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group U: 160-167 **/
	{ CHIP_GPIO, 160, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 161, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 162, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 163, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 164, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 165, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 166, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 167, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
};

GPIO_CFG plat_gpio_b_cfg[] = {
	/** Group A: 00-07 **/
	{ CHIP_GPIO, 0, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 1, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 2, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 3, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 4, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 5, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 6, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 7, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group B: 08-15 **/
	{ CHIP_GPIO, 8, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 9, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 10, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 11, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 12, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 13, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 14, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 15, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group C: 16-23 **/
	{ CHIP_GPIO, 16, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 17, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 18, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 19, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 20, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 21, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 22, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 23, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group D: 24-31 **/
	{ CHIP_GPIO, 24, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 25, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 26, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 27, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 28, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 29, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 30, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 31, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group E: 32-39 **/
	{ CHIP_GPIO, 32, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 33, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 34, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 35, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 36, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 37, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 38, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 39, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group F: 40-47 **/
	{ CHIP_GPIO, 40, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 41, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 42, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 43, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 44, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 45, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 46, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 47, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group G: 48-55 **/
	{ CHIP_GPIO, 48, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 49, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 50, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 51, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 52, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 53, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 54, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 55, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group H: 56-63 **/
	{ CHIP_GPIO, 56, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 57, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 58, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 59, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 60, DISABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 61, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 62, DISABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 63, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },

	/** Group I: 64-71 **/
	{ CHIP_GPIO, 64, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 65, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 66, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 67, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 68, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 69, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 70, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 71, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, GPIO_HIGH, GPIO_INT_DISABLE, NULL },

	/** Group J: 72-79 **/
	{ CHIP_GPIO, 72, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 73, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 74, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 75, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 76, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 77, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 78, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 79, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group K: 80-87 **/
	{ CHIP_GPIO, 80, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 81, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 82, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 83, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 84, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 85, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 86, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 87, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group L: 88-95 **/
	{ CHIP_GPIO, 88, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 89, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 90, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 91, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 92, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 93, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 94, ENABLE, ENABLE, GPIO_OUTPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 95, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },

	/** Group M: 96-103 **/
	{ CHIP_GPIO, 96, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 97, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 98, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 99, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 100, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 101, DISABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 102, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 103, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },

	/** Group N: 104-111 **/
	{ CHIP_GPIO, 104, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 105, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 106, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 107, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 108, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 109, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 110, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 111, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group O: 112-119 **/
	{ CHIP_GPIO, 112, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 113, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 114, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 115, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 116, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 117, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 118, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 119, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group P: 120-127 **/
	{ CHIP_GPIO, 120, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 121, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 122, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 123, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 124, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 125, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 126, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 127, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group Q: 128-135 **/
	{ CHIP_GPIO, 128, ENABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_EDGE_BOTH, ISR_FM_EXP_MAIN_PWR_EN },
	{ CHIP_GPIO, 129, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 130, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 131, ENABLE, DISABLE, GPIO_OUTPUT, GPIO_HIGH, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 132, ENABLE, DISABLE, GPIO_INPUT, GPIO_HIGH, OPEN_DRAIN, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 133, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 134, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 135, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group R: 136-143 **/
	{ CHIP_GPIO, 136, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 137, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 138, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 139, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 140, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 141, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 142, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 143, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group S: 144-151 **/
	{ CHIP_GPIO, 144, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 145, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 146, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 147, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 148, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 149, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 150, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 151, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group T: 152-159 **/
	{ CHIP_GPIO, 152, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 153, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 154, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 155, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 156, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 157, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 158, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 159, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },

	/** Group U: 160-167 **/
	{ CHIP_GPIO, 160, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 161, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 162, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 163, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 164, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 165, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 166, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
	{ CHIP_GPIO, 167, DISABLE, DISABLE, GPIO_INPUT, GPIO_LOW, PUSH_PULL, GPIO_INT_DISABLE, NULL },
};

// clang-format on

void init_card_position_gpio()
{
	// Need to init two GPIO before check expansion card position
	// Due to we doesn't intial GPIO table, using aspeed GPIO API to init GPIO
	const struct device *gpio_dev;
	gpio_dev = device_get_binding("GPIO0_M_P");

	gpio_pin_configure(gpio_dev, (BIC_EXP_ID % GPIO_GROUP_SIZE), GPIO_INPUT);
	gpio_pin_configure(gpio_dev, (BIC_BOARD_ID % GPIO_GROUP_SIZE), GPIO_INPUT);
}

bool pal_load_gpio_config(void)
{
	uint8_t card_position = get_card_position();
	switch (card_position) {
	case CARD_POSITION_1OU:
	case CARD_POSITION_3OU:
		memcpy(&gpio_cfg[0], &plat_gpio_a_cfg[0], sizeof(plat_gpio_a_cfg));
		memcpy(&gpio_name, &opa_gpio_name, sizeof(opa_gpio_name));
		break;
	case CARD_POSITION_2OU:
	case CARD_POSITION_4OU:
		memcpy(&gpio_cfg[0], &plat_gpio_b_cfg[0], sizeof(plat_gpio_b_cfg));
		memcpy(&gpio_name, &opb_gpio_name, sizeof(opb_gpio_name));
		break;
	default:
		LOG_ERR("Can't load GPIO");
		break;
	}

	return 1;
};
