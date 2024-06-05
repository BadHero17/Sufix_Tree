// Ursu Vasile (2024)
#include "functions.h"

int main(int argc, char *argv[]) {
    // Initializarea arborelui de sufixe
    sufix_t arbore = initializeaza_sufix();
    if (!arbore) {
        return -1;
    }
    // Comparam primul argument parametru la apelarea programului
    // cu sirurile -c1, -c2 ... pentru a identifica operatia curent
    if (!strcmp(argv[1], "-c1")) {
        /*--------------------- Operatia 1 ----------------------*/

        // Declaratii de variabile necesare pentru parsare si parsarea
        // datelor de intrare
        FILE *in = fopen(argv[2], "r");
        FILE *out = fopen(argv[3], "w");
        int n = 0;
        fscanf(in, "%d", &n);
        fgetc(in);
        int lungsir = 0;
        size_t temp = 0;
        char *string = NULL;
        int i = 0;
        for (; i < n; i++) {
            lungsir = getline(&string, &temp, in);

            // Sirul de caractere citit nu are dolar la sfarsit,
            // asa ca il adaugam
            string = adauga_dolar(string, &lungsir);
            if (!string) return -1;

            // Inseram cuvantul modificat in arborele de sufixe
            insereaza_cuvant(arbore, string);
            free(string);
            string = NULL;
        }
        // Scriem parcurgerea in latime a arborelui obtinut
        bfs(arbore, out);
        fprintf(out, "\n");
        fclose(in);
        fclose(out);
    } else if (!strcmp(argv[1], "-c2")) {
        /*---------------------- Operatia 2 --------------------------*/

        // Similar cu prima cerinta 1
        FILE *in = fopen(argv[3], "r");
        FILE *out = fopen(argv[4], "w");
        int n = 0;
        int k = atoi(argv[2]);
        fscanf(in, "%d", &n);
        fgetc(in);
        int lungsir = 0;
        size_t temp = 0;
        char *string = NULL;
        int i = 0;
        for (; i < n; i++) {
            lungsir = getline(&string, &temp, in);
            string = adauga_dolar(string, &lungsir);
            if (!string) return -1;
            insereaza_cuvant(arbore, string);
            free(string);
            string = NULL;
        }
        // Scriem numarul de frunze, numarul de sufixe de lungime k
        // si numarul de descendenti maxim pentru un singur nod
        fprintf(out, "%d\n", numara_frunze(arbore));
        fprintf(out, "%d\n", numara_k_sufixe(arbore, 0, k));
        fprintf(out, "%d\n", descendenti_maximi(arbore));
        fclose(in);
        fclose(out);
    } else if (!strcmp(argv[1], "-c3")) {
        /*---------------------- Operatia 3 --------------------------*/

        // Similar cu operatia 1
        FILE *in = fopen(argv[2], "r");
        FILE *out = fopen(argv[3], "w");
        int n = 0, m = 0;
        fscanf(in, "%d", &n);
        fscanf(in, "%d", &m);
        fgetc(in);
        int lungsir = 0;
        size_t temp = 0;
        char *string = NULL;
        int i = 0;
        for (; i < n; i++) {
            lungsir = getline(&string, &temp, in);
            string = adauga_dolar(string, &lungsir);
            if (!string) return -1;
            insereaza_cuvant(arbore, string);
            free(string);
            string = NULL;
        }
        // Dupa ce citim n cuvinte pe care le inseram in arborele de sufixe,
        // citim m sufixe si verificam care dintre ele sunt in arbore
        i = 0;
        for (; i < m; i++) {
            lungsir = getline(&string, &temp, in);
            string = adauga_dolar(string, &lungsir);
            if (!string) return -1;
            fprintf(out, "%d\n", exista_sufix(arbore, string));
            free(string);
            string = NULL;
        }
        fclose(in);
        fclose(out);
    } else if (!strcmp(argv[1], "-c4")) {
        /*----------------------- Operatia 4 --------------------------*/

        // Similar cu operatia 1
        FILE *in = fopen(argv[2], "r");
        FILE *out = fopen(argv[3], "w");
        int n = 0;
        fscanf(in, "%d", &n);
        fgetc(in);
        int lungsir = 0;
        size_t temp = 0;
        char *string = NULL;
        int i = 0;
        for (; i < n; i++) {
            lungsir = getline(&string, &temp, in);
            string = adauga_dolar(string, &lungsir);
            if (!string) return -1;
            insereaza_cuvant(arbore, string);
            free(string);
            string = NULL;
        }

        // Inainte de a printa arborele, compresam caile
        compreseaza_cai(arbore);
        bfs(arbore, out);
        fprintf(out, "\n");
        fclose(in);
        fclose(out);
    }
    distruge_sufix(&arbore);
    return 0;
}