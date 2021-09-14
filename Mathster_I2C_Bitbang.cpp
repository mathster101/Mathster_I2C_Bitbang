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
  void i2c_data_byte_out(uint8_t data);
  void i2c_data_byte_in(uint8_t &data);
  bool i2c_check_ack();
};
*/

void Mathster_I2C_Bitbang::init(int scl, int sda, int frequency)
{
	SCL_PIN = scl;
	SDA_PIN = sda;
	I2C_FREQ = frequency;
	I2C_DELAY = (1 / I2C_FREQ) * 1e6 * 0.5;
	pinMode(SCL_PIN, OUTPUT);
	delayU(20);
	pinMode(SDA_PIN, OUTPUT);
	digitalWrite(SCL_PIN, HIGH);
	digitalWrite(SDA_PIN, HIGH);
	delayU(100);
}

void Mathster_I2C_Bitbang::i2c_start()
{
	digitalWrite(SDA_PIN, LOW);
	delayU(I2C_DELAY);
	digitalWrite(SCL_PIN, LOW);
}

void Mathster_I2C_Bitbang::i2c_end()
{
	delayU(I2C_DELAY);
	digitalWrite(SCL_PIN, HIGH);
	digitalWrite(SDA_PIN, LOW);
	delayU(CLOCK_SKIRT*2);
	digitalWrite(SDA_PIN, HIGH);
}

void Mathster_I2C_Bitbang::i2c_data_byte_out(uint8_t data)
{
	bool bit;
	for (int i = 0; i < 8; i++)
	{
		
		delayU(I2C_DELAY);
		bit = (data << i) & 0b10000000;
		
		if(bit)
			digitalWrite(SDA_PIN, HIGH);
		else 
			digitalWrite(SDA_PIN, LOW);
		delayU(CLOCK_SKIRT);
		digitalWrite(SCL_PIN, HIGH);
		delayU(I2C_DELAY - CLOCK_SKIRT);
		digitalWrite(SCL_PIN, LOW);
	}
}

bool Mathster_I2C_Bitbang::i2c_check_ack()
{
	bool ack;
	pinMode(SDA_PIN, INPUT);
	delayU(I2C_DELAY);
	digitalWrite(SCL_PIN, HIGH);
	ack = digitalRead(SDA_PIN);
	delayU(I2C_DELAY - CLOCK_SKIRT);
	digitalWrite(SCL_PIN, LOW);
	pinMode(SDA_PIN, OUTPUT);
	return !ack; // low sda = high ack
}