// calcule le produit de deux matrices a et b
float integre(float a, float b, float fa, float fb);

// calcule le produit de deux matrices a et b : a*b
void produit_matrice_matrice(float matrice_a[3][3], float matrice_b[3][3], float matrice_solution[3][3]);

// calcule le produit d'une matrice a et d'un vecteur v : a*v
void produit_matrice_vecteur(float matrice[3][3], float vecteur[3], float vecteur_solution[3]);

// calcule la norme 2 d'un vecteur 
float norme_vecteur(float vecteur[3]);

//calcule le produit scalaire de 2 vecteurs
float produit_scalaire(float vecteura[3], float vecteurb[3]);

//calcule le produit vectoriel de 2 vecteurs
void produit_vectoriel(float vecteura[3], float vecteurb[3], float vecteur_sortie[3]);

// calcule l'exponentielle d'une matrice (développement de tailor à l'ordre 2) 
void exponentiel_matrice(float matrice_A[3][3], float delta_t, float matrice_solution1[3][3], float matrice_solution2[3][3])
