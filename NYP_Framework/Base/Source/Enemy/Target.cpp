#include "Target.h"

Target::Target(GenericEntity* entity)
{ board = entity; position = Vector3(0, 0, 0); }

Target::Target()
{
    position = Vector3(0, 0, 0);
    destination = Vector3(0, 0, 0);
}


Target::~Target()
{

}

void Target::Update(const double dt)
{
    int speed = 20;
    if (board->GetPosition().x < destination.x)
        board->SetPositionX(board->GetPosition().x + (speed * dt));
    if (board->GetPosition().x > destination.x)
        board->SetPositionX(board->GetPosition().x - (speed * dt));
    if (board->GetPosition().y < destination.y)
        board->SetPositionX(board->GetPosition().y + (speed * dt));
    if (board->GetPosition().y > destination.y)
        board->SetPositionX(board->GetPosition().y - (speed * dt));
    if (board->GetPosition().z < destination.z)
        board->SetPositionX(board->GetPosition().z + (speed * dt));
    if (board->GetPosition().z > destination.z)
        board->SetPositionX(board->GetPosition().z - (speed * dt));
    else
        return;
}