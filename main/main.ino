#include <Arduino_LSM9DS1.h>
#include <math.h>

float temps_init;
float prev_temps_acc = 0,prev_temps_rot = 0, prev_temps_magn = 0;
float prev_rot_x = 0, prev_rot_y = 0, prev_rot_z = 0;
float prev_acc_x = 0, prev_acc_y = 0, prev_acc_z = 0;

float vit_ang_x = 0, vit_ang_y = 0, vit_ang_z = 0;
float vit_x = 0, vit_y = 0, vit_z = 0;

void setup() {
  //Initialization du moniteur de serie
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started ");

  temps_init = millis()/1000.0;

  //Begin and Test IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

//-------------------------------------------------Affichage

void print_raw(float ta, float tr, float tm, float ax, float ay, float az, float rx, float ry, float rz, float mx, float my, float mz)
{
  int n = 12;
  float donnees[12] = {ta, tr, tm, ax, ay, az, rx, ry, rz, mx, my, mz};
  for (int i = 0; i < n; i++){
    Serial.print(donnees[i]);
    Serial.print(',');
  }
  Serial.println();
}

void print_integre(float temps, float donnee_x, float donnee_y, float donnee_z,float x,float y, float z)
{
  int n = 7;
  float donnees[7]= {temps, donnee_x, donnee_y, donnee_z,x,y,z};
  for (int i = 0; i < n; i++){
    Serial.print(donnees[i]);
    Serial.print(',');
  }
  Serial.println();
}

//-------------------------------------------------Données
void actualize_temps(float *temps_capteur){
  *temps_capteur = millis()/1000.0 - temps_init;
}

void preleve_donnees(float* temps_acc, float* temps_rot, float* temps_magn, float* accel_x, float* accel_y, float* accel_z,float* rot_x,float* rot_y,float* rot_z, float* magn_x, float* magn_y, float* magn_z)
{
  if (IMU.accelerationAvailable()) IMU.readAcceleration(*accel_x, *accel_y, *accel_z);
  actualize_temps(temps_acc);
  delay(10);
  
  if(IMU.gyroscopeAvailable())  IMU.readGyroscope(*rot_x, *rot_y, *rot_z);
  actualize_temps(temps_rot);
  delay(10);
    
  if(IMU.magneticFieldAvailable()) IMU.readMagneticField(*magn_x, *magn_y, *magn_z);
  actualize_temps(temps_magn);
  delay(10);
}

void corrige_donnees(float* ax, float* ay, float* az)
{
  //correction acc
  *ax *= 10; *ay *= 10; *az *= 10;
}

//-------------------------------------------------Calcul

float integre(float a, float b, float fa, float fb) //calcule l'integrale entre les temps a et b, methode des trapèzes
{
  return ((fa+fb)*(b-a))/2;
}



void loop()
{
  
  float accel_x, accel_y, accel_z, rot_x, rot_y, rot_z, magn_x, magn_y, magn_z;
  float temps_acc, temps_rot, temps_magn;

  preleve_donnees(&temps_acc, &temps_rot, &temps_magn, &accel_x, &accel_y, &accel_z, &rot_x, &rot_y, &rot_z, &magn_x, &magn_y, &magn_z);
  corrige_donnees(&accel_x, &accel_y, &accel_z);

  //integration acceleration rotationnel 
  vit_ang_x += integre(prev_temps_rot, temps_rot, prev_rot_x, rot_x);
  vit_ang_y += integre(prev_temps_rot, temps_rot, prev_rot_y, rot_y);
  vit_ang_z += integre(prev_temps_rot, temps_rot, prev_rot_z, rot_z);

  //integration acceleration  
  vit_x += integre(prev_temps_acc, temps_acc, prev_acc_x, accel_x);
  vit_y += integre(prev_temps_acc, temps_acc, prev_acc_y, accel_y);
  vit_z += integre(prev_temps_acc, temps_acc, prev_acc_z, accel_z);


  prev_temps_acc = temps_acc;
  prev_temps_rot = temps_rot;

  prev_rot_x = rot_x;
  prev_rot_y = rot_y;
  prev_rot_z = rot_z;

  prev_acc_x = accel_x;
  prev_acc_y = accel_y;
  prev_acc_z = accel_z;


  print_integre(temps_rot, vit_ang_x, vit_ang_y,  vit_ang_z, rot_x, rot_y, rot_z);


}
