#include <Arduino_LSM9DS1.h>
#include <math.h>

float temps_init;
float prev_temps_acc = 0, prev_temps_rot = 0, prev_temps_magn = 0;
float prev_rot_x = 0, prev_rot_y = 0, prev_rot_z = 0;
float prev_acc_x = 0, prev_acc_y = 0, prev_acc_z = 0;

float vit_ang_x = 0, vit_ang_y = 0, vit_ang_z = 0;
float vit_x = 0, vit_y = 0, vit_z = 0;

float prev_R[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

//convertir e nrad/seconde

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

//-------------------------------------------------Affichage

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

void print_integre(float temps, float donnee_x, float donnee_y, float donnee_z, float x, float y, float z)
{
  int n = 7;
  float donnees[7] = {temps, donnee_x, donnee_y, donnee_z, x, y, z};
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
//-------------------------------------------------Données
void actualize_temps(float *temps_capteur)
{
  *temps_capteur = millis() / 1000.0 - temps_init;
}

void preleve_donnees(float *temps_acc, float *temps_rot, float *temps_magn, float *accel_x, float *accel_y, float *accel_z, float *magn_x, float *magn_y, float *magn_z, float matrice_S[3][3])
{
  if (IMU.accelerationAvailable())
    IMU.readAcceleration(*accel_x, *accel_y, *accel_z);
  actualize_temps(temps_acc);
  delay(10);

  float rot_x, rot_y, rot_z;
  if (IMU.gyroscopeAvailable())
    IMU.readGyroscope(rot_x, rot_y, rot_z);
  actualize_temps(temps_rot);
  matrice_S[0][1] = -rot_z;
  matrice_S[0][2] = rot_y;
  matrice_S[1][2] = -rot_x;

  matrice_S[1][0] = rot_z;
  matrice_S[2][0] = -rot_y;
  matrice_S[2][1] = rot_x;

  for (int i = 0; i <= 2; i++)
    matrice_S[i][i] = 0;

  delay(10);

  if (IMU.magneticFieldAvailable())
    IMU.readMagneticField(*magn_x, *magn_y, *magn_z);
  actualize_temps(temps_magn);
  delay(10);
}

void corrige_donnees(float *ax, float *ay, float *az)
{
  //correction acc
  *ax *= 9.8065;
  *ay *= 9.8065;
  *az *= 9.8065;
}

//-------------------------------------------------Calcul

//calcule l'integrale entre les temps a et b, methode des trapèzes
float integre(float a, float b, float fa, float fb)
{
  return ((fa + fb) * (b - a)) / 2;
  //return (b-a) * fa + (b-a) * fabs(fb-fa) / 2.0;
}

//Calcule le produit de deux matrices A et B
void produit_matrice(float matrice_A[3][3], float matrice_B[3][3], float matrice_solution[3][3])
{
  for (int i = 0; i <= 2; i++)
  {
    for (int j = 0; j <= 2; j++)
    {
      for (int k = 0; k <= 2; k++)
      {
        matrice_solution[i][j] += matrice_A[i][k] * matrice_B[k][j];
      }
    }
  }
}

void produit_matrice_vecteur(float matrice[3][3], float vecteur[3], float vecteur_solution[3])
{
  for (int i = 0; i < 3; i++)
  {
    float somme = 0;
    for (int j = 0; j < 3; j++)
    {
      somme += matrice[i][j] * vecteur[j];
    }
    vecteur_solution[i] = somme;
  }
}

void exponentiel_matrice(float matrice_A[3][3], float delta_t, float matrice_solution[3][3])
{
  float solution_temp[3][3];

  for (int i = 0; i <= 2; i++)
  {
    for (int j = 0; j <= 2; j++)
    {
      solution_temp[i][j] = delta_t * matrice_A[i][j];

      if (i == j)
        solution_temp[i][j] += 1;

      for (int k = 0; k <= 2; k++)
      {
        solution_temp[i][j] += (delta_t * matrice_A[i][k] * delta_t * matrice_A[k][j]) / 2;
      }

      matrice_solution[i][j] = solution_temp[i][j];
    }
  }
  // Serial.println(delta_t);
}

float norme_vecteur(float vecteur[3])
{
  double a = pow(vecteur[0], 2);
  double b = pow(vecteur[1], 2);
  double c = pow(vecteur[2], 2);
  return sqrt(a + b + c);
}

void transpose(float matrice[3][3])
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

void produit_scalaire(float vecteurA[3], float vecteurB[3], float vecteur_sortie[3])
{
  for (int i = 0; i < 3; i++)
  {
    vecteur_sortie[i] = vecteurA[i] * vecteurB[i];
  }
}

void produit_vectoriel(float vecteurA[3], float vecteurB[3], float vecteur_sortie[3])
{
  vecteur_sortie[0] = vecteurA[1] * vecteurB[2] - vecteurA[2] * vecteurB[1];
  vecteur_sortie[1] = vecteurA[2] * vecteurB[0] - vecteurA[0] * vecteurB[2];
  vecteur_sortie[2] = vecteurA[0] * vecteurB[1] - vecteurA[1] * vecteurB[0];
}

//ortogonaliser puis normaliser

void corriger_matrice_rotation(float matrice_rotation[3][3])
{
  transpose(matrice_rotation);

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

  transpose(matrice_rotation);
}

void loop()
{
  float accel_x, accel_y, accel_z, rot_x, rot_y, rot_z, magn_x, magn_y, magn_z;
  float temps_acc, temps_rot, temps_magn;
  float omega[3][3];
  float exp_omega[3][3];
  float R[3][3] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

  preleve_donnees(&temps_acc, &temps_rot, &temps_magn, &accel_x, &accel_y, &accel_z, &magn_x, &magn_y, &magn_z, omega);
  corrige_donnees(&accel_x, &accel_y, &accel_z);

  // Calcul matrice de rotation
  exponentiel_matrice(omega, temps_rot - prev_temps_rot, exp_omega);
  //print_matrice(omega);
  //print_matrice(exp_omega);

  produit_matrice(prev_R, exp_omega, R);

  Serial.println("Matrice rotation");
  print_matrice(R);

  Serial.println("Matrice corrigée");
  corriger_matrice_rotation(R);

  print_matrice(R);
  //print_matrice(exp_omega);
  //Serial.println('1');

  float vecteur_acceleration_body[3] = {accel_x, accel_y, accel_z};
  Serial.println(accel_z);
  float vecteur_acceleration_globale[3];

  produit_matrice_vecteur(R, vecteur_acceleration_body, vecteur_acceleration_globale);
  vecteur_acceleration_globale[2] -= 9.81;

  Serial.println("Vecteur accéleration global: ");
  for (int i = 0; i <= 2; i++)
  {
    Serial.println(vecteur_acceleration_globale[i]);
  }

  prev_temps_rot = temps_rot;
}
