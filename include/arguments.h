#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

const char* args[5] = {
    "help", "random", "gosper-gun", "ark", "gliderless"
};

char* prompt()
{
    printf("Welcome to Jacob's Cellular Automata\n");
    printf("Enter 'start' for a random sequence, or 'cells' to see available cell files: ");
    char input[6];
    scanf("%s", input);
    char start[6] = "start";
    char cells[6] = "cells";
    if (strcmp(input, start) == 0)
    {
        printf("Starting random automata.\n");
        char *file = "";
        return file;
    }
    else if(strcmp(input, cells) == 0)
    {
        struct dirent *entry;
        DIR *cell_dir = opendir("cell_files");
        if (cell_dir == NULL)
        {
            printf("Couldn't find cells\n");
        }
        int i = 0;
        while ((entry = readdir(cell_dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            i++;

            printf("%d\t%s\n", i, entry->d_name);
        }
        closedir(cell_dir);
        cell_dir = opendir("cell_files");
        printf("Enter 1-%d to select a cell file: ", i);
        int selection;
        scanf("%d", &selection);
        int j = 0;
        while ((entry = readdir(cell_dir)) != NULL)
        {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            j++;
            if (selection == j)
            {
                printf("%s\n", entry->d_name);
                char* file = entry->d_name;
                return file;
            }
            
        }
    }
    else
        printf("prompt failed");
}

void parse_args(int argc, char* argv[])
{
    // for (int i = 0; i < 5; i++)
    // {
    //     if (argc == 1 || (strcmp(argv[1], args[i]) != 0))
    //     {
    //         printf("Please enter some valid arguments try \"./automata help\"\n");
    //         exit(0);
    //     }
    // }

    struct dirent *entry;
    DIR *cell_dir = opendir("cell_files");  
    if (cell_dir == NULL)
    {
        printf("Couldn't find cells\n");
    }
    while ((entry = readdir(cell_dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }
    closedir(cell_dir);
}