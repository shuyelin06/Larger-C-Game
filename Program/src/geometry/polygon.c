#include "polygon.h"

// Needed for double max and min values
#include <float.h>
// Needed for sin and cos trig functions
#include <math.h> 
// Needed for memory management
#include <stdlib.h>

// Rotate a polygon counterclockwise about it's center
void polygonRotate(const Polygon* polygon, double theta) {
    // Obtain the polygon's vertices
    Vector* edges = polygon->edges; 

    // Iterate through each vertex
    for ( int i = 0; i < polygon->numEdges + 1; i++ ) {
        // Obtain the original x and y coordinates
        double x = edges[i].x;
        double y = edges[i].y;

        // Rotate each vertex
        edges[i].x = x * cos(theta) - y * sin(theta);
        edges[i].y = x * sin(theta) + y * cos(theta);
    }
}

/* Defining Helper Functions for Random Polygon */
// Creates a random convex polygon
void polygonRandom(Polygon* polygon, double size) {
    // Obtain the polygon's edges
    Vector* edges = polygon->edges;

    // Randomize the polygon vertices, but while ensuring the polygon stays convex
    double angleInc = 2 * 3.14159 / polygon->numEdges;
    for ( int i = 0; i < polygon->numEdges; i++ ) {
        double angle = angleInc * i + ( rand() % 11 + 1. ) / 10. * angleInc;

        edges[i].x = size * cos(angle);
        edges[i].y = size * sin(angle);
    }
    // Ensure the beginning and end vertex of the array are the same
    edges[polygon->numEdges].x = edges[0].x;
    edges[polygon->numEdges].y = edges[0].y;
}

/* Defining Helper Structs and Functions for Polygon Intersection */
// Struct Interval
// Will hold information regarding a polygon's projectiomn onto an axis (the min and max)
// value of it's projection 
typedef struct _Interval Interval;

struct _Interval {
    double min;         // Left Bound
    double max;         // Right Bound
};

// Returns the dot product of two vectors (to be used in projections) 
static inline double vectorDot(const Vector* vectorOne, const Vector* vectorTwo) {
    return ((vectorOne->x * vectorTwo->x) + (vectorOne->y * vectorTwo->y));
}

// Projects a polygon onto some axis line, and returns the interval it spans
static Interval projectOntoAxis(const Polygon* polygon, const Vector* axis) {
    // Instantiate a projection
    Interval projection = {DBL_MAX, -DBL_MAX};

    // Project all edges of the polygon onto the axis
    Vector* center = polygon->center; // Obtain the polygon's center
    Vector* edges = polygon->edges; // Obtain the polygon's edges

    // Iterate through all of the polygon's edges
    for ( int i = 0; i < polygon->numEdges; i++ ) {
        // Find the vertex of the polygon relative to the axis line
        Vector vertex = {
            center->x + edges[i].x, // X-Coordinate
            center->y + edges[i].y  // Y-Coordinate
        };

        // Project that vertex onto the axis line
        double proj = vectorDot(axis, &vertex);

        // Update the interval minimum and maximum, as appropriate
        if ( proj < projection.min ) // Updating the minimum
            projection.min = proj;
        if ( proj > projection.max ) // Updating the maximum
            projection.max = proj;
    }

    // Return the projection
    return projection;
} 

// Checks if intervals of projections overlap
static int doProjectionsOverlap(const Interval projectionOne, const Interval projectionTwo) {
    // If either interval is outside the other, return false (0)
    if ( projectionOne.max < projectionTwo.min || projectionTwo.max < projectionOne.min )
        return NOINTERSECTION;
    // Otherwise, intervals must overlap, so return true (1)
    else {
        return INTERSECTION;
    }
}

// Check if two polygons intersect
int polygonIntersection(const Polygon* polygonOne, const Polygon* polygonTwo) {
    // Check for intersection along the first polygon's edge axes 
    Vector* edgesOne = polygonOne->edges;
    for ( int i = 0; i < polygonOne->numEdges; i++ ) {
        // Find the axis perpendicular to the polygon's edge
        Vector axis = {
            (edgesOne[i+1].y - edgesOne[i].y), // Directional Vector X
            (edgesOne[i].x - edgesOne[i+1].x) // Directional Vector Y
        };

        // Project both polygons onto this axis and check if they overlap
        // If they don't, the polygons cannot intersect by the separating axis theorem
        int projectionOverlap = doProjectionsOverlap( 
                                        projectOntoAxis(polygonOne, &axis), 
                                        projectOntoAxis(polygonTwo, &axis) ); 
        if ( projectionOverlap == NOINTERSECTION ) {
            return NOINTERSECTION;
        }
    }

    // Check for intersection along the second polygon's edge axes 
    Vector* edgesTwo = polygonTwo->edges;
    for ( int i = 0; i < polygonTwo->numEdges; i++ ) {
        // Find the axis perpendicular to the polygon's edge
        Vector axis = {
            (edgesTwo[i+1].y - edgesTwo[i].y), // Directional Vector X
            (edgesTwo[i].x - edgesTwo[i+1].x) // Directional Vector Y
        };

        // Project both polygons onto this axis and check if they overlap
        // If they don't, the polygons cannot intersect by the separating axis theorem
        int projectionOverlap = doProjectionsOverlap( 
                                        projectOntoAxis(polygonOne, &axis), 
                                        projectOntoAxis(polygonTwo, &axis) ); 
        if ( projectionOverlap == NOINTERSECTION ) {
            return NOINTERSECTION;
        }
    }

    // If all projections overlap, the polygons must necessarily intersect
    return INTERSECTION;
}