#include "utils.h"




bool is_correct_format(char *name){
    int i = 0;
    while(name[i] != ' ' && name[i] != '\0'){// Boucle pour parcourir le nom jusqu'à trouver un espace ou la fin de la chaîne.
        i++;
    }
    if(name[i] == '\0'){ // Si aucun espace n'a été trouvé, retourne false.
        return false;
    }
    i++; // Passe au caractère suivant après l'espace.
    if(name[i] == '\0'){  // Si la chaîne se termine immédiatement après l'espace, retourne false.
        return false;
    }
    return true;
}


type_perf get_type_perf(char *type){
    if(strcmp(type, "100m") == 0) { // Compare la chaîne avec "100m".
        return TYPE_100_M;// Retourne TYPE_100_M si les chaînes sont égales.
    }
    else if(strcmp(type, "400m") == 0) {// Compare la chaîne avec "400m".
        return TYPE_400_M;// Retourne TYPE_400_M si les chaînes sont égales.
    }
    else if(strcmp(type, "5000m") == 0) {// Compare la chaîne avec "5000m".
        return TYPE_5000_M;// Retourne TYPE_5000_M si les chaînes sont égales.
    }
    else if(strcmp(type, "Marathon") == 0) { // Compare la chaîne avec "Marathon".
        return TYPE_MARATHON;// Retourne TYPE_MARATHON si les chaînes sont égales.
    }
    else if(strcmp(type, "Relai") == 0) { // Compare la chaîne avec "Relai".
        return TYPE_RELAI; // Retourne TYPE_RELAI si les chaînes sont égales.
    }
    else {
        return -1;// Retourne -1 si aucun type de performance ne correspond.
    }

}

void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);// Boucle pour lire et ignorer les caractères jusqu'à trouver un saut de ligne ('\n') ou la fin de fichier (EOF).
}





