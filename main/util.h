//------------------------ Fonctions d'affichage ------------------------

// Affiche sur le terminale une matrice 33 quelconque
void print_matrice(float Matrice[3][3]);

// Affiche toutes les valeurs prélevées pour le débogage
void print_raw(float ta, float tr, float tm, float ax, float ay, float az, float rx, float ry, float rz, float mx, float my, float mz);




//------------------------ Fonctions traitement de données ------------------------

//Actualize le temps d'un certain capteur
void actualize_temps(float *temps_capteur, float temps_init);

// Prends les valeurs en pointeur des capteur pour corriger les erreurs de biais et autres
void corrige_donnees(float vecteur_acceration_body[3]);

// Prends en paramètre la matrice rotation et la corrige
// void corriger_matrice_rotation(float matrice_rotation[3][3])

// Recupère les données 
void preleve_donnees(float *temps_acc, float *temps_rot, float *temps_mag, float vecteur_acceration_body[3],float *mag_x, float *mag_y, float *mag_z, float matrice_S[3][3],float temps_init);



//------------------------ Fonctions Misc ------------------------

// Prends en paramètre une matrice 33 et la transpose
void transpose_matrice(float Matrice[3][3]);

void copie_matrice(float A[3][3], float B[3][3]);
