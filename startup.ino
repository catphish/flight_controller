// Experimental Quadcopter flight controller
// Copyright Charlie Smurthwaite <charlie@atechmedia.com> 2013-11-17
//
// Startup routine

void setup()
{
  Serial.begin(115200);
  //Serial.println("Beginning takeoff sequence...");
    pinMode(A8, INPUT);
    pinMode(A9, INPUT);
    pinMode(A10, INPUT);
    pinMode(A11, INPUT);
    pinMode(A12, INPUT);
    pinMode(A13, INPUT);
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
    cli();		// switch interrupts off while messing with their settings  
    PCMSK2 |= (1<<PCINT16)|(1<<PCINT17)|(1<<PCINT18)|(1<<PCINT19)|(1<<PCINT20)|(1<<PCINT21);
    PCICR  |= (1<<PCIE2);
  
  //Serial.println("Starting Engines...");
    PORTB=0;    // Clear any motor outputs
    TCCR1A = 0; // Set up high resolution timer
    TCCR1B = 2; // Set up high resolution timer
    sei();	// turn interrupts back on

  //Serial.println("Done.");
}

