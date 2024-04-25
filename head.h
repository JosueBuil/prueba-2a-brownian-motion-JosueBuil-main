#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

#define NIP_SEED 845955
#define N_SHAPES 17890
#define MAX_STRING_LENGTH 256
#define CITY_CENTER ((Point){.x = 676452.6, .y = 4613323})
#define MAX_DISTANCE_CITY 3000
#define GAUSSIAN_FACTOR 0.00005
#define MAX_STEPS 100000

typedef struct Point {
    double x;
    double y;
} Point;

typedef struct PointList {
    Point *place;
    int n_places;
} PointList;

typedef struct Segment {
    Point start;
    Point end;
} Segment;

typedef struct SegmentList {
    Segment *segment;
    int n_segments;
} SegmentList;

typedef struct Parameters {
    int random_seed;
    Point start;
    Point home;
    Point destination;
    double destination_radius;
    double step_size;
    int max_power_up;    
} Parameters;

//**********PARISI RAPUANO*************
#define NormRANu (2.3283063671E-10F)

extern unsigned int irr[256];
extern unsigned int ir1;
extern unsigned char ind_ran,ig1,ig2,ig3;

extern float Random(void);
extern void ini_ran(int SEMILLA);
//************************************

// geometry.c
extern Point substractPoints(Point p1, Point p2);
extern bool intersectionTest(double numerator, double denominator);
extern bool segmentsIntersect(Segment s1, Segment s2);
extern double distPoints(Point p1, Point p2);

// input_output.c
extern void processArguments(Parameters *p, int n_arguments, char **arguments);
extern int countLines(char *filename);
extern int countSegments();
extern void readBuilding(char *filename, Segment *segment, int *n_segments);
extern void loadSegments(SegmentList *city);
extern void loadParameters(char *filename, Parameters *p);
extern void writeSteps(Point *steps, int n_steps);

// movement.c
extern bool pathIntersect(Segment s, SegmentList city);
extern double gaussian2D(Parameters p, Point p_old, Point p_new);
extern int brownianMotion(Parameters parameters, SegmentList city, Point **steps, PointList restaurants);

// parisi.c
extern float randomIn(double min, double max);
extern float Random(void);
extern void  ini_ran(int SEMILLA);

// food.c
extern void loadRestaurants(PointList *restaurants);
extern bool powerUp(Parameters p, PointList restaurants, Point current_point);