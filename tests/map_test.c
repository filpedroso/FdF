#include "../src/fdf.h"

int main()
{
    t_map map;
    char *file_path = "/home/filpedroso/42-repositories/fdf/resources/maps/test_maps/42.fdf";

    parse_map(&map, file_path);

    printf("Map width: %d\n", map.width);
    printf("Map height: %d\n", map.height);
    for(int i = 0; i < map.width * map.height; i++)
        printf("%d ", map.map_data[i]);
    printf("\n");
}
