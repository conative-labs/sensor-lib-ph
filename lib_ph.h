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
	read,
	get_temperature,
	set_temperature,
	cal_low,
	cal_mid,
	cal_high,
	cal_get,
	cal_clear,
	reset,
} lib_ph_cmd_t;

void lib_ph_init(lib_ph_params_t *params_struct);
void lib_ph_cmd(lib_ph_cmd_t command, void *output_buffer);

#endif
