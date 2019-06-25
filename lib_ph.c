#include "lib_ph.h"
#include "lib_ph_config.h"

#define MAGIC_NUMBER_DEFAULT 0x63
#define SLOPE_DEFAULT 0.1984
#define SLOPE_DEFAULT_ACID SLOPE_DEFAULT
#define SLOPE_DEFAULT_ALK SLOPE_DEFAULT
#define PH7_DEFAULT 0

static lib_ph_temperature_t temperature = 25;
static lib_ph_params_t *params;

static void calibration_set_default(void) {
	params->ph7_mv_shift = PH7_DEFAULT;
	params->slope_acid = SLOPE_DEFAULT_ACID;
	params->slope_alk = SLOPE_DEFAULT_ALK;
}

static void params_set_default(void) {
	params->magic_number = MAGIC_NUMBER_DEFAULT;
	calibration_set_default();
}

void lib_ph_init(lib_ph_params_t *ext_params) {
	params = ext_params;
	if(params->magic_number != MAGIC_NUMBER_DEFAULT)
		params_set_default();
}

void lib_ph_cmd(lib_ph_cmd_t cmd, void *buffer) {
	switch(cmd) {
		case read: {
			lib_ph_reading_t *ph_val = buffer;
			*ph_val = sensor_mv() - params->ph7_mv_shift;
			*ph_val /= -(temperature + 273.15)*(*ph_val > 0 ? params->slope_acid : params->slope_alk);
			*ph_val += 7;

			if(*ph_val < 0)
				*ph_val = 0;
			else if(*ph_val > 14)
				*ph_val = 14;
		} break;
		case get_temperature:
			*(lib_ph_temperature_t*)buffer = temperature;
		break;
		case set_temperature:
			temperature = *(lib_ph_temperature_t*)buffer;
		break;
		case cal_mid:
			//reference our calibration point back to PH 7
			params->ph7_mv_shift = (7.0/(*(lib_ph_ph7_mv_shift_t*)buffer)) * sensor_mv();
			params->slope_acid = SLOPE_DEFAULT_ACID;
			params->slope_alk = SLOPE_DEFAULT_ALK;
		break;
		case cal_low:
			params->slope_acid = (sensor_mv() - params->ph7_mv_shift)/(7 - *(lib_ph_slope_t*)buffer);
			params->slope_acid /= (temperature + 273.15);
		break;
		case cal_high:
			params->slope_alk = (sensor_mv() - params->ph7_mv_shift)/(7 - *(lib_ph_slope_t*)buffer);
			params->slope_alk /= (temperature + 273.15);
		break;
		case cal_get:
			((uint8_t *)buffer)[0] = 0;
			if(PH7_DEFAULT != params->ph7_mv_shift)
				((uint8_t *)buffer)[0]++;
			if(SLOPE_DEFAULT_ACID != params->slope_acid)
				((uint8_t *)buffer)[0]++;
			if(SLOPE_DEFAULT_ALK != params->slope_alk)
				((uint8_t *)buffer)[0]++;
		break;
		case cal_clear:
			calibration_set_default();
		break;
		case reset:
			params_set_default();
		break;
	}
}
