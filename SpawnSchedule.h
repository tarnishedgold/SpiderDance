#pragma once

// One entry of the spider spawn schedule
struct SpiderSpawn
{
    float time;
    int line;
    int direction;
    float speed;
};

// One entry of the wave spawn schedule
struct WaveSpawn
{
    float time;
    int level;
};