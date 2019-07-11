# PH Sensor library
	This library is made to manage the usage of a galvanic pH probe.

# How it works!
	The pH value is measured by reading the voltage across the ph sensor in millivolts, and temperature compensated.

	The `lib_ph_cmd` is used to recieve cmds from the *parser or any caller library* and return the required output, you can use calibrate cmds `ph_cal_mid, ph_cal_high, ph_cal_mid` to calibrate the sensor at runtime and use `ph_cal_clear` to restore the default calibration, also you can get the calibration status using `ph_cal_get` it should return `3` if all calibration points are set, lastly `ph_reset` resets all the device params back to default.

	Notice: `MAGIC_NUMBER_DEFAULT` is used for data integrity check.

# How to use!
	Copy lib_ph_config.template.h to lib_ph_config.h and implement the sensor_mv function.
	sensor_mv should return the voltage accross the probe in millitvolts.

License
----

GNU General Public License v3.0

***Copyright (C) 2019 Conative Labs***
