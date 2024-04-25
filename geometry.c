#include "head.h"

/**
 * Given two points (x1, y1) and (x2, y2) returns
 * the point (x1 - x2, y1 - y2).
*/
Point substractPoints(Point p1, Point p2) {
    return (Point){.x = p1.x-p2.x, .y = p1.y-p2.y};
}

/**
 * Performs the test proposed by Franklin Antonio, returning false
 * if the lines do not intersect and true otherwise.
*/
bool intersectionTest(double numerator, double denominator) {
    
    if (denominator > 0)
    {if (numerator < 0 || numerator > denominator)
    return false;
    }
    else if (numerator > 0 || numerator < denominator)
    {return false;}
    return true;
}

/**
 * Returns true if segments s1 and s2 intersect, false otherwise.
*/
bool segmentsIntersect(Segment s1, Segment s2) {
    Point A = substractPoints(s1.end,s1.start);
    Point B = substractPoints(s2.start,s2.end); 
    Point C = substractPoints(s1.start,s2.start);  

    double numeratoralpha   = B.y*C.x-B.x*C.y;
    double numeratorbeta   = A.x*C.y-A.y*C.x;
    double denominator  = A.y*B.x-A.x*B.y;
    if(denominator==0)
    return(false);
    else if(intersectionTest(numeratoralpha,denominator)==false)
    return(false);
    else if(intersectionTest(numeratorbeta,denominator)==false)
    return(false);
    return(true);
}

/**
 * Returns the eucledian distance between points p1 and p2.
*/
double distPoints(Point p1, Point p2) {
    return sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}