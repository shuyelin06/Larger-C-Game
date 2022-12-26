#include "entity.h"

// Create a new entity
Entity* newEntity(int hitboxEdges) {
    // Allocate entity in memory
    Entity *newEntity = malloc(sizeof(Entity));

    // Allocate position of entity
    newEntity->position = malloc(sizeof(Vector));
    // Preinitialize to 0
    newEntity->position->x = 0;
    newEntity->position->y = 0;

    // Allocate velocity of entity
    Vector *velocity = malloc(sizeof(Vector));
    newEntity->velocity = velocity;
    // Preinitialize to 0
    newEntity->velocity->x = 0;
    newEntity->velocity->y = 0;

    // Allocate entity hitbox in memory
    Polygon *hitbox = malloc(sizeof(Polygon));
    // Allocate hitbox vertices and vertex count
    hitbox->numEdges = hitboxEdges;
    hitbox->edges = malloc(sizeof(Vector) * (hitboxEdges + 1));
    // Attach entity position to hitbox
    hitbox->center = newEntity->position;

    // Attach hitbox to entity
    newEntity->hitbox = hitbox;
    
    return newEntity;
}

// Free an entity from memory
void freeEntity(Entity *entity) {
    free(entity->position);
    free(entity->velocity);

    free(entity->hitbox->edges);
    free(entity->hitbox);

    free(entity);
}

// Check if two entities touch
int entityIntersection(const Entity *e1, const Entity *e2) {
    return polygonIntersection(e1->hitbox, e2->hitbox);
}

// Update an entity
void entityUpdate(const Entity *e, int milliElapsed) {
    // Update position
    e->position->x += (e->velocity->x * milliElapsed / 1000);
    e->position->y += (e->velocity->y * milliElapsed / 1000);
    
    // Reset velocities
    e->velocity->x = 0;
    e->velocity->y = 0;
}