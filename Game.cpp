#include "Game.h"

    Game::Game() : window(sf::VideoMode(WINDOW_SIZE, WINDOW_SIZE), WINDOW_TITLE),
        player(PLAYER_SPEED, LINE_COOLDOWN, PLAYER_MIN_X, WINDOW_SIZE, LINE_SPACING, LINES_COUNT - 1, PLAYER_START_POS)
    {
        window.setFramerateLimit(FPS_LIMIT);
    }

    static bool loadTexture(sf::Texture& texture, const std::string& path) {
    // Checking file uploads
        if (!texture.loadFromFile(path)) {
            std::cout << "Cannot load file: " << path << "\n";
            return false;
        }
        return true;
    }

    bool Game::run() {
        if (!createText()) { return false; }
        if (!loadTextures()) { return false; }

        loadSpiderSpawnSchedule(SPIDER_SCHEDULE_FILE);
        loadWaveSpawnSchedule(WAVE_SCHEDULE_FILE);

        setmusic();
        player.setTexture(birdtexture, DIRECTION_RIGHT);

        clock.restart();
        eventClock.restart();
        clock_light_flash.restart();

        gameCycle();
        return true;
    }

    void Game::setmusic() {
        if (!music.openFromFile(MUSIC_FILE)) {
            return;
        }
        music.play();
        music.setVolume(MUSIC_VOLUME);
    }

    bool Game::createText() {

        if (!MyFont.loadFromFile(FONT_FILE)) {
            std::cout << "Cannot load file: " << FONT_FILE << "\n";
            return false;
        }

        text.setFont(MyFont);
        text.setString(HINT_TEXT);
        text.setCharacterSize(TEXT_SIZE);
        text.setFillColor(sf::Color::White);

        return true;
    }

    void Game::shakeText() {
        text.setPosition(std::rand() % TEXT_SHAKE_RANGE + TEXT_POS_X, std::rand() % TEXT_SHAKE_RANGE + TEXT_POS_Y);
    }

    bool Game::loadTextures() {

        if (!loadTexture(backgroundTexture, BACKGROUND_FILE) ||
            !loadTexture(birdtexture, BIRD_FILE) ||
            !loadTexture(birdtexture_walk, BIRD_WALK_FILE) ||
            !loadTexture(birdtexture_switchline, BIRD_SWITCHLINE_FILE) ||
            !loadTexture(spider0, SPIDER_FILE)) {
            return false;
        }

        background_sprite.setTexture(backgroundTexture);
        background_sprite.setColor(sf::Color(COLOR_MAX, COLOR_MAX, COLOR_MAX, BACKGROUND_ALPHA));

        // Lines (threads)
        for (int i = 0; i < LINES_COUNT; i++) {
            lines[i].setSize(sf::Vector2f(WINDOW_SIZE, LINE_THICKNESS));
            lines[i].setFillColor(LINE_COLOR);
            lines[i].setPosition(0, FIRST_LINE_Y + i * LINE_SPACING);
        }

        return true;
    }

    void Game::loadSpiderSpawnSchedule(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cout << "File " << filename << " not found\n";
            return;
        }

        SpiderSpawnEvents.clear();

        float time;
        int line;
        int direction;
        float speed;

        const int MIN_LINE = 0;
        const int MAX_LINE = LINES_COUNT - 1;
        const float MIN_SPEED = 0;
        const float MAX_SPEED = 1000;

        int recordNumber = 0; // used to report which record is invalid

        while (file >> time >> line >> direction >> speed) {
            recordNumber++;
            if (time < 0) {
                std::cout << "Error in record " << recordNumber << ": time must not be negative\n";
                continue;
            }
            if (line < MIN_LINE || line > MAX_LINE) {
                std::cout << "Error in record " << recordNumber << ": line must be between "
                    << MIN_LINE << " and " << MAX_LINE << "\n";
                continue;
            }
            if (direction != DIRECTION_RIGHT && direction != DIRECTION_LEFT) {
                std::cout << "Error in record " << recordNumber << ": direction must be "
                    << DIRECTION_RIGHT << " or " << DIRECTION_LEFT << "\n";
                continue;
            }
            if (speed < MIN_SPEED || speed > MAX_SPEED) {
                std::cout << "Error in record " << recordNumber << ": speed must be between "
                    << MIN_SPEED << " and " << MAX_SPEED << "\n";
                continue;
            }
            SpiderSpawnEvents.push_back({ time, line, direction, speed });
        }
    }

    void Game::loadWaveSpawnSchedule(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cout << "File " << filename << " not found\n";
            return;
        }

        WaveSpawnEvents.clear();

        float time;
        int level;

        int recordNumber = 0; // used to report which record is invalid

        const int MIN_LEVEL = -5;
        const int MAX_LEVEL = 5;

        while (file >> time >> level) {
            recordNumber++;
            if (time < 0) {
                std::cout << "Error in record " << recordNumber << ": time must not be negative\n";
                continue;
            }
            if (level < MIN_LEVEL || level > MAX_LEVEL) {
                std::cout << "Error in record " << recordNumber << ": level must be between "
                    << MIN_LEVEL << " and " << MAX_LEVEL << "\n";
                continue;
            }
            WaveSpawnEvents.push_back({ time, level });
        }
    }

    void Game::playerMove(float deltaTime) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.moveLeft(deltaTime);
            player.setDirection(DIRECTION_LEFT);
            player.setTexture(birdtexture_walk, player.getDirection());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.moveRight(deltaTime);
            player.setDirection(DIRECTION_RIGHT);
            player.setTexture(birdtexture_walk, player.getDirection());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            player.moveUp();
            player.setTexture(birdtexture_switchline, player.getDirection());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            player.moveDown();
            player.setTexture(birdtexture_switchline, player.getDirection());
        }

        // Back to the default texture when no key is pressed
        if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right)))
        {
            player.setTexture(birdtexture, player.getDirection());
        }
    }

    void Game::spawnSpider(int line, int direction, float speed) {
        float x;
        if (direction == DIRECTION_LEFT) {
            x = WINDOW_SIZE;
        }
        else {
            x = 0;
        }
        float y;
        y = lines[line].getPosition().y;
        spiders.emplace_back(line, direction, speed, sf::Vector2f(x, y), 0, WINDOW_SIZE + SPIDER_RIGHT_BOUND_PADDING);
        spiders.back().setTexture(spider0);
    }

    void Game::spawnWave(int level) {
        waves.emplace_back(level);
    }

    void Game::spiderSpawner() {
        if (SpiderSpawnIndex < SpiderSpawnEvents.size() &&
            eventClock.getElapsedTime().asSeconds() >= SpiderSpawnEvents[SpiderSpawnIndex].time) {
            spawnSpider(SpiderSpawnEvents[SpiderSpawnIndex].line,
                SpiderSpawnEvents[SpiderSpawnIndex].direction,
                SpiderSpawnEvents[SpiderSpawnIndex].speed);
            SpiderSpawnIndex++;
        }
    }

    void Game::waveSpawner() {
        if (WaveSpawnIndex < WaveSpawnEvents.size() &&
            eventClock.getElapsedTime().asSeconds() >= WaveSpawnEvents[WaveSpawnIndex].time) {
            spawnWave(WaveSpawnEvents[WaveSpawnIndex].level);
            WaveSpawnIndex++;
        }
    }

    void Game::spiderMove(float deltaTime) {
        for (int i = 0; i < spiders.size(); i++) {
            spiders[i].move(deltaTime);
        }
    }

    void Game::deleteSpider() {
        for (int i = (int)spiders.size() - 1; i >= 0; i--) {
            if (spiders[i].isOutOfBounds()) { spiders.erase(spiders.begin() + i); }
        }
    }

    void Game::render() {
        window.clear(sf::Color(windowColor, windowColor, windowColor));
        window.draw(background_sprite);
        for (int i = 0; i < LINES_COUNT; i++) { window.draw(lines[i]); }
        player.draw(window);
        for (int i = 0; i < spiders.size(); i++) { spiders[i].draw(window); }
        for (auto& wave : waves) { wave.draw(window); }
        window.draw(text);
        window.display();
    }

    void Game::collision() {
        for (int i = 0; i < spiders.size(); i++) {
            if ((std::fabs(spiders[i].getSpritePosX() - player.getSpritePosX()) < SPIDER_HIT_DISTANCE) &&
                spiders[i].getCurrentLine() == player.getCurrentLine()) {
                player.damage();
            }
        }
    }

    void Game::wave_collision() {
        for (auto& wave : waves) {
            if (wave.get_wave_peak() && player.getCurrentLine() <= wave.get_level()) {
                player.damage();
            }
        }
    }

    void Game::gameCycle() {

        while (window.isOpen()) {
            // Time since the previous frame, keeps movement consistent across different FPS
            float deltaTime = clock.restart().asSeconds();

            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) { window.close(); }
            }

            // Player movement
            playerMove(deltaTime);

            // Spawn spiders and waves according to their schedules
            spiderSpawner();
            waveSpawner();

            // Spider movement
            spiderMove(deltaTime);

            // Remove spiders that left the screen
            deleteSpider();

            // Start health regeneration (restart the timer and set the flag)
            if (player.getHealth() == LOW_HEALTH && !player.getIsRestoring()) {
                player.startHealthRestore();
            }

            // Update whether the player can take damage
            player.updateCanTakeDamage();

            // Update regeneration state
            player.updateHealthRestore();

            // Player color: gradually returns to normal while regenerating
            if (player.getIsRestoring()) {
                color1 = COLOR_MAX;
                color2 = COLOR_MAX * player.getRestoreProgress() * RESTORE_COLOR_FACTOR;
                color3 = COLOR_MAX * player.getRestoreProgress() * RESTORE_COLOR_FACTOR;
            }
            // Red when health is low
            else if (player.getHealth() == LOW_HEALTH) {
                color1 = COLOR_MAX;
                color2 = 0;
                color3 = 0;
            }
            // White when fully healthy
            else {
                color1 = COLOR_MAX;
                color2 = COLOR_MAX;
                color3 = COLOR_MAX;
            }

            // Blinking effect via alpha while the player is invulnerable
            if (player.gethealthTimer() < player.gethealthCooldown() && !player.getcanTakeDamage()) {
                alpha = (sin(player.gethealthTimer() * BLINK_FREQUENCY) * BLINK_AMPLITUDE + BLINK_BASE_ALPHA);
            }
            else { alpha = COLOR_MAX; }

            player.setColor(color1, color2, color3, alpha);

            // Update waves
            for (auto& wave : waves) {
                wave.run();
            }

            // Remove finished waves
            for (int i = (int)waves.size() - 1; i >= 0; i--) {
                if (!waves[i].get_isAlive()) {
                    waves.erase(waves.begin() + i);
                }
            }

            // Intro light flash: window color fades from white to black
            float introTime = clock_light_flash.getElapsedTime().asSeconds();
            windowColor = COLOR_MAX - introTime * FLASH_FADE_SPEED;
            if (windowColor < 0) {
                windowColor = 0;
            }

            // Hint text: shake and fade out
            shakeText();
            TextColor = COLOR_MAX - introTime * TEXT_FADE_SPEED;
            if (TextColor < 0) {
                TextColor = 0;
            }
            text.setFillColor(sf::Color(COLOR_MAX, COLOR_MAX, COLOR_MAX, TextColor));

            // Collisions are resolved before rendering so damage effects
            // (color, blinking) appear in the same frame
            collision();
            wave_collision();

            render();

            // Close the game when the player is dead
            if (player.isDead()) { window.close(); }
        }
    }