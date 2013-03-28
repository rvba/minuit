/* 
 * Copyright (c) 2012 Milovann Yanatchkov 
 *
 * This file is part of Minuit, a free software
 * licensed under the GNU General Public License v2
 * see /LICENSE for more information
 *
 */

#include "util.h"

/*recherche linéaire*/
/*recherche séquentielle de mots dans un tableau (p34)*/

int u_lookup(char *word, char *array[])
{
	int i;
	for(i=0;array[i] != NULL;i++)
		if (strcmp(word,array[i]) == 0)
			return i;

	return -1;
}

/*recherche binaire*/
/*la table doit être triée*/

int u_lookup_bin(char *name,Nameval tab[],int ntab)
{
	int low,high,mid,cmp;
	
	low=0;
	high = ntab-1;
	while (low <= high)
	{
		mid = (low + high)/2;
		cmp = strcmp(name,tab[mid].name);
		if(cmp<0) // partie inférieure
			high = mid -1;
		else if (cmp > 0)//partie supérieure
			low = mid + 1;
		else
			return mid;//occurence trouvée
	}

	return -1; //aucune occurence
}






