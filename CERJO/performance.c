#include "performance.h" // Inclusion de l'en-tête définissant la structure performance et les prototypes de fonctions associés.
#include <malloc.h> // Inclusion de l'en-tête pour l'allocation dynamique de mémoire.
#include <stdio.h> // Inclusion de l'en-tête pour les fonctions d'entrée/sortie standard.


performance *create_performance(struct tm date, type_perf type, struct tm result, int relai_info){
    performance *perf = malloc(sizeof(performance)); // Allocation de mémoire pour une nouvelle structure performance.
    if (!perf) { // Vérifie si l'allocation a échoué.
        perror("malloc"); // Affiche un message d'erreur si l'allocation échoue.
        exit(EXIT_FAILURE); // Termine le programme en cas d'échec de l'allocation.
    }

    perf->date = date; // Assigne la date de la performance.
    perf->type = type; // Assigne le type de performance.
    perf->relai_info = relai_info; // Assigne les informations de relais.
    perf->result = result; // Assigne le résultat de la performance.
    perf->next = NULL; // Initialise le pointeur next à NULL.
    perf->prev = NULL; // Initialise le pointeur prev à NULL.
    //printf("Performance created\n");
    return perf; // Retourne le pointeur vers la nouvelle structure performance.
}

void add_performance(list_perf *performances, performance *performance){
    if(performances == NULL || performances->first == NULL){ // Vérifie si la liste est vide.

        performances->first = performance; // Assigne la performance comme premier élément de la liste.
        performances->last = performance; // Assigne la performance comme dernier élément de la liste.
        //printf("First performance added to list\n");
    } else { // Si la liste n'est pas vide.
        performances->last->next = performance; // Ajoute la performance à la fin de la liste.
        performance->prev = performances->last; // Met à jour le pointeur prev de la performance ajoutée.
        performances->last = performance; // Met à jour le pointeur last de la liste.
        //printf("Performance added to list\n");
    }

}


void add_performance_without_join(list_perf *performances, performance *perf){
    if(performances->first == NULL){ // Vérifie si la performance à supprimer est la première de la liste.
        performance *new_performance = create_performance(perf->date, perf->type, perf->result, perf->relai_info); // Crée une nouvelle performance avec les mêmes détails.
        performances->first = new_performance; // Assigne la nouvelle performance comme premier élément de la liste.
        performances->last = new_performance; // Assigne la nouvelle performance comme dernier élément de la liste.
        //printf("First performance added to list\n");
    } else {
        performance *new_performance = create_performance(perf->date, perf->type, perf->result, perf->relai_info);
        new_performance->prev = performances->last; // Met à jour le pointeur prev de la nouvelle performance.
        performances->last->next = new_performance; // Lie la nouvelle performance à la fin de la liste.
        performances->last = new_performance; // Met à jour le pointeur last de la liste.

        //printf("Performance added to list\n");
    }
}


void delete_performance(list_perf *performances, performance *performance){
    if(performances->first == performance){ // Vérifie si la performance à supprimer est la première de la liste.
        performances->first = performance->next; // Met à jour le pointeur first pour qu'il pointe vers la performance suivante.
    }
    if(performances->last == performance){ // Vérifie si la performance à supprimer est la dernière de la liste.
        performances->last = performance->prev; // Met à jour le pointeur last pour qu'il pointe vers la performance précédente.
    }
    if(performance->prev != NULL){ // Vérifie si la performance à supprimer a une performance précédente.
        performance->prev->next = performance->next; // Met à jour le pointeur next de la performance précédente.
    }
    if(performance->next != NULL){ // Vérifie si la performance à supprimer a une performance suivante.
        performance->next->prev = performance->prev; // Met à jour le pointeur prev de la performance suivante.
    }
    free(performance); // Libère la mémoire allouée pour la performance supprimée.
}



list_perf *get_performances_from_file(char *filename){

    FILE *file = fopen(filename, "r");// Ouvre le fichier en mode lecture.
    if (!file) { // Vérifie si l'ouverture a échoué.
        perror("fopen");  // Affiche un message d'erreur si l'ouverture échoue.
        exit(EXIT_FAILURE); // Termine le programme en cas d'échec de l'ouverture.
    }

    list_perf *performances = malloc(sizeof(list_perf)); // Alloue de la mémoire pour une nouvelle liste de performances.
    performances->first = NULL; // Initialise le pointeur first à NULL.
    performances->last = NULL; // Initialise le pointeur last à NULL

    char line[100]; // Déclare un tableau pour stocker chaque ligne lue du fichier.

    while (fgets(line, sizeof(line), file) != NULL) { // Lit chaque ligne du fichier.
        struct tm date;
        char type_temp[10];// Déclare un tableau pour stocker le type de performance sous forme de chaîne.
        struct tm result;
        int relai_info = 0; // Initialise les informations de relais à 0.

        sscanf(line, "%d/%d/%d %s %d:%d:%d %d;", &date.tm_mday, &date.tm_mon, &date.tm_year, type_temp, &result.tm_hour, &result.tm_min, &result.tm_sec, &relai_info); // Analyse la ligne pour extraire les informations de la performance.

        type_perf type; // Convertit la chaîne de type en type_perf.
        if(strcmp(type_temp, "100m") == 0){
            type = TYPE_100_M;
        } else if(strcmp(type_temp, "400m") == 0){
            type = TYPE_400_M;
        } else if(strcmp(type_temp, "5000m") == 0){
            type = TYPE_5000_M;
        } else if(strcmp(type_temp, "Marathon") == 0){
            type = TYPE_MARATHON;
        } else if(strcmp(type_temp, "Relai") == 0){
            type = TYPE_RELAI;
        } else {
            printf("Error while parsing the line\n");  // Affiche un message d'erreur en cas de type inconnu.
            exit(EXIT_FAILURE);
        }

        performance *performance = create_performance(date, type, result, relai_info); // Crée une nouvelle performance avec les informations extraites.
        add_performance(performances, performance); // Ajoute la performance à la liste des performances.
    }

    fclose(file);
    return performances;
}

void save_performances_to_file(char *filename, list_perf *performances) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("fopen"); // Affiche un message d'erreur si l'ouverture échoue.
        exit(EXIT_FAILURE);
    }


    performance *cursor = performances->first; // Initialise le curseur au premier élément de la liste.
    while (cursor != NULL) {  // Boucle pour parcourir toutes les performances.
        char *type; // Déclare un pointeur pour le type de performance sous forme de chaîne.
        switch (cursor->type) { // Convertit le type de performance en chaîne de caractères.
            case TYPE_100_M:
                type = "100m";
                break;
            case TYPE_400_M:
                type = "400m";
                break;
            case TYPE_5000_M:
                type = "5000m";
                break;
            case TYPE_MARATHON:
                type = "Marathon";
                break;
            case TYPE_RELAI:
                type = "Relai";
                break;
        }
        if (cursor->type == TYPE_RELAI) { // Écrit les informations de la performance dans le fichier.
            fprintf(file, "%d/%d/%d %s %d:%d:%d %d;\n", cursor->date.tm_mday, cursor->date.tm_mon, cursor->date.tm_year,type, cursor->result.tm_hour, cursor->result.tm_min, cursor->result.tm_sec, cursor->relai_info);
        } else {
            fprintf(file, "%d/%d/%d %s %d:%d:%d;\n", cursor->date.tm_mday, cursor->date.tm_mon, cursor->date.tm_year,type, cursor->result.tm_hour, cursor->result.tm_min, cursor->result.tm_sec);
        }
        cursor = cursor->next; // Passe à la performance suivante.
    }
    fclose(file);
}

void *join_perfs(list_perf *performances1, list_perf *performances2){
    if(performances1->first == NULL){
        performances1->first = performances2->first; // Assigne le premier élément de la deuxième liste comme premier élément de la première liste.
        performances1->last = performances2->last; // Assigne le dernier élément de la deuxième liste comme dernier élément de la première liste.
    } else {
        performances1->last->next = performances2->first; // Lire le dernier élément de la première liste au premier élément de la deuxième liste.
        performances2->first->prev = performances1->last; // Met à jour le pointeur prev du premier élément de la deuxième liste.
        performances1->last = performances2->last;
    }
    return NULL;
}

list_perf *copy_list(list_perf *perfs){
    list_perf *new_perfs = malloc(sizeof(list_perf));
    new_perfs->first = NULL;
    new_perfs->last = NULL;
    performance *cursor = perfs->first;
    while(cursor != NULL){ // Boucle pour parcourir tous les éléments de la liste.
        performance *new_perf = create_performance(cursor->date, cursor->type, cursor->result, cursor->relai_info); // Crée une nouvelle performance avec les mêmes informations.
        add_performance(new_perfs, new_perf); // Ajoute la nouvelle performance à la nouvelle liste.
        cursor = cursor->next;
    }
    return new_perfs;
}


list_perf *order_list_by_date(list_perf *performances){
    performance *cursor = performances->first; // Initialise le curseur au premier élément de la liste.
    performance *perf_array[100];
    int i = 0; // Initialise un compteur
    while(cursor != NULL){   
        perf_array[i] = cursor;
        cursor = cursor->next;
        i++;
    }
    // Tri des performances par date en utilisant une version simplifiée du tri à bulles.
    for(int j = 0; j < i; j++){
        for(int k = 0; k < i; k++){
            if(perf_array[j]->date.tm_year > perf_array[k]->date.tm_year){
                performance *temp = perf_array[j];
                perf_array[j] = perf_array[k];
                perf_array[k] = temp;
            } else if(perf_array[j]->date.tm_year == perf_array[k]->date.tm_year){
                if(perf_array[j]->date.tm_mon > perf_array[k]->date.tm_mon){
                    performance *temp = perf_array[j];
                    perf_array[j] = perf_array[k];
                    perf_array[k] = temp;
                } else if(perf_array[j]->date.tm_mon == perf_array[k]->date.tm_mon){
                    if(perf_array[j]->date.tm_mday > perf_array[k]->date.tm_mday){
                        performance *temp = perf_array[j];
                        perf_array[j] = perf_array[k];
                        perf_array[k] = temp;
                    }
                }
            }
        }
    }
    list_perf *ordered_performances = malloc(sizeof(list_perf));   // Crée une nouvelle liste triée.
    ordered_performances->first = perf_array[0];
    ordered_performances->last = perf_array[i - 1];
    for(int j = 0; j < i - 1; j++){
        perf_array[j]->next = perf_array[j + 1];
    }
    perf_array[i - 1]->next = NULL;
    return ordered_performances;
}


list_perf *order_list_by_type(list_perf *performances){
    performance *cursor = performances->first;
    performance *perf_array[100];
    int i = 0;
    while(cursor != NULL){
        perf_array[i] = cursor;
        cursor = cursor->next;
        i++;
    }
    for(int j = 0; j < i; j++){ // Tri des performances par type en utilisant une version simplifiée du tri à bulles.
        for(int k = 0; k < i; k++){
            if(perf_array[j]->type > perf_array[k]->type){
                performance *temp = perf_array[j];
                perf_array[j] = perf_array[k];
                perf_array[k] = temp;
            }
        }
    }
    list_perf *ordered_performances = malloc(sizeof(list_perf));
    ordered_performances->first = perf_array[0];
    ordered_performances->last = perf_array[i - 1];
    for(int j = 0; j < i - 1; j++){
        perf_array[j]->next = perf_array[j + 1];
    }
    perf_array[i - 1]->next = NULL;
    return ordered_performances;
}

list_perf *order_list_by_result(list_perf *performances){
    performance *cursor = performances->first;
    performance *perf_array[100];
    int i = 0;
    while(cursor != NULL){
        perf_array[i] = cursor;
        cursor = cursor->next;
        i++;
    }
    for(int j = 0; j < i; j++){  // Tri des performances par résultat en utilisant une version simplifiée du tri à bulles.
        for(int k = 0; k < i; k++){
            if(perf_array[j]->result.tm_hour < perf_array[k]->result.tm_hour){
                performance *temp = perf_array[j];
                perf_array[j] = perf_array[k];
                perf_array[k] = temp;
            } else if(perf_array[j]->result.tm_hour == perf_array[k]->result.tm_hour){
                if(perf_array[j]->result.tm_min < perf_array[k]->result.tm_min){
                    performance *temp = perf_array[j];
                    perf_array[j] = perf_array[k];
                    perf_array[k] = temp;
                } else if(perf_array[j]->result.tm_min == perf_array[k]->result.tm_min){
                    if(perf_array[j]->result.tm_sec < perf_array[k]->result.tm_sec){
                        performance *temp = perf_array[j];
                        perf_array[j] = perf_array[k];
                        perf_array[k] = temp;
                    }
                }
            }
        }
    }
    list_perf *ordered_performances = malloc(sizeof(list_perf)); // Crée une nouvelle liste triée.
    ordered_performances->first = perf_array[0];
    ordered_performances->last = perf_array[i - 1];
    for(int j = 0; j < i - 1; j++){
        perf_array[j]->next = perf_array[j + 1];
    }
    perf_array[i - 1]->next = NULL;
    return ordered_performances;
}


void display_performance(performance *performance){ // Fonction pour afficher les détails d'une performance
    char *type;
    switch(performance->type){ // Détermine la représentation sous forme de chaîne de caractères du type de performance
        case TYPE_100_M:
            type = "100m";
            break;
        case TYPE_400_M:
            type = "400m";
            break;
        case TYPE_5000_M:
            type = "5000m";
            break;
        case TYPE_MARATHON:
            type = "Marathon";
            break;
        case TYPE_RELAI:
            type = "Relai";
            break;
    }
    printf("Date: %02d/%02d/%04d, Type: %s, Result: %02d:%02d:%02d\n", performance->date.tm_mday, performance->date.tm_mon, performance->date.tm_year, type, performance->result.tm_hour, performance->result.tm_min, performance->result.tm_sec);
    if (performance->type == TYPE_RELAI){
        printf(" -> Position of the torchbearer : %d/4\n", performance->relai_info);
    }
}


void display_performances(list_perf *performances) {
    printf("\n\n - Ordering methods - \n\n");

    char choice1;
    do {
        printf("Do you want to order by date (y/n) : ");
        scanf(" %c", &choice1);
    } while (choice1 != 'y' && choice1 != 'n');


    if (choice1 == 'y') {
        performances = order_list_by_date(performances);
    }else{
        char choice2;
        do {
            printf("Do you want to order by type (y/n) : ");
            scanf(" %c", &choice2);
        } while (choice2 != 'y' && choice2 != 'n');

        if (choice2 == 'y') {
            performances = order_list_by_type(performances);
        }else{
            char choice3;
            do {
                printf("Do you want to order by result (y/n) : ");
                scanf(" %c", &choice3);
            } while (choice3 != 'y' && choice3 != 'n');

            if (choice3 == 'y') {
                performances = order_list_by_result(performances);
            }
        }


    }

 // Cette fonction permet à l'utilisateur de choisir entre trois méthodes de tri : par date, par type ou par résultat.
    performance *cursor = performances->first;
    while (cursor != NULL) {
        display_performance(cursor);
        cursor = cursor->next;
    }
}




void display_performances2(all_perfs performances) { // Cette fonction permet à l'utilisateur de choisir entre trois méthodes de tri pour chaque athlète : par date, par type ou par résultat.
    printf("\n\n - Ordering methods - \n\n");

    char choice1;
    do {
        printf("Do you want to order by date (y/n) : ");
        scanf(" %c", &choice1);
    } while (choice1 != 'y' && choice1 != 'n');

    if (choice1 == 'y') {
        for(int i = 0; i < performances.nbAthletes; i++){
            printf("Athlete : %s\n", performances.names[i]);
            list_perf *ordered_performances = order_list_by_date(performances.performances[i]);
            performance *cursor = ordered_performances->first;
            while (cursor != NULL) {
                display_performance(cursor);
                cursor = cursor->next;
            }
        }
    }else{
        char choice2;
        do {
            printf("Do you want to order by type (y/n) : ");
            scanf(" %c", &choice2);
        } while (choice2 != 'y' && choice2 != 'n');

        if (choice2 == 'y') {
            for(int i = 0; i < performances.nbAthletes; i++){
                printf("Athlete : %s\n", performances.names[i]);
                list_perf *ordered_performances = order_list_by_type(performances.performances[i]);
                performance *cursor = ordered_performances->first;
                while (cursor != NULL) {
                    display_performance(cursor);
                    cursor = cursor->next;
                }
            }
        }else{
            char choice3;
            do {
                printf("Do you want to order by result (y/n) : ");
                scanf(" %c", &choice3);
            } while (choice3 != 'y' && choice3 != 'n');

            if (choice3 == 'y') {
                for(int i = 0; i < performances.nbAthletes; i++){
                    printf("Athlete : %s\n", performances.names[i]);
                    list_perf *ordered_performances = order_list_by_result(performances.performances[i]);
                    performance *cursor = ordered_performances->first;
                    while (cursor != NULL) {
                        display_performance(cursor);
                        cursor = cursor->next;
                    }
                }
            }
        }
    }
}

all_perfs get_all_performances(){ //Cette fonction récupère les performances de tous les athlètes à partir des fichiers dans un répertoire spécifié.   
    DIR *d;
    struct dirent *dir;
    d = opendir(DATA_PATH);
    all_perfs all_perfs;
    all_perfs.names = malloc(sizeof(char *) * 100);
    all_perfs.performances = malloc(sizeof(list_perf *) * 100);
    all_perfs.nbAthletes = 0;
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
                char *name = malloc(sizeof(char) * 100);
                if (!name) {
                    perror("malloc");
                    exit(EXIT_FAILURE);
                }
                strcpy(name, dir->d_name);

                for(size_t i = 0; i < strlen(name); i++){
                    if(name[i] == '_'){
                        name[i] = ' ';
                    }
                    if(name[i] == '.'){
                        name[i] = '\0';
                    }
                }
                all_perfs.names[all_perfs.nbAthletes] = name;
                char path[150];
                strcpy(path, DATA_PATH);
                strcat(path, dir->d_name);
                list_perf *performances = get_performances_from_file(path);
                all_perfs.performances[all_perfs.nbAthletes] = performances;
                all_perfs.nbAthletes++;
            }
        }
    }
    return all_perfs;

}


list_perf *sort_performances_by_type_athlete(list_perf *performances, type_perf type){  //Cette fonction trie les performances d'un athlète selon un type de performance spécifié.
    list_perf *sorted_performances = malloc(sizeof(list_perf));
    sorted_performances->first = NULL;
    sorted_performances->last = NULL;
    performance *cursor = performances->first;

    while(cursor != NULL){
        if(cursor->type == type){
            printf("Adding performance\n");
            add_performance_without_join(sorted_performances, cursor);
        }
        cursor = cursor->next;
    }

    return sorted_performances;
}

list_perf *sort_performances_by_date_athlete(list_perf *performances, struct tm date){ //Cette fonction trie les performances d'un athlète par date spécifiée.
    list_perf *sorted_performances = malloc(sizeof(list_perf));
    sorted_performances->first = NULL;
    sorted_performances->last = NULL;
    performance *cursor = performances->first;
    while(cursor != NULL){
        if(cursor->date.tm_mday == date.tm_mday && cursor->date.tm_mon == date.tm_mon && cursor->date.tm_year == date.tm_year){
            add_performance_without_join(sorted_performances, cursor);
        }
        cursor = cursor->next;
    }
    return sorted_performances;
}

list_perf *sort_performances_by_type_and_date_athlete(list_perf *performances, type_perf type, struct tm date){ //Cette fonction trie les performances d'un athlète par type et par date spécifiée.
    list_perf *sorted_performances = malloc(sizeof(list_perf)); //Cette fonction trie les performances d'un athlète par type et par date spécifiée.
    sorted_performances->first = NULL;
    sorted_performances->last = NULL;
    performance *cursor = performances->first;
    while(cursor != NULL){
        if(cursor->date.tm_mday == date.tm_mday && cursor->date.tm_mon == date.tm_mon && cursor->date.tm_year == date.tm_year && cursor->type == type){
            add_performance_without_join(sorted_performances, cursor);
        }
        cursor = cursor->next;
    }
    return sorted_performances;
}


all_perfs sort_performances_by_type(all_perfs performances, type_perf type){ //Cette fonction trie toutes les performances des athlètes par type de performance spécifié.
    all_perfs sorted_performances;
    sorted_performances.names = malloc(sizeof(char *) * 100);
    sorted_performances.performances = malloc(sizeof(list_perf *) * 100);
    sorted_performances.nbAthletes = 0;
    for(int i = 0; i < performances.nbAthletes; i++){
        list_perf *sorted_performances_athlete = sort_performances_by_type_athlete(performances.performances[i], type);
        if(sorted_performances_athlete->first != NULL){
            sorted_performances.names[sorted_performances.nbAthletes] = performances.names[i];
            sorted_performances.performances[sorted_performances.nbAthletes] = sorted_performances_athlete;
            sorted_performances.nbAthletes++;
        }
    }
    return sorted_performances;
}

all_perfs sort_performances_by_date(all_perfs performances, struct tm date){ // Cette fonction trie toutes les performances des athlètes par date spécifiée.
    all_perfs sorted_performances;
    sorted_performances.names = malloc(sizeof(char *) * 100);
    sorted_performances.performances = malloc(sizeof(list_perf *) * 100);
    sorted_performances.nbAthletes = 0;
    for(int i = 0; i < performances.nbAthletes; i++){
        list_perf *sorted_performances_athlete = sort_performances_by_date_athlete(performances.performances[i], date);
        if(sorted_performances_athlete->first != NULL){
            sorted_performances.names[sorted_performances.nbAthletes] = performances.names[i];
            sorted_performances.performances[sorted_performances.nbAthletes] = sorted_performances_athlete;
            sorted_performances.nbAthletes++;
        }
    }
    return sorted_performances;
}

all_perfs sort_performances_by_type_and_date(all_perfs performances, type_perf type, struct tm date){ //Cette fonction trie toutes les performances des athlètes par type et par date spécifiée.
    all_perfs sorted_performances;
    sorted_performances.names = malloc(sizeof(char *) * 100);
    sorted_performances.performances = malloc(sizeof(list_perf *) * 100);
    sorted_performances.nbAthletes = 0;
    for(int i = 0; i < performances.nbAthletes; i++){
        list_perf *sorted_performances_athlete = sort_performances_by_type_and_date_athlete(performances.performances[i], type, date);
        if(sorted_performances_athlete->first != NULL){
            sorted_performances.names[sorted_performances.nbAthletes] = performances.names[i];
            sorted_performances.performances[sorted_performances.nbAthletes] = sorted_performances_athlete;
            sorted_performances.nbAthletes++;
        }
    }
    return sorted_performances;
}


performance *best_performance(list_perf *performances, type_perf type) { //Cette fonction recherche la meilleure performance d'un type spécifié parmi toutes les performances données.
    performance *best = NULL;
    performance *cursor = performances->first;
    while (cursor != NULL) {
        if (cursor->type == type && (best == NULL || cursor->result.tm_sec < best->result.tm_sec)) {
            best = cursor;
        }
        cursor = cursor->next;
    }
    return best;
}

performance *worst_performance(list_perf *performances, type_perf type) { //Cette fonction recherche la pire performance d'un type spécifié parmi toutes les performances données.
    performance *worst = NULL;
    performance *cursor = performances->first;
    while (cursor != NULL) {
        if (cursor->type == type && (worst == NULL || cursor->result.tm_sec > worst->result.tm_sec)) {
            worst = cursor;
        }
        cursor = cursor->next;
    }
    return worst;
}

struct tm average_performance(list_perf *performances, type_perf type) { //Cette fonction calcule la performance moyenne pour un type de performance spécifié parmi toutes les performances données.
    int count = 0;
    int total = 0;
    performance *cursor = performances->first;
    while (cursor != NULL) {
        if (cursor->type == type) {
            total += cursor->result.tm_hour * 3600 + cursor->result.tm_min * 60 + cursor->result.tm_sec;
            count++;
        }
        cursor = cursor->next;
    }
    struct tm avg;
    if (count != 0) {
        int average = total / count;
        avg.tm_hour = average / 3600;
        average -= avg.tm_hour * 3600;
        avg.tm_min = average / 60;
        average -= avg.tm_min * 60;
        avg.tm_sec = average;
    } else {
        avg.tm_hour = 0;
        avg.tm_min = 0;
        avg.tm_sec = 0;
    }
    return avg;
}

struct tm performance_difference(list_perf *performances, type_perf type, struct tm date1, struct tm date2) { //Cette fonction calcule la différence de performance entre deux dates spécifiées pour un type de performance donné parmi toutes les performances données.
    performance *perf1 = NULL;
    performance *perf2 = NULL;
    performance *cursor = performances->first;
    while (cursor != NULL) {
        if (cursor->type == type) {
            if (cursor->date.tm_year == date1.tm_year && cursor->date.tm_mon == date1.tm_mon && cursor->date.tm_mday == date1.tm_mday) {
                perf1 = cursor;
            }
            if (cursor->date.tm_year == date2.tm_year && cursor->date.tm_mon == date2.tm_mon && cursor->date.tm_mday == date2.tm_mday) {
                perf2 = cursor;
            }
        }
        cursor = cursor->next;
    }

    struct tm diff; // Cette fonction calcule la différence de performance entre deux dates spécifiées pour un type de performance donné parmi toutes les performances données.
    if (perf1 != NULL && perf2 != NULL) {
        int time1 = perf1->result.tm_hour * 3600 + perf1->result.tm_min * 60 + perf1->result.tm_sec;
        int time2 = perf2->result.tm_hour * 3600 + perf2->result.tm_min * 60 + perf2->result.tm_sec;
        int difference = abs(time1 - time2);
        diff.tm_hour = difference / 3600;
        difference -= diff.tm_hour * 3600;
        diff.tm_min = difference / 60;
        difference -= diff.tm_min * 60;
        diff.tm_sec = difference;
    } else {
        diff.tm_hour = 0;
        diff.tm_min = 0;
        diff.tm_sec = 0;
    }

    return diff;
}

performance *check_performance_for_a_date(list_perf *performances, struct tm date){ //Cette fonction vérifie s'il existe une performance pour une date spécifique dans la liste des performances données.
    performance *cursor = performances->first;
    while(cursor != NULL){
        if(cursor->date.tm_year == date.tm_year && cursor->date.tm_mon == date.tm_mon && cursor->date.tm_mday == date.tm_mday){
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

//Cette fonction calcule la régression linéaire pour un type de performance donné.
linear_regression calculate_linear_regression(list_perf *performances, type_perf type) {
    int n = 0;
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;

    performance *cursor = performances->first;
    while (cursor != NULL) {
        if (cursor->type == type) {
            double x = (double) cursor->date.tm_year + (double) cursor->date.tm_mon / 12 + (double) cursor->date.tm_mday / 365;
            double y = (double) cursor->result.tm_hour * 3600 + (double) cursor->result.tm_min * 60 + (double) cursor->result.tm_sec;
            sum_x += x;
            sum_y += y;
            sum_xy += x * y;
            sum_xx += x * x;
            n++;
        }
        cursor = cursor->next;
    }

    linear_regression lr;
    lr.slope = (n * sum_xy - sum_x * sum_y) / (n * sum_xx - sum_x * sum_x);
    lr.intercept = (sum_y - lr.slope * sum_x) / n;

    return lr;
}













