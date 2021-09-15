#include "Mathster_I2C_Bitbang.h"
#define delayU delayMicroseconds

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

void Mathster_I2C_Bitbang::i2c_data_byte_in(uint8_t &data)
{
	bool bit;
	pinMode(SDA_PIN, INPUT);
	for (int i = 0; i < 8; i++)
	{
		
		delayU(I2C_DELAY);
		digitalWrite(SCL_PIN, HIGH);
		bit = digitalRead(SDA_PIN);
		data = (data << 1) | (uint8_t)bit;
		delayU(I2C_DELAY - CLOCK_SKIRT);
		digitalWrite(SCL_PIN, LOW);
	}
	pinMode(SDA_PIN, OUTPUT);
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

bool Mathster_I2C_Bitbang::write(uint8_t device_addr, uint8_t data)
{
	i2c_start();
	i2c_data_byte_out(device_addr << 1);

	if(!i2c_check_ack())
	{
		i2c_end();
		return false;
	}
	i2c_data_byte_out(data);

	if(!i2c_check_ack())
	{
		i2c_end();
		return false;
	}
	i2c_end();
}

bool Mathster_I2C_Bitbang::request_byte(uint8_t device_addr, uint8_t &data)
{
	delayU(I2C_DELAY);// just for spacing
	i2c_start();
	i2c_data_byte_out((device_addr<<1) | 0x01); //read mode
	
	if(!i2c_check_ack())
	{
		i2c_end();
		return false;
	}
	i2c_data_byte_in(data);
	
	digitalWrite(SDA_PIN, HIGH); //master pushes SDA high for NACK
	delayU(I2C_DELAY);
	digitalWrite(SCL_PIN, HIGH);
	delayU(I2C_DELAY);
	digitalWrite(SCL_PIN, LOW);
	delayU(CLOCK_SKIRT);
	digitalWrite(SDA_PIN, LOW);
	delayU(I2C_DELAY - CLOCK_SKIRT);
	
	i2c_end();
	return true;
}

//______________________________________________________//
//change according to the platform being used
void Mathster_I2C_Bitbang::set_pin_mode(int pin, int mode)
{
	pinMode(pin, mode);
}

void Mathster_I2C_Bitbang::set_pin_state(int pin, bool state)
{
	digitalWrite(pin, state);
}