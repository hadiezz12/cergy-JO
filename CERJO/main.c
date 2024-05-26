#include "utils.h"
void menu(list_athletes *athletes);

void add_athlete_choice(list_athletes *athletes) {
    char name[50]; // Déclare une chaîne pour stocker le nom de l'athlète.
    do {
        printf("Enter the name of the athlete (Name FamilyName): "); // Demande à l'utilisateur de saisir le nom de l'athlète.
        fgets(name, 50, stdin); // Lit le nom de l'athlète depuis l'entrée standard.
    } while (!is_correct_format(name)); // Continue à demander le nom tant que le format est incorrect.
    name[strlen(name)-1] = '\0'; // Remplace le dernier caractère (saut de ligne) par un terminateur de chaîne.
    athlete *ath = create_athlete(name); // Crée un nouvel athlète avec le nom saisi.
    create_athlete_file(ath); // Crée un fichier pour l'athlète.
    add_athlete(athletes, ath); // Ajoute l'athlète à la liste des athlètes.
    menu(athletes); // Retourne au menu principal.
}

void add_performance_choice(list_athletes *athletes) {
    char name[50]; // Déclare une chaîne pour stocker le nom de l'athlète.
    do {
        printf("Enter the name of the athlete (Name FamilyName): "); // Demande à l'utilisateur de saisir le nom de l'athlète.
        fgets(name, 50, stdin); // Lit le nom de l'athlète depuis l'entrée standard.
    } while (!is_correct_format(name)); // Continue à demander le nom tant que le format est incorrect.
    athlete *ath = find_by_name(athletes, name); // Trouve l'athlète par son nom dans la liste des athlètes.
    if (ath == NULL) { // Vérifie si l'athlète n'a pas été trouvé.
        printf("Athlete not found\n"); // Affiche un message d'erreur.
        menu(athletes); // Retourne au menu principal.
    }


    struct tm date; // Déclare une structure pour stocker la date de la performance.
    do {
        printf("Enter the date of the performance (dd/mm/yyyy): "); // Demande à l'utilisateur de saisir la date de la performance.
        scanf(" %d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year); // Lit la date depuis l'entrée standard.
    } while (date.tm_mday < 1 || date.tm_mday > 31 || date.tm_mon < 1 || date.tm_mon > 12 || date.tm_year < 1900); // Continue à demander la date tant qu'elle est incorrecte.

    char type[10]; // Déclare une chaîne pour stocker le type de performance.
    do {
        printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): "); // Demande à l'utilisateur de saisir le type de performance.
        fgets(type, 10, stdin); // Lit le type de performance depuis l'entrée standard.
        type[strlen(type)-1] = '\0'; // Remplace le dernier caractère (saut de ligne) par un terminateur de chaîne.
    } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0); // Continue à demander le type tant qu'il est incorrect.
    type_perf real_type = get_type_perf(type); // Convertit le type de performance en type_perf.

    int relai_info = 0; // Initialise la position du porteur de relais à 0.
    if (strcmp(type, "Relai") == 0) { // Vérifie si le type de performance est "Relai".
        do {
            printf("Enter the position of the torchbearer [1-4]: "); // Demande à l'utilisateur de saisir la position du porteur de relais.
            scanf(" %d", &relai_info); // Lit la position depuis l'entrée standard.
            flush_input(); // Vide le tampon d'entrée pour éviter les problèmes de saisie.
        } while (relai_info < 1 || relai_info > 4); // Continue à demander la position tant qu'elle est incorrecte.
    }

    struct tm result; // Déclare une structure pour stocker le résultat de la performance.
    do {
        printf("Enter the result of the performance (hh:mm:ss): "); // Demande à l'utilisateur de saisir le résultat de la performance.
        scanf(" %d:%d:%d", &result.tm_hour, &result.tm_min, &result.tm_sec); // Lit le résultat depuis l'entrée standard.
    } while (result.tm_hour < 0 || result.tm_hour > 23 || result.tm_min < 0 || result.tm_min > 59 || result.tm_sec < 0 || result.tm_sec > 59); // Continue à demander le résultat tant qu'il est incorrect.

    performance *perf = create_performance(date, real_type, result, relai_info); // Crée une nouvelle performance avec les détails saisis.
    add_performance(ath->performances, perf); // Ajoute la performance à la liste des performances de l'athlète.

    for(size_t i = 0; i < strlen(ath->name); i++){ // Boucle pour remplacer les espaces par des underscores dans le nom de l'athlète.
        if(name[i] == ' '){
            name[i] = '_';
        }
    }
    char path[150]; // Déclare une chaîne pour stocker le chemin du fichier.
    strcpy(path, DATA_PATH); // Copie le chemin de base dans `path`.
    strcat(path, name); // Concatène le nom de l'athlète au chemin de base.
    strcat(path, ".txt"); // Ajoute l'extension ".txt" au chemin.
    save_performances_to_file(path, ath->performances); // Sauvegarde les performances de l'athlète dans le fichier.
    menu(athletes); // Retourne au menu principal.
}

void display_training_session_choice(list_athletes *athletes){

    printf("\n\n - Sorting methods - \n\n");

    char choice1;
    do {
        printf("Do you want to sort by athlete (y/n) :");
        scanf(" %c", &choice1); // Lit le choix de l'utilisateur pour trier par athlète.
        flush_input(); // Vide le tampon d'entrée.
    }while(choice1 != 'y' && choice1 != 'n'); // Continue à demander tant que le choix n'est pas valide.

    char choice2;
    do {
        printf("Do you want to sort by training session (y/n) : ");
        scanf(" %c", &choice2); // Lit le choix de l'utilisateur pour trier par session d'entraînement.
        flush_input(); // Vide le tampon d'entrée.
    }while(choice2 != 'y' && choice2 != 'n'); // Continue à demander tant que le choix n'est pas valide.


    char choice3;
    do {
        printf("Do you want to sort by date (y/n) : ");
        scanf(" %c", &choice3); // Lit le choix de l'utilisateur pour trier par date.
        flush_input(); // Vide le tampon d'entrée.
    } while(choice3 != 'y' && choice3 != 'n'); // Continue à demander tant que le choix n'est pas valide.


    // Si l'utilisateur choisit de trier par athlète seulement.
    if(choice1 == 'y' && choice2 == 'n' && choice3 == 'n'){
        printf("Sort by athlete\n");
        char name[50];
        do {
            printf("Enter the name of the athlete (Name FamilyName): ");
            fgets(name, 50, stdin); // Lit le nom de l'athlète.

        } while (!is_correct_format(name)); // Continue à demander tant que le format n'est pas correct.
        athlete *ath = find_by_name(athletes,name); // Trouve l'athlète par son nom.
        if (ath != NULL){
            printf("Displaying performances of %s\n", ath->name);
            display_performances(ath->performances); // Affiche les performances de l'athlète.
        }else{
            printf("Athlete not found\n"); // Affiche un message si l'athlète n'est pas trouvé.
        }

    }
    // Si l'utilisateur choisit de trier par session d'entraînement seulement.
    else if(choice1 == 'n' && choice2 == 'y' && choice3 == 'n'){
        printf("Sort by training session\n");
        char type[10];
        do {
            printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
            fgets(type, 10, stdin); // Lit le type de performance.
            type[strlen(type)-1] = '\0'; // Remplace le dernier caractère par un terminateur de chaîne.

        } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0); // Continue à demander tant que le type n'est pas correct.
        type_perf real_type = get_type_perf(type);  // Convertit le type en type_perf.

        display_performances2(sort_performances_by_type(get_all_performances(),real_type)); // Affiche les performances triées par type.
    }
    else if(choice1 == 'n' && choice2 == 'n' && choice3 == 'y'){ // Si l'utilisateur choisit de trier par date seulement. 
        printf("Sort by date\n");
        struct tm date;
        do {
            printf("Enter the date of the performance (dd/mm/yyyy): ");

            scanf(" %d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year); // Lit la date.

        } while (date.tm_mday < 1 || date.tm_mday > 31 || date.tm_mon < 1 || date.tm_mon > 12 || date.tm_year < 1900); // Continue à demander tant que la date n'est pas correcte.

        display_performances2(sort_performances_by_date(get_all_performances(),date)); // Affiche les performances triées par date.
    }
    else if(choice1 == 'y' && choice2 == 'y' && choice3 == 'n'){ // Si l'utilisateur choisit de trier par athlète et par type.
        printf("Sort by athlete and type\n");
        char name[50];
        do {
            printf("Enter the name of the athlete (Name FamilyName): ");
            fgets(name, 50, stdin); // Lit le nom de l'athlète.

        } while (!is_correct_format(name)); // Continue à demander tant que le format n'est pas correct.
        athlete *ath = find_by_name(athletes,name); // Trouve l'athlète par son nom.
        if (ath != NULL){
            char type[10];
            do {
                printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
                fgets(type, 10, stdin); // Lit le type de performance.
                type[strlen(type)-1] = '\0'; // Remplace le dernier caractère par un terminateur de chaîne.

            } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0); // Continue à demander tant que le type n'est pas correct.
            type_perf real_type = get_type_perf(type); // Convertit le type en type_perf.
            printf("Displaying performances of %s\n", ath->name); 
            display_performances(sort_performances_by_type_athlete(ath->performances,real_type)); // Affiche les performances triées par type pour l'athlète.
        }else{
            printf("Athlete not found\n"); // Affiche un message si l'athlète n'est pas trouvé.
        }

    }
    else if(choice1 == 'y' && choice2 == 'n' && choice3 == 'y'){ // Si l'utilisateur choisit de trier par athlète et par date.
        printf("Sort by athlete and date\n");
        char name[50];
        do {
            printf("Enter the name of the athlete (Name FamilyName): ");
            fgets(name, 50, stdin); // Lit le nom de l'athlète.

        } while (!is_correct_format(name)); // Continue à demander tant que le format n'est pas correct.
        athlete *ath = find_by_name(athletes,name); // Trouve l'athlète par son nom.
        if (ath != NULL){
            struct tm date;
            do {
                printf("Enter the date of the performance (dd/mm/yyyy): ");

                scanf(" %d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year); // Lit la date.

            } while (date.tm_mday < 1 || date.tm_mday > 31 || date.tm_mon < 1 || date.tm_mon > 12 || date.tm_year < 1900); // Continue à demander tant que la date n'est pas correcte.
            printf("Displaying performances of %s\n", ath->name);

            display_performances(sort_performances_by_date_athlete(ath->performances,date)); // Affiche les performances triées par date pour l'athlète.
        }else{
            printf("Athlete not found\n"); // Affiche un message si l'athlète n'est pas trouvé.
        }

    }
    else if(choice1 == 'n' && choice2 == 'y' && choice3 == 'y'){
        printf("Sort by training session and date\n");
        char type[10];
        do {
            printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
            fgets(type, 10, stdin); // Lit le type de performance.
            type[strlen(type)-1] = '\0'; // Remplace le dernier caractère par un terminateur de chaîne.

        } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0); // Continue à demander tant que le type n'est pas correct.
        type_perf real_type = get_type_perf(type); // Convertit le type en type_perf.
        struct tm date;
        do {
            printf("Enter the date of the performance (dd/mm/yyyy): ");

            scanf(" %d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year); // Lit la date.

        } while (date.tm_mday < 1 || date.tm_mday > 31 || date.tm_mon < 1 || date.tm_mon > 12 || date.tm_year < 1900); // Continue à demander tant que la date n'est pas correcte.
        display_performances2(sort_performances_by_type_and_date(get_all_performances(),real_type,date)); // Affiche les performances triées par type et par date.
    }
    else if(choice1 == 'y' && choice2 == 'y' && choice3 == 'y'){
        printf("Sort by athlete, training session and date\n");
        char name[50];
        do {
            printf("Enter the name of the athlete (Name FamilyName): ");
            fgets(name, 50, stdin); // Lit le nom de l'athlète.

        } while (!is_correct_format(name));
        athlete *ath = find_by_name(athletes,name); // Trouve l'athlète par son nom.
        if (ath != NULL){
            char type[10];
            do {
                printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
                fgets(type, 10, stdin); // Lit le type de performance.
                type[strlen(type)-1] = '\0'; // Remplace le dernier caractère par un terminateur de chaîne.

            } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0); // Continue à demander tant que le type n'est pas correct
            type_perf real_type = get_type_perf(type); // Convertit le type en type_perf.
            struct tm date;
            do {
                printf("Enter the date of the performance (dd/mm/yyyy): ");
                scanf(" %d/%d/%d", &date.tm_mday, &date.tm_mon, &date.tm_year); // Lit la date.
            } while (date.tm_mday < 1 || date.tm_mday > 31 || date.tm_mon < 1 || date.tm_mon > 12 || date.tm_year < 1900); // Continue à demander tant que la date n'est pas correcte.

            printf("Displaying performances of %s\n", ath->name);
            display_performances(sort_performances_by_type_and_date_athlete(ath->performances, real_type, date)); // Affiche les performances triées par type et par date pour l'athlète.
        } else {
            printf("Athlete not found\n"); // Affiche un message si l'athlète n'est pas trouvé.
        }
    }
    else {
        printf("No sorting method selected\n"); // Affiche un message si aucune méthode de tri n'est sélectionnée.
    }
    menu(athletes); // Retourne au menu principal.

}



void display_performance_choice(list_athletes *athletes){ // Affichage des options du sous-menu de gestion des performances
    printf("\n\n - Welcome to the performance management submenu - \n\n");
    printf("1. Display athlete performance summary\n");
    printf("2. Display best athletes for JO\n");
    printf("3. Display athlete progression\n");
    printf("4. Display linear regression\n");
    printf("5. Return to main menu\n");

    int choice; // Obtenir le choix de l'utilisateur et le valider
    do {
        printf("\nEnter your choice: ");
        scanf(" %d", &choice);
        flush_input();
    } while(choice < 1 || choice > 4);

    char name[50];   // Déclarer des variables pour le nom, le type, les dates, l'athlète et le type de performance
    char type[10];
    struct tm date1, date2;
    athlete *ath = NULL;
    type_perf real_type;
    switch (choice) {
        case 1: // Saisir le nom de l'athlète et le type de performance, puis afficher le résumé de la performance.
            do {
                printf("Enter the name of the athlete (Name FamilyName): ");
                fgets(name, 50, stdin);

            } while (!is_correct_format(name));
            ath = find_by_name(athletes,name);
            do {
                printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
                fgets(type, 10, stdin);
                type[strlen(type)-1] = '\0';

            } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0);
            real_type = get_type_perf(type);
            athlete_performance_summary(athletes, ath->name, real_type);
            break;
        case 2: // Saisir le type de performance et afficher les meilleurs athlètes pour le JO
            do {
                printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
                fgets(type, 10, stdin);
                type[strlen(type)-1] = '\0';

            } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0);
            real_type = get_type_perf(type);
            best_athletes_for_jo(athletes, real_type);
            break;
        case 3:  // Saisir le nom de l'athlète, le type et les dates de deux performances, puis afficher la progression.
            do {
                printf("Enter the name of the athlete (Name FamilyName): ");
                fgets(name, 50, stdin);

            } while (!is_correct_format(name));
            ath = find_by_name(athletes,name);
            do {
                printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
                fgets(type, 10, stdin);
                type[strlen(type)-1] = '\0';

            } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0);
            real_type = get_type_perf(type);
            do {
                printf("Enter the date of the first performance to compare (dd/mm/yyyy): ");

                scanf(" %d/%d/%d", &date1.tm_mday, &date1.tm_mon, &date1.tm_year);
            } while (date1.tm_mday < 1 || date1.tm_mday > 31 || date1.tm_mon < 1 || date1.tm_mon > 12 || date1.tm_year < 1900);
            performance *perf1 = check_performance_for_a_date(ath->performances, date1);
            if (perf1 == NULL) {
                printf("No performance found for this date\n");
                break;
            }
            do {
                printf("Enter the date of the second performance to compare (dd/mm/yyyy): ");

                scanf(" %d/%d/%d", &date2.tm_mday, &date2.tm_mon, &date2.tm_year);
            } while (date2.tm_mday < 1 || date2.tm_mday > 31 || date2.tm_mon < 1 || date2.tm_mon > 12 || date2.tm_year < 1900);
            performance *perf2 = check_performance_for_a_date(ath->performances, date2);
            if (perf2 == NULL) {
                printf("No performance found for this date\n");
                break;
            }

            athlete_progression(ath, real_type, date1, date2);
            break;

        case 4:  // Introduire le nom de l'athlète et le type de performance, calculer la régression linéaire et afficher le résultat.
            do {
                printf("Enter the name of the athlete (Name FamilyName): ");
                fgets(name, 50, stdin);

            } while (!is_correct_format(name));
            ath = find_by_name(athletes,name);

            do {
                printf("Enter the type of the performance (100m, 400m, 5000m, Marathon, Relai): ");
                fgets(type, 10, stdin);
                type[strlen(type)-1] = '\0';

            } while (strcmp(type, "100m") != 0 && strcmp(type, "400m") != 0 && strcmp(type, "5000m") != 0 && strcmp(type, "Marathon") != 0 && strcmp(type, "Relai") != 0);
            real_type = get_type_perf(type);
            linear_regression lr = calculate_linear_regression(ath->performances, real_type);
            printf("y = %fx + %f\n", lr.slope, lr.intercept);

            break;


        case 5:  // Retour au menu principal
            printf("Returning to main menu\n");
            menu(athletes);
            break;
    }
    menu(athletes); // Retour au menu principal après avoir traité le choix de l'utilisateur
}

void menu(list_athletes *athletes){ // Déclaration de la fonction menu, qui prend un pointeur vers une structure list_athletes.
    printf("\n\n - Welcome to the performance management system - \n\n"); // Affichage d'un message de bienvenue et des options du menu.
    printf("1. Add an athlete\n");
    printf("2. Add a performance\n");
    printf("3. Display training sessions\n");
    printf("4. Display performances\n");
    printf("5. Exit\n");

    int choice; // Déclaration de la variable qui stockera le choix de l'utilisateur.
    do { // Boucle do-while pour demander le choix de l'utilisateur jusqu'à ce qu'il soit valide.
        printf("\nEnter your choice: "); // Demande à l'utilisateur de saisir son choix.
        scanf(" %d", &choice); // Lecture du choix de l'utilisateur
        flush_input(); // Appel de la fonction flush_input pour nettoyer le buffer d'entrée (doit être définie ailleurs).
    } while(choice < 1 || choice > 5); // Vérifie que le choix est entre 1 et 5 inclus.
    switch (choice) { // Structure switch pour exécuter le code correspondant au choix de l'utilisateur
        case 1:
            add_athlete_choice(athletes); // Appel de la fonction pour ajouter un athlète.

            break;
        case 2:
            add_performance_choice(athletes); // Appel de la fonction pour ajouter une performance.
            break;
        case 3:
            display_training_session_choice(athletes); // Appel de la fonction pour afficher les sessions d'entraînement.
            break;
        case 4:
            display_performance_choice(athletes); // Appel de la fonction pour afficher les performances.
            break;
        case 5:
            printf("Exit\n");
            break;
    }
}




int main() { 
    // Initialise la liste des athlètes en appelant la fonction init_all_athletes
    // et stocke le pointeur retourné dans la variable ath.
    list_athletes *ath = init_all_athletes(); 
    // Appelle la fonction menu pour afficher le menu principal
    // et permettre à l'utilisateur d'interagir avec le système de gestion des performances.
    menu(ath);

    // Retourne 0 pour indiquer que le programme s'est terminé avec succès.

    return 0;
}
