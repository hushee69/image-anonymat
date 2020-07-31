#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libimage.h"

#define TAILLE_INIT			8		// taille pour stocker les coordonnes du cercle
void demande_double(char *message, double *p_double)
{
	int res_scanf; /* le resultat du scanf */
	
	printf("%s", message);
	res_scanf = scanf("%lf", p_double);
	if( res_scanf != 1 )
	{
		printf("Lecture d'un double impossible!\n");
		exit(1);
	}
}

void demande_chaine_1001(char * message, char *chaine)
{
	int res_scanf; /* le resultat du scanf */
	
	printf("%s", message);
	res_scanf = scanf("%1000s", chaine);
	if( res_scanf != 1 )
	{
		printf("Lecture d'une chaine impossible!\n");
		exit(1);
	}
}

image duplique_image(image in)
{
	int x_size = in.largeur, y_size = in.hauteur;
	fprintf(stderr, "largeur in: %d\nhauteur in: %d\n", x_size, y_size);
	
	image ret = nouvelle_image(x_size, y_size);
	
	int x, y;
	couleur temp;
	
	for( y = 0; y < y_size; ++y )
	{
		for( x = 0; x < x_size; ++x )
		{
			temp = lire_couleur(in, x, y);
			change_couleur(ret, x, y, temp);
		}
	}
	
	return ret;
}

int point_dans_cercle(int origin_x, int origin_y, int x, int y, float r)
{
	int x0 = x - origin_x, y0 = y - origin_y;
	float p = (x0 * x0) + (y0 * y0) - (r * r);
	
	if( p <= 0 )
	{
		return 1;
	}
	
	return -1;
}

image remplit_cercle(image in, int origin_x, int origin_y, float radius, couleur fill_color)
{
	int y, x;
	int rad = floor(radius);
	int x_coord, y_coord, in_cercle;
	image ret = in;
	
	// pour le hauteur
	for( y = -rad; y <= rad; ++y )
	{
		// pour le largeur
		for( x = -rad; x <= rad; ++x )
		{
			x_coord = origin_x + x;
			y_coord = origin_y + y;
			in_cercle = point_dans_cercle(origin_x, origin_y, x_coord, y_coord, radius);
			// si le point est dans le cercle
			if( in_cercle == 1 )
			{
				change_couleur(ret, x_coord, y_coord, fill_color);
			}
		}
	}
	
	fprintf(stderr, "cercle remplit avec le couleur moyen\n");
	
	return ret;
}

couleur moyenne_couleur(image in, int origin_x, int origin_y, double radius)
{
	int y, x;
	int rad = floor(radius);
	int x_coord, y_coord, in_cercle;
	int pixel_count = 0;			// pour compter combien de pixels y en a dans le cercle, utile pour calculer la moyenne
	couleur c_temp;
	couleur moyenne;
	
	moyenne.rouge = moyenne.vert = moyenne.bleu = 0;
	
	// calculer la moyenne ici
	for( y = -rad; y <= rad; ++y )
	{
		for( x = -rad; x <= rad; ++x )
		{
			x_coord = origin_x + x;
			y_coord = origin_y + y;
			in_cercle = point_dans_cercle(origin_x, origin_y, x_coord, y_coord, radius);
			
			if( in_cercle == 1 )
			{
				pixel_count++;
				c_temp = lire_couleur(in, x_coord, y_coord);
				moyenne.rouge += c_temp.rouge;
				moyenne.vert += c_temp.vert;
				moyenne.bleu += c_temp.bleu;
			}
		}
	}
	
	moyenne.rouge = moyenne.rouge / pixel_count;
	moyenne.vert = moyenne.vert / pixel_count;
	moyenne.bleu = moyenne.bleu / pixel_count;
	
	return moyenne;
}

int main()
{
	char nom_image_in[1001];  /* nom du fichier image lu */
	char nom_image_out[1001]; /* nom du fichier image produit */
	double cr;                /* le rayon d'un cercle */
	double cx, cy;            /* les coordonnees du centre du cercle */
	image in;                 /* image d'origine */
	image out;
	
	demande_chaine_1001("Nom du fichier image 'in' ?\n", nom_image_in);
	in = lire_image(nom_image_in);
	
	out = duplique_image(in);
	fprintf(stderr, "image duplique avec succes\n");
	
	couleur couleur_moyenne;
	
	demande_double("Rayon du cercle ?\n", &cr);
	while( cr > 0 )
	{
		demande_double("Coordonnee X du centre ?\n", &cx);
		demande_double("Coordonnee Y du centre ?\n", &cy);
		
		couleur_moyenne = moyenne_couleur(in, cx, cy, cr);
		fprintf(stderr, "moyenne dans le cercle de rayon %g: (%d, %d, %d)\n", cr, couleur_moyenne.rouge, couleur_moyenne.vert, couleur_moyenne.bleu);
		out = remplit_cercle(out, cx, cy, cr, couleur_moyenne);
		
		demande_double("Rayon du cercle ?\n", &cr);
	}
	
	demande_chaine_1001("Nom du fichier image 'out' ?\n", nom_image_out);
	ecrire_image(out, nom_image_out);
	
	detruire_image(in);
	detruire_image(out);
	
	return 0;
}

