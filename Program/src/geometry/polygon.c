#include "polygon.h"

#include <float.h>
#include <stdlib.h>

// Free the memory allocated for a polygon
void freePolygon(Polygon* polygon) {
    free(polygon->center);
    free(polygon->edges);
}

// Return the dot product of two vectors
double vectorDot(const Vector* vectorOne, const Vector* vectorTwo) {
    return ((vectorOne->x * vectorTwo->x) + (vectorOne->y * vectorTwo->y));
}

// Helper Functions and Structs for polygon intersection
typedef struct _Interval Interval;
struct _Interval {
    double min;
    double max;
};

// Projects a polygon onto a given axis line, and returns the interval it spans
static Interval projectOntoAxis(const Polygon* polygon, const Line* axis) {
    // Instantiate a projection
    Interval projection;
    projection.min = DBL_MAX;
    projection.max = DBL_MIN;

    // Project all edges of the polygon onto the axis
    Vector* center = polygon->center;
    Vector* edges = polygon->edges;
    for ( int i = 0; i < polygon->numEdges; i++ ) {
        // Find the vertex of the polygon relative to the axis line
        Vector relativeToAxis;
        relativeToAxis.x = center->x + edges[i].x - axis->point.x;
        relativeToAxis.y = center->y + edges[i].y - axis->point.y;

        // Project that vertex onto the axis
        double proj = (vectorDot(&(axis->direction), &relativeToAxis) / vectorDot(&(axis->direction), &axis->direction));

        // Take the minimum value
        if ( proj < projection.min ) {
            projection.min = proj;
        } 
        // Take the maximum value
        if ( proj > projection.max ) {
            projection.max = proj;
        }
    }

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
    Vector* center;
    Vector* edges;

    // Project polygons onto the first polygon's axis 
    center = polygonOne->center;
    edges = polygonOne->edges;
    for ( int i = 0; i < polygonOne->numEdges; i++ ) {
        // Find the axis perpendicular to the polygon's edge
        Line axis;

        axis.point.x = center->x + edges[i].x;
        axis.point.y = center->y + edges[i].y;
        axis.direction.x = -(edges[i+1].y - edges[i].y);
        axis.direction.y = edges[i+1].x - edges[i].x;

        // Project both polygons onto the axis
        Interval projectionOne = projectOntoAxis(polygonOne, &axis);
        Interval projectionTwo = projectOntoAxis(polygonTwo, &axis);

        // Check if the projections overlap
        // If they don't, there is no polygon intersection
        if ( !doProjectionsOverlap(projectionOne, projectionTwo) ) {
            return NOINTERSECTION;
        }
    }

    // Project polygons onto the second polygon's axis 
    edges = polygonTwo->edges;
    center = polygonTwo->center;
    for ( int i = 0; i < polygonTwo->numEdges; i++ ) {
        // Find the axis perpendicular to the polygon's edge
        Line axis;

        axis.point.x = edges[i].x + center->x;
        axis.point.y = edges[i].y + center->y;
        axis.direction.x = -(edges[i+1].y - edges[i].y);
        axis.direction.y = edges[i+1].x - edges[i].x;

        // Project both polygons onto the axis
        Interval projectionOne = projectOntoAxis(polygonOne, &axis);
        Interval projectionTwo = projectOntoAxis(polygonTwo, &axis);

        // Check if the projections overlap
        // If they don't, there is no polygon intersection
        if ( !doProjectionsOverlap(projectionOne, projectionTwo) ) {
            return NOINTERSECTION;
        }
    }

    // If all projections overlap, the polygons intersect
    return INTERSECTION;
}