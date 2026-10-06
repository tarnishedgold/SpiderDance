#include "Wave.h"

Wave::Wave(int waveLevel) : level(waveLevel) {
        shift_according_level();
        for (int i = 0; i < LINES_COUNT; i++) {
            shapes[i].setSize(sf::Vector2f(WINDOW_SIZE, WINDOW_SIZE));
            shapes[i].setOrigin(0, 0);
            shapes[i].setPosition(0, -WINDOW_SIZE);
        }
    }

void Wave::run() {

        if (isAlive) {

            calculate_shift();
            calculate_coeff_ShapeBlueColor();
            calculate_coeff_ShapeRedColor();
            shapes_move();
            shapes_recolor();
            calculate_wave_peak();

            if (Clock.getElapsedTime().asSeconds() >= WAVE_DURATION) {
                isAlive = false;
                Clock.restart();
            }

        }
    }

void Wave::calculate_shift() {
        float time = Clock.getElapsedTime().asSeconds();
        // Radians are used here; the starting position of the sine is shifted
        // so that it doesn't start from zero but from about -1
        shift = sin((time - SHIFT_PHASE) * SHIFT_SPEED);
    }

void Wave::calculate_coeff_ShapeBlueColor() {
        float time = Clock.getElapsedTime().asSeconds();
        coeff_ShapeBlueColor = sin(time * BLUE_PULSE_SPEED);
    }

void Wave::calculate_coeff_ShapeRedColor() {
        float time = Clock.getElapsedTime().asSeconds();
        coeff_ShapeRedColor = sin(time * RED_PULSE_SPEED) * RED_PULSE_AMPLITUDE;
    }

void Wave:: shapes_move() {
        for (int i = 0; i < LINES_COUNT; i++) {
            // Convert shift (-1..1) to progress (0..1)
            float progress = (shift + 1) / 2;

            // Start position (top edge is above the screen)
            float startY = -WINDOW_SIZE;

            // End position: the bottom edge is on the line,
            // so the top edge = line position - shape height
            float endY = linePositions[i] - WINDOW_SIZE;

            // Current position (of the top edge)
            float y = startY + progress * (endY - startY);

            shapes[i].setPosition(0, y);
        }
    }

void Wave::shapes_recolor() {
        float invertedBlue = 1 - coeff_ShapeBlueColor;
        float invertedRed = 1 - coeff_ShapeRedColor;
        for (int i = 0; i < LINES_COUNT; i++) {
            if (level >= RED_TINT_MIN_LEVEL) {
                shapes[i].setFillColor(sf::Color(WAVE_BASE_RED_COLOR + invertedRed * WAVE_COLOR_RANGE_RED, WAVE_BASE_GREEN_COLOR, WAVE_BASE_BLUE_COLOR + invertedBlue * WAVE_COLOR_RANGE_BLUE, WAVE_BASE_ALPHA));
            }
            else {
                shapes[i].setFillColor(sf::Color(WAVE_BASE_RED_COLOR, WAVE_BASE_GREEN_COLOR, WAVE_BASE_BLUE_COLOR + invertedBlue * WAVE_COLOR_RANGE_BLUE, WAVE_BASE_ALPHA));
            }

        }
    }

void Wave::draw(sf::RenderWindow& window) {
        // The last shape is drawn first
        for (int i = LINES_COUNT - 1; i >= 0; i--) {
            window.draw(shapes[i]);
        }
    }

void Wave::calculate_wave_peak() {
        if (shift >= PEAK_THRESHOLD) {
            wave_peak = true;
        }
        else {
            wave_peak = false;
        }
    }

bool Wave::get_wave_peak() const {
        return wave_peak;
    }

int Wave::get_level() const {
        return level;
    }

void Wave::shift_according_level() {
        float basePositions[LINES_COUNT] = { LINE_Y_POSITIONS[0], LINE_Y_POSITIONS[1], LINE_Y_POSITIONS[2] };

        // A wave of level N comes down to just below line N:
        // the top level gets no extra shift, every level below it is one step higher
        int offset = (level - TOP_LEVEL) * LEVEL_STEP;

        for (int i = 0; i < LINES_COUNT; i++) {
            linePositions[i] = basePositions[i] + offset + LINE_OFFSET;
        }
    }

bool Wave::get_isAlive() const {
        return isAlive;
    }