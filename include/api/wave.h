#ifndef _API_WAVE_H_
#define _API_WAVE_H_

/*
 *  main.c
 *  Gestion des fichiers WAVE
 *
 *  Created by Pouet on 26/11/09.
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

/* Pour forcer l'alignement mémoire.
 * Normalement yen a pas besoin, mais au cas où ...
 * (merci Fvirtman ^^ ) */
#pragma pack(push, 1)

/* Nombre de secondes pour notre fichier */

/* Formats audios
 * (je ne connais que le PCM ...) */
enum F_AUDIO {
	PCM = 1
};

/* Taille de l'en-tête subTaille1 */
enum T_AUDIO {
	T_PCM = 16
};

/* Canaux :
 *	Mono = 1 seule enceinte
 *	Stéréo = 2 enceintes
 *	GDC = 3 enceintes (gauche, droite, centre)
 *	GD_GD = 4 enceintes (avant gauche et droite, arrière gauche et droite)
 *	GDCS = 5 enceintes (GDC + Surround)
 *	All = 6 enceintes (GDC, centre gauche, centre droit, surround) */
enum CAN {
	MONO = 1,
	STEREO = 2,
	GDC = 3,
	GD_GD = 4,
	GDCS = 5,
	ALL = 6
};

/* Fréquence d'échantillonnage
 * Valeurs en hz*/
enum F_ECH {
	F_8K = 8000,
	F_11K = 11025,
	F_22K = 22050,
	F_41K = 44100,
	F_48K = 48000, /* Encodage d'un CD audio ;) */
	F_96K = 96000
};

/* Bits par échantillon */
enum BPE {
	BPE_8 = 8,
	BPE_16 = 16,
	BPE_24 = 24,
	BPE_32 = 32
};

#define T_ENTETE 44

/* L'en-tête fait 44 octets.
 * Il ne faut pas oublier d'enlever sizeof(void*) si
 * on utilise sizeof sur notre structure. */
struct wave {
	/* Le mot "RIFF" */
	char riff[4];
	/* La taille du fichier - 8
	 * Ou subTaille2 + 44 - 8 */
	int32_t taille;
	/* Le mot "WAVE" */
	char wave[4];
	
	/* Le mot "fmt " */
	char fmt[4];
	/* Taille du header jusqu'à "data" */
	int32_t subTaille1;
	/* Format du fichier */
	int16_t formatAudio;
	/* Nombres de canaux */
	int16_t nombreCanaux;
	/* Fréquence d'échantillonnage */
	int32_t freqEch;
	/* ByteRate
	 * Nombre d'ocets par seconde
	 ** Calcul :
	 *===> freqEch * nombreCanaux * bitsParEch / 8 */
	int32_t ByteRate;
	/* Alignement
	 * Nombre d'octets par échantillon
	 * Englobe tous les canaux !
	 ** Calcul :
	 *===> nombreCanaux * bitsParEch / 8 */
	int16_t align;
	/* Bits par échantillon */
	int16_t bitsParEch;
	
	/* Le mot "data" et la 
	 * taille des données */
	char Ndata[4];
	/* Taille des données */
	int32_t subTaille2;
	
	/* A allouer dynamiquement.
	 * Contiendra les données */
	int16_t *data;
};

void initWave(struct wave *wav) {
	/* Les constantes symboliques */
	strncpy(wav->riff, "RIFF", 4);
	strncpy(wav->wave, "WAVE", 4);
	strncpy(wav->fmt, "fmt ", 4);
	strncpy(wav->Ndata, "data", 4);
	
	//wav->taille = wav->subTaille2 = 0;
	
	wav->subTaille1 = T_PCM;
	wav->formatAudio = PCM;
	wav->nombreCanaux = STEREO;
	wav->freqEch = F_41K;
	wav->bitsParEch = BPE_16;
	wav->ByteRate = wav->freqEch * wav->nombreCanaux * (wav->bitsParEch / 8);
	wav->align = wav->nombreCanaux * (wav->bitsParEch / 8);
	
	/* On utilise la constante N_SEC pour créer notre fichier */
	/* On aura un fichier de N_SEC secondes */
	/* On peut calculer subTaille2 en fonction du byterate. Ce qui nous donne : */
	wav->subTaille2 = wav->ByteRate * N_SEC;
	
	/* On calcule la taille du fichier */
	wav->taille = wav->subTaille2 + 44 - 8;
	
	/* On alloue la partie données */
	wav->data = new int16_t[wav->subTaille2];
}

// void genererPiste(struct wave *wav) {
// 	int16_t *data = wav->data;
// 	int cpt;
// 	double val;
	
// 	/* On crée une sinusoïde pour créer un jooooli son :D */
// 	for (cpt = 0, val = 0.0; cpt < wav->subTaille2/2; cpt += 2, val += 0.0284951714612 0.0142475857306) {
// 		/* On met la même valeur pour les 2 enceintes */
// 		/* Essayez d'en mettre un à 0 et de laisser l'autre ^^
// 		 * Vous aurez un bruit que sur une seule enceinte ;) */
// 		data[cpt] = sin(val*2) * 32267;
// 		data[cpt+1] = sin(val*2) * 32267;
// 	}
// }

// int main(void) {
	
// 	/* On écris l'en-tête SANS LE wav->data */
// 	fwrite(&wav, T_ENTETE, 1, fich);
// 	/* On écris les data */
// 	fwrite(wav.data, wav.subTaille2, 1, fich);
	
// 	/* On libère notre mémoire, et on ferme le fichier */
// 	free(wav.data);
// 	fclose(fich);
// 	/* Voilà notre fichier son est créé ^^ */
// 	return EXIT_SUCCESS;
// }
#endif