// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Startup routine

void setup()
{
  Serial.begin(115200);
  //Serial.println("Beginning takeoff sequence...");
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(18, INPUT);
    pinMode(19, INPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);

  //Serial.println("Initializing Gyros and DMP...");
    Wire.begin();
    TWBR = 24;                       // 400kHz I2C clock
    mpu.initialize();                // Initialize MTP6050
    devStatus = mpu.dmpInitialize(); // Initislize DMP
    if (devStatus == 0) {
      //mpu.setXGyroOffset(88);
      //mpu.setYGyroOffset(-59);
      //mpu.setZGyroOffset(8);
      mpu.setDMPEnabled(true);
      mpuIntStatus = mpu.getIntStatus();
      dmpReady = true;
      packetSize = mpu.dmpGetFIFOPacketSize();
    }
  //Serial.println("Initializing barometer...");
    barometer.initialize();
    bmpGetPressure();
    initial_pressure = pressure;

  //Serial.println("Enabling controls...");
    attachInterrupt(0, readrc0, CHANGE);
    attachInterrupt(1, readrc1, CHANGE);
    attachInterrupt(5, readrc2, CHANGE);
    attachInterrupt(4, readrc3, CHANGE);

  //Serial.println("Starting Engines...");
    PORTB=0;    // Clear any motor outputs
    TCCR1A = 0; // Set up high resolution timer
    TCCR1B = 2; // Set up high resolution timer

  //Serial.println("Done.");
}

