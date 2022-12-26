#pragma once

#include <stdlib.h>

#include "../geometry/polygon.h"

#define ENTITY_LAND 0
#define ENTITY_OBJECT 1

typedef struct _Entity Entity;

struct _Entity {
    Vector* position;
    Vector* velocity;

    Polygon* hitbox;
};

// Create a new entity
Entity* newEntity(int hitboxEdges);

// Free an entity from memory
void freeEntity(Entity *entity);

// Check if two entities touch
int entityIntersection(const Entity *e1, const Entity *e2);

// Update entity (milliseconds elapsed)
void entityUpdate(const Entity *e, int milliElapsed);