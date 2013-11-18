// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Startup routine

void setup()
{
  Serial.begin(115200);
  Serial.println("Beginning takeoff sequence...");
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);
  Serial.println("Initializing accelerometers...");
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif
  //Serial.println(F("Initializing I2C devices..."));
  mpu.initialize();

  // verify connection
  //Serial.println(F("Testing device connections..."));
  Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));  
  
  //Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();
  
  // make sure it worked (returns 0 if so)
  if (devStatus == 0) {
    mpu.setXGyroOffset(88);
    mpu.setYGyroOffset(-59);
    mpu.setZGyroOffset(8);
    //mpu.setXAccelOffset(365);
    //mpu.setYAccelOffset(1575);
    //mpu.setZAccelOffset(1788);
    // turn on the DMP, now that it's ready
    //Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);

    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready!"));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    // ERROR!
    // 1 = initial memory load failed
    // 2 = DMP configuration updates failed
    // (if it's going to break, usually the code will be 1)
    Serial.print(F("DMP Initialization failed (code "));
    Serial.print(devStatus);
    Serial.println(F(")"));
  }
  pid_pitch.SetOutputLimits(-200, 200);
  pid_roll.SetOutputLimits (-200, 200);
  pid_pitch.SetMode(AUTOMATIC);
  pid_roll.SetMode (AUTOMATIC);
  
  Serial.println("Calibrating barometer...");
  barometer.initialize();
  Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");
  bmpGetPressure();
  initial_pressure = pressure;
  Serial.println("Calibrating Controller...");
  attachInterrupt(0, readrc0, CHANGE);
  attachInterrupt(1, readrc1, CHANGE);
  attachInterrupt(5, readrc2, CHANGE);
  attachInterrupt(4, readrc3, CHANGE);
  delay(1000);
  Serial.println("Starting Engines...");
  front_left.attach(8);
  front_right.attach(9);
  rear_left.attach(10);
  rear_right.attach(11);
  
  for(n=0;n<20;n++) {
    front_left.writeMicroseconds(1000);
    front_right.writeMicroseconds(1000);
    rear_left.writeMicroseconds(1000);
    rear_right.writeMicroseconds(1000);
    delay(100);
  }
  Serial.println("Done.");
}
