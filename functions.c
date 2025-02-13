/*
Project 2: Predefined unctions
Created by: Rachel Zheng
Submitted to: Xiao Qin
Last date updated: 2/12/25

--readfile(): opens and loads the content of a text file and returns a pointer 
to the contents stored in the file. Argument is a pointer to the file (*filename)

--count_words(): utilizes a for loop to count the number of words and returns 
an integer. Argument is a pointer to the contents saved (*msg)

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

char *readfile(const char *filename){
    // pointer to file
    FILE *fd = fopen(filename, "r");
    if (fd != NULL){
        printf("File loaded: %s\n", filename);
        //pointer & allocates memory 
        char *msg = (char*)malloc(1001);
        //loads contents of file
        size_t text = fread(msg, 1, 1000, fd);
        msg[text] = '\0';
        fclose(fd);
        //returns pointer
        return msg;
    }
    else{
        printf("Can't open file\n");
        return NULL;
    }
}

int count_words(char *msg){
    // checks if there is content
    if (msg == NULL){
        return 0;
    }
    
    int wordCounter = 0;
    int isWord = 0;
    //for loop for each of the characters and counting # of words
    for (int i = 0; i < strlen(msg); i++){
        char curr = msg[i];
        // if space, tab, newline, exclamation mark, period, comma, question mark
        //if isWord == 1, then we have reached end of the word, set it to 0
        if(curr == ' ' || curr == '\t' || curr == '\n' || 
            curr == '!' || curr == '.' || curr ==',' || curr == '?'){
            if(isWord ==1){
                isWord = 0;
            }
        }else{      
            //else if current (curr) is a character then set isWord to 1 to indicate beginning of word
            //increase word counter
            if(isWord==0){
                wordCounter++;
                isWord = 1;
            }

        }
    }
    //returns integer
    return wordCounter;
}