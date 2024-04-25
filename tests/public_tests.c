#include "../head.h"
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define RUN_TEST(test_function) do { \
    n_test++; \
    printf("%d) ", n_test); \
    status = test_function(); \
    if(status) { \
        printf("OK\n"); \
        passed++; \
    } \
} while(0)

#define EPSILON 1e-1
#define APPROX_EQUAL(x, y) (fabs((x) - (y)) < EPSILON)


bool test_processArguments() {
    Parameters p;
    char *arguments[] = {"brownian.exe", "42"};

    printf("Testing processArguments...");

    p.random_seed = 0;
    processArguments(&p, 0, arguments);
    if (p.random_seed != NIP_SEED) {
        printf("\n\tERROR: Wrong seed when there are no arguments, expected %d, obtained %d\n", NIP_SEED, p.random_seed);
        return false;
    }

    p.random_seed = 0;
    processArguments(&p, 1, arguments);
    if (p.random_seed != NIP_SEED) {
        printf("\n\tERROR: Wrong seed when there is 1 argument, expected %d, obtained %d\n", NIP_SEED, p.random_seed);
        return false;
    }

    p.random_seed = 0;
    processArguments(&p, 3, arguments);
    if (p.random_seed != NIP_SEED) {
        printf("\n\tERROR: Wrong seed when there are 3 arguments, expected %d, obtained %d\n", NIP_SEED, p.random_seed);
        return false;
    }

    p.random_seed = 0;
    processArguments(&p, 2, arguments);
    if (p.random_seed != 42) {
        printf("\n\tERROR: Wrong seed when there are 2 arguments, expected 42, obtained %d\n", p.random_seed);
        return false;
    }

    return true;

}

bool test_countSegments() {
    int n_segments, n_segments_expected;

    printf("Testing countSegments...");

    n_segments = countSegments();
    n_segments_expected = 366558;
    if (n_segments != n_segments_expected) {
        printf("\n\tERROR: Wrong number of segments, expected %d, obtained %d\n", n_segments_expected, n_segments);
        return false;
    }

    return true;
}

bool test_loadSegments() {
    Segment segment;
    Point p1, p2;
    SegmentList city;
    int s, n_segments;

    printf("Testing loadSegments...");

    n_segments = 366558;
    city.segment = malloc(n_segments * sizeof *city.segment);
    loadSegments(&city);

    if (city.n_segments != n_segments) {
        printf("\n\tERROR: Wrong number of segments, expected %d, obtained %d\n", n_segments, city.n_segments);
        return false;
    }

    s = 0;
    p1 = (Point){.x = 678526.14, .y = 4612295.7};
    p2 = (Point){.x = 678522.42, .y = 4612304.93};
    segment = (Segment){.start = p1, .end = p2};

    if (!APPROX_EQUAL(city.segment[s].start.x, segment.start.x) || 
        !APPROX_EQUAL(city.segment[s].start.y, segment.start.y) ||
        !APPROX_EQUAL(city.segment[s].end.x, segment.end.x) ||
        !APPROX_EQUAL(city.segment[s].end.y, segment.end.y)) {
        printf("\n\tERROR: Wrong segment in position %d, expected (%.1lf, %.1lf) -> (%.1lf, %.1lf), obtained (%.1lf, %.1lf) -> (%.1lf, %.1lf)\n",\
        s, segment.start.x, segment.start.y, segment.end.x, segment.end.y,\
        city.segment[s].start.x, city.segment[s].start.y, city.segment[s].end.x, city.segment[s].end.y);

        return false;
    }

    s = 366557;
    p1 = (Point){.x = 678933.611, .y = 4614622.4345};
    p2 = (Point){.x = 678929.492, .y = 4614621.9225};
    segment = (Segment){.start = p1, .end = p2};

    if (!APPROX_EQUAL(city.segment[s].start.x, segment.start.x) || 
        !APPROX_EQUAL(city.segment[s].start.y, segment.start.y) ||
        !APPROX_EQUAL(city.segment[s].end.x, segment.end.x) ||
        !APPROX_EQUAL(city.segment[s].end.y, segment.end.y)) {
        printf("\n\tERROR: Wrong segment in position %d, expected (%.1lf, %.1lf) -> (%.1lf, %.1lf), obtained (%.1lf, %.1lf) -> (%.1lf, %.1lf)\n",
        s, segment.start.x, segment.start.y, segment.end.x, segment.end.y,
        city.segment[s].start.x, city.segment[s].start.y, city.segment[s].end.x, city.segment[s].end.y);

        return false;
    }

    free(city.segment);

    return true;
}

bool test_writeSteps() {
    Point steps[10];
    FILE *f;
    int new_lines, n_lines = 10;

    printf("Testing writeSteps...");

    for(int i = 0; i < n_lines; i++)
        steps[i] = (Point){.x = 1, .y = 1};

    writeSteps(steps, n_lines);

    f = fopen("results/metropolis.txt", "r");
    if (f == NULL) {
        printf("\n\tERROR: The file results/metropolis.txt does not exist\n");
        return false;
    }
    fclose(f);

    new_lines = countLines("results/metropolis.txt");
    if (new_lines != n_lines) {
        printf("\n\tERROR: The file results/metropolis.txt should have %d lines but has %d lines\n", n_lines, new_lines);
        return false;
    }

    return true;
}

bool test_substractPoints() {
    Point p1, p2, p_final;
    
    printf("Testing substractPoints...");

    p1 = (Point){.x = 1, .y = 2};
    p2 = (Point){.x = 2, .y = 5};
    p_final = substractPoints(p1, p2);

    if (p_final.x != -1 ||
        p_final.y != -3) {
        printf("\n\tERROR: (%.0lf, %.0lf) - (%.0lf, %.0lf) should be (%.0lf, %.0lf) but is (%.0lf, %.0lf)\n",
        p1.x, p1.y, p2.x, p2.y, 1.0, 3.0, p_final.x, p_final.y);
        return false;
    }

    return true;
}

bool test_intersectionTest() {
    double numerator, denominator;

    printf("Testing intersectionTest...");

    denominator = 1; numerator = -1;
    if (intersectionTest(numerator, denominator) == true) {
        printf("\n\tERROR: Wrong intersection test, with num = %.0lf den = %.0lf expected false but got true\n", numerator, denominator);
        return false;
    }

    denominator = 1; numerator = 2;
    if (intersectionTest(numerator, denominator) == true) {
        printf("\n\tERROR: Wrong intersection test, with num = %.0lf den = %.0lf expected false but got true\n", numerator, denominator);
        return false;
    }

    denominator = 1; numerator = 1;
    if (intersectionTest(numerator, denominator) == false) {
        printf("\n\tERROR: Wrong intersection test, with num = %.0lf den = %.0lf expected true but got false\n", numerator, denominator);
        return false;
    }

    denominator = -1; numerator = 1;
    if (intersectionTest(numerator, denominator) == true) {
        printf("\n\tERROR: Wrong intersection test, with num = %.0lf den = %.0lf expected false but got true\n", numerator, denominator);
        return false;
    }

    denominator = -1; numerator = -2;
    if (intersectionTest(numerator, denominator) == true) {
        printf("\n\tERROR: Wrong intersection test, with num = %.0lf den = %.0lf expected false but got true\n", numerator, denominator);
        return false;
    }

    denominator = -2; numerator = -1;
    if (intersectionTest(numerator, denominator) == false) {
        printf("\n\tERROR: Wrong intersection test, with num = %.0lf den = %.0lf expected true but got false\n", numerator, denominator);
        return false;
    }

    return true;
}

bool test_segmentsIntersect() {
    Segment s1, s2;
    
    printf("Testing segmentsIntersect...");

    s1 = (Segment){.start = (Point){.x = 0, .y = 1}, .end = (Point){.x = 1, .y = 0}};
    s2 = (Segment){.start = (Point){.x = 0, .y = 0}, .end = (Point){.x = 1, .y = 1}};
    if (segmentsIntersect(s1, s2) == false) {
        printf("\n\tERROR: segments (%.0lf, %.0lf) - (%.0lf, %.0lf) and (%.0lf, %.0lf) - (%.0lf, %.0lf) should intersect, but got false\n",
        s1.start.x, s1.start.y, s1.end.x, s1.end.y, s2.start.x, s2.start.y, s2.end.x, s2.end.y);
        return false;
    }

    s1 = (Segment){.start = (Point){.x = 1, .y = 0}, .end = (Point){.x = 0, .y = 1}};
    s2 = (Segment){.start = (Point){.x = 0, .y = 0}, .end = (Point){.x = 1, .y = 1}};
    if (segmentsIntersect(s1, s2) == false) {
        printf("\n\tERROR: segments (%.0lf, %.0lf) - (%.0lf, %.0lf) and (%.0lf, %.0lf) - (%.0lf, %.0lf) should intersect, but got false\n",
        s1.start.x, s1.start.y, s1.end.x, s1.end.y, s2.start.x, s2.start.y, s2.end.x, s2.end.y);
        return false;
    }

    s1 = (Segment){.start = (Point){.x = 0, .y = 1}, .end = (Point){.x = 1, .y = 0}};
    s2 = (Segment){.start = (Point){.x = 0, .y = 0}, .end = (Point){.x = -1, .y = -1}};
    if (segmentsIntersect(s1, s2) == true) {
        printf("\n\tERROR: segments (%.0lf, %.0lf) - (%.0lf, %.0lf) and (%.0lf, %.0lf) - (%.0lf, %.0lf) should not intersect, but got true\n",
        s1.start.x, s1.start.y, s1.end.x, s1.end.y, s2.start.x, s2.start.y, s2.end.x, s2.end.y);
        return false;
    }

    return true;
}

bool test_distPoints() {
    Point p1, p2;
    double distance, expected;
    
    printf("Testing distPoints...");

    p1 = (Point){.x = 0, .y = 1};
    p2 = (Point){.x = 2, .y = 3};

    expected = sqrt(8);
    distance = distPoints(p1, p2);
    if (!APPROX_EQUAL(distance, expected)) {
        printf("\n\tERROR: the distance between (%.0lf, %.0lf) and (%.0lf, %.0lf) should be %.1lf but got %.1f\n", p1.x, p1.y, p2.x, p2.y, expected, distance);
        return false;
    }

    return true;
}

bool test_pathIntersect() {
    Segment segment;
    SegmentList city;
    
    printf("Testing pathIntersect...");

    city.n_segments = 2;
    city.segment = malloc(city.n_segments * sizeof *city.segment);

    city.segment[0] = (Segment){.start = (Point){.x = 0, .y = 0}, .end = (Point){.x = 1, .y = 1}};
    city.segment[1] = (Segment){.start = (Point){.x = 1, .y = 1}, .end = (Point){.x = 5, .y = 1}};

    segment = (Segment){.start = (Point){.x = 2, .y = 2}, .end = (Point){.x = 0, .y = 0}};
    if (pathIntersect(segment, city) == false) {
        printf("\n\tERROR: in a city with the obstacles:\n");
        for(int i = 0; i < city.n_segments; i++)
            printf("\t\t segment[%d] = (%.0lf, %.0lf) - (%.0lf, %.0lf)\n",
                    i, city.segment[i].start.x, city.segment[i].start.y, city.segment[i].end.x, city.segment[i].end.y);

        printf("\tThe path (%.0lf, %.0lf) - (%.0lf, %.0lf) should intersect with one of them, but pathIntersect returned false\n",
                segment.start.x, segment.start.y, segment.end.x, segment.end.y);
        return false;
    }

    segment = (Segment){.start = (Point){.x = 2, .y = 2}, .end = (Point){.x = 3, .y = 3}};
    if (pathIntersect(segment, city) == true) {
        printf("\n\tERROR: in a city with the obstacles:\n");
        for(int i = 0; i < city.n_segments; i++)
            printf("\t\t segment[%d] = (%.0lf, %.0lf) - (%.0lf, %.0lf)\n",
                    i, city.segment[i].start.x, city.segment[i].start.y, city.segment[i].end.x, city.segment[i].end.y);

        printf("\tThe path (%.0lf, %.0lf) - (%.0lf, %.0lf) should not intersect with any of them, but pathIntersect returned true\n",
                segment.start.x, segment.start.y, segment.end.x, segment.end.y);
        return false;
    }

    free(city.segment);

    return true;
}

bool test_gaussian2D() {
    Parameters p;
    Point p_new, p_old;
    double result, expected;

    printf("Testing gaussian2D...");

    p.home = (Point){.x = 0, .y = 0};
    p_new = (Point){.x = sqrt(30000), .y = 0};
    p_old = (Point){.x = 0, .y = sqrt(10000)};

    result = gaussian2D(p, p_old, p_new);
    expected = exp(-1);

    if (!APPROX_EQUAL(result, expected)) {
        printf("\n\tERROR: For points new = (%.0lf, %0.lf) old = (%.0lf, %.0lf), expected %.2lf but got %.2lf\n",
                p_new.x, p_new.y, p_old.x, p_old.y, expected, result);
        return false;
    }

    return true;
}

bool test_brownianMotion() {
    Parameters parameters;
    SegmentList city;
    Point *steps;
    int n_steps;
    PointList restaurants;
    
    printf("Testing brownianMotion...");

    parameters.start = (Point){.x = 676452.6, .y = 4613323};
    parameters.destination = (Point){.x = 676452.6 + 1, .y = 4613323 + 1};
    parameters.destination_radius = 20;
    parameters.step_size = 10;
    parameters.home = (Point){.x = 0, .y = 0};

    restaurants.place = malloc(sizeof(Point));
    restaurants.place[0] = (Point){.x = 0, .y = 0};
    restaurants.n_places = 1;

    city.n_segments = 0;
    steps = NULL;
    n_steps = brownianMotion(parameters, city, &steps, restaurants);

    if (n_steps != 1) {
        printf("\n\tERROR: For start (%.0lf, %0.lf) and destination (%.0lf, %.0lf), with radius = %.0lf, the particle should arrive in 1 step but arrived in %d\n",
                parameters.start.x, parameters.start.y, parameters.destination.x, parameters.destination.y, parameters.destination_radius, n_steps);
        return false;
    }

    if (steps != NULL)
        free(steps);

    parameters.step_size = 0;
    parameters.destination_radius = 0;

    city.n_segments = 0;
    steps = NULL;
    n_steps = brownianMotion(parameters, city, &steps, restaurants);

    if (n_steps != MAX_STEPS) {
        printf("\n\tERROR: For start (%.0lf, %0.lf) and destination (%.0lf, %.0lf), with radius = %.0lf and step = %.0lf, the number of steps should be %d but got %d\n",
                parameters.start.x, parameters.start.y, parameters.destination.x, parameters.destination.y, parameters.destination_radius, parameters.step_size, MAX_STEPS, n_steps);
        return false;
    }

    if (steps != NULL) {
        free(steps);
    }

    free(restaurants.place);

    return true;
}

bool test_powerUp() {
    Parameters p;
    Point current_point;
    PointList restaurants;

    printf("Testing powerUp...");

    restaurants.place = malloc(2 * sizeof(Point));
    restaurants.place[0] = (Point){.x = 5, .y = 5};
    restaurants.place[1] = (Point){.x = -5, .y = -5};
    restaurants.n_places = 2;
    p.destination_radius = 2;

    current_point = (Point){.x = 4, .y = 4};
    if (!powerUp(p, restaurants, current_point)) {
        printf("\n\tERROR: There is a restaurant (%.0lf, %.0lf), within reach of the particle (%.0lf, %.0lf), expected true\n",
        restaurants.place[0].x, restaurants.place[0].y, current_point.x, current_point.y);
        return false;
    }
    
    current_point = (Point){.x = -4, .y = -4};
    if (!powerUp(p, restaurants, current_point)) {
        printf("\n\tERROR: There is a restaurant (%.0lf, %.0lf), within reach of the particle (%.0lf, %.0lf), expected true\n",
        restaurants.place[1].x, restaurants.place[1].y, current_point.x, current_point.y);
        return false;
    }

    current_point = (Point){.x = 0, .y = 0};
    if (powerUp(p, restaurants, current_point)) {
        printf("\n\tERROR: The restaurants [(%.0lf, %.0lf), (%.0lf, %.0lf)] are too far from (%.0lf, %.0lf), expected false\n",
        restaurants.place[0].x, restaurants.place[0].y, restaurants.place[1].x, restaurants.place[1].y, current_point.x, current_point.y);
        return false;
    }

    free(restaurants.place);

    return true;
}

int main() {
    int passed = 0, n_test = 0;
    bool status = true;

    printf("************************************\n* TESTING BROWNIAN MOTION CODE *\n************************************\n\n");
    RUN_TEST(test_processArguments);
    RUN_TEST(test_countSegments);
    RUN_TEST(test_loadSegments);
    RUN_TEST(test_writeSteps);
    RUN_TEST(test_substractPoints);
    RUN_TEST(test_intersectionTest);
    RUN_TEST(test_segmentsIntersect);
    RUN_TEST(test_distPoints);
    RUN_TEST(test_pathIntersect);
    RUN_TEST(test_gaussian2D);
    RUN_TEST(test_brownianMotion);
    RUN_TEST(test_powerUp);

    printf("\nTests succesfully completed: %d/%d\n", passed, n_test);

    return n_test - passed;
}
