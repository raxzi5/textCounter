/*
Project 2: Word Counter 
Created by: Rachel Zheng
Submitted to: Xiao Qin
Last date updated: 2/12/25

This program creates a parent and child process and allows parallel processing
by utilizing pipes as a form of communication. The parent process reads a text 
file and sends the contents of the file to the child process. The child process 
reads the number of words and returns the count to the parent process. 

*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "functions.h"

#define BUFFER_SIZE 1024
#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]){

    //CASE:1 File name not provided (only one argument)
    if(argc != 2){
        printf("Usage: %s, <filename>\n", argv[0]);
        return 1;
    }

    //CASE 2&3 : File DNE and/or File can't be opened
    char *filename = argv[1];
    if(fopen(filename, "r") == NULL){
        printf("ERROR: Failed to read file. Check if file exists\n");
        return 1;
    } 
    
    //CASE 4: File is not .txt file 
    const char *ext = strrchr(filename, '.'); 
    if (!ext || strcmp(ext, ".txt") != 0){
        printf("ERROR: Filename must be a text file.\n");
        return 1;
    }

    //Create pipes & verify sucess
    pid_t pid;
    int Child_toParent[2], Parent_toChild[2];
    if(pipe(Child_toParent) == -1 || pipe(Parent_toChild) == -1){
        fprintf(stderr, "Pipe Failed!\n");
        return 1;
    }

    //Create fork & verfiy sucess
    pid = fork();
    if (pid==0){
        //child process
        // Child needs to write to parent, parent needs to read from child
        //close unused pipe ends
        close(Child_toParent[READ_END]);
        close(Parent_toChild[WRITE_END]);

        //child reads msg from Parent pipe
        char read_msg[BUFFER_SIZE];
        read(Parent_toChild[READ_END], read_msg, BUFFER_SIZE);
        printf("Process 2 finishes receiving data from Process 1...\n");

        //calling on count_words function to calculate # of words
        printf("Process 2 is counting words now...\n");
        int words = count_words(read_msg);

        //sends back count on child write pipe
        printf("Process 2 is sending the results back to Process 1...\n");
        write(Child_toParent[WRITE_END], &words, 2);

        //close used pipes ends
	    close(Child_toParent[WRITE_END]);
	    close(Parent_toChild[READ_END]);
		
    }
    else if(pid>0){
        //parent process
        //parent needs to write to child, child needs to read from parent
        //close unused pipe ends
        close(Parent_toChild[READ_END]);
        close(Child_toParent[WRITE_END]);

        //reading text file and assigning pointer
        char *msg = readfile(filename);
	    printf("Process 1 is reading file '%s' now...\n", filename);

        //parent pipe write end: content of text file to pipe
        write(Parent_toChild[WRITE_END], msg, strlen(msg)+1);
        printf("Process 1 starts sending data to Process 2...\n");
        
        //child pipe read end: count of words
        int count = 0;
        read(Child_toParent[READ_END], &count, sizeof(int));
        printf("Process 1: The total number of words is %d.\n", count);
        
        //free memory 
        free(msg);
        
        //close used pipes
        close(Parent_toChild[WRITE_END]);
        close(Child_toParent[READ_END]);

    }
    else{
        //fork failed
        fprintf(stderr, "Fork failed\n");
        return 1;
    }
    return 0;
}
