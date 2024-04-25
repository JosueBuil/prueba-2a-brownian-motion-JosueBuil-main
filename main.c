#include "head.h"


int main(int argc, char **argv) {
    Parameters parameters;
    SegmentList city;
    Point *steps;
    PointList restaurants;
    int n_steps, t0;
    
    /* Initialize the system */
    printf("Loading data...\n");
    processArguments(&parameters, argc, argv);
    ini_ran(parameters.random_seed);

    city.n_segments = countSegments();
    city.segment = malloc(city.n_segments * sizeof *city.segment);

    loadSegments(&city);
    loadParameters("data/computacional.txt", &parameters);

    restaurants.n_places = 1;
    restaurants.place = malloc(restaurants.n_places * sizeof *restaurants.place);
    loadRestaurants(&restaurants);

    /* Run the simulation */
    printf("Running simulation...\n");

    t0 = time(NULL);
    n_steps = brownianMotion(parameters, city, &steps, restaurants);
    writeSteps(steps, n_steps);
    
    printf("Simulation finished after %d seconds\n", (int)time(NULL) - t0);

    /* Clean up */
    free(city.segment);
    free(steps);
    free(restaurants.place);
}