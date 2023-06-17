#include "Mathster_I2C_Bitbang.h"
#define delayU delayMicroseconds

void Mathster_I2C_Bitbang::init(int scl, int sda, int frequency)
{
	SCL_PIN = scl;
	SDA_PIN = sda;
	I2C_FREQ = frequency;
	I2C_DELAY = (1 / I2C_FREQ) * 1e6 * 0.5;
	CLOCK_SKIRT = (int)(I2C_DELAY / 2);
	CLOCK_SKIRT < 2 ? CLOCK_SKIRT = 1 : true;
	set_pin_mode(SCL_PIN, OUTPUT);
	delayU(20);
	set_pin_mode(SDA_PIN, OUTPUT);
	set_pin_state(SCL_PIN, HIGH);
	set_pin_state(SDA_PIN, HIGH);
	delayU(100);
}

void Mathster_I2C_Bitbang::i2c_start()
{
	set_pin_state(SDA_PIN, LOW);
	delayU(I2C_DELAY);
	set_pin_state(SCL_PIN, LOW);
}

void Mathster_I2C_Bitbang::i2c_end()
{
	delayU(I2C_DELAY);
	set_pin_state(SCL_PIN, HIGH);
	set_pin_state(SDA_PIN, LOW);
	delayU(CLOCK_SKIRT*2);
	set_pin_state(SDA_PIN, HIGH);
}

void Mathster_I2C_Bitbang::i2c_data_byte_out(uint8_t data)
{
	bool bit;
	for (int i = 0; i < 8; i++)
	{
		delayU(I2C_DELAY);
		bit = (data << i) & 0b10000000;
		
		if(bit)
			set_pin_state(SDA_PIN, HIGH);
		else 
			set_pin_state(SDA_PIN, LOW);
		delayU(CLOCK_SKIRT);
		set_pin_state(SCL_PIN, HIGH);
		delayU(I2C_DELAY - CLOCK_SKIRT);
		set_pin_state(SCL_PIN, LOW);
	}
}

void Mathster_I2C_Bitbang::i2c_data_byte_in(uint8_t &data)
{
	bool bit;
	set_pin_mode(SDA_PIN, INPUT);
	for (int i = 0; i < 8; i++)
	{
		delayU(I2C_DELAY);
		set_pin_state(SCL_PIN, HIGH);
		bit = read_pin_state(SDA_PIN);
		data = (data << 1) | (uint8_t)bit;
		delayU(I2C_DELAY - CLOCK_SKIRT);
		set_pin_state(SCL_PIN, LOW);
	}
	set_pin_mode(SDA_PIN, OUTPUT);
}

bool Mathster_I2C_Bitbang::i2c_check_ack()
{
	bool ack;
	set_pin_mode(SDA_PIN, INPUT);
	delayU(I2C_DELAY);
	set_pin_state(SCL_PIN, HIGH);
	ack = read_pin_state(SDA_PIN);
	delayU(I2C_DELAY - CLOCK_SKIRT);
	set_pin_state(SCL_PIN, LOW);
	set_pin_mode(SDA_PIN, OUTPUT);
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
	
	set_pin_state(SDA_PIN, HIGH); //master pushes SDA high for NACK
	delayU(I2C_DELAY);
	set_pin_state(SCL_PIN, HIGH);
	delayU(I2C_DELAY);
	set_pin_state(SCL_PIN, LOW);
	delayU(CLOCK_SKIRT);
	set_pin_state(SDA_PIN, LOW);
	delayU(I2C_DELAY - CLOCK_SKIRT);
	
	i2c_end();
	return true;
}

bool Mathster_I2C_Bitbang::request_bytes(uint8_t device_addr, uint8_t *buffer, int num_bytes)
{
	delayU(I2C_DELAY);
	i2c_start();
	i2c_data_byte_out((device_addr << 1) | 0x01);
	
	if(!i2c_check_ack())
	{
		i2c_end();
		return false;
	}

	int data = 0;
	for (int i = 0; i < num_bytes; i++)
	{
		i2c_data_byte_in(buffer[i]);
		if(i != (num_bytes - 1))
		{
			set_pin_state(SDA_PIN, LOW); //master pushes SDA low for ACK
			delayU(CLOCK_SKIRT * 2);
			set_pin_state(SCL_PIN, HIGH);
			delayU(I2C_DELAY);
			set_pin_state(SCL_PIN, LOW);
			delayU(I2C_DELAY);
		}
		else
		{
			set_pin_state(SDA_PIN, HIGH); //master pushes SDA high for NACK
			delayU(I2C_DELAY);
			set_pin_state(SCL_PIN, HIGH);
			delayU(I2C_DELAY);
			set_pin_state(SCL_PIN, LOW);
			delayU(CLOCK_SKIRT);
			set_pin_state(SDA_PIN, LOW);
			delayU(I2C_DELAY - CLOCK_SKIRT);
		}
	}

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
bool Mathster_I2C_Bitbang::read_pin_state(int pin)
{
	return digitalRead(pin);
}