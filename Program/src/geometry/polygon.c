#include "polygon.h"

// Needed for double max and min values
#include <float.h>

/* Defining Helper Structs and Functions for Polygon Intersection */
// Struct Interval
// Will hold information regarding a polygon's projectiomn onto an axis (the min and max)
// value of it's projection 
typedef struct _Interval Interval;

struct _Interval {
    double min;         // Left Bound
    double max;         // Right Bound
};

// Struct Line
// Represents a line, with a point and directional vector.
typedef struct _Line Line;

struct _Line {
    Vector point;       // Reference Point
    Vector direction;   // Directional Vector
};

// Returns the dot product of two vectors (to be used in projections) 
static inline double vectorDot(const Vector* vectorOne, const Vector* vectorTwo) {
    return ((vectorOne->x * vectorTwo->x) + (vectorOne->y * vectorTwo->y));
}

// Projects a polygon onto some axis line, and returns the interval it spans
static Interval projectOntoAxis(const Polygon* polygon, const Line* axis) {
    // Instantiate a projection
    Interval projection = {DBL_MAX, DBL_MIN};

    // Project all edges of the polygon onto the axis
    Vector* center = polygon->center; // Obtain the polygon's center
    Vector* edges = polygon->edges; // Obtain the polygon's edges

    // Iterate through all of the polygon's edges
    for ( int i = 0; i < polygon->numEdges; i++ ) {
        // Find the vertex of the polygon relative to the axis line
        Vector relativeToAxis = {
            center->x + edges[i].x - axis->point.x, // X-Coordinate
            center->y + edges[i].y - axis->point.y  // Y-Coordinate
        };

        // Project that vertex onto the axis line
        double proj = (vectorDot(&(axis->direction), &relativeToAxis) / vectorDot(&(axis->direction), &axis->direction));

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
        return 0;
    // Otherwise, intervals must overlap, so return true (1)
    else
        return 1;
}

// Check if two polygons intersect
int polygonIntersection(const Polygon* polygonOne, const Polygon* polygonTwo) {
    Vector* center; // Reference to a polygon's center
    Vector* edges; // Reference to a polygon's edges

    // Check for intersection along the first polygon's edge axes 
    center = polygonOne->center;
    edges = polygonOne->edges;

    for ( int i = 0; i < polygonOne->numEdges; i++ ) {
        // Find the axis perpendicular to the polygon's edge
        Line axis = {
            center->x + edges[i].x, // X-Coordinate
            center->y + edges[i].y, // Y-Coordinate
            {
                -(edges[i+1].y - edges[i].y), // Directional Vector X
                edges[i+1].x - edges[i].x // Directional Vector Y
            }
        };

        // Project both polygons onto this axis
        Interval projectionOne = projectOntoAxis(polygonOne, &axis);
        Interval projectionTwo = projectOntoAxis(polygonTwo, &axis);

        // Check if the projections overlap
        // If they don't, the polygons cannot intersect by the separating axis theorem
        if ( !doProjectionsOverlap(projectionOne, projectionTwo) ) {
            return NOINTERSECTION;
        }
    }

    // Check for intersection along the second polygon's edge axes 
    edges = polygonTwo->edges;
    center = polygonTwo->center;

    for ( int i = 0; i < polygonTwo->numEdges; i++ ) {
        // Find the axis perpendicular to the polygon's edge
        Line axis = {
            center->x + edges[i].x, // X-Coordinate
            center->y + edges[i].y, // Y-Coordinate
            {
                -(edges[i+1].y - edges[i].y), // Directional Vector X
                edges[i+1].x - edges[i].x // Directional Vector Y
            }
        };

        // Project both polygons onto this axis
        Interval projectionOne = projectOntoAxis(polygonOne, &axis);
        Interval projectionTwo = projectOntoAxis(polygonTwo, &axis);

        // Check if the projections overlap
        // If they don't, the polygons cannot intersect by the separating axis theorem
        if ( !doProjectionsOverlap(projectionOne, projectionTwo) ) {
            return NOINTERSECTION;
        }
    }

    // If all projections overlap, the polygons must necessarily intersect
    return INTERSECTION;
}