#include "head.h"

/**
 * Sets the value of random seed. If a number was received through the command
 * line, sets it as random seed. Otherwise, it sets it to NIP_SEED.
*/
void processArguments(Parameters *p, int n_arguments, char **arguments) {

    int numero;
        if (n_arguments == 2) {
        numero = atoi(arguments[1]);
        p->random_seed = numero; 
    } 
    else {
        p->random_seed = NIP_SEED;
    } 
}

/**
 * Counts the number of lines in a file.
*/
int countLines(char *filename) {
    char ch;
    FILE *f;
    int count = 0;

    f = fopen(filename, "r");
    while((ch = fgetc(f)) != EOF) {
        if(ch == '\n') {
            count++;
        }
    }
    fclose(f);

    return count;
}

/**
 * Counts the total number of segments in all the buildings.
*/
int countSegments() {
    int sum = 0; 
    int i=0;
    char nombre[MAX_STRING_LENGTH];

    for(i=0; i<N_SHAPES; i++)
    {
    sprintf(nombre,"data/building_coordinates/coordinates_%d.txt",i);
    sum += countLines(nombre)-2;
    }
        return(sum);
}


/**
 * Reads the segments contained in the file named filename, stores
 * them in the array named segment, and increases the value of n_segments
 * by the amount of segments read.
*/
void readBuilding(char *filename, Segment *segment, int *n_segments) {
    FILE *f;
    int c, n;
    Point p1, p2;
    char trash[MAX_STRING_LENGTH];

    f = fopen(filename, "r");
    // Skip header
    c = fscanf(f, "%s %s", trash, trash);
    if (c != 2) printf("Error skipping header in %s\n", filename);

    n = 0;
    c = fscanf(f, "%lf %lf", &p1.x, &p1.y);
    if (c != 2) printf("Error reading first line in %s\n", filename);
    while(fscanf(f, "%lf %lf", &p2.x, &p2.y) != EOF) {
        segment[n] = (Segment){.start = p1, .end = p2};
        p1 = p2;
        n++;
    }
    fclose(f);

    *n_segments += n;
}

/**
 * Reads the N_SHAPES building files and stores the segments in city.segments and
 * the total number of segments in the list in city.n_segments.
*/
void loadSegments(SegmentList *city) {
    city->n_segments = 0;
    char nombre[MAX_STRING_LENGTH]; 
    for (int i = 0; i < N_SHAPES; i++)
    {                                                
        sprintf(nombre, "data/building_coordinates/coordinates_%d.txt", i); 
        readBuilding(nombre, city->segment + city->n_segments, &(city->n_segments));
    }
}
/**
 * Reads the points in data/computacional.txt and stores them in p.
*/
void loadParameters(char *filename, Parameters *p) {
    FILE *f;
    double value;
    char name[MAX_STRING_LENGTH];

    f = fopen(filename, "r");
    while(fscanf(f, "%s %lf", name, &value) != EOF) {
        if (!strcmp(name, "x_start")) p->start.x = value;
        if (!strcmp(name, "y_start")) p->start.y = value;
        if (!strcmp(name, "x_ciencias")) p->home.x = value;
        if (!strcmp(name, "y_ciencias")) p->home.y = value;
        if (!strcmp(name, "x_destination")) p->destination.x = value;
        if (!strcmp(name, "y_destination")) p->destination.y = value;
        if (!strcmp(name, "destination_radius")) p->destination_radius = value;
        if (!strcmp(name, "step_size")) p->step_size = value;
    }
    fclose(f);
}

/**
 * If METROPOLIS is defined, stores all the steps taken by the particle
 * in the file "results/metropolis.txt". Otherwise, stores them in
 * "results/brownian.txt". In both cases, the file will end up with n_steps
 * lines, each of them with two columns representing the x and y coordinates
 * of the step.
*/
void writeSteps(Point *steps, int n_steps) {
    
    FILE *f;
    int i=0;

    #if defined ( METROPOLIS ) && ! defined ( POWERUP )
    f = fopen("results/metropolis.txt", "w"); 
    # elif defined ( METROPOLIS ) && defined ( POWERUP )
    f = fopen("results/powerup.txt", "w");
    # else
    f = fopen("results/brownian.txt", "w");
    # endif

    for(i=0; i<n_steps;i++)
    {
    fprintf(f,"%f %f \n",steps[i].x,steps[i].y);
    }
    fclose(f);
}





