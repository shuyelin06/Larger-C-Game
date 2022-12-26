#pragma once

#include <SDL2/SDL.h>
#include <geometry/polygon.h>

// Renders a polygon using SDL2
void renderPolygon(SDL_Renderer *renderer, const Polygon *polygon);

// Renders a line using SDL2
void renderLine(SDL_Renderer *renderer, const Line* line);