#include "anagrammes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>

#define BUFSIZE 512

int main(int argc, char *argv[]) {
  char buf[BUFSIZE];

  struct word_array tableauDeMot;
  word_array_create(&tableauDeMot);
  struct word_array resultDico;
  word_array_create(&resultDico);
  struct word_array resultTab;
  word_array_create(&resultTab);

  struct word_dict dico;
  word_dict_create(&dico);

  word_array_read_file(&tableauDeMot,"dictionnaire.txt");
  word_dict_fill_with_array(&dico,&tableauDeMot);

  bool sortie = false;

while(sortie != true){
	  printf("Saisir un mot: ");
	  fgets(buf, BUFSIZE, stdin);

	clean_newline(buf, BUFSIZE);

	  //recherche dans un tableau de mot
	  struct timeval beginTab, endTab;
	  gettimeofday(&beginTab,NULL);
	  word_array_search_anagrams_wildcard(&tableauDeMot,buf,&resultTab);
	  gettimeofday(&endTab,NULL);
	  size_t timeTab = (size_t)endTab.tv_usec - (size_t)beginTab.tv_usec;

	  word_array_sort(&resultTab);
	  printf("\n");
	  word_array_print(&resultTab);
	  printf("Le nombre d'anagrammes trouvées ,avec un tableau de mot, est de %zu.\n", resultTab.size);
	 word_array_destroy(&resultTab);
	  printf("Le temps de recherche des anagrammes dans un tableau de mot est de %ld microsecondes.\n",timeTab);
	  printf("\n");
	  //recherche dans un dictionnaire
	  struct timeval beginDico, endDico;
	  gettimeofday(&beginDico,NULL);
	  word_dict_search_anagrams_wildcard(&dico,buf,&resultDico);
	  gettimeofday(&endDico,NULL);
	  int timeDico = (int)endDico.tv_usec - (int)beginDico.tv_usec;

	  word_array_sort(&resultDico);
	  printf("\n");
	  word_array_print(&resultDico);
	  printf("Le nombre d'anagrammes trouvées ,avec un dictionnaire, est de %zu.\n", resultDico.size);
	 word_array_destroy(&resultDico);
	  printf("Le temps de recherche des anagrammes dans un dictionnaire de mot est de %i microsecondes.\n",timeDico);
	  printf("\n");
  
 

  printf("Pour continuer appuyer sur '1' sinon '2' pour quitter le programme\n");

		fgets(buf, BUFSIZE, stdin);

		clean_newline(buf, BUFSIZE);

		if(buf[0] == '2'){

			sortie = true;

		}

		 word_array_create(&resultTab);
  word_array_create(&resultDico);
}

  //Libération de la mémoire
  word_array_destroy(&tableauDeMot);
  word_dict_destroy(&dico);
  word_array_destroy(&resultDico);
  word_array_destroy(&resultTab);

  return 0;
}
