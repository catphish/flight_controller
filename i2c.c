#include <avr/io.h>
#include <stdint.h>

void i2c_init(void)
{
    //set SCL to 400kHz
    TWSR = 0x00;
    TWBR = 0x0C;
    //enable TWI
    TWCR = (1<<TWEN);
}

void i2c_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

void i2c_stop(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2c_write(uint8_t u8data)
{
    TWDR = u8data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t i2c_read_ack(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t i2c_read_nack(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}

uint8_t i2c_get_status(void)
{
    uint8_t status;
    //mask status
    status = TWSR & 0xF8;
    return status;
}

uint8_t i2c_read_byte(uint8_t device_address, uint8_t register_address) {
  i2c_start();
  i2c_write(device_address<<1);
  i2c_write(register_address);
  i2c_start();
  i2c_write((device_address<<1)|1);
  int r = i2c_read_nack();
  i2c_stop();
  return(r);
}

void i2c_read_bytes(uint8_t device_address, uint8_t register_address, char* buffer, uint8_t length) {
  int n;
  i2c_start();
  i2c_write(device_address<<1);
  i2c_write(register_address);
  i2c_start();
  i2c_write((device_address<<1)|1);
  for(n=0;n<length-1;n++) {
    buffer[n] = i2c_read_ack();
  }
  buffer[length-1] = i2c_read_nack();
  i2c_stop();
}

void i2c_write_bytes(uint8_t device_address, uint8_t register_address, uint8_t *data, uint8_t length) {
  i2c_start();
  i2c_write(device_address<<1);
  i2c_write(register_address);
  int n;
  for(n=0;n<length;n++) {
    i2c_write(data[n]);
  }
  i2c_stop();
}

void i2c_write_byte(uint8_t device_address, uint8_t register_address, uint8_t data) {
  i2c_write_bytes(device_address, register_address, &data, 1);
}

void i2c_set_bit(uint8_t device_address, uint8_t register_address, uint8_t bit, uint8_t value) {
  uint8_t byte;
  byte = i2c_read_byte(0x68, register_address);
  if(value) {
    byte |= (1<<bit);
  } else {
    byte &= (255-(1<<bit));
  }
  i2c_write_byte(0x68, register_address, byte);
}
