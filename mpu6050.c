// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// MPU6050 driver utilizing DMP

#include "settings.h"
#include "i2c.h"
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "dmp_6axis.c"
#include <string.h>

char mpu5060_whoami() {
  return(i2c_read_byte(0x68, 0x75));
}

uint16_t mpu5060_fifo_count() {
//  return(i2c_read_byte(0x68, 0x3A));
  uint16_t count;
  count  = i2c_read_byte(0x68, 0x72) << 8;
  count |= i2c_read_byte(0x68, 0x73);
  return(count);
}

uint16_t mpu5060_status() {
  return(i2c_read_byte(0x68, 0x3A));
}

void mpu5060_reset_fifo() {
  i2c_set_bit(0x68, 0x6A, 2, 1);
}

void mpu6050_set_memory_bank(uint8_t bank) {
  bank &= 0x1F;
  i2c_write_byte(0x68, 0x6D, bank);
}
  
void mpu6050_set_memory_address(uint8_t addr) {
  i2c_write_byte(0x68, 0x6E, addr);
}

void mpu6050_write_prog_memory() {
  uint8_t chunk_size;
  int i;
  for (i=0; i < MPU6050_DMP_CODE_SIZE;) {
    mpu6050_set_memory_bank(i/256);
    mpu6050_set_memory_address(i%256);
    chunk_size = 16;
    if (i + chunk_size > MPU6050_DMP_CODE_SIZE) chunk_size = MPU6050_DMP_CODE_SIZE - i;
    i2c_write_bytes(0x68, 0x6F, (unsigned char *)dmp_prog+i, chunk_size);
    i+=chunk_size;
  }
}

void mpu6050_apply_updates(const unsigned char * update, uint8_t size) {
  int bank, addr, length, i;
  for(i=0; i<size;) {
    bank   = update[i];
    addr   = update[i+1];
    length = update[i+2];
    if(length) {
      mpu6050_set_memory_bank(bank);
      mpu6050_set_memory_address(addr);
      i2c_write_bytes(0x68, 0x6F, (unsigned char *)update+i+3, length);
    }
    if(length == 0) length=1;
    i += length+3;
  }
}

void mpu6050_set_clock_source() {
  // PLL_ZGYRO - 0x03
  uint8_t byte;
  byte = i2c_read_byte(0x68, 0x6B);
  byte = byte | 0x03;
  i2c_write_byte(0x68, 0x6B, byte);
}

void mpu6050_init() {
  // Reset
  i2c_set_bit(0x68, 0x6B, 7, 1);
  _delay_ms(30);
  
  // Disable sleep
  i2c_set_bit(0x68, 0x6B, 6, 0);
  
  // Disable I2C master
  i2c_write_byte(0x68, 0x25, 0x68);
  i2c_set_bit(0x68, 0x6A, 1, 1);
  
  // Write the DMP program
  mpu6050_write_prog_memory();
  
  // Apply config to DMP program
  mpu6050_apply_updates(dmp_config, MPU6050_DMP_CONFIG_SIZE);
  
  // I have no idea hat this does (RA_INT_ENABLE)
  i2c_write_byte(0x68, 0x38, 0x32);
  
  // Set Clock Source
  mpu6050_set_clock_source();
  
  // Setting DMP and FIFO_OFLOW interrupts enabled
  i2c_write_byte(0x68, 0x38, 0x12);
  
  // Setting sample rate to 200Hz
  i2c_write_byte(0x68, 0x19, 0x04);
  
  // Setting external frame sync and DLPF bandwidth
  i2c_write_byte(0x68, 0x19, (1<<3) | 3);
  
  // Set gyro range
  i2c_write_byte(0x68, 0x1B, 0x18);
  
  // Set DMP config bytes
  i2c_write_byte(0x68, 0x70, 0x03);
  i2c_write_byte(0x68, 0x71, 0x00);
  
  // Clearing OTP Bank flag
  i2c_set_bit(0x68, 0x00, 0, 0);
  
  // Apply updates to DMP program
  mpu6050_apply_updates(dmp_updates, MPU6050_DMP_UPDATES_SIZE);
  
  // Reset FIFO
  mpu5060_reset_fifo();
  
  // Enable FIFO
  i2c_set_bit(0x68, 0x6A, 6, 1);
  
  // Enable DMP
  i2c_set_bit(0x68, 0x6A, 7, 1);
  
  // Reset DMP
  i2c_set_bit(0x68, 0x6A, 3, 1);
  
}

void mpu6050_get_data(int16_t *y, int16_t *p, int16_t *r, int16_t *gy, int16_t *gp, int16_t *gr) {
  char buffer[42];
  int int_status, fifo_count;
  
  float q[4];
  float g[4];
  
  int_status = mpu5060_status();
  fifo_count = mpu5060_fifo_count();
  
  if ((int_status & 0x10) || fifo_count == 1024) {
    printf("FIFO buffer overflow!\n");
    mpu5060_reset_fifo();
  } else if (int_status & 0x02) {
    // Allow the buffer to fill if necessary
    while (fifo_count < 42) fifo_count = mpu5060_fifo_count();
    // Empty the buffer, we're ony interested in the last item
    while (fifo_count > 81) {
      i2c_read_bytes(0x68, 0x74, buffer, 42);
      fifo_count -= 42;
    }
    q[0] = (float)((buffer[0] << 8)  | (buffer[1] & 0xff))   / 16384.0f;
    q[1] = (float)((buffer[4] << 8)  | (buffer[5] & 0xff))   / 16384.0f;
    q[2] = (float)((buffer[8] << 8)  | (buffer[9] & 0xff))   / 16384.0f;
    q[3] = (float)((buffer[12] << 8) | (buffer[13]& 0xff))  / 16384.0f;
    
    g[0] = 2 * (q[1] * q[3] - q[0] * q[2]);
    g[1] = 2 * (q[0] * q[1] + q[2] * q[3]);
    g[2] = q[0] * q[0] - q[1] * q[1] - q[2]*q[2] + q[3]*q[3];
    
    *y = (atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1))*180;
    *p = (-atan(g[0] / sqrt(g[1] * g[1] + g[2] * g[2])))*180;
    *r = (atan(g[1] / sqrt(g[0] * g[0] + g[2] * g[2])))*180;
    
    char motion6[6];
    i2c_read_bytes(0x68, 0x43, motion6, 6);

    *gr = (motion6[0] << 8) | (motion6[1] & 0xff);
    *gp = (motion6[2] << 8) | (motion6[3] & 0xff);
    *gy = (motion6[4] << 8) | (motion6[5] & 0xff);
  }
}
