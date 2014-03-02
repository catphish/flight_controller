char mpu5060_whoami();
uint16_t mpu5060_status();
void mpu6050_reset();
void mpu6050_init();
uint16_t mpu5060_fifo_count();
void mpu5060_reset_fifo();
void mpu6050_get_data(int16_t *y, int16_t *p, int16_t *r, int16_t *gy, int16_t *gp, int16_t *gr);

