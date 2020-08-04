//This file will print the final lines of a given file based on the input of a file and how many lines to print

#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NUM_LINES 100000
#define DEFAULT_NUM_LINES 10

/*
 *This function takes in a file and an integer and 
prints out the final n lines of the file (based off the provided input).
If no integer was provided, the default to print is 10 lines (or the whole file) whichever is smaller.
The array of strings is on the stack but the function calls read_line which
returns pointers to data on the heap.
 */
void print_last_n(FILE *fp, int n) {

    char *original[n];
    char **n_lines_original = original;
    
    
    char **n_lines = original;
    char **startArray = original;
        
    int line_count = 0;
    
    char **first = n_lines;
    char **last = n_lines;

    char *line;
    
    while ((line=read_line(fp)) != NULL){
        
        line_count ++;
        if (line_count > n){
            
            n_lines = n_lines_original;
            if (line_count % n == 0){
                n_lines_original -= n;
                n_lines_original ++;
                free(n_lines[0]);
               
            }
            else{
                
                free(n_lines_original[0]);
                n_lines_original ++;
            }
            
            n_lines[0] = line;
            
            first = n_lines_original;
            last = n_lines;
            
        }
        else{
            
            n_lines[0] = line;
            first = n_lines_original;
            last = n_lines;
            
        }
        
        n_lines ++;
        
           
        
    }
    int start = first - startArray;
    int end = last - startArray;
    
    
    for (int i = start; i != end; i = (i+1) % n){
        char *elem = startArray[i];
        printf("%s\n", elem);
        free(elem);
    }
    printf("%s\n", startArray[end]);
    free(startArray[end]);
    
      
}

// ------- DO NOT EDIT ANY CODE BELOW THIS LINE (but do add comments!)  -------
//This function takes in a string and a maximum integer that will allow to
//convert the number of lines given as input to the main into an integer
int convert_arg(const char *str, int max) {
    char *end;
    long n = strtol(str, &end, 10);
    if (*end != '\0') {
        error(1, 0, "Invalid number '%s'", str);
    }
    if (n < 1 || n > max) {
        error(1, 0, "%s is not within the acceptable range [%d, %d]", str, 1, max);
    }
    return n;
}
//This function will call the above functions to retrieve the inputs and
//call the function to print the last n lines of a file
int main(int argc, char *argv[]) {
    int num_lines = DEFAULT_NUM_LINES;

    if (argc > 1 && argv[1][0] == '-') {
        num_lines = convert_arg(argv[1] + 1, MAX_NUM_LINES);
        argv++;
        argc--;
    }

    FILE *fp;
    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }
    print_last_n(fp, num_lines);
    fclose(fp);
    return 0;
}
