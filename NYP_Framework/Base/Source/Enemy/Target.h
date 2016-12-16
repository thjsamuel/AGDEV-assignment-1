#ifndef TARGET_H
#define TARGET_H

#include "../GenericEntity.h"

struct Target
{
    Target();
    Target(GenericEntity* entity);
    ~Target();
    GenericEntity* board;
    Vector3 position;
    Vector3 destination;
    void Update(const double dt);
};

#endif