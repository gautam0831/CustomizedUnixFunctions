//This function sorts an input file and takes in different flags to either sort alphabetically, numerically, by string length, and also provides the option to exclude duplicate entries
#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_LINE_LEN 4096
#define MIN_NLINES 100

typedef int (*cmp_fn_t)(const void *p, const void *q);
//Function takes in two void * pointers and allows for alphabetical ordering
int cmp_pstr(const void *p, const void *q) {
    
    return strcmp(*(const char **)p, *(const char **)q);
}
//Function takes in two void * pointers and allows for ordering by length of string
int cmp_pstr_len(const void *p, const void *q) {
    int p_len = strlen(*(const char **)p);
    int q_len = strlen(*(const char **)q);
    
    return p_len - q_len;
}
//Function takes in two void * pointers and allows for ordering by numeric value
int cmp_pstr_numeric(const void *p, const void *q) {

    return atoi(*(const char **)p) - atoi(*(const char **)q); 
}
/*
This function takes in a poitner to a file, a comparison function pointer, boolean values for both the -u flag and the -r flag. 
This function does not return anything but prints out the lines in sorted order, whether that be in alphabetical, numeric, or by length order. The lines can also be printed in reverse order if the -r flag is provided.
 */
void sort_lines(FILE *fp, cmp_fn_t cmp, bool uniq, bool reverse) {
   
    int arrSize = MIN_NLINES;
    char **strArr_original = malloc(sizeof(char *)*arrSize);
    assert(strArr_original != NULL);
    char **strArr = strArr_original;
    
    
    char arr[MAX_LINE_LEN];
    size_t count = 0;
    
    char *line;
    while ((line = fgets(arr, MAX_LINE_LEN, fp))!=NULL){ //Writes line into stack array
        
        if (uniq){
            if (count >= arrSize){ //Checks if needing more space on heap array
                arrSize *= 2;
                strArr_original = realloc(strArr_original, sizeof(char *)*arrSize);
                assert(strArr_original != NULL);
                
            }
            line = strdup(line);
            size_t oldcnt = count;
            char **new_line = binsert(&line, strArr_original, &count, sizeof(strArr_original[0]), cmp);
            
            assert(strcmp(line, *new_line) == 0);
            if (oldcnt == count){ //Frees line if it was a duplicate and is not being added to the array
                free(line);
            }
            
        }
        else{
            line = strdup(line);
            assert(line != NULL);
                
            if (count >= arrSize){
                arrSize = arrSize * 2;
                strArr_original = realloc(strArr_original, sizeof(char *)*arrSize);
                assert(strArr_original != NULL);
                strArr = strArr_original + count; //Repositions strArr_original after reallocation of space
            
            }
            strArr[0] = line;
            strArr ++;
            count ++;
        }
        
      
    }

    if (!uniq){ //Using qsort only once when -u flag is not provided
        size_t size = sizeof(strArr_original[0]);
        qsort(strArr_original, count, size, cmp);
    }
    if (!reverse){ //Printing lines in forward order
        for (int i = 0; i < count; i ++){
            printf("%s", strArr_original[i]);
            free(strArr_original[i]);
        }
    }
    else{//Printing lines in reverse order
        for (int i = count-1; i >= 0; i --){
            printf("%s", strArr_original[i]);
            free(strArr_original[i]);
        }
    }
    free(strArr_original);

    
}
//This main function handles arguments and flags inputted
//by client and sets cmp equal to the appropriate comparison
//function.The main function later calls the sort_lines function
//and then returns 0.
int main(int argc, char *argv[]) {
    cmp_fn_t cmp = cmp_pstr;
    bool uniq = false;
    bool reverse = false;

    int opt = getopt(argc, argv, "lnru");
    while (opt != -1) {
        if (opt == 'l') {
            cmp = cmp_pstr_len;
        } else if (opt == 'n') {
            cmp = cmp_pstr_numeric;
        } else if (opt == 'r') {
            reverse = true;
        } else if (opt == 'u') {
            uniq = true;
        } else {
            return 1;
        }

        opt = getopt(argc, argv, "lnru");
    }

    FILE *fp = stdin;
    if (optind < argc) {
        fp = fopen(argv[optind], "r");
        if (fp == NULL) {
            error(1, 0, "cannot access %s", argv[optind]);
        }
    }
    sort_lines(fp, cmp, uniq, reverse);
    fclose(fp);
    return 0;
}
