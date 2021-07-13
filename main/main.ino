#include <Arduino_LSM9DS1.h>
#include <math.h>
#include "calcul.h"
#include "util.h"

float temps_init = 0;
float prev_temps_acc = 0, prev_temps_rot = 0, prev_temps_mag = 0;
float prev_rot_x = 0, prev_rot_y = 0, prev_rot_z = 0;
float prev_acc[3];
float vit[3] = {0.0, 0.0, 0.0};
float prev_R[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
int c = 0;

//convertir en rad/seconde   

void setup()
{
  //Initialization du moniteur de serie
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Started ");


  //Begin and Test IMU
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
  delay(3000);
  temps_init = millis() / 1000.0;
}

//-------------------------------------------------Calcul

//ortogonaliser puis normaliser
void corriger_matrice_rotation(float matrice_rotation[3][3])
{
  transpose_matrice(matrice_rotation);

  float norme_vect_1 = norme_vecteur(matrice_rotation[0]);
  for (int i = 0; i < 3; i++) matrice_rotation[0][i] /= norme_vect_1;                             // normalisation vecteur 1  
 
  float scalaire_1_2 = produit_scalaire(matrice_rotation[0], matrice_rotation[1]);
  for (int i = 0; i < 3; i++) matrice_rotation[1][i] -= matrice_rotation[0][i] * scalaire_1_2;    // orthogonalisation vecteur 2 
  float norme_vect_2 = norme_vecteur(matrice_rotation[1]);
  for (int i = 0; i < 3; i++)  matrice_rotation[1][i] /= norme_vect_2;                            // normalisation vecteur 2 
  
  float vectoriel_1_2[3];
  produit_vectoriel(matrice_rotation[0], matrice_rotation[1], vectoriel_1_2);
  for (int i = 0; i < 3; i++) matrice_rotation[2][i] = vectoriel_1_2[i];                          // orthogonalisation et normalisation vecteur 3
  
  transpose_matrice(matrice_rotation);
}

void loop()
{
  float vecteur_acceration_body[3]={0.0, 0.0, 0.0};
  float mag_x, mag_y, mag_z;
  float temps_acc, temps_rot, temps_mag;
  float omega[3][3],exp_omega[3][3],exp_omega3[3][3], R[3][3];


  preleve_donnees(&temps_acc, &temps_rot, &temps_mag, vecteur_acceration_body, &mag_x, &mag_y, &mag_z, omega, temps_init);
  corrige_donnees(vecteur_acceration_body);

  for (int i = 0; i < 3; i++){
    //Serial.print(vecteur_acceration_body[i]); Serial.println(",");
  }
 Serial.println();

  // Calcul et correction de la matrice de rotation
  exponentiel_matrice(omega, temps_rot - prev_temps_rot, exp_omega,exp_omega3);
  produit_matrice_matrice(prev_R, exp_omega, R);


  if (c<10){
    //print_matrice(R);
    //print_matrice(exp_omega);
    c++;
  }

  //Serial.println("-------------------------------------------");
  float vecteur_acceration_globale_no_corec[3];
  //transpose_matrice(R);
  //print_matrice(R);

  produit_matrice_vecteur(R, vecteur_acceration_body, vecteur_acceration_globale_no_corec);

  for (int i = 0; i < 3; i++){
    //Serial.print(vecteur_acceration_globale_no_corec[i]); Serial.println(",");
  }
  //Serial.println();
  
  //transpose_matrice(R);  
  //print_matrice(R);


  //corriger_matrice_rotation(R);
  //print_matrice(R);

  float vecteur_acceration_globale[3];
  produit_matrice_vecteur(R, vecteur_acceration_body, vecteur_acceration_globale);


  for (int i = 0; i < 3; i++){
    //Serial.print(vecteur_acceration_globale[i]); Serial.println(",");
  }
  //Serial.println();
  
  //vecteur_acceration_globale[2] = fabs(vecteur_acceration_globale[2]) - 9.81; 
  //vecteur_acceration_globale_no_corec[2] += 9.81;

  
  Serial.print(temps_acc);
  Serial.print(",");
  
  for (int i = 0; i < 3; i++)
  {
    Serial.print(vecteur_acceration_body[i]); Serial.print(",");
  }
    for (int i = 0; i < 3; i++)
  {
    Serial.print(vecteur_acceration_globale[i]); Serial.print(",");
  }
  for (int i = 0; i < 3; i++)
  {
    Serial.print(vecteur_acceration_globale_no_corec[i]); Serial.print(",");
  }
    for (int i = 0; i < 3; i++)
  {
    Serial.print(R[i][0]); Serial.print(",");
  }
  for (int i = 0; i < 3; i++)
  {
    Serial.print(R[i][1]); Serial.print(",");
  }
    for (int i = 0; i < 3; i++)
  {
    Serial.print(R[i][2]); Serial.print(",");
  }



  
  Serial.print(1.0);

  
  Serial.println();

  //integre( temps_rot, omega[2][1] )
  for (int i = 0; i < 3; i++){
    //vit[i] += integre(prev_temps_acc, temps_acc, prev_acc[i], vecteur_acceration_globale[i]);
    //Serial.print(vit[i]);
    //Serial.print(",");
  }

  
  //Serial.println();
  
  copie_matrice(R,prev_R);
  prev_temps_rot = temps_rot;
}
