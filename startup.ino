// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Startup routine

void setup()
{
  Serial.begin(115200);
  Serial3.begin(9600);
  // Initializing controller ports
  pinMode(A8, INPUT);
  pinMode(A9, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);
  pinMode(A12, INPUT);
  pinMode(A13, INPUT);
  pinMode(A14, INPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  // Initializing Gyros and DMP
  Wire.begin();
  TWBR = 24;                       // 400kHz I2C clock
  devStatus = mpu.dmpInitialize(); // Initislize DMP

  mpu.setXGyroOffset(50);
  mpu.setYGyroOffset(-55);
  mpu.setZGyroOffset(6);
  mpu.setZAccelOffset(888);

  if (devStatus == 0) {
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  }

  msSetup();
  mag.initialize();

  // Enabling controls
  cli();		// switch interrupts off while messing with their settings  
  PCMSK2 |= (1<<PCINT16)|(1<<PCINT17)|(1<<PCINT18)|(1<<PCINT19)|(1<<PCINT20)|(1<<PCINT21)|(1<<PCINT22);
  PCICR  |= (1<<PCIE2);

  // Starting Engines
  PORTB=0;    // Clear any motor outputs
  TCCR1A = 0; // Set up high resolution timer
  TCCR1B = 2; // Set up high resolution timer
  sei();      // turn interrupts back on
}

