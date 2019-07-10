/*
  Copyright (C) 2019 Conative Labs

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program. If not, see <https://www.gnu.org/licenses/>

*/

#ifndef lib_ph_h
#define lib_ph_h

#include <stdint.h>

typedef float lib_ph_ph7_mv_shift_t;
typedef float lib_ph_slope_t;
typedef float lib_ph_temperature_t;
typedef float lib_ph_reading_t;

typedef struct lib_ph_params_t {
	uint8_t magic_number;
	lib_ph_ph7_mv_shift_t ph7_mv_shift;
	lib_ph_slope_t slope_acid, slope_alk;
} lib_ph_params_t;

typedef enum lib_ph_cmd_t {
	ph_read,
	ph_get_temperature,
	ph_set_temperature,
	ph_cal_low,
	ph_cal_mid,
	ph_cal_high,
	ph_cal_get,
	ph_cal_clear,
	ph_reset,
} lib_ph_cmd_t;

void lib_ph_init(lib_ph_params_t *params_struct);
void lib_ph_cmd(lib_ph_cmd_t command, void *output_buffer);

#endif
