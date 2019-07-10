# PH Sensor library

This library is made to manage the usage of analog output PH sensor.

# How it works!
 The PH value is measure by reading the voltage across the ph sensor with mv, and then calculated with temperature as parameter to compute the PH value, as you can see here:
 `*ph_val /= -(temperature + 273.15)*(*ph_val > 0 ? params->slope_acid : params->slope_alk);`
 The `lib_ph_cmd` is used to recieve cmds from the *parser or any caller library* and return the required output, you can use calibrate cmds `ph_cal_mid, ph_cal_high, ph_cal_mid` to calibrate the sensor in runtime and use `ph_cal_clear` to clear back to default calibration, also you can make sure that all is calibrated using `ph_cal_get` it should return `3` if all is calibrated to new values other than default, lastly `ph_reset` resets all the device params back to default.
 
 Notice: `MAGIC_NUMBER_DEFAULT` is used for data integrity check, in case of saving the data in any storage that data corruption can occur.

# How to use!

  - Implement the sensor_mv function in the template, and rename the file to lib_ph_config.h
  -- sensor_mv should return the instantous reading in mvoltage from the sensor
  - Implement the Parser that interface with the cmds enum, head to [here](https://github.com/conative-labs/Interfaces-Parser-Template) to know how

License
----

GNU General Public License v3.0

***Copyright (C) 2019 Conative Labs***