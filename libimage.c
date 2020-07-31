/* Paul Gaborit - Ecole des mines d'Albi-Carmaux - 1997-2016 */
#include "libimage.h"
#include <string.h>
#include <math.h>

/* Definition de deux 'couleur' de couleurs particulieres */
couleur noir = { 0, 0, 0};
couleur blanc = { 255, 255, 255};

/* Fonction d'allocation d'une image d'une taille donnee */
image nouvelle_image(int largeur, int hauteur)
{
  image im;

  im.largeur = largeur;
  im.hauteur = hauteur;
  im.tabPoints = (couleur *) malloc( sizeof(couleur) * largeur * hauteur);
  if (im.tabPoints == NULL) {
    fprintf(stderr,
            "Pas assez de memoire ! (image: %d x %d)\n",
            largeur, hauteur);
    exit(1);
  }
  return im;
}

/* Fonction de destruction d'une image precedemment allouee */
void detruire_image(image im)
{
  free(im.tabPoints);
}

/* Fonction "lire_couleur" retourne la couleur du point de
   coordonnées x, y de l'image im */
couleur lire_couleur(image im, int x, int y)
{
  if (   (x >= 0)
         && (x < im.largeur)
         && (y >= 0)
         && (y < im.hauteur) ) {
    return im.tabPoints[ x + (y * im.largeur) ];
  } else {
    fprintf(stderr,
            "lire_couleur: point (%d, %d) hors image (%d, %d)!\n",
            x, y,
            im.largeur, im.hauteur);
    exit(1);
  }
}

/* La fonction "change_couleur" donne la couleur "nouv" au point de
   coordonnees x, y de l'image "im" */
void change_couleur(image im, int x, int y, couleur nouv)
{
  if ((x >= 0)
      && (x < im.largeur)
      && (y >= 0)
      && (y < im.hauteur))
    {
      if (nouv.rouge < 0) {
        nouv.rouge = 0;
        fprintf(stderr, "libimage(change_couleur): rouge < 0 !\n");
      } else if (nouv.rouge > 255) {
        nouv.rouge = 255;
        fprintf(stderr, "libimage(change_couleur): rouge > 255 !\n");
      }
      if (nouv.vert < 0) {
        nouv.vert = 0;
        fprintf(stderr, "libimage(change_couleur): vert < 0 !\n");
      } else if (nouv.vert > 255) {
        nouv.vert = 255;
        fprintf(stderr, "libimage(change_couleur): vert > 255 !\n");
      }
      if (nouv.bleu < 0) {
        nouv.bleu = 0;
        fprintf(stderr, "libimage(change_couleur): bleu < 0 !\n");
      } else if (nouv.bleu > 255) {
        nouv.bleu = 255;
        fprintf(stderr, "libimage(change_couleur): bleu > 255 !\n");
      }
      im.tabPoints[ x + (y * im.largeur) ] = nouv;
    } else {
      fprintf(stderr, "libimage(change_couleur): (%d, %d) hors image !\n",
              x, y);
    }
}

/* La fonction "lire_image" lit, dans le fichier de nom
   "nom", une image (stockee au format PPM) */
image lire_image(char * nom)
{
  FILE          *fich;
  char           type[255];
  int            x, y;
  int            largeur, hauteur;
  couleur        coul;
  unsigned char  val;
  image          im;
  int            profondeur;

  /* ouverture du fichier */
  fich = fopen(nom, "rb");
  if (fich == NULL) {
    fprintf(stderr, "Le fichier %s n'est pas lisible !\n", nom);
    exit(1);
  }

  /* lecture du type */
  int res = fscanf(fich, "%254s", type);
  if (res != 1 || strcmp(type, "P6") != 0) {
    fprintf(stderr, "Le fichier '%s' n'est pas au format ppm (P6)\n", nom);
    exit(1);
  }

  /* lecture de la fin de ligne */
  while(getc(fich) != '\n') {}

  /* lecture des commentaires */
  {
    char next_car = getc(fich);
    while(next_car == '#') {
      while(getc(fich) != '\n');
      next_car = getc(fich);
    }
    ungetc(next_car, fich);
  }

  /* lecture de la taille de l'image */
  res = fscanf(fich, "%d %d", &largeur, &hauteur);
  /* lecture de la profondeur */
  res += fscanf(fich, "%d", &profondeur);
  /* lecture du dernier retour a la ligne */
  res += fscanf(fich, "%c", &val);

  if (res != 4) {
    fprintf(stderr, "Le fichier '%s' contient un mauvais entete\n", nom);
    exit(1);
  }
  
  im = nouvelle_image(largeur, hauteur);

  for (y = 0; y < hauteur; y++) {
    for (x = 0; x < largeur; x++) {
      res = fscanf(fich, "%c", &val);
      coul.rouge = (int)(val * 256) / (profondeur + 1);
      res += fscanf(fich, "%c", &val);
      coul.vert = (int) (val * 256) / (profondeur + 1);
      res += fscanf(fich, "%c", &val);
      coul.bleu = (int) (val * 256) / (profondeur + 1);
      change_couleur(im, x, y, coul);
      if (res != 3) {
	fprintf(stderr, "Le fichier '%s' contient une couleur bizarre\n", nom);
	exit(1);
      }
    }
  }
  fclose(fich);

  fprintf(stderr,
          "LireNouvelleImage: Lecture du  fichier '%s' qui contient une image %dx%d\n",
          nom,
          im.largeur, im.hauteur);

  return im;
}

/* La fonction "ecrire_image" enregistre l'image "im" dans un fichier
   de nom "nom" (au format PPM) */
void ecrire_image(image im, char * nom)
{
  FILE    *fich;
  int     x,y;
  couleur coul;

  fich = fopen(nom, "wb");
  if (fich == NULL) {
    fprintf(stderr, "Ecriture du fichier %s impossible\n", nom);
    exit(1);
  }
  fprintf(fich, "P6\n");
  fprintf(fich, "# Cree par libimage.a - (C) 1997 P.Gaborit - EMAC\n");
  fprintf(fich, "%d %d\n", im.largeur, im.hauteur);
  fprintf(fich, "255\n");

  for (y = 0; y < im.hauteur; y++) {
    for (x = 0; x < im.largeur; x++) {
      coul = lire_couleur(im, x, y);
      fprintf(fich, "%c", coul.rouge);
      fprintf(fich, "%c", coul.vert);
      fprintf(fich, "%c", coul.bleu);
    }
  }
  fclose(fich);

  fprintf(stderr,
          "EcrireImage: Ecriture d'une image %dx%d dans le fichier '%s'\n",
          im.largeur, im.hauteur, nom);
}
