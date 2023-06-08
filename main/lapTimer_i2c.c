/*
 * lapTimer_i2c.c
 *
 *  Created on: 1 Jun 2023
 *      Author: mirek
 */

#include "lapTimer_i2c.h"

  uint8_t rxbuffer[3], txbuffer[1];
  VL53L0X_Dev_t sensor;

  void print_pal_error(VL53L0X_Error Status){
      char buf[VL53L0X_MAX_STRING_LENGTH];
      VL53L0X_GetPalErrorString(Status, buf);
      printf("API Status: %i : %s\n", Status, buf);
  }

  void print_range_status(VL53L0X_RangingMeasurementData_t* pRangingMeasurementData){
      char buf[VL53L0X_MAX_STRING_LENGTH];
      uint8_t RangeStatus;

      /*
       * New Range Status: data is valid when pRangingMeasurementData->RangeStatus = 0
       */

      RangeStatus = pRangingMeasurementData->RangeStatus;

      VL53L0X_GetRangeStatusString(RangeStatus, buf);
      printf("Range Status: %i : %s\n", RangeStatus, buf);

  }


void i2c_initialize(void){

  i2c_config_t i2c_config = {
      .mode = I2C_MODE_MASTER,
      .sda_io_num = GPIO_NUM_6,
      .scl_io_num = GPIO_NUM_7,
      .sda_pullup_en = GPIO_PULLUP_ENABLE,
      .scl_pullup_en = GPIO_PULLUP_ENABLE,
      .master.clk_speed = 400000,
      .clk_flags = 0,
  };

  ESP_ERROR_CHECK(i2c_param_config(I2C_NUM_0, &i2c_config));
  ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_MASTER, 0, 0, 0));

  sensor.comms_type = I2C;
  sensor.I2cDevAddr = 0x52;
  sensor.comms_speed_khz = 400;

  int8_t Status = VL53L0X_DataInit(&sensor);
  Status = VL53L0X_StaticInit(&sensor);
}

void i2c_read(void){
	  vTaskDelay(100/portTICK_PERIOD_MS);
}
