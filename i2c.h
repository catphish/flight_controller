#include <stdint.h>

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_write(uint8_t u8data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
uint8_t i2c_get_status(void);
uint8_t i2c_read_byte(uint8_t device_address, uint8_t register_address);
void i2c_write_bytes(uint8_t device_address, uint8_t register_address, uint8_t *data, uint8_t length);
void i2c_write_byte(uint8_t device_address, uint8_t register_address, uint8_t data);
void i2c_set_bit(uint8_t device_address, uint8_t register_address, uint8_t bit, uint8_t value);
void i2c_read_bytes(uint8_t device_address, uint8_t register_address, char* buffer, uint8_t length);
