#include <SDL2/SDL.h>
#include <stdio.h>

#include "geometry/polygon.h"

#define SCREEN_WIDTH 1280 
#define SCREEN_HEIGHT 720

int main(int argc, char** argv){
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    // ---
    Vector* center = malloc(sizeof(Vector));
    center->x = SCREEN_WIDTH / 2;
    center->y = SCREEN_HEIGHT / 2 - 5;

    Polygon polygonOne;
    polygonOne.center = center;
    printf("%d", polygonOne.center->x);
    polygonOne.numEdges = 3;
    polygonOne.edges = malloc(sizeof(Vector) * (3 + 1));

    Vector v1 = {-50, 0};
    Vector v2 = {-50, 50};
    Vector v3 = {50, 50};
    Vector v4 = {-50, 0};
    polygonOne.edges[0] = v1;
    polygonOne.edges[1] = v2;
    polygonOne.edges[2] = v3;
    polygonOne.edges[3] = v4;

    // printf("%d", polygonOne.edges[0].x);
    // ---
    Vector* center2 = malloc(sizeof(Vector));
    center2->x = SCREEN_WIDTH - 200;
    center2->y = SCREEN_HEIGHT / 2;

    Polygon polygonTwo;
    polygonTwo.center = center2;
    polygonTwo.numEdges = 3;
    polygonTwo.edges = malloc(sizeof(Vector) * (3 + 1));

    Vector _v1 = {-50, 0};
    Vector _v2 = {-50, 50};
    Vector _v3 = {50, 50};
    Vector _v4 = {-50, 0};
    polygonTwo.edges[0] = _v1;
    polygonTwo.edges[1] = _v2;
    polygonTwo.edges[2] = _v3;
    polygonTwo.edges[3] = _v4;

    int running = 0;
    while(running == 0){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = 1;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    center->y += 5;
                    break;
                    
                default:
                    break;
            }
        }

        
        // printf("SKSKS");
        if ( polygonIntersection(&polygonOne, &polygonTwo) ) {
            // printf("Intersect");
        } else {
            // printf("Do not intersect");
            
            center2->x -= 0.5;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        for ( int i = 0; i < polygonOne.numEdges; i++ ) {
            Vector one = polygonOne.edges[i];
            Vector two = polygonOne.edges[i + 1];

            SDL_RenderDrawLine(renderer, 
                polygonOne.center->x + one.x, 
                polygonOne.center->y + one.y, 
                polygonOne.center->x + two.x, 
                polygonOne.center->y + two.y);
        }
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
        for ( int i = 0; i < polygonTwo.numEdges; i++ ) {
            Vector one = polygonTwo.edges[i];
            Vector two = polygonTwo.edges[i + 1];

            SDL_RenderDrawLine(renderer, 
                polygonTwo.center->x + one.x, 
                polygonTwo.center->y + one.y, 
                polygonTwo.center->x + two.x, 
                polygonTwo.center->y + two.y);
        }

        SDL_RenderPresent(renderer);
    }
    
    freePolygon(&polygonOne);
    freePolygon(&polygonTwo);

    return 0;
}