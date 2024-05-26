
#include "performance.h"
#include <stdbool.h>
#ifndef WILLIAMBODJRENO_ATHLETE_H
#define WILLIAMBODJRENO_ATHLETE_H

typedef struct ath {
    char name[100];
    list_perf *performances;
    int nbPerformances;
    struct ath *next;
    struct ath *prev;
} athlete;

//Liste des athletes
typedef struct {
    athlete *first;
    athlete *last;
    int nbAthletes;
} list_athletes;

athlete *create_athlete(char *name);

void init_athlete_perf(athlete *ath, list_perf *performances);
void add_athlete(list_athletes *athletes, athlete *ath);
void add_athlete_without_join(list_athletes *athletes, athlete *ath);
void delete_athlete(list_athletes *athletes, athlete *ath);
list_athletes *init_all_athletes();
void display_athletes(list_athletes *athletes);
void create_athlete_file(athlete *ath);
athlete *find_by_name(list_athletes *athletes,char *name);

void athlete_performance_summary(list_athletes *athletes, char *name, type_perf type);
void best_athletes_for_jo(list_athletes *athletes, type_perf type);
void athlete_progression(athlete *ath, type_perf type, struct tm date1, struct tm date2);
list_athletes *best_three_athletes(list_athletes *athletes, type_perf type);

int compare_athletes(athlete *a, athlete *b, type_perf type);
athlete *first_with_perf(list_athletes *athletes, type_perf type);
bool athlete_has_perf(athlete *ath, type_perf type);
#endif //WILLIAMBODJRENO_ATHLETE_H
