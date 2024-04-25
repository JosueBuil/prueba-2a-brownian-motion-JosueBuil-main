#include "head.h"

/**
 * Checks if the segment s intersects with any wall in the city.
*/
bool pathIntersect(Segment s, SegmentList city) {

    int i = 0;
    for (i = 0; i < city.n_segments; i++)
    {                                                  
        if(segmentsIntersect(s,city.segment[i])==true)
        return true;
    }
        return false;
} 


/**
 * If METROPOLIS is defined, returns the quotient P(p_new)/P(p_old) 
 * where P(x,y) is the 2D Gaussian probability centered in p.home
 * with 0.5/sigma**2 = GAUSSIAN_FACTOR. Otherwise, returns 1.
*/
double gaussian2D(Parameters p, Point p_old, Point p_new) {



#ifdef METROPOLIS
//Como vamos ha ahcer el cociente, las constantes de normalización se anularan por lo que no es necesario calcularlas.
double prob_old = exp(-GAUSSIAN_FACTOR*((p_old.x-p.home.x)*(p_old.x-p.home.x)+(p_old.y-p.home.y)*(p_old.y-p.home.y)));
double prob_new = exp(-GAUSSIAN_FACTOR*((p_new.x-p.home.x)*(p_new.x-p.home.x)+(p_new.y-p.home.y)*(p_new.y-p.home.y)));

return(prob_new/prob_old);
#else
return 1;
#endif

}

/**
 * Simulates a brownian particle moving from start to destination. Whenever
 * the next step intersects with a wall, the particle just stays still.
 * The process ends when the particle reaches the destination or the number 
 * of steps is over MAX_STEPS. The function stores each step in steps and returns
 * the total number of steps taken.
*/
int brownianMotion(Parameters parameters, SegmentList city, Point **steps, PointList restaurants) {
    
    *steps = malloc(sizeof(Point) * (MAX_STEPS+1));

    (*steps)[0].x = parameters.start.x;
    (*steps)[0].y = parameters.start.y;

    float epsilon_x = 0;   
    float epsilon_y = 0;
    double w = 0; 

    Point Nuevo;
    Nuevo.x = 0;
    Nuevo.y = 0;

    Segment LineaNueva;
    LineaNueva.start.x = 0;
    LineaNueva.start.y = 0;
    LineaNueva.end.x = 0;
    LineaNueva.end.y = 0;

    int i = 0;
    int energizate = 0;
    for (i = 0; i < MAX_STEPS; i++) {

    epsilon_x = randomIn(-parameters.step_size, parameters.step_size);   
    epsilon_y = randomIn(-parameters.step_size, parameters.step_size);   
    w = randomIn(0, 1);        

    #if defined POWERUP
    
        if(powerUp(parameters,restaurants,(*steps)[i])==true && energizate < parameters.max_power_up)
    {
        epsilon_x=2*epsilon_x;
        epsilon_y=2*epsilon_y;
        energizate += 1;
    }
    #endif

    Nuevo.x = (*steps)[i].x;
    Nuevo.y = (*steps)[i].y;
    Nuevo.x = Nuevo.x + epsilon_x;
    Nuevo.y = Nuevo.y + epsilon_y;
    
    LineaNueva.start = (*steps)[i];
    LineaNueva.end = Nuevo;
          
    if (gaussian2D(parameters, (*steps)[i], Nuevo) > w && pathIntersect(LineaNueva, city) == false && distPoints(Nuevo, CITY_CENTER) < MAX_DISTANCE_CITY) {
        (*steps)[i+1].x = Nuevo.x;
        (*steps)[i+1].y = Nuevo.y;} 
        else {
        (*steps)[i+1].x = (*steps)[i].x;
        (*steps)[i+1].y = (*steps)[i].y;
        }   
        if (distPoints(parameters.destination, (*steps)[i+1]) < parameters.destination_radius) 
        {return i+1;}
    }
    return i;
}