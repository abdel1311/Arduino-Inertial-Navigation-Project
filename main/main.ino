#include <Arduino_LSM9DS1.h>
#include <math.h>
#include "calcul.h"
#include "util.h"

float temps_init;
float prev_temps_acc = 0, prev_temps_rot = 0, prev_temps_mag = 0;
float prev_acc_x = 0, prev_acc_y = 0, prev_acc_z = 0;

float vit_ang_x = 0, vit_ang_y = 0, vit_ang_z = 0;
float vit_x = 0, vit_y = 0, vit_z = 0;

float prev_R[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

//convertir e nrad/seconde




vecteur r = {1.1, 2.2, 3.3};




void setup()
{
  //Initialization du moniteur de serie
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("Started ");

  temps_init = millis() / 1000.0;

  //Begin and Test IMU
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
}

void corriger_matrice_rotation(float matrice_rotation[3][3])
{
  transpose_matrice(matrice_rotation);

  //calcul des normes des vecteurs
  float norme_vect_1 = norme_vecteur(matrice_rotation[0]);
  float norme_vect_2 = norme_vecteur(matrice_rotation[1]);

  Serial.print("norme vecteur 1 : ");
  Serial.println(norme_vect_1);

  Serial.print("norme vecteur 2 : ");
  Serial.println(norme_vect_2);

  //orthogonalization des vecteurs 1 et 2
  float unit_vect_1[3] = {matrice_rotation[0][0] / norme_vect_1, matrice_rotation[0][1] / norme_vect_1, matrice_rotation[0][2] / norme_vect_1};
  float vecteur2_[3] = {matrice_rotation[1][0], matrice_rotation[1][1], matrice_rotation[1][2]};

  Serial.print("norme unit vect 1 : ");
  Serial.println(norme_vecteur(unit_vect_1));
  //Serial.println(norme_vecteur(unit_vect_2));

  Serial.println("--------");
  Serial.println(matrice_rotation[0][0]);
  Serial.println("--------");

  /*   for(int i = 0; i < 3; i++){
    Serial.println(unit_vect_1[i]);
    Serial.println(unit_vect_2[i]);
  } */

  /*
  //re-remplissage de la matrice_rotation de départ
  float matrice_rotation_sortie[3][3] = {{unit_vect_1[0], unit_vect_1[1], unit_vect_1[2]},
                                         {unit_vect_2[0] - unit_vect_1[0], unit_vect_2[1] - unit_vect_1[1], unit_vect_2[2] - unit_vect_1[2]},
                                         {unit_vect_2[0] * unit_vect_1[0], unit_vect_2[1] * unit_vect_1[1], unit_vect_2[2] * unit_vect_1[2]} };
  */

  float scalaire_1_2[3];
  produit_scalaire(unit_vect_1, vecteur2_, scalaire_1_2);

  matrice_rotation[0][0] = unit_vect_1[0];
  matrice_rotation[0][1] = unit_vect_1[1];
  matrice_rotation[0][2] = unit_vect_1[2];

  matrice_rotation[1][0] = vecteur2_[0] - unit_vect_1[0] * scalaire_1_2[0];
  matrice_rotation[1][1] = vecteur2_[1] - unit_vect_1[1] * scalaire_1_2[1];
  matrice_rotation[1][2] = vecteur2_[2] - unit_vect_1[2] * scalaire_1_2[2];

  for (int i = 0; i < 2; i++)
  {
    matrice_rotation[1][i] /= norme_vecteur(vecteur2_);
  }

  float vectoriel_1_2[3];
  produit_vectoriel(matrice_rotation[0], matrice_rotation[1], vectoriel_1_2);

  matrice_rotation[2][0] = vectoriel_1_2[0];
  matrice_rotation[2][1] = vectoriel_1_2[1];
  matrice_rotation[2][2] = vectoriel_1_2[2];

  /*   float matrice_rotation_sortie[3][3] = {{unit_vect_1[0], unit_vect_1[1], unit_vect_1[2]},
                                         {unit_vect_2[0] - unit_vect_1[0] * scalaire_1_2[0],
                                          unit_vect_2[1] - unit_vect_1[1] * scalaire_1_2[1],
                                          unit_vect_2[2] - unit_vect_1[2] * scalaire_1_2[2]},
                                         {vectoriel_1_2[0], vectoriel_1_2[1], vectoriel_1_2[2]}}; */

  transpose_matrice(matrice_rotation);
}



void loop()
{
  float acc_x, acc_y, acc_z, mag_x, mag_y, mag_z;
  float temps_acc, temps_rot, temps_mag;
  float omega[3][3];
  float exp_omega[3][3];
  float R[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

  preleve_donnees(&temps_acc, &temps_rot, &temps_mag, &acc_x, &acc_y, &acc_z, &mag_x, &mag_y, &mag_z, omega, temps_init);
  corrige_donnees(&acc_x, &acc_y, &acc_z);

  // Calcul matrice de rotation
  exponentiel_matrice(omega, temps_rot - prev_temps_rot, exp_omega);
  //print_matrice(omega);
  //print_matrice(exp_omega);

  produit_matrice_matrice(prev_R, exp_omega, R);

  Serial.println("Matrice rotation");
  print_matrice(R);

  Serial.println("Matrice corrigée");
  corriger_matrice_rotation(R);

  print_matrice(R);
  //print_matrice(exp_omega);
  //Serial.println('1');

  float vecteur_acceration_body[3] = {acc_x, acc_y, acc_z};
  Serial.println(acc_z);
  float vecteur_acceration_globale[3];

  produit_matrice_vecteur(R, vecteur_acceration_body, vecteur_acceration_globale);
  vecteur_acceration_globale[2] -= 9.81;

  Serial.println("Vecteur accéleration global: ");
  for (int i = 0; i <= 2; i++)
  {
    Serial.println(vecteur_acceration_globale[i]);
  }

  prev_temps_rot = temps_rot;
}
