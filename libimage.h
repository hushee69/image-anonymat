/* Paul Gaborit - Ecole des mines d'Albi-Carmaux - 1997-2016 */
#ifndef LIB_IMAGE_H
#define LIB_IMAGE_H

#include <stdio.h>
#include <stdlib.h>

/* Declaration du type 'couleur' en RGB */
typedef struct {
  int   rouge; // intensite entre 0 et 255
  int   vert; // intensite entre 0 et 255
  int   bleu; // intensite entre 0 et 255
} couleur;

/* Declaration de deux 'couleur' particulieres */
extern couleur noir;
extern couleur blanc;

/* Declaration du type 'image' permettant de stocker une image de
   taille quelconque  */
typedef struct {
  int      largeur; // largeur (nombre de colonnes)
  int      hauteur; // hauteur (nombre de lignes)
  couleur *tabPoints; // pointeur sur le tableau de tous les points
} image;

/*      Fonction d'allocation d'une image d'une taille donnee */
image nouvelle_image(int largeur, int hauteur);

/*      Fonction de destruction d'une image precedemment allouee */
void detruire_image(image im);

/*
  Systeme de coordonnees type informatique :

  0--- x -----> largeur
  |
  |
  y
  |
  |
  v
  hauteur
*/

/* La fonction "lire_couleur" retourne la couleur du point de
   coordonnées x,y de l'image "im" */
couleur lire_couleur(image im, int x, int y);

/* La fonction "change_couleur" donne la couleur "nouc" au point de
   coordonnees x,y de l'image "im" */
void change_couleur(image im, int x, int y, couleur nouv);

/* La fonction "lire_image" lit, dans le fichier de nom
   "nom_fichier", une image (stockee au format PPM) */
image lire_image(char * nom_fichier);

/* La fonction "ecrire_image" enregistre l'image "im" dans un fichier
   de nom "nom_fichier" (au format PPM) */
void ecrire_image(image im, char * nom_fichier);

#endif
