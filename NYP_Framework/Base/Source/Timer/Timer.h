#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
    Timer(float limiter) : countdown(0.f), run(false) { limit = limiter; };
    ~Timer() {};
    void Update(const float dt, int multiplier = 5);
    float countdown;
    bool run;
private:
    float limit;
};

#endif