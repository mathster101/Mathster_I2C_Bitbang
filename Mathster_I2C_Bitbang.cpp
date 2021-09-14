#include "Mathster_I2C_Bitbang.h"
#define delayU delayMicroseconds
/*
class Mathster_I2C_Bitbang {
public:
  int SCL_PIN, SDA_PIN;
  float I2C_FREQ;
  float I2C_DELAY;
  void init(int scl, int sda, int frequency)
  void i2c_start();
  void i2c_end();
  void i2c_data_byte_out();
  void i2c_data_byte_in();
};
*/

void Mathster_I2C_Bitbang::init(int scl, int sda, int frequency)
{
	SCL_PIN = scl;
	SDA_PIN = sda;
	I2C_FREQ = frequency;
	I2C_DELAY = (1 / I2C_FREQ) * 1e6 * 0.5;
	pinMode(SCL_PIN, OUTPUT);
	pinMode(SDA_PIN, OUTPUT);
	digitalWrite(SCL_PIN, HIGH);
	digitalWrite(SDA_PIN, HIGH);
	delayU(100);
}