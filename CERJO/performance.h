
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
//

#ifndef WILLIAMBODJRENO_PERFORMANCE_H
#define WILLIAMBODJRENO_PERFORMANCE_H
#define DATA_PATH "C:\\Users\\swann\\OneDrive\\Bureau\\desktop\\FIVERR\\williambodjreno\\data/"
typedef enum {TYPE_100_M, TYPE_400_M, TYPE_5000_M, TYPE_MARATHON, TYPE_RELAI} type_perf;

typedef struct perf{
    struct tm date;
    type_perf type;
    int relai_info;
    struct tm result;
    struct perf *next;
    struct perf *prev;
} performance;

typedef struct {
    performance *first;
    performance *last;
} list_perf;

typedef struct {
    char **names;
    list_perf **performances;
    int nbAthletes;
} all_perfs;

typedef struct {
    double slope;
    double intercept;
} linear_regression;



void add_performance(list_perf *performances, performance *performance);
void delete_performance(list_perf *performances, performance *performance);
performance *create_performance(struct tm date, type_perf type, struct tm result, int relai_info);
list_perf *get_performances_from_file(char *filename);
void save_performances_to_file(char *filename, list_perf *performances);
void *join_perfs(list_perf *ref, list_perf *performances2);
void add_performance_without_join(list_perf *performances, performance *performance);
performance *check_performance_for_a_date(list_perf *performances, struct tm date);

void display_performance(performance *performance);
void display_performances(list_perf *performances);
void display_performances2(all_perfs performances);

all_perfs get_all_performances();

list_perf *sort_performances_by_type_athlete(list_perf *performances, type_perf type);
list_perf *sort_performances_by_date_athlete(list_perf *performances, struct tm date);
list_perf *sort_performances_by_type_and_date_athlete(list_perf *performances, type_perf type, struct tm date);

all_perfs sort_performances_by_type(all_perfs performances, type_perf type);
all_perfs sort_performances_by_date(all_perfs performances, struct tm date);
all_perfs sort_performances_by_type_and_date(all_perfs performances, type_perf type, struct tm date);



performance *best_performance(list_perf *performances, type_perf type);
performance *worst_performance(list_perf *performances, type_perf type);
struct tm average_performance(list_perf *performances, type_perf type);
struct tm performance_difference(list_perf *performances, type_perf type, struct tm date1, struct tm date2);
linear_regression calculate_linear_regression(list_perf *performances, type_perf type);



#endif //WILLIAMBODJRENO_PERFORMANCE_H
