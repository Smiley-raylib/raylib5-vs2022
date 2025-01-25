#pragma once

struct Timer
{
    float current = 0.0f;
    float total = 0.0f;
};

inline void Tick(Timer& timer, float dt)
{
    timer.current += dt;
}

inline void Reset(Timer& timer)
{
    timer.current = 0.0f;
}

inline bool Expired(Timer timer)
{
    return timer.current >= timer.total;
}
