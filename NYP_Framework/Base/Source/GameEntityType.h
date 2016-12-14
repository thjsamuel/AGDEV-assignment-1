#ifndef GAME_ENTITY_TYPE_H
#define GAME_ENTITY_TYPE_H

struct EntityType
{
    EntityType(){ entity_type = TARGET; };
    ~EntityType(){};

    enum ENTITY_TYPE
    {
        TEXT = 0,
        TARGET,
        MAX_ENTITY_TYPE
    };

    ENTITY_TYPE entity_type;
};

#endif