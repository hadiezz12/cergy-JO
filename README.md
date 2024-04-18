#include <stdio.h>
#include <stdlib.h>
#define MAX_TAILLE 100
typedef struct {
    char nom[50];
    char prenom[50];
    int age;
    
} Athlete;

typedef struct {
    char date[20];
    char type_epreuve[50];
    float resultat;

} Performance;


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
