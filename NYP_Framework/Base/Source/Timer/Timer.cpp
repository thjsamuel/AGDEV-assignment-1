#include "Timer.h"

void Timer::Update(const float dt, int multiplier)
{
    if (countdown < limit && run)
        countdown += (multiplier * dt);
}