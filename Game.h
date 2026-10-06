#pragma once
#include "Player.h"
#include "Spider.h"
#include "Wave.h"
#include "SpawnSchedule.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


class Game {
private:

    // ---------- Settings ----------
    // (declared first, because window and player below are initialized from them)
    static const int LINES_COUNT = 3; // static const so it can be used as an array size

    // Window
    const int WINDOW_SIZE = 480;
    const int FPS_LIMIT = 60;
    const std::string WINDOW_TITLE = "spider_dance";
    const int BACKGROUND_ALPHA = 125;

    // Directions
    const int DIRECTION_LEFT = -1;
    const int DIRECTION_RIGHT = 1;

    // Colors
    const int COLOR_MAX = 255;

    // Lines (threads)
    const int LINE_THICKNESS = 2;
    const int LINE_SPACING = WINDOW_SIZE / (LINES_COUNT * 2); // distance between neighbouring lines
    const int FIRST_LINE_Y = WINDOW_SIZE / 2 - LINE_SPACING;  // lines are centered vertically
    // Inverse of the wave base color (78, 87, 166)
    const sf::Color LINE_COLOR = sf::Color(255 - 78, 255 - 87, 255, 100);

    // Player
    const int PLAYER_SPEED = 256;
    const float LINE_COOLDOWN = 0.1;
    const int PLAYER_MIN_X = 0;
    const int PLAYER_FEET_OFFSET = 10; // the sprite center is this far above its line
    const sf::Vector2f PLAYER_START_POS = sf::Vector2f(WINDOW_SIZE / 2, WINDOW_SIZE / 2 - PLAYER_FEET_OFFSET);
    const int LOW_HEALTH = 1;          // at this health the player turns red and starts regenerating
    const float RESTORE_COLOR_FACTOR = 0.55; // how far the color fades back during regeneration
    const float BLINK_FREQUENCY = 10;
    const float BLINK_AMPLITUDE = 32;
    const float BLINK_BASE_ALPHA = 128;

    // Spiders
    const float SPIDER_HIT_DISTANCE = 8;   // horizontal distance that counts as a collision
    const int SPIDER_RIGHT_BOUND_PADDING = 40;

    // Intro effect and text
    const float FLASH_FADE_SPEED = 320;    // brightness units per second
    const float TEXT_FADE_SPEED = 25;
    const std::string HINT_TEXT = "Use the key arrows to move";
    const int TEXT_SIZE = 15;
    const int TEXT_POS_X = 6;
    const int TEXT_POS_Y = WINDOW_SIZE / 2 + 6;
    const int TEXT_SHAKE_RANGE = 2;        // random offset in pixels

    // Audio
    const float MUSIC_VOLUME = 25;

    // Files
    const std::string MUSIC_FILE = "music.ogg";
    const std::string FONT_FILE = "PressStart2P-Regular.ttf";
    const std::string BACKGROUND_FILE = "pale_beach.png";
    const std::string BIRD_FILE = "bird.png";
    const std::string BIRD_WALK_FILE = "bird_walk.png";
    const std::string BIRD_SWITCHLINE_FILE = "bird_switchline.png";
    const std::string SPIDER_FILE = "spider0.png";
    const std::string SPIDER_SCHEDULE_FILE = "spiderList.txt";
    const std::string WAVE_SCHEDULE_FILE = "waveList.txt";

    // ---------- Helpers ----------
    sf::Clock clock_light_flash; // intro light-flash effect
    sf::Clock clock;             // used for deltaTime
    sf::Clock eventClock;        // timeline for scheduled events (e.g. spider spawns)

    // ---------- Entities ----------
    sf::RenderWindow window;
    Player player;
    std::vector<Spider> spiders;
    std::vector<Wave> waves;

    // Player color channels
    sf::Uint8 alpha = COLOR_MAX;
    sf::Uint8 color1 = COLOR_MAX;
    sf::Uint8 color2 = COLOR_MAX;
    sf::Uint8 color3 = COLOR_MAX;

    // Spider spawn schedule: {time, line, direction, speed}
    std::vector<SpiderSpawn> SpiderSpawnEvents;
    int SpiderSpawnIndex = 0;

    // Wave spawn schedule: {time, level}
    std::vector<WaveSpawn> WaveSpawnEvents;
    int WaveSpawnIndex = 0;

    // ---------- Textures ----------
    // Lines (threads)
    sf::RectangleShape lines[LINES_COUNT];
    // Bird
    sf::Texture birdtexture;
    sf::Texture birdtexture_walk;
    sf::Texture birdtexture_switchline;
    // Spider
    sf::Texture spider0;

    // Background
    sf::Texture backgroundTexture;
    sf::Sprite background_sprite;

    // Window clear color (fades from white to black at start)
    float windowColor;

    // Text
    sf::Font MyFont;
    sf::Text text;
    float TextColor;

    // Music (optional)
    sf::Music music;

public:

    Game();

    bool run();

    void setmusic();

    bool createText();

    void shakeText();

    bool loadTextures();

    void loadSpiderSpawnSchedule(const std::string& filename);

    void loadWaveSpawnSchedule(const std::string& filename);

    void playerMove(float deltaTime);

    void spawnSpider(int line, int direction, float speed);

    void spawnWave(int level);

    void spiderSpawner();

    void waveSpawner();

    void spiderMove(float deltaTime);

    void deleteSpider();

    void render();

    void collision();

    void wave_collision();

    void gameCycle();
};