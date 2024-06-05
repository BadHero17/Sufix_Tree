// Ursu Vasile (2024)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET_SIZE 26

// Definitia tipului pentru un nod din arborele de sufixuri
// Fiecare nod pastreaza un vector de 27 de pointer la noduri,
// un sir de caractere, care initial va contine un singur caracter
// si dupa compresarea cailor poate contine mai multe caractere,
// un intreg ce reprezinta numarul de copii setati (nenuli)
// si un intreg ce reprezinta numarul de caractere in sirul de
// caractere (necesar pentru compresare)
typedef struct nod {
    int lung_info;
    int nr_copii;
    char *info;
    struct nod *copii[ALPHABET_SIZE + 1];
} nod_t, *sufix_t;

// Definitia celulii din coada
// Coada e implementata pe baza de lista simplu inlantuita
// Fiecare celula pastraza un nod din arborele de sufixe
// Si nivelul pe care se regaseste (necesar pentru formatare)
typedef struct celula_coada {
    sufix_t nod_coada;
    int nivel;
    struct celula_coada *urm;
} celula_coada_t, *element_coada_t;

// Definitia tipului de date pentru coada, un pointer la
// primul element din coada si unul la ultimul
typedef struct coada {
    element_coada_t inc, sf;
} coada_t;

// Declaratii functii
sufix_t aloca_nod(char x);
sufix_t initializeaza_sufix();
void distruge_subarb(sufix_t nod);
void distruge_sufix(sufix_t *arb);
int scrie_sufix(sufix_t arb, char *string);
int insereaza_cuvant(sufix_t arb, char *string);
coada_t *initializeaza_coada();
int insereaza_coada(coada_t *q, sufix_t nod, int nivel);
void scoate_coada(coada_t *q);
element_coada_t element_curent(coada_t *q);
int coada_vida(coada_t *q);
void distruge_coada(coada_t **q);
void bfs(sufix_t arb, FILE *out);
int numara_frunze(sufix_t arb);
int numara_k_sufixe(sufix_t arb, int nivel, int k);
int descendenti_maximi(sufix_t arb);
int exista_sufix(sufix_t arb, char *string);
char *adauga_dolar(char *string, int *lung_string);
int compreseaza_cai(sufix_t arb);