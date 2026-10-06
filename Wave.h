#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

class Wave {
private:
    static const int LINES_COUNT = 3; // number of lines; static const so it can be used as an array size
    int WINDOW_SIZE = 480;
    sf::Clock Clock;

    sf::RectangleShape shapes[LINES_COUNT];
    float coeff_ShapeBlueColor = 0;
    float coeff_ShapeRedColor = 0;
    float shift = 0;

    float WAVE_DURATION = 5;
    int LINE_Y_POSITIONS[LINES_COUNT] = { 160, 240, 320 };
    int linePositions[LINES_COUNT] = { LINE_Y_POSITIONS[0], LINE_Y_POSITIONS[1], LINE_Y_POSITIONS[2] };

    bool isAlive = true;
    bool wave_peak = false;

    int level = 2;

    int LINE_OFFSET = 10;

    int WAVE_BASE_RED_COLOR = 78;
    int WAVE_BASE_GREEN_COLOR = 87;
    int WAVE_BASE_BLUE_COLOR = 166;
    int WAVE_BASE_ALPHA = 100;

    int WAVE_COLOR_RANGE_RED = 80;
    int WAVE_COLOR_RANGE_BLUE = 30;
    int RED_TINT_MIN_LEVEL = 0;                     // waves of this level and above get the red tint

    float SHIFT_SPEED = 1;                          // speed of the wave motion (radians per second)
    float SHIFT_PHASE = 1;                          // phase shift of the sine, in radians
    float BLUE_PULSE_SPEED = 1;                     // speed of the blue color pulsation
    float RED_PULSE_SPEED = 1;                      // speed of the red color pulsation
    float RED_PULSE_AMPLITUDE = 1;                  // strength of the red color pulsation
    float PEAK_THRESHOLD = 0.98;                    // shift value from which the wave counts as "at peak"

    int TOP_LEVEL = 2;                              // level whose wave reaches the lowest line
    int LEVEL_STEP = 80;                            // vertical shift per level (distance between lines)

public:

    Wave(int waveLevel);

    void run();

    void calculate_shift();

    void calculate_coeff_ShapeBlueColor();

    void calculate_coeff_ShapeRedColor();

    void shapes_move();

    void shapes_recolor();

    void draw(sf::RenderWindow& window);

    void calculate_wave_peak();

    bool get_wave_peak() const;

    int get_level() const;

    void shift_according_level();

    bool get_isAlive() const;
};