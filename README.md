#include <stdio.h>
#include <stdlib.h>
#define MAX_TAILLE 100
typedef struct {
    char nom[50];
    char prenom[50];
    int age;
    
} Athlete;

struct Performance {
    char event_type[50];
    float best_time;
    float worst_time;
    float average_time;
};
// Function to open a file with a given name and mode
FILE* open_file(const char* filename, const char* mode) {
    FILE* fp = fopen(filename, mode);
    if (fp == NULL) {
        printf("Error: Unable to open file '%s'\n", filename);
        exit(EXIT_FAILURE);
    }
    return fp;
}

// Function to save training data to a file
void save_training(const char* filename, const struct Training* training) {
    FILE* fp = open_file(filename, "ab");
    fwrite(training, sizeof(struct Training), 1, fp);
    fclose(fp);
}

// Function to load training data from a file
void load_training(const char* filename, struct Training* training_array, int* num_trainings) {
    FILE* fp = open_file(filename, "rb");
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    rewind(fp);
    *num_trainings = size / sizeof(struct Training);
    fread(training_array, sizeof(struct Training), *num_trainings, fp);
    fclose(fp);
}

// Function to create a file for a new athlete
void create_athlete_file(const char* athlete_name) {
    char filename[100];
    sprintf(filename, "%s.txt", athlete_name);
    FILE* fp = open_file(filename, "wb");
    fclose(fp);
}

// Function to add training data to an athlete's file
void add_training_to_athlete(const char* athlete_name, const struct Training* training) {
    char filename[100];
    sprintf(filename, "%s.txt", athlete_name);
    save_training(filename, training);
}
 // Function to add a new training session for an athlete
void add_training_session(const char* athlete_name) {
    struct Training new_training;
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", new_training.date);
    printf("Enter event type: ");
    scanf("%s", new_training.event_type);
    printf("Enter time (in seconds): ");
    scanf("%f", &new_training.time);
    add_training_to_athlete(athlete_name, &new_training);
}

// Function to display training history for an athlete
void display_training_history(const char* athlete_name) {
    struct Training training_array[100]; // Assuming maximum 100 training sessions
    int num_trainings = 0;
    load_training(athlete_name, training_array, &num_trainings);
    printf("\nTraining History for %s:\n", athlete_name);
    printf("---------------------------------------------------\n");
    printf("Date\t\tEvent Type\t\tTime (s)\n");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < num_trainings; ++i) {
        printf("%s\t%s\t\t%.2f\n", training_array[i].date, training_array[i].event_type, training_array[i].time);
    }
}

  // Function to check if a relay training already exists for a given date
int relay_training_exists(const char* filename, const char* date) {
    struct Training training;
    FILE* fp = open_file(filename, "rb");
    while (fread(&training, sizeof(struct Training), 1, fp)) {
        if (strcmp(training.date, date) == 0 && strcmp(training.event_type, "Relay") == 0) {
            fclose(fp);
            return 1; // Relay training exists
        }
    }
    fclose(fp);
    return 0; // Relay training does not exist
}

// Function to add a relay training session for an athlete
void add_relay_training(const char* athlete_name) {
    char date[20];
    struct Training new_training;

    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", date);

    // Check if a relay training already exists for the given date
    if (relay_training_exists(athlete_name, date)) {
        printf("Error: A relay training session already exists for the given date.\n");
        return;
    }

    // Populate training details
    strcpy(new_training.date, date);
    strcpy(new_training.event_type, "Relay");

    printf("Enter the number of participants: ");
    int num_participants;
    scanf("%d", &num_participants);

    // Validate number of participants
    if (num_participants < 2 || num_participants > 8) {
        printf("Error: Number of participants must be between 2 and 8.\n");
        return;
    }

    // Input details for each participant
    for (int i = 0; i < num_participants; ++i) {
        printf("Enter the position of participant %d: ", i + 1);
        scanf("%d", &new_training.position);
        printf("Enter the time for participant %d (in seconds): ", i + 1);
        scanf("%f", &new_training.time);
        // Additional input validation can be added here

        // Save training data for each participant
        add_training_to_athlete(athlete_name, &new_training);
    }

    printf("Relay training session added successfully.\n");
}
// Function to calculate athlete progression between two dates
void calculate_progression(const char* athlete_name, const char* start_date, const char* end_date) {
    struct Training training_array[100]; // Assuming maximum 100 training sessions
    int num_trainings = 0;
    load_training(athlete_name, training_array, &num_trainings);
    
    // Calculate the average time of all training sessions within the specified range
    float sum_times = 0;
    int count = 0;
    for (int i = 0; i < num_trainings; ++i) {
        // Check if the training session date is within the specified range
        if (strcmp(training_array[i].date, start_date) >= 0 && strcmp(training_array[i].date, end_date) <= 0) {
            sum_times += training_array[i].time;
            ++count;
        }
    }
    float average_time = (count > 0) ? sum_times / count : 0;
    
    printf("\nProgression for %s between %s and %s:\n", athlete_name, start_date, end_date);
    printf("---------------------------------------------------------------\n");
    printf("Date\t\tEvent Type\tTime\tImprovement\tBetter than Average\n");
    printf("---------------------------------------------------------------\n");
    
    // Initialize variables for tracking previous time and improvement
    float previous_time = 0.0; // Assuming no previous time for the first session
    
    // Iterate through training sessions
    for (int i = 0; i < num_trainings; ++i) {
        // Check if the training session date is within the specified range
        if (strcmp(training_array[i].date, start_date) >= 0 && strcmp(training_array[i].date, end_date) <= 0) {
            // Calculate improvement in time compared to previous session
            float improvement = previous_time - training_array[i].time;
            
            // Check if the current time is better than the average time
            char* better_than_average = (training_array[i].time < average_time) ? "Yes" : "No";
            
            // Display progression data for the athlete
            printf("%s\t%s\t%.2f\t%.2f\t\t%s\n", training_array[i].date, training_array[i].event_type, training_array[i].time, improvement, better_than_average);
            
            // Update previous time for the next iteration
            previous_time = training_array[i].time;
        }
    }
}
//tris 
void swap(struct Training* a, struct Training* b) {
    struct Training temp = *a; // We create a temporary variable to store the value of a.
    *a = *b; // We assign the value of b to a.
    *b = temp; // We assign the value of the temporary variable (which holds the original value of a) to b.
}

// Bubble Sort algorithm for sorting training sessions by date
void bubble_sort_date(struct Training training_array[], int num_trainings) {
    for (int i = 0; i < num_trainings - 1; ++i) { // We iterate through the array.
        for (int j = 0; j < num_trainings - i - 1; ++j) { // We iterate through the unsorted part of the array.
            if (strcmp(training_array[j].date, training_array[j + 1].date) > 0) { // We compare adjacent elements.
                swap(&training_array[j], &training_array[j + 1]); // If they are in the wrong order, we swap them.
            }
        }
    }
}

// Insertion Sort algorithm for sorting training sessions by event type
void insertion_sort_event_type(struct Training training_array[], int num_trainings) {
    for (int i = 1; i < num_trainings; ++i) { // We start with the second element of the array.
        struct Training key = training_array[i]; // We store the current element in a variable.
        int j = i - 1; // We set j to the index before the current element.
        while (j >= 0 && strcmp(training_array[j].event_type, key.event_type) > 0) { // We compare elements and move them to the right if they are greater than the current element.
            training_array[j + 1] = training_array[j]; // We move elements to the right.
            j = j - 1; // We move to the previous element.
        }
        training_array[j + 1] = key; // We insert the current element in its correct position.
    }
}

// Quick Sort algorithm for sorting training sessions by time
int partition(struct Training training_array[], int low, int high) {
    struct Training pivot = training_array[high]; // We choose the last element as the pivot.
    int i = low - 1; // We set the index of the smaller element.
    for (int j = low; j <= high - 1; ++j) { // We iterate through the array.
        if (training_array[j].time < pivot.time) { // If the current element is smaller than the pivot.
            ++i; // We increment the index of the smaller element.
            swap(&training_array[i], &training_array[j]); // We swap the elements.
        }
    }
    swap(&training_array[i + 1], &training_array[high]); // We swap the pivot element with the element at the correct position.
    return (i + 1); // We return the index of the pivot element.
}

void quick_sort_time(struct Training training_array[], int low, int high) {
    if (low < high) { // If there is more than one element in the array.
        int pi = partition(training_array, low, high); // We partition the array.
        quick_sort_time(training_array, low, pi - 1); // We recursively sort the left subarray.
        quick_sort_time(training_array, pi + 1, high); // We recursively sort the right subarray.
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
