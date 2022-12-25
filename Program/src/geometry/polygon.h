#pragma once

// Struct Vector
/*
    Contains the x and y coordinate of a given vector.
*/
typedef struct _Vector Vector;

struct _Vector {
    double x;
    double y;
};

// Struct Line
/*
    Contains the line's point, and directional vector
*/
typedef struct _Line Line;

struct _Line {
    Vector point;
    Vector direction;
};

// Struct Polygon
/*
    Contains the positions of the polygon's edges (relative to it's center),
    as well as the polygon's center.
*/
typedef struct _Polygon Polygon;

struct _Polygon {
    Vector* center;

    int     numEdges;
    Vector* edges;
};

// Struct PolygonIntersection
/*
    Returns information regarding a polygon intersection
*/
#define INTERSECTION 1
#define NOINTERSECTION 0

// Frees the memory allocated to a polygon
void freePolygon(Polygon* polygon);

// Returns 1 if two polygons intersect, 0 otherwise
int polygonIntersection(const Polygon* polygonOne, const Polygon* polygonTwo);

// Returns the dot product of 2 vectors
double vectorDot(const Vector* vectorOne, const Vector* vectorTwo);