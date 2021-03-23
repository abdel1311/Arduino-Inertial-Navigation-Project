#include <math.h>
#include "util.h"

//------------------------------------------------- INTEGRATION -------------------------------------------------

// Calcule l'integrale entre les temps a et b, methode des trapèzes
float integre(float a, float b, float fa, float fb)
{
  return ((fa + fb) * (b - a)) / 2; 
}


//------------------------------------------------- CALCULE TENSORIEL -------------------------------------------------

// Calcule le produit de deux matrices A et B : A*B
void produit_matrice_matrice(float matrice_A[3][3], float matrice_B[3][3], float matrice_solution[3][3])
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

// Calcule le produit d'une matrice A et d'un vecteur v : A*v
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
// Calcule l'exponentielle d'une matrice (développement de tailor à l'ordre 2) 
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
}

//------------------------------------------------- CALCULE VECTORIEL -------------------------------------------------

//Calcule la norme 2 d'un vecteur 
float norme_vecteur(float vecteur[3])
{
  return sqrt(pow(vecteur[0],2) + pow(vecteur[1],2) + pow(vecteur[2],2));
}

//Calcule le produit scalaire de 2 vecteurs
void produit_scalaire(float vecteurA[3], float vecteurB[3], float vecteur_sortie[3])
{
  for (int i = 0; i < 3; i++)
  {
    vecteur_sortie[i] = vecteurA[i] * vecteurB[i];
  }
}

//Calcule le produit vectoriel de 2 vecteurs
void produit_vectoriel(float vecteurA[3], float vecteurB[3], float vecteur_sortie[3])
{
  vecteur_sortie[0] = vecteurA[1] * vecteurB[2] - vecteurA[2] * vecteurB[1];
  vecteur_sortie[1] = vecteurA[2] * vecteurB[0] - vecteurA[0] * vecteurB[2];
  vecteur_sortie[2] = vecteurA[0] * vecteurB[1] - vecteurA[1] * vecteurB[0];
}


