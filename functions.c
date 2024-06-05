// Ursu Vasile (2024)
#include "functions.h"

// Functie care calculeaza maximul intre doi intregi
int max(int a, int b) {
    return a < b ? b : a;
}

// Functie care calculeaza minimul intre doi intregi
int min(int a, int b) {
    return a < b ? a : b;
}

// Functie care modifica un sir de caractere, adaugand un '$' la
// sfarsit si punand-ul exact pe numarul de caractere pe care il are
// Intoarce NULL daca realloc esueaza
char *adauga_dolar(char *string, int *lung_string) {
    char *test = realloc(string, (*lung_string + 1) * sizeof(char));
    if (!test) {
        free(string);
        (*lung_string) = 0;
        return NULL;
    }
    string = test;
    string[(*lung_string) - 1] = '$';
    string[(*lung_string)] = 0;
    (*lung_string)++;
    return string;
}

// Functie care aloca un nod intr-un arbore de sufixe
// Nodul va fi creat cu un singur caracter in info
// si toti copii nuli
// Intoarce NULL daca alocarile esueaza
sufix_t aloca_nod(char x) {
    sufix_t nou = calloc(1, sizeof(nod_t));
    if (!nou) {
        return NULL;
    }
    nou->info = calloc(2, sizeof(char));
    if (!nou->info) {
        free(nou);
        return NULL;
    }
    nou->info[0] = x;
    nou->lung_info = 2;
    return nou;
}

// Functia data initializeaza un arbore de sufixe
// Radacina nu are nicio informatie utila, asa ca
// info va fi setat la NULL
sufix_t initializeaza_sufix() {
    sufix_t radacina = aloca_nod(0);
    return radacina;
}

// Functia data distruge toate nodurile cu pornesc din nodul
// dat ca parametru, dar nu elibereaza nodul respectiv
void distruge_subarb(sufix_t nod) {
    int i = 0;
    for (; i < ALPHABET_SIZE; i++) {
        if (nod->copii[i]) { 
            distruge_subarb(nod->copii[i]);
            free(nod->copii[i]);
        }
    }
    free(nod->info);
}

// Functie ce distruge un arbore de sufixe si elibereaza toata
// memoria aferenta
void distruge_sufix(sufix_t *arb) {
    distruge_subarb(*arb);
    free(*arb);
    *arb = NULL;
}

// Functie recursiva care introduce nodurile necesare pentru a avea
// sufixul dat de string (care trebuie sa se termine in '$') in
// arbore
// Intoarce 1 daca a introdus cu succes nodul si 0 daca a esuat
// vreo alocare in proces
int scrie_sufix(sufix_t arb, char *string) {
    int succes = 1;
    if (*string == '$') {
        if (arb->copii[0]) {
            return 1;
        }
        sufix_t aux = aloca_nod(*string);
        if (!aux) return 0;
        arb->copii[0] = aux;
        arb->nr_copii++;
        return 1;
    } else {
        if (arb->copii[*string + 1 - 'a']) {
            succes = scrie_sufix(arb->copii[*string + 1 - 'a'], string + 1);
        } else {
            sufix_t aux = aloca_nod(*string);
            if (!aux) return 0;
            arb->copii[*string + 1 - 'a'] = aux;
            arb->nr_copii++;
            succes = scrie_sufix(arb->copii[*string + 1 - 'a'], string + 1);
        }
        
    }
    return succes;
}

// Functia data insereaza un cuvant (care tebuie sa se termine in '$')
// in arborele de sufixe, introducand pe rand toate sufixele
// sirului de caractere string in arbore, incepand de la cel mai scurt
// pana la intreg cuvantul
// Inserarea sufixelor se face prin apel la scrie_sufix
// Intoarce 1 daca inserarea cuvantului a reusit si 0 daca
// vreo alocare a esuat
int insereaza_cuvant(sufix_t arb, char *string) {
    int i = 0;
    while (string[i]) i++;
    i--;
    for (; i>=0; i--) {
        int rez = scrie_sufix(arb, string + i);
        if (!rez) return 0;
    }
    return 1;
}

// Functie ce creeaza o coada si seteaza inceputul si sfarsitul
// la NULL
// Intoarce NULL daca alocarea esueaza 
coada_t *initializeaza_coada() {
    coada_t *aux = malloc(sizeof(celula_coada_t));
    if (!aux) {
        return NULL;
    }
    aux->inc = NULL;
    aux->sf = NULL;
    return aux;
}

// Functie ce introduce un element in coada
// Intoarce 1 daca reuseste si 0 daca esueaza
int insereaza_coada(coada_t *q, sufix_t nod, int nivel) {
    element_coada_t aux = malloc(sizeof(celula_coada_t));
    if (!aux) return 0;
    aux->nod_coada = nod;
    aux->nivel = nivel;
    if (q->sf == NULL) {
        q->sf = aux;
        q->inc = aux;
        aux->urm = NULL;
    } else {
        q->sf->urm = aux;
        aux->urm = NULL;
        q->sf = aux;
    }
    return 1;
}

// Functie care scoate un element din coada
// NU intoarce elementul scos
void scoate_coada(coada_t *q) {
    if (!q->inc) return;
    if (q->inc == q->sf) {
        free(q->inc);
        q->inc = NULL;
        q->sf = NULL;
    } else {
        element_coada_t aux = q->inc;
        q->inc = q->inc->urm;
        free(aux);
    }
}

// Functie ce intoarce primul element din coada
element_coada_t element_curent(coada_t *q) {
    return q->inc;
}

// Functie ce verifica daca o coada e vida
// Intoarce 1 daca da si 0 daca nu
int coada_vida(coada_t *q) {
    if (q->inc == NULL) {
        return 1;
    }
    return 0;
}

// Functie ce elibereaza memoria unei cozi, NU sterge elementele,
// este de datoria apelantului sa se asigure ca coada e goala la
// momentul apelarii
void distruge_coada(coada_t **q) {
    free((*q)->inc);
    free((*q)->sf);
    free(*q);
    *q = NULL;
}

// Functie ce realizeaza o parcurgere Breadth First Search
// pe un arbore de sufixe si scrie toata informatia continuta in
// noduri
// Outputul e formatat astfel incat toate nodurile de pe un nivel
// sunt scrise pe un rand
void bfs(sufix_t arb, FILE *out) {
    coada_t *q = initializeaza_coada();
    if (!q) { 
        fprintf(out, "Eroare\n");
        return;
    }
    insereaza_coada(q, arb, 0);
    int ultim_nivel = 0;
    while (!coada_vida(q)) {
        element_coada_t curent = element_curent(q);
        if (ultim_nivel && ultim_nivel < curent->nivel) {
            fprintf(out, "\n");
        }
        ultim_nivel = curent->nivel;
        if (*curent->nod_coada->info) {
            fprintf(out, "%s ", curent->nod_coada->info);
        }
        int i = 0;
        for (; i <= ALPHABET_SIZE; i++) {
            if (curent->nod_coada->copii[i]) {
                insereaza_coada(q, curent->nod_coada->copii[i], curent->nivel + 1);
            }
        }
        scoate_coada(q);
    }
    distruge_coada(&q);
}

// Functie recursiva ce determina cate frunze are un arbore de sufixe,
// nu verifica faptul ca fii trebuie sa fie in numar de 0,
// Ci informatia din nod, care trebuie sa fie '$' intr-o frunza
// Intoarce numarul de frunze
int numara_frunze(sufix_t arb) {
    int ans = 0;
    if (*arb->info == '$') return 1;
    int i = 0;
    for (; i <= ALPHABET_SIZE; i++) {
        if (arb->copii[i]) {
            ans += numara_frunze(arb->copii[i]);
        }
    }
    return ans;
}

// Functie recursiva ce numara cate sufixe de lungime exact k
// sunt in arborele de sufixe
// Intoarce numarul de k-sufixe
int numara_k_sufixe(sufix_t arb, int nivel, int k) {
    if (nivel == k + 1) {
        if (*arb->info == '$') {
            return 1;
        } else {
            return 0;
        }
    }
    int sum = 0;
    int i = 0;
    for (; i < ALPHABET_SIZE; i++) {
        if (arb->copii[i]) {
            sum += numara_k_sufixe(arb->copii[i], nivel + 1, k);
        }
    }
    return sum;
}

// Functie ce determina care e numarul maxim de descendenti ai
// unui singur nod
// Intoarce doar numarul maxim de descendenti, nu si nodul
int descendenti_maximi(sufix_t arb) {
    int maax = 0;
    int i = 0;
    for (; i <= ALPHABET_SIZE; i++) {
        if (arb->copii[i]) {
            maax = max(maax, descendenti_maximi(arb->copii[i]));
        }
    }
    maax = max(maax, arb->nr_copii);
    return maax;
}

// Functie ce cauta un sufix in arbore si determina existenta acestuia
// Intoarce 1 daca sufixul e prezent in arbore si 0 daca nu
int exista_sufix(sufix_t arb, char *string) {
    if (*string == '$') {
        if (arb->copii[0]) {
            return 1;
        } else {
            return 0;
        }
    }
    if (arb->copii[*string + 1 - 'a']) {
        return exista_sufix(arb->copii[*string + 1 - 'a'], string + 1);
    }
    return 0;
}

// Functie care realizeaza compresarea cailor in arborele de sufixe
// Daca urmeaza inserari dupa compresarea cailor din arbore, rezultatul
// Oricarei alte functii devine nedeterminat, functia e designata sa fie apelata
// Doar dupa ce sau terminat inserarile
// Intoarce 1 daca a reusit si 0 daca nu
int compreseaza_cai(sufix_t arb) {
    int test = 1;
    int i = 0;
    for (; i <= ALPHABET_SIZE; i++) {
        if (arb->copii[i]) {
            test = compreseaza_cai(arb->copii[i]);
        }
        if (!test) return 0;
    }
    if (arb->nr_copii == 1) {
        i = 0;
        for (; i <= ALPHABET_SIZE; i++) {
            if (arb->copii[i]) {
                if (!i) {
                    return 1;
                }
                sufix_t nou = arb->copii[i];
                int lungime_noua = nou->lung_info;
                lungime_noua++;
                char *temp = realloc(arb->info, lungime_noua * sizeof(char));
                if (!temp) {
                    free(arb->info);
                    return 0;
                }
                arb->info = temp;
                strcpy(arb->info + 1, nou->info);
                int j = 0;
                for (; j < ALPHABET_SIZE; j++) {
                    arb->copii[j] = nou->copii[j];
                }
                arb->lung_info = lungime_noua;
                free(nou->info);
                free(nou);
                return 1;
            }
        }
    }
    return 1;
}