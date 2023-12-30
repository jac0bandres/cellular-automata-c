#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <errno.h>
#include <string.h>

typedef struct Map
{
    int **map;
    int capacity;
    float rate;
    int size;
    int dimensions[2];
    int generation;
    int view_fd[2];
    int **buffer;
    int buffer_size;
} Map;

void write_map_from_file(char *file_path, Map *map)
{
    FILE *fptr;
    size_t buffer_size = 32;
    char *buffer = (char *)malloc(buffer_size * sizeof(char));
    size_t read;
    char dir[1000];
    strcpy(dir, "cell_files/");
    strcat(dir, file_path);
    // strcat(dir, file_path);
    fptr = fopen(dir, "r");

    if (buffer == NULL)
        exit(1);

    if (fptr == NULL)
    {
        printf("Error opening file.");
        return;
    }
    else
        printf("Opening file %s\n", dir);

    map->dimensions[0] = 0;
    int line_y = 0;
    while ((read = getline(&buffer, &buffer_size, fptr)) != -1)
    {
        if (buffer[0] == 33)
            continue;

        int char_x = 0;
        for (int i = 0; i < read; i++)
        {
            if (buffer[i] == 79)
            {
                if (char_x > map->dimensions[0])
                    map->dimensions[0] = char_x;
            }
            char_x++;
        }
        line_y++;
    }
    map->size = 0;
    fclose(fptr);
    free(buffer);
    fptr = fopen(dir, "r");

    buffer = (char *)malloc(buffer_size * sizeof(char));
    int margin_y = floor(line_y / 2);
    int margin_x = floor(map->dimensions[0] / 2);
    map->dimensions[1] = line_y + (2 * margin_y);
    map->dimensions[0] = map ->dimensions[0] + (2 * margin_x);
    printf("(%d, %d) ", map->dimensions[0], map->dimensions[1]);
    map->capacity = map->dimensions[0] * map->dimensions[1];
    map->map = (int **)malloc(map->capacity * sizeof(int *));
    line_y = 0;
    while ((read = getline(&buffer, &buffer_size, fptr)) != -1)
    {
        if (buffer[0] == 33)
            continue;

        int char_x = 0;
        for (int i = 0; i < read; i++)
        {
            if (buffer[i] == 79)
            {
                int *cell = (int *)malloc(2 * sizeof(int));
                cell[0] = char_x + margin_x;
                cell[1] = line_y + margin_y;
                map->map[map->size] = cell;
                map->size++;
            }
            char_x++;
        }
        line_y++;
    }
    free(buffer);
    fclose(fptr);
}

void write_map_random(Map *map)
{
    map->dimensions[0] = 50;
    map->dimensions[1] = 50;
    map->capacity = map->dimensions[0] * map->dimensions[1];
    map->map = (int **)malloc(map->capacity * sizeof(int *));
    for (int i = 0; i < map->capacity / 2; i++)
    {
        int *cell = (int *)malloc(2 * sizeof(int));

        cell[0] = floor(0 + rand() % 50);
        cell[1] = floor(0 + rand() % 50);

        map->size = i;
        map->map[i] = cell;
    }
}

void add_cell(int x, int y, Map *map)
{
    int *cell = (int *)malloc(2 * sizeof(int));
    cell[0] = x;
    cell[1] = y;
    map->buffer[map->buffer_size] = cell;
    map->buffer_size++;
}

bool find_cell_state(int x, int y, Map *map)
{
    for (int i = 0; i < map->size; i++)
    {
        int *cell = map->map[i];
        // printf("(%d, %d) ", cell[0], cell[1]);

        if (cell[0] == x && cell[1] == y)
        {
            return true;
        }
    }
    return false;
}

int iterate_neighbors(int x, int y, Map *map)
{
    int population = 0;
    for (int xx = -1; xx < 2; xx++)
    {
        for (int yy = -1; yy < 2; yy++)
        {
            if (xx == 0 && yy == 0)
            {
                // printf("skip");
                continue;
            }

            int cell = find_cell_state(x + xx, y + yy, map);
            if (cell != false)
            {
                population++;
                // printf("population: %d", population);
            }
        }
    }
    return population;
}

void config_cell(int x, int y, bool state, int population, Map *map)
{
    if (state == false && population == 3)
    {
        add_cell(x, y, map);
    }
    else if (state == true && (population == 2 || population == 3))
    {
        add_cell(x, y, map);
    }
}

void clean_map(int size, int **map)
{
    for (int i = 0; i < size; i++)
    {
        free(map[i]);
    }
    map = NULL;
}

void print_map(int size, int **map)
{
    for (int i = 0; i < size; i++)
    {
        int *cell = map[i];
        printf("(%d, %d) ", cell[0], cell[1]);
    }
}

void play_map(Map *map)
{
    // print_map(map->size, map->map);
    map->buffer_size = 0;
    map->buffer = (int **)malloc(map->capacity * sizeof(int *));

    for (int x = 0; x < map->dimensions[0]; x++)
    {
        for (int y = 0; y < map->dimensions[1]; y++)
        {
            bool state = find_cell_state(x, y, map);
            int population = iterate_neighbors(x, y, map);
            config_cell(x, y, state, population, map);
        }
    }
    clean_map(map->size, map->map);
    map->size = map->buffer_size;
    map->map = map->buffer;
    map->buffer = NULL;
}

void init_map(char *file_path, Map *map)
{
    if (file_path != "")
    {
        write_map_from_file(file_path, map);
    }
    else
    {
        write_map_random(map);
    }
}