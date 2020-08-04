//This file is an imlpementation of the ls command utilizing scandir and the dirent struct and orders the directories and files in a particular order
#include <dirent.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <error.h>
#include <string.h>
#include <stdlib.h>

enum { SORT_BY_NAME, SORT_BY_TYPE };
enum { EXCLUDE_DOT, INCLUDE_DOT };


/* On the myth filesystem, the only file type information that is accurate is
 * directory/not-directory used here. Other type info in struct dirent is 
 * not reliable.
 */

typedef struct dirent dirent;
//Determines if the entry is a directory 
bool is_dir(const struct dirent *dirptr) {
    return dirptr->d_type == DT_DIR;
}

//Takes a pointer to a dirent struct and returns 0 if the entry starts with a dot and returns 1 otherwise. This function will be called if the -a flag is NOT provided by client
int filterNoA(const dirent* item){
    const char *name = item->d_name;
    if (name[0] == '.'){
        return 0;
    }
    return 1;
}

//Takes in two double pointers to dirent structs and will be used to order two entries in alphabetic order
int order_alpha(const dirent **arr1, const dirent **arr2){

    const char *str1 = arr1[0]->d_name;
    const char *str2 = arr2[0]->d_name;
    
    return strcmp(str1, str2);
}
//Takes in two double pointers to dirent structs and will be used to order the entries in directory-first order (if both entries are directories or non-directories, they are sorted in alphabetic order
int order_dir(const dirent **arr1, const dirent **arr2){
    const dirent *elem1 = *arr1;
    const dirent *elem2 = *arr2;
    if (is_dir(elem1) && !is_dir(elem2)){
        return -1;
    }
    else if (is_dir(elem2) && !is_dir(elem1)){
        return 1;
    }
    else{
        return strcmp(elem1->d_name, elem2->d_name);
    }
    
    
}

typedef int (*filter_fn_type)(const dirent*);
typedef int (*order_fn_type)(const dirent **, const dirent **);
/*
This function takes in a pointer to the directory path, an integer value for the filter, and integer value for the ordering. The function prints out the directories and file entries in the provided directory. If the filter is nonzero, all entries are printed. If the order is nonzero, the entries are ordered by directories following file entries. If zero, the entries are ordered alphabetically. 
 */
void ls(const char *dirpath, int filter, int order) {

    struct dirent **namelist;
    int n;

    filter_fn_type filter_fn;
    order_fn_type order_fn;
    
    if (filter){
        filter_fn = NULL;
    }
    else {
        filter_fn = filterNoA;
    }

    if (order){
        order_fn = order_dir;
    }
    else{
        order_fn = order_alpha;
    }

    
    n = scandir(dirpath, &namelist, filter_fn, order_fn);
    int i = 0;
    if (n < 0)
        error(0, 0, "cannot access %s", dirpath);
    else {
        while (i<n) {
            if (is_dir(namelist[i])){
                printf("%s/\n", namelist[i]->d_name);
            }
            else{
                printf("%s\n", namelist[i]->d_name);
            }
            
            free(namelist[i]);
            ++i;
        }
        free(namelist);
    }
}


//Main functions handles input and determines the filter and order integer values based on the flags provided in input
int main(int argc, char *argv[]) {
    int order = SORT_BY_NAME;
    int filter = EXCLUDE_DOT;
    
    
    int opt = getopt(argc, argv, "az");
    while (opt != -1) {
        if (opt == 'a') {
            filter = INCLUDE_DOT;
        } else if (opt == 'z') {
            order = SORT_BY_TYPE;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "az");
    }
    
    if (optind < argc - 1) {
        for (int i = optind; i < argc; i++) {
            printf("%s:\n", argv[i]);
            ls(argv[i], filter, order);
            printf("\n");
        }
    } else {
       
        ls(optind == argc -1 ? argv[optind] : ".", filter, order);
    }
    
    return 0;
}
