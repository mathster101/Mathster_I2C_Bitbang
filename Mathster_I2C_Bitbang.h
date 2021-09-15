#include "Arduino.h"



class Mathster_I2C_Bitbang {
public:
	int SCL_PIN, SDA_PIN;
	float I2C_FREQ;
	float I2C_DELAY;
	int CLOCK_SKIRT = 1;
	void init(int scl, int sda, int frequency);
	void i2c_start();
	void i2c_end();
	void i2c_data_byte_out(uint8_t data);
	void i2c_data_byte_in(uint8_t &data);
	bool i2c_check_ack();
	bool write(uint8_t device_addr, uint8_t data);
	bool request_byte(uint8_t device_addr, uint8_t &data);
};