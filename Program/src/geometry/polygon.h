#pragma once

// Represents whether or not two polygons intersect
#define INTERSECTION 1
#define NOINTERSECTION 0

// Struct Vector
// Contains the x and y coordinate of a given vector in 2D space
typedef struct _Vector Vector;

struct _Vector {
    double x;           // X-Coordinate
    double y;           // Y-Coordinate
};

// Struct Line
// Represents a line, with a point and directional vector.
typedef struct _Line Line;

struct _Line {
    Vector point;       // Reference Point
    Vector direction;   // Directional Vector
};

// Struct Polygon
// Contains the positions of the polygon's edges (relative to it's center),
// as well as the polygon's center.
typedef struct _Polygon Polygon;

// Note: The edges member variable is an array of vertices, where the first and last entry are the same vector
struct _Polygon {
    Vector* center;     // Reference to the Center of the Polygon

    int     numEdges;   // Number of Edges of the Polygon
    Vector* edges;      // Location of the Polygon's Edges
};

// Returns if two polygons intersect
int polygonIntersection(const Polygon* polygonOne, const Polygon* polygonTwo);

// Rotate a polygon counterclockwise about it's center
void polygonRotate(const Polygon* polygon, double theta);

// Creates a random convex polygon
void polygonRandom(Polygon* polygon, double size);