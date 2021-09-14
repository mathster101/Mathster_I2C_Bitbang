#include "Arduino.h"



class Mathster_I2C_Bitbang {
public:
  int SCL_PIN, SDA_PIN;
  float I2C_FREQ;
  float I2C_DELAY;
  void init(int scl, int sda, int frequency);
  void i2c_start();
  void i2c_end();
  void i2c_data_byte_out();
  void i2c_data_byte_in();
};