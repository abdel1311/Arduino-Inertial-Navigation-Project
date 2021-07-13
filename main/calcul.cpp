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

  
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      matrice_solution[i][j] = 0.0;

      for (int k = 0; k < 3; k++)
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
    float somme = 0.;
    for (int j = 0; j < 3; j++)
    {
      somme += matrice[i][j] * vecteur[j];
    }
    vecteur_solution[i] = somme;
  }
}

// Calcule l'exponentielle d'une matrice (développement de tailor à l'ordre 2) 
void exponentiel_matrice(float matrice_A[3][3], float delta_t, float matrice_solution1[3][3], float matrice_solution2[3][3])
{
  float solution_temp[3][3];
  float I[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
  float M[3][3];
  float M2[3][3];
  float M3[3][3];



  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      M[i][j] = delta_t * matrice_A[i][j];
       
    }
  }

  produit_matrice_matrice( M, M, M2);

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      M2[i][j] /=2.;
    }
  }


  produit_matrice_matrice( M2, M, M3);

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      M2[i][j] /=6.;
    }
  }


  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      matrice_solution1[i][j] = I[i][j] + M[i][j] + M2[i][j];
    }
  }

  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      matrice_solution2[i][j] = I[i][j]+M[i][j]+M2[i][j] + M3[i][j];
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
float produit_scalaire(float vecteurA[3], float vecteurB[3])
{
  float res = 0;
  for (int i = 0; i < 3; i++)
  {
    res+= vecteurA[i] * vecteurB[i];
  }
  return res;
}

//Calcule le produit vectoriel de 2 vecteurs
void produit_vectoriel(float vecteurA[3], float vecteurB[3], float vecteur_sortie[3])
{
  vecteur_sortie[0] = vecteurA[1] * vecteurB[2] - vecteurA[2] * vecteurB[1];
  vecteur_sortie[1] = vecteurA[2] * vecteurB[0] - vecteurA[0] * vecteurB[2];
  vecteur_sortie[2] = vecteurA[0] * vecteurB[1] - vecteurA[1] * vecteurB[0];
}
