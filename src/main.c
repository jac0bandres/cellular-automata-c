#include <stdlib.h>
#include <stdio.h>

#include "map.h"
#include "graphics.h"
#include "arguments.h"

int main(int argc, char *argv[])
{
    Map map;
    View view;
    char *cell_file = prompt();
    init_map(cell_file, &map);
    init_window(map.dimensions, &view);
    while (view.state == true)
    {
        poll_input(&view);
        draw(map.size, map.map, &view);
        play_map(&map);
    }
    return 0;
}