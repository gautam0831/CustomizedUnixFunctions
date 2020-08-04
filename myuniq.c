//This file prints out unique lines from a file along with the number of
//occurrences of each unique line.
#include "samples/prototypes.h"
#include <errno.h>
#include <error.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>

/* initial estimate of number of uniq lines
 * resize-as-you-go, add in increments of 100
 */
#define ESTIMATE 100

typedef struct duplicates{
    char *string;
    int count;
} duplicates;

/*
 *This function takes in a pointer to a file and prints out the unique lines
of the file along with the number of occurrences of each unique line. The function
creates space on the heap for an array of structs that hold the infomration of the string and the count of occurrence and calls read_line to read each individual line to assess whether or not this is a duplicate line. 
All allocated memory on the heap is also equally freed in this implementation, needing no freeing from the client.
 */
void print_uniq_lines(FILE *fp) {
    
    int curr_size = ESTIMATE;
    
    duplicates *arr = malloc(sizeof(duplicates)*curr_size);
    assert(arr != NULL);
    
    char *line;
  
    int counter = 0;
    while ((line=read_line(fp)) != NULL){
        
        if (counter >= curr_size){
            curr_size += ESTIMATE;
            arr = realloc(arr, sizeof(duplicates)*curr_size);
            
        }
        
        for (int i = 0; i < curr_size; i++){
            
            if (i >= counter){
                arr[i].string = line;
                arr[i].count = 1;
                
                counter ++;
                break;
            }
            else if (strcmp(line, arr[i].string) == 0){
                arr[i].count ++;
                free(line);
               
                break;
            }
            
        }
        

             
    } 
    
    for (int i = 0; i < counter; i++){
        int line_count = arr[i].count;
        char *printed_string = arr[i].string;
        printf("%7d %s\n", line_count, printed_string);
        free(arr[i].string);   
    }
    
    free(arr);
  
}


//This main function takes in the arguments to the function itself and calls
//the print_uniq_lines function to print the unique lines of the file.
int main(int argc, char *argv[]) {
    FILE *fp;

    if (argc == 1) {
        fp = stdin;
    } else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            error(1, errno, "cannot access '%s'", argv[1]);
        }
    }
    print_uniq_lines(fp);
    fclose(fp);
    return 0;
}
