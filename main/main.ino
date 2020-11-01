#include <Arduino_LSM9DS1.h>
int T_a;
int T_g;
int T_m;

void setup() {

  //Begin and Test Serial Port
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started ");

   //Begin and Test IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //Accelerometer configuration
  Serial.println("===================================== ");
  Serial.print("Accelerometer sample rate = ");
  T_a = IMU.accelerationSampleRate();
  Serial.print(T_a);
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  //Gyroscope configuration
  Serial.println("===================================== ");
  Serial.print("Gyroscope sample rate = ");
  T_g = IMU.gyroscopeSampleRate();
  Serial.print(T_g);
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("gX\tgY\tgZ");


  //Magnetometer configuration
  Serial.println("===================================== ");
  Serial.print("Magnetic field sample rate = ");
  T_m = IMU.magneticFieldSampleRate();
  Serial.print(T_m);
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Magnetic Field in uT");
  Serial.println("X\tY\tZ");


  Serial.println("===================================== ");
}



void loop() {
  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z);
  }

}
