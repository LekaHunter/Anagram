#include "anagrammes.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define LETTERS_MAX 100

/*------------------- Part 1-----------------------*/

/**
 *Camparer deux chaines de caracteres
 *@param str1 La première chaine de caractères à comparer
 *@param str2 La deuxième chaine de caractères à comparer
 *@return true si les deux chaines de caractères sont identique
 */

bool str_compare(const char *str1, const char *str2) {
  size_t i = 0;

  while(str1[i] != '\0' && str2[i] != '\0'){

    if(str1[i] != str2[i]){

      return false;
    }

    ++i;
  }

  return true;
}

/**
 *Vérifie si deux chaines de caracteres sont des anagrammes
 *@param str1 La première chaine de caractères à vérifier
 *@param str2 La deuxième chaine de caractères à vérifier
 *@return true si les deux chaines de caractères sont des anagrammes
 */

bool string_are_anagrams(const char *str1, const char *str2) {
  size_t sizeStr1 = strlen(str1);
  size_t sizeStr2 = strlen(str2);

  if(sizeStr1 != sizeStr2){
    return false;
  }

  char *dup1 = string_duplicate(str1);
  char *dup2 = string_duplicate(str2);

  string_sort_letters(dup1);
  string_sort_letters(dup2);

  bool res = str_compare(dup1,dup2);

  free(dup1);
  free(dup2);

  return res;
}

/**
 *Duplique une chaine de caractère 
 *@param str Chaine de caractere à dupliquer
 *@result tmp L'adresse du premier caractere de la chaine de caractere résultante
 */

char *string_duplicate(const char *str) {
	size_t len = strlen(str)+1;
	
	char *tmp = malloc(len*sizeof(char));

  strcpy(tmp,str);

  return tmp;
}

/**
 *Trie les caracteres d'une chaine de caractere
 *@param str Chaine de caractere à trier
 */

void string_sort_letters(char *str) {
  size_t size = strlen(str);
  
  for(size_t i = 0; i < size; ++i){

    char tmp = str[i];
    size_t j = i;

    while(j > 0 && str[j - 1] > tmp){

      str[j] = str[j - 1];

      --j;

    }

    str[j] = tmp;

  }

}

/**
 *Nettoyes un buffeur du caractere '\n' saut de ligne
 *@param buf Le buffeur à nettoyer
 *@param size La taille du buffeur
 */

void clean_newline(char *buf, size_t size) {
  size_t i = 0;

 while(buf[i] != '\n' && buf[i] != '\0' && i < size){

    ++i;

  }

  buf[i] = '\0';

}

/*--------------------- Part 2-------------------*/

/**
 *Création d'un tableau de mot
 *@param self Le tableau de mot à créer
 */

void word_array_create(struct word_array *self) {
  self->capacity = 100;
  self->size = 0;
  self->data = (char **)malloc(self->capacity*sizeof(char *));
}

/**
 *Destruction d'un tableau de mot
 *@param self Le tableau de mot à détruire
 */

void word_array_destroy(struct word_array *self){

  for(size_t i = 0; i < self->size; ++i){
    free(self->data[i]);
  }

    free(self->data);

    self->capacity = 0;
    self->size = 0;
}

/**
 *Ajouter une chaine de caractere dans un tableau de mot
 *@param self Le tableau de mot ou l'on va ajouter la chaine de caractere
 *@param word La chaine de caractere à ajouter au tableau de mot
 */

void word_array_add(struct word_array *self, const char *word){

  char *tmp = string_duplicate(word);

  size_t sizeWord = strlen(tmp);


  if(self->size == self->capacity){

    self->capacity = self->capacity * 2;  

    char **data = (char **)calloc(self->capacity,sizeof(char *));
      
    memcpy(data,self->data,self->size*sizeof(char *));

    free(self->data);
    
    self->data = data;
  }  

  self->data[self->size] = (char *)malloc(LETTERS_MAX*sizeof(char));

  for(size_t i = 0; i < sizeWord; ++i){

    self->data[self->size][i] = tmp[i];

  }
    
  self->data[self->size][sizeWord] = '\0';

  self->size = self->size + 1;

  free(tmp);
}

/**
 *Chercher les anagrammes d'une chaine de caractere et placer les anagrammes trouvées dans un nouveau tableau de mot
 *@param self Le tableau de mot ou on va charcher les anagrammes de la chaine de caractere
 *@param word La chaine de caractere que l'on veut trouver les anagrammes
 *@result Le tableau de mot ou l'on va ranger les anagrammes de la chaine de caractere que l'on trouve
 */

void word_array_search_anagrams(const struct word_array *self, const char *word, struct word_array *result) {
	
  for(size_t i = 0; i < self->size; ++i){

    if(string_are_anagrams(word,self->data[i])){

      word_array_add(result,self->data[i]);

    }

  }

}

/**
 *Echanger la placer de deux chaine de caractre
 *@param data Le tableau de pointeur vers des chaines de caracteres que l'on va manipuler
 *@param i L'indice de la première chaine de caractere à échanger
 *@param j L'indice de la deuxième chaine de caractere à échanger
 */

void array_swap(char **data, size_t i, size_t j){
	char *tmp = data[i];
	data[i] = data[j];
	data[j] = tmp;
}

/**
 *Trier un tableau de pointeur vers des caracteres par rapport à un pivot (Partitionnage)
 *@param data Le tableau de pointeur vers des chaines de caracteres que l'on va trier
 *@param i L'indice du premier élement du tableau
 *@param j L'indice du dernier élement du tableau
 *@result l L'indice du pivot 
 */

ssize_t array_partition(char **data, ssize_t i, ssize_t j){
	ssize_t pivot_ind = i;
	const char *pivot = data[pivot_ind];

	array_swap(data, i, j);
	ssize_t l = i;

	for(ssize_t k = i;k < j; ++k){

		if(strcmp(data[k], pivot) < 0){
			array_swap(data, k, l);
			++l;
		}
	}

	array_swap(data, l, j);

	return l;
}

/**
 *Trie rapide par partition d'un tableau de pointeur vers des chaines de caracteres
 *@param data Le tableau de pointeur vers des chaines de caracteres que l'on va trier
 *@param i L'indice du premier élement du tableau
 *@param j L'indice du dernier élement du tableau
 */

void array_quick_sort_partial(char **data, ssize_t i, ssize_t j){

	if(i < j){
		ssize_t p = array_partition(data, i, j);
		array_quick_sort_partial(data, i, p - 1);
		array_quick_sort_partial(data, p + 1, j);
	}
}

/**
 *Trier un tableau de mot avec un trie rapide par partition
 *@param self Le tableau de mot à trier
 */

void word_array_sort(struct word_array *self) {

	array_quick_sort_partial(self->data, 0, self->size - 1);

}

/**
 *Afficher les chaines de caracteres dans un tableau de mot
 *@param self Le tableau de mot à afficher
 */

void word_array_print(const struct word_array *self) {

	for(size_t i = 0; i < self->size; ++i){

    printf("%zu : %s\n", i,self->data[i]);

	}

}

/**
 *FONCTION DONNER 
 *Remplir un tableau de mot avec un fichier
 *@param self Le tableau de mot à remplir
 *@param filename Le nom du fichier en chaine de caractere
 */

void word_array_read_file(struct word_array *self, const char *filename) {
  char word[LETTERS_MAX];

  FILE *fp = fopen(filename, "r");

  if (fp == NULL) {
    fprintf(stderr, "Error when opening file '%s'\n", filename);
    return;
  }

  while (!feof(fp)) {
    fgets(word, LETTERS_MAX, fp);

    if(!feof(fp)){
      clean_newline(word, LETTERS_MAX);
      word_array_add(self, word);
    }
  }

  fclose(fp);
}

/*--------------------------------- Part 3-----------------------------------*/

/**
 *Déstruction d'une liste chainé de chaine de caractere
 *@param self Liste chainé à detruire
 */

void word_dict_bucket_destroy(struct word_dict_bucket *self){

  while(self != NULL){

    struct word_dict_bucket *curr = self;

    self = curr->next;

    free(curr);

  }

  self=NULL;

}

/**
 *Ajouter une chaine de caractere à une liste chainé de chaine de caractere
 *@param self La liste chainé ou l'on va ajouter la chaine de caractere
 *@param word La chaine de caractere à ajouter dans la liste chainé
 *@result La liste chainé mis en paramètre avec le word 
 */

struct word_dict_bucket *word_dict_bucket_add(struct word_dict_bucket *self, const char *word){

    struct word_dict_bucket *newBucket = (struct word_dict_bucket *)malloc(sizeof(struct word_dict_bucket));

    newBucket->word = word;
    newBucket->next = self;
    self = newBucket;

    return self;
}

/**
 *Création dun dictionnaire de chaine de caractere
 *@param self Le Dictionnaire à créer
 */

void word_dict_create(struct word_dict *self){

  self->count = 0;
  self->size = 10;
  self->buckets = (struct word_dict_bucket **)malloc(self->size*sizeof(struct word_dict_bucket *));

  for (size_t i = 0; i < self->size; ++i){
    self->buckets[i] = NULL;
  }

}

/**
 *Détruire une dictionnaire de chaine de caractere
 *@param self Le dictionnaire à détruire
 */

void word_dict_destroy(struct word_dict *self){

  for(size_t i = 0; i < self->size; ++i){

    word_dict_bucket_destroy(self->buckets[i]);

  }

  self->count = 0;
  self->size = 0;
  free(self->buckets);
}

/**
 *Calculer le hash d'une chaine de caractere
 *@param key La chaine de caractere à caluler le hash
 *@result hash un entier 64bits hash de key
 */

size_t fnv_hash(const char *key){
  char *tmp = string_duplicate(key);
  size_t size = strlen(tmp);
  string_sort_letters(tmp);

  uint64_t FNV_offset_basis = 14695981039346656037LU;
  uint64_t FNV_prime = 1099511628211;

  size_t hash = FNV_offset_basis;

  for(size_t i = 0; i < size; ++i){

    size_t c = (size_t)tmp[i];
    hash = hash ^ c;
    hash = hash * FNV_prime;

  }

  free(tmp);
  return hash;
}

/**
 *Re Calculer le hash d'un dictionnaire de mot
 *@param self Le dictionnaire de mot à re-calculer le hash de chaque chaine de caractere
 */

void word_dict_rehash(struct word_dict *self){
  size_t newSize = self->size*2;

  struct word_dict_bucket **res = (struct word_dict_bucket **)malloc(newSize*sizeof(struct word_dict_bucket *));

  for(size_t i = 0; i < newSize; ++i){

    res[i] = NULL;

  }

  for(size_t j = 0; j < self->size; ++j){

    struct word_dict_bucket *tmp = self->buckets[j];

    if(tmp != NULL){

      while(tmp != NULL){

        size_t hash = fnv_hash(tmp->word);
        size_t index = hash%newSize;

        res[index] = word_dict_bucket_add(res[index],tmp->word);

        tmp = tmp->next;

      }

    }

  }

  for(size_t i = 0; i < self->size; ++i){

    word_dict_bucket_destroy(self->buckets[i]);

  }

  free(self->buckets);
  self->size = newSize;
  self->buckets = res;

}

/**
 *Ajouter une chaine de caractere dans un dictionnaire de mot
 *@param self Le dictionnaire de mot ou l'on va ajouter la chainde de caractere
 *@param word La chaine de caractere que l'on veut ajouter au dictionnaire de mot
 */

void word_dict_add(struct word_dict *self,const char *word){
  
  double factCompress = (double)self->count/self->size;//on calcule le facteur de compression

  if(factCompress > 0.5){

    word_dict_rehash(self);//rehash le dictionnaire
    
  }

  size_t index = fnv_hash(word); 

  size_t indexHash = index % self->size;

  self->buckets[indexHash] = word_dict_bucket_add(self->buckets[indexHash],word);

  self->count = self->count + 1;

}

/**
 *Remplir un dictionnaire de mot avec un tableau de mot
 *@param self le dictionnaire de mot à remplir 
 *@param array Le tableau de mot à mettre dans le dictionnaire de mot
 */

void word_dict_fill_with_array(struct word_dict *self, const struct word_array *array) {

  for(size_t i = 0; i < array->size; ++i){

    word_dict_add(self,array->data[i]);

  }

}

/**
 *Chercher des anagrammes d'une chaine de caractere dans un dictionnaire de mot et on stocke les resultats trouvé dans un tableau de mot
 * @param self Le dictionnaire de mot ou on va chercher les anagrammes de la chaine de caractere
 * @param word La chaine de caractere que l'on veut trouver les anagrammes
 * @param result Le tableau de mot ou on va stocker les angrammes trouver dans le dictionnaire de mot
 */

void word_dict_search_anagrams(const struct word_dict *self,const char *word, struct word_array *result){
  size_t hash = fnv_hash(word);

  size_t indiceAna = hash % self->size;

  struct word_dict_bucket *curr = self->buckets[indiceAna];

  while(curr != NULL){

    if(string_are_anagrams(word,curr->word)){

      word_array_add(result,curr->word);

    }
    
    curr=curr->next;
  }

}

/*--------------------- Part 4 ---------------------------------------*/

/**
 *Création d'un joker
 *@param self Le joker a créer
 */
void wildcard_create(struct wildcard *self) {
  self->count = 0;

  for(size_t i = 0; i < WILDCARDS_MAX; ++i){

    self->index[i] = -1;

  }

}

/**
 *Recherche de Joker dans une chaine de caractère  
 *@param self Une structure de wildcard pour stocker les informations des Joker de la chaine de caractere 
 *@param word Le mot ou l'on va chercher les Joker
*/

void wild_card_search(struct wildcard *self,const char *word){

  for(size_t i=0;i<strlen(word);++i){

    if(word[i]=='*'){

      ++self->count;

      size_t k=0;

      while(k<WILDCARDS_MAX && self->index[k]!=-1){

        k++;

      }

      self->index[k]=i;

    }

  }

}

/*
 *
 *@param 
 *@param
 *@result
*/

bool string_are_anagrams_wildcard(const char *str1, const char *str2) {
  bool res = true;
  size_t sizeStr1 = strlen(str1);
  size_t sizeStr2 = strlen(str2);

  if(sizeStr1 != sizeStr2){
    return false;
  }

  char *word1 = string_duplicate(str1);
  char *word2 = string_duplicate(str2);

  string_sort_letters(word1);
  string_sort_letters(word2);

  int tab1[26];
  int tab2[26];

  for(size_t k=0; k<26 ;++k){
    tab1[k]=0;
    tab2[k]=0;
  }
   size_t nbreJoker=0;

  for(size_t i=0; i<sizeStr2; ++i){

        if(word2[i]=='*'){

            ++nbreJoker;
        }
  }

  if(nbreJoker == 0){
    free(word1);
  free(word2);
    return string_are_anagrams(str1,str2);
  }

  for(size_t k=0; k<sizeStr1; ++k){
    tab1[word1[k]-'a'] = tab1[word1[k]-'a'] + 1;
    if(word2[k]!='*'){
      tab2[word2[k]-'a'] = tab2[word2[k]-'a'] + 1;
    }
  }

  for(size_t i=0;i<26;++i){
    if(tab1[i]!=tab2[i]){
      if(tab1[i]<tab2[i]){
        res = false;
      }else{
        if(nbreJoker!=0){
          --nbreJoker;
        }else{
          if(nbreJoker==0){
            res = false;
          }
        }
      }
    }
  }

  free(word1);
  free(word2);

  return res;
}

/*
 *
 *@param 
 *@param
 *@result
*/

void word_array_search_anagrams_wildcard(const struct word_array *self, const char *word, struct word_array *result) {

  for(size_t i = 0; i < self->size; ++i){

    if(string_are_anagrams_wildcard(self->data[i],word)){

      word_array_add(result,self->data[i]);

    }
    
  }

}

/*
 *
 *@param 
 *@param
 *@result
*/

void word_dict_search_anagrams_wildcard(const struct word_dict *self, const char *word, struct word_array *result) {
  char *tmp = string_duplicate(word);
  struct wildcard joker;
  wildcard_create(&joker);

  wild_card_search(&joker,tmp);

  switch (joker.count){
  case 0 :

    word_dict_search_anagrams(self,word,result);

    break;
  case 1 :

    for(size_t i = 97; i < 123; ++i){
      tmp[joker.index[0]] = (char)i;
      word_dict_search_anagrams(self,tmp,result);
    }

    free(tmp);

    break;
  case 2 :

    for(size_t j = 97; j < 123; ++j){
      tmp[joker.index[0]] = (char)j;
      for(size_t k = 97; k < 123; ++k){
        tmp[joker.index[1]] = (char)k;
        word_dict_search_anagrams(self,tmp,result);
      }

    }

    free(tmp);
    break;
  case 3 :

    for(size_t j = 97; j < 123; ++j){
      tmp[joker.index[0]] = (char)j;
      for(size_t k = 97; k < 123; ++k){
        tmp[joker.index[1]] = (char)k;
        for(size_t h = 97; h < 123; ++h){
          tmp[joker.index[2]] = (char)h;
          word_dict_search_anagrams(self,tmp,result);
        }
      }

    }

    free(tmp);
    break;
  case 4 :

    for(size_t j = 97; j < 123; ++j){
      tmp[joker.index[0]] = (char)j;
      for(size_t k = 97; k < 123; ++k){
        tmp[joker.index[1]] = (char)k;
        for(size_t h = 97; h < 123; ++h){
          tmp[joker.index[2]] = (char)h;
          for(size_t h = 97; h < 123; ++h){
            tmp[joker.index[3]] = (char)h;
            word_dict_search_anagrams(self,tmp,result);
          }
        }
      }

    }

    free(tmp);
    break;
  }
}
