#include "Arduino.h"



class Mathster_I2C_Bitbang {
public:
	void init(int scl, int sda, int frequency);
	bool write(uint8_t device_addr, uint8_t data);
	bool request_byte(uint8_t device_addr, uint8_t &data);
	bool request_bytes(uint8_t device_addr, uint8_t *buffer, int num_bytes);

private:
	int SCL_PIN, SDA_PIN;
	float I2C_FREQ;
	float I2C_DELAY;
	int CLOCK_SKIRT = 1;//1 uS
	void i2c_start();
	void i2c_end();
	void i2c_data_byte_out(uint8_t data);
	void i2c_data_byte_in(uint8_t &data);
	bool i2c_check_ack();
	void set_pin_state(int pin, bool state);
	void set_pin_mode(int pin, int mode);
	bool read_pin_state(int pin);
};