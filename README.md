#include <stdio.h>
#include <stdlib.h>
#define MAX_TAILLE 100
typedef struct {
    char nom[50];
    char prenom[50];
    int age;
    
} Athlete;

struct Performance {
    char type_evenement[50];
    float meilleur_temps;
    float pire_temps;
    float temps_moyen;
};
// Fonction pour ouvrir un fichier avec un nom et un mode donnés
FILE* ouvrir_fichier(const char* nom_fichier, const char* mode) {
    FILE* fp = fopen(nom_fichier, mode);
    if (fp == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier '%s'\n", nom_fichier);
        exit(EXIT_FAILURE);
    }
    return fp;
}

// Fonction pour sauvegarder les données d'entraînement dans un fichier
void sauvegarder_entrainement(const char* nom_fichier, const struct Entrainement* entrainement) {
    FILE* fp = ouvrir_fichier(nom_fichier, "ab");
    fwrite(entrainement, sizeof(struct Entrainement), 1, fp);
    fclose(fp);
}

// Fonction pour charger les données d'entraînement depuis un fichier
void charger_entrainement(const char* nom_fichier, struct Entrainement* tableau_entrainements, int* nb_entrainements) {
    FILE* fp = ouvrir_fichier(nom_fichier, "rb");
    fseek(fp, 0, SEEK_END);
    long taille = ftell(fp);
    rewind(fp);
    *nb_entrainements = taille / sizeof(struct Entrainement);
    fread(tableau_entrainements, sizeof(struct Entrainement), *nb_entrainements, fp);
    fclose(fp);
}

// Fonction pour créer un fichier pour un nouvel athlète
void creer_fichier_athlete(const char* nom_athlete) {
    char nom_fichier[100];
    sprintf(nom_fichier, "%s.txt", nom_athlete);
    FILE* fp = ouvrir_fichier(nom_fichier, "wb");
    fclose(fp);
}

// Fonction pour ajouter des données d'entraînement au fichier d'un athlète
void ajouter_entrainement_athlete(const char* nom_athlete, const struct Entrainement* entrainement) {
    char nom_fichier[100];
    sprintf(nom_fichier, "%s.txt", nom_athlete);
    sauvegarder_entrainement(nom_fichier, entrainement);
}
 // Fonction pour ajouter une nouvelle session d'entraînement pour un athlète
void ajouter_session_entrainement(const char* nom_athlete) {
    struct Entrainement nouvel_entrainement;
    printf("Entrez la date (AAAA-MM-JJ) : ");
    scanf("%s", nouvel_entrainement.date);
    printf("Entrez le type d'événement : ");
    scanf("%s", nouvel_entrainement.type_evenement);
    printf("Entrez le temps (en secondes) : ");
    scanf("%f", &nouvel_entrainement.temps);
    ajouter_entrainement_athlete(nom_athlete, &nouvel_entrainement);
}

// Fonction pour afficher l'historique d'entraînement pour un athlète
void afficher_historique_entrainements(const char* nom_athlete) {
    struct Entrainement tableau_entrainements[100]; // En supposant un maximum de 100 sessions d'entraînement
    int nb_entrainements = 0;
    charger_entrainement(nom_athlete, tableau_entrainements, &nb_entrainements);
    printf("\nHistorique d'entraînement pour %s :\n", nom_athlete);
    printf("---------------------------------------------------\n");
    printf("Date\t\tType d'événement\t\tTemps (s)\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < nb_entrainements; ++i) {
        printf("%s\t%s\t\t%.2f\n", tableau_entrainements[i].date, tableau_entrainements[i].type_evenement, tableau_entrainements[i].temps);
    }
}

  // Fonction pour vérifier si un entraînement en relais existe déjà pour une date donnée
int entrainement_relais_existe(const char* nom_fichier, const char* date) {
    struct Entrainement entrainement;
    FILE* fp = ouvrir_fichier(nom_fichier, "rb");
    while (fread(&entrainement, sizeof(struct Entrainement), 1, fp)) {
        if (strcmp(entrainement.date, date) == 0 && strcmp(entrainement.type_evenement, "Relais") == 0) {
            fclose(fp);
            return 1; // L'entraînement en relais existe
        }
    }
    fclose(fp);
    return 0; // L'entraînement en relais n'existe pas
}

// Fonction pour ajouter une session d'entraînement en relais pour un athlète
void ajouter_entrainement_relais(const char* nom_athlete) {
    char date[20];
    struct Entrainement nouvel_entrainement;

    printf("Entrez la date (AAAA-MM-JJ) : ");
    scanf("%s", date);

    // Vérifier si un entraînement en relais existe déjà pour la date donnée
    if (entrainement_relais_existe(nom_athlete, date)) {
        printf("Erreur : Une session d'entraînement en relais existe déjà pour la date donnée.\n");
        return;
    }

    // Remplir les détails de l'entraînement
    strcpy(nouvel_entrainement.date, date);
    strcpy(nouvel_entrainement.type_evenement, "Relais");

    printf("Entrez le nombre de participants : ");
    int nb_participants;
    scanf("%d", &nb_participants);

    // Valider le nombre de participants
    if (nb_participants < 2 || nb_participants > 8) {
        printf("Erreur : Le nombre de participants doit être compris entre 2 et 8.\n");
        return;
    }

    // Saisir les détails pour chaque participant
    for (int i = 0; i < nb_participants; ++i) {
        printf("Entrez la position du participant %d : ", i + 1);
        scanf("%d", &nouvel_entrainement.position);
        printf("Entrez le temps pour le participant %d (en secondes) : ", i + 1);
        scanf("%f", &nouvel_entrainement.temps);
        // Validation supplémentaire peut être ajoutée ici

        // Sauvegarder les données d'entraînement pour chaque participant
        ajouter_entrainement_athlete(nom_athlete, &nouvel_entrainement);
    }

    printf("Session d'entraînement en relais ajoutée avec succès.\n");
}
// Fonction pour calculer la progression de l'athlète entre deux dates
void calculer_progression(const char* nom_athlete, const char* date_debut, const char* date_fin) {
    struct Entrainement tableau_entrainements[100]; // En supposant un maximum de 100 sessions d'entraînement
    int nb_entrainements = 0;
    charger_entrainement(nom_athlete, tableau_entrainements, &nb_entrainements);
    
    // Calculer le temps moyen de toutes les sessions d'entraînement dans la plage spécifiée
    float somme_temps = 0;
    int count = 0;
    for (int i = 0; i < nb_entrainements; ++i) {
        // Vérifier si la date de la session d'entraînement est dans la plage spécifiée
        if (strcmp(tableau_entrainements[i].date, date_debut) >= 0 && strcmp(tableau_entrainements[i].date, date_fin) <= 0) {
            somme_temps += tableau_entrainements[i].temps;
            ++count;
        }
    }
    float temps_moyen = (count > 0) ? somme_temps / count : 0;
    
    printf("\nProgression pour %s entre %s et %s :\n", nom_athlete, date_debut, date_fin);
    printf("---------------------------------------------------------------\n");
    printf("Date\t\tType d'événement\tTemps\tAmélioration\tMeilleur que la moyenne\n");
    printf("---------------------------------------------------------------\n");
    
    // Initialiser les variables pour suivre le temps précédent et l'amélioration
    float temps_precedent = 0.0; // Supposant qu'il n'y a pas de temps précédent pour la première session
    
    // Parcourir les sessions d'entraînement
    for (int i = 0; i < nb_entrainements; ++i) {
        // Vérifier si la date de la session d'entraînement est dans la plage spécifiée
        if (strcmp(tableau_entrainements[i].date, date_debut) >= 0 && strcmp(tableau_entrainements[i].date, date_fin) <= 0) {
            // Calculer l'amélioration du temps par rapport à la session précédente
            float amelioration = temps_precedent - tableau_entrainements[i].temps;
            
            // Vérifier si le temps actuel est meilleur que le temps moyen
            char* meilleur_que_moyenne = (tableau_entrainements[i].temps < temps_moyen) ? "Oui" : "Non";
            
            // Afficher les données de progression pour l'athlète
            printf("%s\t%s\t%.2f\t%.2f\t\t%s\n", tableau_entrainements[i].date, tableau_entrainements[i].type_evenement, tableau_entrainements[i].temps, amelioration, meilleur_que_moyenne);
            
            // Mettre à jour le temps précédent pour la prochaine itération
            temps_precedent = tableau_entrainements[i].temps;
        }
    }
}
//tri 
void echanger(struct Entrainement* a, struct Entrainement* b) {
    struct Entrainement temp = *a; // Nous créons une variable temporaire pour stocker la valeur de a.
    *a = *b; // Nous attribuons la valeur de b à a.
    *b = temp; // Nous attribuons la valeur de la variable temporaire (qui contient la valeur d'origine de a) à b.
}

// Algorithme de tri à bulles pour trier les sessions d'entraînement par date
void tri_a_bulles_date(struct Entrainement tableau_entrainements[], int nb_entrainements) {
    for (int i = 0; i < nb_entrainements - 1; ++i) { // Nous parcourons le tableau.
        for (int j = 0; j < nb_entrainements - i - 1; ++j) { // Nous parcourons la partie non triée du tableau.
            if (strcmp(tableau_entrainements[j].date, tableau_entrainements[j + 1].date) > 0) { // Nous comparons les éléments adjacents.
                echanger(&tableau_entrainements[j], &tableau_entrainements[j + 1]); // S'ils ne sont pas dans le bon ordre, nous les échangeons.
            }
        }
    }
}

// Algorithme de tri par insertion pour trier les sessions d'entraînement par type d'événement
void tri_insertion_type_evenement(struct Entrainement tableau_entrainements[], int nb_entrainements) {
    for (int i = 1; i < nb_entrainements; ++i) { // Nous commençons par le deuxième élément du tableau.
        struct Entrainement cle = tableau_entrainements[i]; // Nous stockons l'élément actuel dans une variable.
        int j = i - 1; // Nous définissons j sur l'index avant l'élément actuel.
        while (j >= 0 && strcmp(tableau_entrainements[j].type_evenement, cle.type_evenement) > 0) { // Nous comparons les éléments et les déplaçons vers la droite s'ils sont supérieurs à l'élément actuel.
            tableau_entrainements[j + 1] = tableau_entrainements[j]; // Nous déplaçons les éléments vers la droite.
            j = j - 1; // Nous passons à l'élément précédent.
        }
        tableau_entrainements[j + 1] = cle; // Nous insérons l'élément actuel à sa position correcte.
    }
}

// Algorithme de tri rapide pour trier les sessions d'entraînement par temps
int partition(struct Entrainement tableau_entrainements[], int bas, int haut) {
    struct Entrainement pivot = tableau_entrainements[haut]; // Nous choisissons le dernier élément comme pivot.
    int i = bas - 1; // Nous définissons l'index du plus petit élément.
    for (int j = bas; j <= haut - 1; ++j) { // Nous parcourons le tableau.
        if (tableau_entrainements[j].temps < pivot.temps) { // Si l'élément actuel est plus petit que le pivot.
            ++i; // Nous incrémentons l'index du plus petit élément.
            echanger(&tableau_entrainements[i], &tableau_entrainements[j]); // Nous échangeons les éléments.
        }
    }
    echanger(&tableau_entrainements[i + 1], &tableau_entrainements[haut]); // Nous échangeons l'élément pivot avec l'élément à la bonne position.
    return (i + 1); // Nous retournons l'index de l'élément pivot.
}

void tri_rapide_temps(struct Entrainement tableau_entrainements[], int bas, int haut) {
    if (bas < haut) { // S'il y a plus d'un élément dans le tableau.
        int pi = partition(tableau_entrainements, bas, haut); // Nous partitionnons le tableau.
        tri_rapide_temps(tableau_entrainements, bas, pi - 1); // Nous trions récursivement le sous-tableau gauche.
        tri_rapide_temps(tableau_entrainements, pi + 1, haut); // Nous trions récursivement le sous-tableau droit.
    }
}

void consulterPerformances(Athlete* athletes, int nbAthletes, int idAthlete) {
    FILE *fichier;
    char nomFichier[MAX_TAILLE];
    
    sprintf(nomFichier, "performances_%s_%s.txt", athletes[idAthlete].prenom, athletes[idAthlete].nom);
    
    fichier = fopen(nomFichier, "r");
    
    if (fichier == NULL) {
        printf("Erreur : Impossible d'ouvrir le fichier.\n");
        return;
    }
    
    char date[MAX_TAILLE], typeEpreuve[MAX_TAILLE];
    float resultat;
    printf("Performances de %s %s :\n", athletes[idAthlete].prenom, athletes[idAthlete].nom);
    while (fscanf(fichier, "%s %s %f", date, typeEpreuve, &resultat) != EOF) {
        printf("Date : %s, Type d'épreuve : %s, Résultat : %f\n", date, typeEpreuve, resultat);
    }

    fclose(fichier);
}
void ajouter_athlete(Athlete* athletes, int* nbAthletes);
void ajouter_session_entrainement(Athlete* athletes, int nbAthletes);
void ajouter_session_relais(Athlete* athletes, int nbAthletes);
void consulter_historique_entrainement(Athlete* athletes, int nbAthletes);
void calculer_progression_athlete(Athlete* athletes, int nbAthletes);

int main() {
    Athlete athletes[10]; // Tableau de 10 athlètes, vous pouvez ajuster la taille selon vos besoins
    int nbAthletes = 0; // Nombre d'athlètes

    int choix;
    do {
        printf("\nMenu :\n");
        printf("1. Ajouter un nouvel athlète\n");
        printf("2. Ajouter une session d'entraînement individuelle\n");
        printf("3. Ajouter une session d'entraînement en relais\n");
        printf("4. Consulter l'historique d'entraînement d'un athlète\n");
        printf("5. Calculer la progression d'un athlète\n");
        printf("6. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                ajouter_athlete(athletes, &nbAthletes);
                break;
            case 2:
                ajouter_session_entrainement(athletes, nbAthletes);
                break;
            case 3:
                ajouter_session_relais(athletes, nbAthletes);
                break;
            case 4:
                consulter_historique_entrainement(athletes, nbAthletes);
                break;
            case 5:
                calculer_progression_athlete(athletes, nbAthletes);
                break;
            case 6:
                printf("Au revoir !\n");
                break;
            default:
                printf("Choix invalide. Veuillez choisir un numéro entre 1 et 6.\n");
                break;
        }
    } while (choix != 6);

    return 0;
}
