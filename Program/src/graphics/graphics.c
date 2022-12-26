#include "graphics.h"

#include "../settings.h"

// Render a Polygon using SDL2
void renderPolygon(SDL_Renderer *renderer, const Polygon *polygon) {
    Vector* center = polygon->center;
    Vector* edges = polygon->edges;

    Vector vertexOne;
    Vector vertexTwo = { 
        (center->x + edges[0].x) * PIXELS_PER_UNIT, 
        SCREEN_HEIGHT - (center->y + edges[0].y) * PIXELS_PER_UNIT
    };

    for ( int i = 1; i < polygon->numEdges + 1; i++ ) {
        vertexOne = vertexTwo;

        vertexTwo.x = (center->x + edges[i].x) * PIXELS_PER_UNIT;
        vertexTwo.y = SCREEN_HEIGHT - (center->y + edges[i].y) * PIXELS_PER_UNIT;

        SDL_RenderDrawLine(renderer, 
            vertexOne.x, vertexOne.y, 
            vertexTwo.x, vertexTwo.y
        );
    }
}

// Renders a line
void renderLine(SDL_Renderer *renderer, const Line* line) {
    SDL_RenderDrawLine(renderer, 
        line->point.x * PIXELS_PER_UNIT, 
        SCREEN_HEIGHT - line->point.y * PIXELS_PER_UNIT,
        (line->point.x + line->direction.x) * PIXELS_PER_UNIT,
        SCREEN_HEIGHT - (line->point.y + line->direction.y) * PIXELS_PER_UNIT
    );
}