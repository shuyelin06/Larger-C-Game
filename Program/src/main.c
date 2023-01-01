#include <SDL2/SDL.h>
#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <settings.h>

#include <geometry/polygon.h>
#include <graphics/graphics.h>
#include <entity/entity.h>

#include <SDL2/SDL_image.h>

int main(int argc, char** argv){
    // Initialize Random Number Generator 
    srand(time(NULL));

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Error: SDL failed to initialize\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    // Initialize Window
    SDL_Window *window = SDL_CreateWindow("SLD test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if(!window){
        printf("Error: Failed to open window\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }

    if ( IMG_Init(IMG_INIT_PNG) == 0 ) {
        printf("Image fail");
        return 1;
    }
    SDL_Surface* sur = IMG_Load("C:\\Users\\shuye\\Documents\\GitHub\\Larger-C-Game\\Program\\src\\pineapple.png");
    if (sur == NULL) {
	    printf("Error loading image");
	    return 5;
    }
    

    // Initialize Renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("Error: Failed to create renderer\nSDL Error: '%s'\n", SDL_GetError());
        return 1;
    }
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, sur);
    if (tex == NULL) {
	    printf("Error creating texture");
	    return 6;
    }
    SDL_FreeSurface(sur);

    

    // List of Entities
    Entity *entityOne = newEntity(6);
    entityOne->position->x = SCREEN_CENTER_X - 25;
    entityOne->position->y = SCREEN_CENTER_Y;

    polygonRandom(entityOne->hitbox, 5);

    Entity *entityTwo = newEntity(8);
    entityTwo->position->x = SCREEN_CENTER_X;
    entityTwo->position->y = SCREEN_CENTER_Y;

    polygonRandom(entityTwo->hitbox, 5);
    
    // Keyboard State
    const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

    // Player Entity (Controllable)
    Entity *player = entityOne;

    // List of Entities
    Entity *entities[2];
    entities[0] = player; // Player will always be the first entity
    entities[1] = entityTwo;
    

    // Time Elapsed
    Uint64 lastUpdate = SDL_GetTicks64();

    int running = 0;
    while(running == 0){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    running = 1;
                    break;

                default:
                    break;
            }
        }
        
        // Keyboard Input
        if ( keyboard[SDL_SCANCODE_W] ) {
            entityOne->velocity->y = 25;}
        if ( keyboard[SDL_SCANCODE_A] )
            entityOne->velocity->x = -25;
        if ( keyboard[SDL_SCANCODE_S] )
            entityOne->velocity->y = -25;
        if ( keyboard[SDL_SCANCODE_D] )
            entityOne->velocity->x = 25;

        int intersect;

        // Update Timer
        Uint64 time = SDL_GetTicks64();
        // If change in time is sufficiently large, update the entities
        if ( time - lastUpdate > 0 ) {
            int numEntities = sizeof(entities) / sizeof(entities[0]);
            for ( int i = 0; i < numEntities; i++ ) {
                entityUpdate(entities[i], time - lastUpdate);
            }

            // polygonRotate(entityOne->hitbox, 0.001);

            intersect = entityIntersection(entityOne, entityTwo);

            lastUpdate = time;
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
        renderPolygon(renderer, entityOne->hitbox);
        
        if ( intersect ) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            renderPolygon(renderer, entityTwo->hitbox);
        } else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            renderPolygon(renderer, entityTwo->hitbox);
        }
        SDL_Rect rect = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 50, 50};
        SDL_RenderCopy(renderer, tex, NULL, &rect);
        SDL_RenderPresent(renderer);
    }

    free(entityOne);
    free(entityTwo);

    SDL_DestroyTexture(tex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    IMG_Quit();
    SDL_Quit();

    return 0;
}