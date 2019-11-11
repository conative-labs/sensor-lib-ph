#include "unity.h"
#include "lib_ph.h"

#include "mock_lib_ph_config_test.h"

#define MAGIC_NUMBER_DEFAULT 0x63



void setUP(void)
{
}

void tearDown(void)
{
}


void test_lib_ph_init_withMagicNumberAlreadySet(void)
{
  lib_ph_params_t params;
  params.ph7_mv_shift = 13221; // some random number
  params.slope_alk = 13221;
  params.slope_acid = 13221;
  params.magic_number = MAGIC_NUMBER_DEFAULT; // mock the magic number 
  lib_ph_init(&params);
  TEST_ASSERT_EQUAL_FLOAT(13221, params.ph7_mv_shift); 
  TEST_ASSERT_EQUAL_FLOAT(13221, params.slope_alk);
  TEST_ASSERT_EQUAL_FLOAT(13221, params.slope_acid);
}

void test_lib_ph_init_withoutMagicNumberAlreadySet(void)
{
  lib_ph_params_t params;
  params.ph7_mv_shift = 13221; // some random number
  params.slope_alk = 13221;
  params.slope_acid = 13221;
  lib_ph_init(&params);
  TEST_ASSERT_EQUAL_FLOAT(0, params.ph7_mv_shift);
  TEST_ASSERT_EQUAL_FLOAT(0.1984, params.slope_alk);
  TEST_ASSERT_EQUAL_FLOAT(0.1984, params.slope_acid);
}

void test_lib_ph_cmd_ReadAlkValue(void)
{
  lib_ph_params_t params;
  lib_ph_reading_t reading;
  sensor_mv_ExpectAndReturn(-500);
  lib_ph_init(&params);

  params.slope_alk = 1;

  lib_ph_cmd(ph_read, &reading);

  TEST_ASSERT_EQUAL_FLOAT(8.67701, reading);
}

void test_lib_ph_cmd_ReadAcidValue(void)
{
  lib_ph_params_t params;
  lib_ph_reading_t reading;
  sensor_mv_ExpectAndReturn(500);
  lib_ph_init(&params);

  params.slope_acid = 1;

  lib_ph_cmd(ph_read, &reading);

  TEST_ASSERT_EQUAL_FLOAT(5.32299, reading);
}

void test_lib_ph_cmd_ReadValueLessThanZero(void)
{
  lib_ph_params_t params;
  lib_ph_reading_t reading;
  sensor_mv_ExpectAndReturn(415); // this will generate value < 0
  lib_ph_init(&params);

  lib_ph_cmd(ph_read, &reading);

  TEST_ASSERT_EQUAL_FLOAT(0, reading);
}

void test_lib_ph_cmd_ReadValueMoreThan14(void)
{
  lib_ph_params_t params;
  lib_ph_reading_t reading;
  sensor_mv_ExpectAndReturn(-415); // this will generate value > 14
  lib_ph_init(&params);

  lib_ph_cmd(ph_read, &reading);

  TEST_ASSERT_EQUAL_FLOAT(14, reading);
}

void test_lib_ph_cmd_ReadValueWithShift(void)
{
  lib_ph_params_t params;
  lib_ph_reading_t reading;
  sensor_mv_ExpectAndReturn(415);
  lib_ph_init(&params);

  params.ph7_mv_shift = 100;
  lib_ph_cmd(ph_read, &reading);

  TEST_ASSERT_EQUAL_FLOAT(1.67482, reading); // instead of zero effect of shift
}

void test_lib_ph_cmd_TemperatureSetAndGet(void)
{
  lib_ph_params_t params;
  lib_ph_temperature_t temp = 23.5, temp_val; 
  lib_ph_init(&params);

  lib_ph_cmd(ph_set_temperature, &temp);
  lib_ph_cmd(ph_get_temperature, &temp_val);

  TEST_ASSERT_EQUAL_FLOAT(23.5, temp_val);
}

void test_lib_ph_cmd_CalMid(void)
{
  lib_ph_params_t params;
  lib_ph_ph7_mv_shift_t shift = 90;
  sensor_mv_ExpectAndReturn(10);
  lib_ph_init(&params);

  lib_ph_cmd(ph_cal_mid, &shift);

  TEST_ASSERT_EQUAL_FLOAT(0.777778, params.ph7_mv_shift);
}


void test_lib_ph_cmd_CalLow(void)
{
  lib_ph_params_t params;
  lib_ph_slope_t slope = 2;
  sensor_mv_ExpectAndReturn(1000);
  lib_ph_init(&params);

  lib_ph_cmd(ph_cal_low, &slope);

  TEST_ASSERT_EQUAL_FLOAT(0.674195, params.slope_acid);
}


void test_lib_ph_cmd_CalHigh(void)
{
  lib_ph_params_t params;
  lib_ph_slope_t slope = 2;
  sensor_mv_ExpectAndReturn(1000);
  lib_ph_init(&params);

  lib_ph_cmd(ph_cal_high, &slope);

  TEST_ASSERT_EQUAL_FLOAT(0.674195, params.slope_alk);
}

void test_lib_cmd_CalGetNoneCaliberated(void)
{
  lib_ph_params_t params;
  uint8_t get;
  
  lib_ph_init(&params);
  lib_ph_cmd(ph_cal_get, &get);

  TEST_ASSERT_EQUAL(0, get);
}

void test_lib_cmd_CalGetShiftCaliberated(void)
{
  lib_ph_params_t params;
  uint8_t get ; 
  lib_ph_init(&params);

  params.ph7_mv_shift = 123; //random values
  lib_ph_cmd(ph_cal_get, &get);

  TEST_ASSERT_EQUAL(1, get);
}

void test_lib_cmd_CalGetAcidCaliberated(void)
{
  lib_ph_params_t params;
  uint8_t get;
  
  lib_ph_init(&params);


  params.slope_acid =123;
  lib_ph_cmd(ph_cal_get, &get);

  TEST_ASSERT_EQUAL(1, get);
}

void test_lib_cmd_CalGetAlkCaliberated(void)
{
  lib_ph_params_t params;
  uint8_t get;
  
  lib_ph_init(&params);


  params.slope_alk = 123;
  lib_ph_cmd(ph_cal_get, &get);

  TEST_ASSERT_EQUAL(1, get);
}


void test_lib_cmd_CalGetAllCaliberated(void)
{
  lib_ph_params_t params;
  uint8_t get;
  
  lib_ph_init(&params);

  params.ph7_mv_shift = 123; //random values
  params.slope_acid =123;
  params.slope_alk = 123;
  lib_ph_cmd(ph_cal_get, &get);

  TEST_ASSERT_EQUAL(3, get);
}

void test_lib_ph_init_CalClear(void)
{
  lib_ph_params_t params;

  lib_ph_init(&params);
  params.ph7_mv_shift = 13221; // some random number
  params.slope_alk = 13221;
  params.slope_acid = 13221;

  lib_ph_cmd(ph_cal_clear, NULL);
  TEST_ASSERT_EQUAL_FLOAT(0, params.ph7_mv_shift);
  TEST_ASSERT_EQUAL_FLOAT(0.1984, params.slope_alk);
  TEST_ASSERT_EQUAL_FLOAT(0.1984, params.slope_acid);
}


void test_lib_ph_init_Reset(void)
{
  lib_ph_params_t params;

  lib_ph_init(&params);
  params.ph7_mv_shift = 13221; // some random number
  params.slope_alk = 13221;
  params.slope_acid = 13221;
  params.magic_number = 0x53;
  lib_ph_cmd(ph_reset, NULL);
  TEST_ASSERT_EQUAL_FLOAT(0, params.ph7_mv_shift);
  TEST_ASSERT_EQUAL_FLOAT(0.1984, params.slope_alk);
  TEST_ASSERT_EQUAL_FLOAT(0.1984, params.slope_acid);
  TEST_ASSERT_EQUAL(MAGIC_NUMBER_DEFAULT, params.magic_number);
}
