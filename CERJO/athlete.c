#include "athlete.h" // Inclusion de l'en-tête définissant la structure athlete et les prototypes de fonctions associés.
#include "performance.h"// Inclusion de l'en-tête définissant la structure performance et les prototypes de fonctions associés.
#include <dirent.h> // Inclusion de l'en-tête pour la manipulation des répertoires.
//Antoine Dubois
//Benjamin Martin
//Lucas Moreau
//Alexandre Girard
//Julien Lefebvre
//Mathieu Dupont
//Gabriel Mercier
//Thomas Renaud
//Nicolas Laurent
//Hugo Garcia

// Fonction pour créer un nouvel athlète.
athlete *create_athlete(char *name){ // Allocation de mémoire pour une nouvelle structure athlete.
    athlete *ath = malloc(sizeof(athlete));
    if (!ath) {// Vérification si l'allocation a échoué.
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strcpy(ath->name, name);// Copie du nom de l'athlète dans la structure athlete.
    ath->performances = malloc(sizeof(list_perf));// Allocation de mémoire pour la liste des performances.
    ath->nbPerformances = 0;// Initialisation du nombre de performances à 0.
    ath->next = NULL;// Initialisation du nombre de performances à 0.
    ath->prev = NULL; // Initialisation du pointeur prev à NULL.
    //printf("Athlete created\n");
    return ath;
}
// Fonction pour initialiser les performances d'un athlète.
void init_athlete_perf(athlete *ath, list_perf *performances){
    ath->performances = performances;// Assignation de la liste des performances à l'athlète
    performance *cursor = ath->performances->first;// Initialisation du curseur à la première performance.
    while(cursor != NULL){// Boucle pour compter le nombre de performances.
        ath->nbPerformances++;// Incrémentation du compteur de performances
        cursor = cursor->next; // Passage à la performance suivante.
    }
    //printf("Athlete performances initialized\n");
}
// Fonction pour ajouter un athlète à la liste des athlètes
void add_athlete(list_athletes *athletes, athlete *ath){
    if(athletes->first == NULL){// Vérification si la liste est vide.
        athletes->first = ath;
        athletes->last = ath;
        //printf("First athlete added to list\n");
    } else {
        athletes->last->next = ath;// Ajout de l'athlète à la fin de la liste.
        ath->prev = athletes->last; // Mise à jour du pointeur prev de l'athlète ajouté.
        athletes->last = ath;// Mise à jour du pointeur last de la liste.
        //printf("Athlete added to list\n");
    }
    athletes->nbAthletes++;// Incrémentation du compteur d'athlètes.
}
// Fonction pour ajouter un athlète à la liste sans le joindre à d'autres structures.
void add_athlete_without_join(list_athletes *athletes, athlete *ath){
    if(athletes->first == NULL){ // Vérification si la liste est vide
        athlete *ath2 = create_athlete(ath->name);
        ath2->performances = ath->performances;
        ath2->nbPerformances = ath->nbPerformances;
        athletes->first = ath2;
        athletes->last = ath2;
        //printf("First athlete added to list\n");
    } else {
        athlete *ath2 = create_athlete(ath->name);
        ath2->performances = ath->performances;
        ath2->nbPerformances = ath->nbPerformances;
        athletes->last->next = ath2;
        ath2->prev = athletes->last;
        athletes->last = ath2;
        //printf("Athlete added to list\n");

    }
    athletes->nbAthletes++; // Incrémentation du compteur d'athlètes.
}


void delete_athlete(list_athletes *athletes, athlete *ath){
    printf("a"); // Affiche 'a' (probablement pour le débogage).
    if(athletes->first == ath){// Vérifie si l'athlète à supprimer est le premier de la liste.
        athletes->first = ath->next;// Met à jour le pointeur `first` pour qu'il pointe vers le deuxième athlète.
        athletes->first->prev = NULL;
        //printf("First athlete deleted\n");
    } else if(athletes->last == ath){ // Vérifie si l'athlète à supprimer est le dernier de la liste.
        athletes->last = ath->prev;// Met à jour le pointeur `last` pour qu'il pointe vers l'avant-dernier athlète.
        athletes->last->next = NULL;
        //printf("Last athlete deleted\n");
    } else {
        ath->prev->next = ath->next;// Lie l'athlète précédent avec l'athlète suivant
        ath->next->prev = ath->prev;// Lie l'athlète suivant avec l'athlète précédent.
        //printf("Athlete deleted\n");
    }
}


list_athletes *init_all_athletes(){
    list_athletes *athletes = malloc(sizeof(list_athletes));// Allocation de mémoire pour la liste des athlètes.
    if (!athletes) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    athletes->first = NULL;
    athletes->last = NULL;
    athletes->nbAthletes = 0;

    DIR *d;// Déclare un pointeur pour le répertoire
    struct dirent *dir;// Déclare une structure pour stocker les informations sur les fichiers.
    d = opendir(DATA_PATH);// Ouvre le répertoire spécifié par DATA_PATH
    if (d) {
        while ((dir = readdir(d)) != NULL) {// Parcourt tous les fichiers du répertoire.
            if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){// Ignore les entrées spéciales "." et "..".
                char *name = malloc(sizeof(char) * 100);
                if (!name) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(name, dir->d_name);// Copie le nom du fichier dans la variable `name`.
                for(size_t i = 0; i < strlen(name); i++){
                    if(name[i] == '_'){
                        name[i] = ' ';
                    }
                    if(name[i] == '.'){
                        name[i] = '\0';
                    }
                }
                athlete *ath = create_athlete(name);
                char path[100];// Déclare une variable pour stocker le chemin complet du fichier.
                strcpy(path, DATA_PATH);// Copie le chemin de base dans `path`.
                strcat(path, dir->d_name);// Concatène le nom du fichier au chemin de base.
                list_perf *performances = get_performances_from_file(path); // Récupère les performances à partir du fichier
                init_athlete_perf(ath, performances); // Initialise les performances de l'athlète.
                add_athlete(athletes, ath);// Ajoute l'athlète à la liste.
            }
        }
        closedir(d);
    }

    //printf("All athletes initialized\n");
    return athletes;

}

void display_athletes(list_athletes *athletes){
    athlete *cursor = athletes->first;
    while(cursor != NULL){// Boucle pour parcourir tous les athlètes.
        printf("Athlete : %s\n", cursor->name);
        performance *cursor2 = cursor->performances->first;// Initialise le curseur pour les performances.
        while(cursor2 != NULL){// Boucle pour parcourir toutes les performances de l'athlète.
            printf("Performance : %d\n", cursor2->type);// Affiche le type de performance.
            cursor2 = cursor2->next;// Passe à la performance suivante.
        }
        cursor = cursor->next;// Passe à l'athlète suivant.
    }
    //printf("All athletes displayed\n");
}


void create_athlete_file(athlete *ath) {
    char name[100] = "";
    strcpy(name, ath->name);// Boucle pour traiter le nom.
    for(size_t i = 0; i < strlen(name); i++){
        if(name[i] == ' '){
            name[i] = '_';
        }
    }
    char path[150]; // Initialise une chaîne pour le chemin du fichier.
    strcpy(path, DATA_PATH);
    strcat(path, name);
    strcat(path, ".txt");// Ajoute l'extension ".txt" au chemin.
    FILE *file = fopen(path, "w");//Ouvre le fichier en mode écriture
    if (!file) {// Vérifie si l'ouverture a échoué
        perror("fopen");
        exit(EXIT_FAILURE);
    }
    fclose(file);
    //printf("File created\n");
}


athlete *find_by_name(list_athletes *athletes,char *name){
    name[strlen(name)-1] = '\0';// Retire le dernier caractère de la chaîne (souvent un saut de ligne).// Retire le dernier caractère de la chaîne (souvent un saut de ligne).
    athlete *cursor = athletes->first;
    while(cursor != NULL){
        if(strcmp(cursor->name, name) == 0){// Compare le nom courant avec le nom recherché.
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;

}


void athlete_performance_summary(list_athletes *athletes, char *name, type_perf type) {
    athlete *ath = find_by_name(athletes, name);// Trouve l'athlète par son nom.
    if (ath != NULL) {
        performance *perf = best_performance(ath->performances, type);
        printf("Best time: %d:%d:%d\n", perf->result.tm_hour, perf->result.tm_min, perf->result.tm_sec);
        perf = worst_performance(ath->performances, type);
        printf("Worst time: %d:%d:%d\n", perf->result.tm_hour, perf->result.tm_min, perf->result.tm_sec);
        struct tm avg = average_performance(ath->performances, type);
        printf("Average time: %d:%d:%d\n", avg.tm_hour, avg.tm_min, avg.tm_sec);
    } else {
        printf("Athlete not found\n");
    }
}

void best_athletes_for_jo(list_athletes *athletes, type_perf type) {
    list_athletes *best_athletes = best_three_athletes(athletes, type);
    athlete *ath = best_athletes->first;
    printf("\nBest athletes ");
    while (ath != NULL) {
        printf("\n%s", ath->name);
        ath = ath->next;
    }
}

//Cette fonction affiche la progression d'un athlète entre deux dates pour un type de performance donné.
void athlete_progression(athlete *ath, type_perf type, struct tm date1, struct tm date2) {
    printf("Progression for %s\n", ath->name);
    if (ath != NULL) {
        struct tm diff = performance_difference(ath->performances, type, date1, date2);
        printf("Time difference: %d:%d:%d\n", diff.tm_hour, diff.tm_min, diff.tm_sec);
    } else {
        printf("Athlete not found\n");
    }
}

int compare_athletes(athlete *a, athlete *b, type_perf type) {
    struct tm avg_a = average_performance(a->performances, type);
    struct tm avg_b = average_performance(b->performances, type);

    if (avg_a.tm_hour < avg_b.tm_hour) {// Compare les heures des performances moyennes.
        return -1;// a est meilleur que b.
    } else if (avg_a.tm_hour > avg_b.tm_hour) {
        return 1;
    } else {// Si les heures sont égales.
        if (avg_a.tm_min < avg_b.tm_min) {
            return -1;
        } else if (avg_a.tm_min > avg_b.tm_min) {
            return 1;
        } else {
            if (avg_a.tm_sec < avg_b.tm_sec) {// Si les sec sont égales.
                return -1;
            } else if (avg_a.tm_sec > avg_b.tm_sec) {
                return 1;
            } else {
                return 0;
            }
        }
    }
}

list_athletes *best_three_athletes(list_athletes *athletes, type_perf type) {
    list_athletes *best_athletes = malloc(sizeof(list_athletes));
    if (!best_athletes) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    best_athletes->first = NULL;
    best_athletes->last = NULL;
    best_athletes->nbAthletes = 0;

    athlete *cursor = athletes->first;
    int added_count = 0;
// Ajoute le premier athlète si non nul et ayant des performances du type donné.
    if (cursor != NULL && athlete_has_perf(cursor, type)) {
        athlete *new_athlete = malloc(sizeof(athlete));
        if (!new_athlete) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        *new_athlete = *cursor;
        new_athlete->next = NULL;
        new_athlete->prev = NULL;

        best_athletes->first = new_athlete;
        best_athletes->last = new_athlete;
        best_athletes->nbAthletes++;// Incrémente le compteur d'athlètes.
        added_count++;// Incrémente le compteur des athlètes ajoutés.
    }

    cursor = cursor->next;// Passe à l'athlète suivant.


    while (cursor != NULL && added_count < 3) {// Boucle pour ajouter les trois meilleurs athlètes.
        if (athlete_has_perf(cursor, type)) { // Vérifie si l'athlète a des performances du type donné.
            athlete *new_athlete = malloc(sizeof(athlete));// Alloue de la memoire pour nouvelle athlete.
            if (!new_athlete) {
                perror("malloc");
                exit(EXIT_FAILURE);
            }
            *new_athlete = *cursor;
            new_athlete->next = NULL;
            new_athlete->prev = NULL;

            athlete *current = best_athletes->first;// Initialise le curseur au premier athlète de la liste des meilleurs athlètes.
            athlete *previous = NULL;// Initialise le pointeur `previous` à NULL
            while (current != NULL && compare_athletes(new_athlete, current, type) < 0) {
                previous = current;// Met à jour `previous` avec le courant.
                current = current->next;
            }

            if (previous == NULL) {// Si l'insertion se fait en tête de liste
                new_athlete->next = best_athletes->first;// Lie le nouvel athlète au premier de la liste.
                if (best_athletes->first)// Si la liste n'était pas vide.
                    best_athletes->first->prev = new_athlete;
                best_athletes->first = new_athlete;
            } else {// Si l'insertion se fait ailleurs dans la liste.
                new_athlete->next = current;
                new_athlete->prev = previous;
                previous->next = new_athlete;
                if (current)
                    current->prev = new_athlete;// Met à jour le pointeur `prev` du courant.
            
            }

            if (best_athletes->nbAthletes < 3) // Vérifie si la liste des meilleurs athlètes a moins de trois éléments.
                best_athletes->nbAthletes++; // Incrémente le compteur d'athlètes.
            added_count++;// Incrémente le compteur des athlètes ajoutés.
        }
        cursor = cursor->next;// Passe à l'athlète suivant
    }

    return best_athletes; // Retourne la liste des trois meilleurs athlètes
}

athlete *first_with_perf(list_athletes *athletes, type_perf type) {
    athlete *cursor = athletes->first;// Initialise le curseur au premier athlète de la liste.
    while (cursor != NULL) {// Boucle pour parcourir tous les athlètes.
        performance *perf = cursor->performances->first; // Initialise le curseur pour les performances.
        while (perf != NULL) {// Boucle pour parcourir toutes les performances de l'athlète.
            if (perf->type == type) { // Vérifie si la performance est du type donné.
                return cursor; // Retourne l'athlète si une performance du type donné est trouvée.
            }
            perf = perf->next;// Passe à la performance suivante.
        }
        cursor = cursor->next;
    }
    return NULL;
}

bool athlete_has_perf(athlete *ath, type_perf type) {
    performance *perf = ath->performances->first;// Initialise le curseur pour les performances.
    while (perf != NULL) { // Boucle pour parcourir toutes les performances de l'athlète.
        if (perf->type == type) {// Vérifie si la performance est du type donné.
            return true;// Retourne vrai si une performance du type donné est trouvée.
        }
        perf = perf->next;// Passe à la performance suivante.
    }
    return false;
}