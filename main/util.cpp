#include <Arduino_LSM9DS1.h>
#include <math.h>
#include "calcul.h"


//-------------------------------------------------Affichage-------------------------------------------------

void print_raw(float ta, float tr, float tm, float ax, float ay, float az, float rx, float ry, float rz, float mx, float my, float mz)
{
  int n = 12;
  float donnees[12] = {ta, tr, tm, ax, ay, az, rx, ry, rz, mx, my, mz};
  for (int i = 0; i < n; i++)
  {
    Serial.print(donnees[i]);
    Serial.print(',');
  }
  Serial.println();
}

void print_matrice(float Matrice[3][3])
{

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      Serial.print(Matrice[i][j]);
      Serial.print('\t');
    }
    Serial.println();
  }
  Serial.println("\n\n");
}



//------------------------------------------------- Traitement données -------------------------------------------------
void actualize_temps(float *temps_capteur, float temps_init)
{
  *temps_capteur = millis() / 1000.0 - temps_init;
}


void corrige_donnees(float *ax, float *ay, float *az)
{
  //correction acc
  *ax *= 9.8065;
  *ay *= 9.8065;
  *az *= 9.8065;
}

// Recupère les données 
void preleve_donnees(float *temps_acc, float *temps_rot, float *temps_mag, float *acc_x, float *acc_y, float *acc_z,
 float *mag_x, float *mag_y, float *mag_z, float matrice_S[3][3],float temps_init)
{
  if (IMU.accelerationAvailable()) IMU.readAcceleration(*acc_x, *acc_y, *acc_z);
  actualize_temps(temps_acc, temps_init);
  delay(10);

  float rot_x, rot_y, rot_z;
  if (IMU.gyroscopeAvailable()) IMU.readGyroscope(rot_x, rot_y, rot_z);
  actualize_temps(temps_rot, temps_init);
  matrice_S[0][1] = -rot_z;
  matrice_S[0][2] = rot_y;
  matrice_S[1][2] = -rot_x;
  matrice_S[1][0] = rot_z;
  matrice_S[2][0] = -rot_y;
  matrice_S[2][1] = rot_x;
  for (int i = 0; i <= 2; i++) matrice_S[i][i] = 0;
  delay(10);

  if (IMU.magneticFieldAvailable()) IMU.readMagneticField(*mag_x, *mag_y, *mag_z);
  actualize_temps(temps_mag, temps_init);
  delay(10);
}


//------------------------------------------------- Misc -------------------------------------------------

void transpose_matrice(float matrice[3][3])
{

  float matrice_sortie[3][3];

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      matrice_sortie[j][i] = matrice[i][j];
    }
  }

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      matrice[i][j] = matrice_sortie[i][j];
    }
  }
}


//ortogonaliser puis normaliser

