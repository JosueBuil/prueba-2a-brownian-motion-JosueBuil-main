#include "head.h"

/**
 * Loads the coordinates of any number of restaurants contained
 * in data/provisioning.txt
*/
void loadRestaurants(PointList *restaurants) {
    FILE *f;
    int c, n;
    Point p;
    char trash[MAX_STRING_LENGTH];

    f = fopen("data/provisioning.txt", "r");
    c = fscanf(f, "%s %s", trash, trash);
    if (c != 2) printf("Error skipping header in provisioning.txt\n");

    n = 0;
    while(fscanf(f, "%lf %lf", &p.x, &p.y) != EOF) {
        (*restaurants).n_places++;
        (*restaurants).place = realloc((*restaurants).place, (*restaurants).n_places * sizeof *((*restaurants).place));

        (*restaurants).place[n] = (Point){.x = p.x, .y = p.y};
        n++;
    }
    fclose(f);
}

/**
 * Determines if the current point is close enough to any restaurant. Returns true
 * if it is, false otherwise.
*/
bool powerUp(Parameters p, PointList restaurants, Point current_point) {
    int i = 0;
    for(i=0;i<restaurants.n_places;i++){
        if(distPoints(current_point,restaurants.place[i])<p.destination_radius)
        return true;
    }
    return false;
}