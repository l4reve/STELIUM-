#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <ctime> 

const int SCREEN_WIDTH = 700;
const int SCREEN_HEIGHT = 700;

enum GameState { TITLE_SCREEN, PLAYING, LEVEL_UP, GAME_OVER };

struct Player {
    SDL_Rect rect;
    int speed;
    int health;
    int maxHealth;
    int powerUpsCollected;
    bool isInvincible;
    int invincibilityTimer;
    int points;
    Uint32 lastShotTime;
};

struct Bullet {
    SDL_Rect rect;
    int speed;
};

struct Enemy {
    SDL_Rect rect;
    int speed;
};

struct PowerUp {
    SDL_Rect rect;
    int type;
    int speed;
};

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* playerTexture = nullptr;
SDL_Texture* enemyTexture = nullptr;
SDL_Texture* bulletTexture = nullptr;
SDL_Texture* powerUpTextures[1] = { nullptr }; 
SDL_Texture* backgroundTexture = nullptr;
TTF_Font* font = nullptr;
std::vector<Bullet> bullets;
std::vector<Enemy> enemies;
std::vector<PowerUp> powerUps;

bool initSDL();
void closeSDL();
SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer);
void handleInput(SDL_Event& e, bool& quit, Player& player, GameState& gameState);
void updateGame(Player& player, GameState& currentLevel);
void spawnEnemies(Player& player);
void moveEnemies();
void spawnPowerUps(Player& player);
void movePowerUps();
void handleCollisions(Player& player, GameState& gameState);
void handleBulletEnemyCollisions(Player& player);
void handlePowerUpCollection(Player& player, GameState& currentLevel);
void renderGame(SDL_Renderer* renderer, GameState gameState, Player& player);
void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font);
void renderBullets(SDL_Renderer* renderer);
void renderEnemies(SDL_Renderer* renderer);
void renderPowerUps(SDL_Renderer* renderer);
void renderHealthBar(SDL_Renderer* renderer, Player& player);
bool checkCollision(SDL_Rect a, SDL_Rect b);
void shootBullet(Player& player);

int main(int argc, char* args[]) {
    if (!initSDL()) {
        std::cerr << "Failed to initialize SDL!" << std::endl;
        return -1;
    }

    Mix_Music* backgroundMusic = Mix_LoadMUS("Sound/music.mp3");
    if (!backgroundMusic) {
        std::cerr << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    Mix_PlayMusic(backgroundMusic, -1);

    playerTexture = loadTexture("Image/green.png", renderer);
    enemyTexture = loadTexture("Image/Gray1.png", renderer);
    bulletTexture = loadTexture("Image/bullet.png", renderer);
    powerUpTextures[0] = loadTexture("Image/powerup2.png", renderer);
    backgroundTexture = loadTexture("Image/Space Background_med.png", renderer);

    if (!playerTexture || !enemyTexture || !bulletTexture || !powerUpTextures[0] || !backgroundTexture) {
        std::cerr << "Failed to load media!" << std::endl;
        closeSDL();
        return -1;
    }

    Player player = { {SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, 50, 50}, 5, 100, 100, 0, false, 0, 0, 0 };
    GameState gameState = TITLE_SCREEN;

    bool quit = false;
    SDL_Event e;

    srand(time(nullptr)); // Seed the random number generator

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
                Mix_HaltMusic();
                Mix_FreeMusic(backgroundMusic);
            }
            else {
                handleInput(e, quit, player, gameState);
            }
        }

        updateGame(player, gameState);
        renderGame(renderer, gameState, player);
    }

    closeSDL();
    return 0;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Stelium", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    font = TTF_OpenFont("Font/slkscre.ttf", 28); // Load your font
    if (font == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void closeSDL() {
    TTF_CloseFont(font);
    font = nullptr;

    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(enemyTexture);
    SDL_DestroyTexture(bulletTexture);
    SDL_DestroyTexture(backgroundTexture);
    for (int i = 0; i < 1; ++i) {
        SDL_DestroyTexture(powerUpTextures[i]);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    renderer = nullptr;

    Mix_CloseAudio(); 

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(const std::string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = nullptr;
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == nullptr) {
        std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
    }
    else {
        newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (newTexture == nullptr) {
            std::cerr << "Unable to create texture from " << path << "! SDL_Error: " << SDL_GetError() << std::endl;
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

void handleInput(SDL_Event& e, bool& quit, Player& player, GameState& gameState) {
    if (gameState == TITLE_SCREEN) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= SCREEN_WIDTH / 2 - 50 && mouseX <= SCREEN_WIDTH / 2 + 50 && mouseY >= SCREEN_HEIGHT / 2 - 25 && mouseY <= SCREEN_HEIGHT / 2 + 25) {
                // Clicked on PLAY button
                gameState = PLAYING;
            }
            else if (mouseX >= SCREEN_WIDTH / 2 - 50 && mouseX <= SCREEN_WIDTH / 2 + 50 && mouseY >= SCREEN_HEIGHT / 2 + 50 && mouseY <= SCREEN_HEIGHT / 2 + 75) {
                // Clicked on QUIT button
                quit = true;
            }
        }
    }
    else if (gameState == PLAYING) {
        if (e.type == SDL_MOUSEMOTION) {
            int mouseX = e.motion.x;
            // Update player's position to follow the mouse horizontally
            player.rect.x = mouseX - player.rect.w / 2;
            // Ensure the player stays within the screen bounds
            if (player.rect.x < 0) player.rect.x = 0;
            if (player.rect.x > SCREEN_WIDTH - player.rect.w) player.rect.x = SCREEN_WIDTH - player.rect.w;
        }
        
        spawnPowerUps(player);
    }
    else if (gameState == GAME_OVER) {
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (mouseX >= SCREEN_WIDTH / 2 - 50 && mouseX <= SCREEN_WIDTH / 2 + 50 && mouseY >= SCREEN_HEIGHT / 2 + 70 && mouseY <= SCREEN_HEIGHT / 2 + 90) {
                // Clicked on "PLAY AGAIN" button
                gameState = PLAYING;
                // Reset player's attributes
                player.health = player.maxHealth;
                player.points = 0;
            }
            else if (mouseX >= SCREEN_WIDTH / 2 && mouseX <= SCREEN_WIDTH / 2 + 50 && mouseY >= SCREEN_HEIGHT / 2 + 110 && mouseY <= SCREEN_HEIGHT / 2 + 150) {
                // Clicked on "QUIT" button
                quit = true;
            }
        }
    }
}

void updateGame(Player& player, GameState& gameState) {
    if (gameState == PLAYING) {
        Uint32 currentTime = SDL_GetTicks();

        // Handle automatic shooting
        if (currentTime - player.lastShotTime > 200) {
            shootBullet(player);
            player.lastShotTime = currentTime;
        }

        // Move bullets
        for (auto& bullet : bullets) {
            bullet.rect.y -= bullet.speed;
        }
        bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](Bullet& b) { return b.rect.y < 0; }), bullets.end());

        // Move enemies
        moveEnemies();

        // Spawn enemies
        spawnEnemies(player);

        // Move power-ups
        movePowerUps();

        // Spawn power-ups
        spawnPowerUps(player);

        // Handle collisions
        handleCollisions(player, gameState);

        // Handle bullet-enemy collisions
        handleBulletEnemyCollisions(player);

        // Handle power-up collection
        handlePowerUpCollection(player, gameState);

        // Check and update background based on player's score 
        // HARD : 200 && MED : 60 
        if (player.points >= 200) {
            backgroundTexture = loadTexture("Image/Space Background_hard.png", renderer);
        }
        else if (player.points >= 60) {
            backgroundTexture = loadTexture("Image/Space Background_ez.png", renderer);
        }
    }
    else if (gameState == GAME_OVER) {
        // Stop the music when the game is over
        Mix_HaltMusic();
    }
}


void shootBullet(Player& player) {
    Bullet newBullet;
    newBullet.rect = { player.rect.x + player.rect.w / 2 - 5, player.rect.y, 10, 20 }; // Adjust size and position as needed
    newBullet.speed = 10;
    bullets.push_back(newBullet);
}

void spawnEnemies(Player& player) {
    int spawnRate = 1500; // Adjust difficulty
    int speed = 2;
   
    // Gradually decrease spawn rate and increase speed
    if (player.points >= 60) {
        spawnRate = 800; 
        speed = 3; 
    }
    else if (player.points >= 200) {
        spawnRate = 500; 
        speed = 4; 
    }

    if (rand() % spawnRate == 0) {
        Enemy enemy;
        enemy.rect = { rand() % (SCREEN_WIDTH - 50), 0, 50, 50 };
        enemy.speed = 2; // Adjust speed for difficulty
        enemies.push_back(enemy);
    }
}

void moveEnemies() {
    for (auto& enemy : enemies) {
        enemy.rect.y += enemy.speed;
    }
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return e.rect.y > SCREEN_HEIGHT; }), enemies.end());
}

void spawnPowerUps(Player& player) {
    // Adjust spawn rate and speed based on game progression
    int spawnRate = 1000; // Initial spawn rate
    int speed = 2; // Initial speed

    // Gradually decrease spawn rate and increase speed
    if (player.points >= 60) {
        spawnRate = 750; // Example adjustment based on player's points
        speed = 3; // Example adjustment based on player's points
    }
    else if (player.points >= 200) {
        spawnRate = 300; // Example adjustment based on player's points
        speed = 4; // Example adjustment based on player's points
    }

    // Spawn power-up
    if (rand() % spawnRate == 0) {
        PowerUp powerUp;
        powerUp.rect = { rand() % (SCREEN_WIDTH - 30), 0, 30, 30 };
        powerUp.type = 0; // For simplicity, always spawn the same type of power-up
        powerUp.speed = speed; // Adjust speed for difficulty
        powerUps.push_back(powerUp);
    }
}

void movePowerUps() {
    // Move power-ups and remove them if they go out of screen
    for (auto& powerUp : powerUps) {
        powerUp.rect.y += powerUp.speed;
    }
    powerUps.erase(std::remove_if(powerUps.begin(), powerUps.end(), [](PowerUp& p) { return p.rect.y > SCREEN_HEIGHT; }), powerUps.end());
}

void handleCollisions(Player& player, GameState& gameState) {
    // Handle collisions with enemies
    for (auto& enemy : enemies) {
        if (enemy.rect.y >= SCREEN_HEIGHT) {
            player.health -= 10; // Reduce player's health
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy& e) { return e.rect.y >= SCREEN_HEIGHT; }), enemies.end()); // Remove the enemy that reached the bottom
        }
    }

    // Handle collisions with screen boundaries
    if (player.rect.x < 0) player.rect.x = 0;
    if (player.rect.x > SCREEN_WIDTH - player.rect.w) player.rect.x = SCREEN_WIDTH - player.rect.w;

    // Check for game over
    if (player.health <= 0) {
        gameState = GAME_OVER;
    }
}


void handleBulletEnemyCollisions(Player& player) {
    for (auto bulletIt = bullets.begin(); bulletIt != bullets.end(); ) {
        bool bulletRemoved = false;
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            if (checkCollision(bulletIt->rect, enemyIt->rect)) {
                player.points += 10; // Update points
                enemyIt = enemies.erase(enemyIt); // Remove the enemy
                bulletIt = bullets.erase(bulletIt); // Remove the bullet
                bulletRemoved = true;
                // Increase player's health by 10 for each enemy killed
                player.health = std::min(player.health + 20, player.maxHealth);
                break;
            }
            else {
                ++enemyIt;
            }
        }
        if (!bulletRemoved) {
            ++bulletIt;
        }
    }
}

/*
void handlePowerUpCollection(Player& player, GameState& gameState) {
    for (auto powerUpIt = powerUps.begin(); powerUpIt != powerUps.end(); ) {
        if (checkCollision(player.rect, powerUpIt->rect)) {
            player.health = std::min(player.health + 20, player.maxHealth); // Increase health
            powerUpIt = powerUps.erase(powerUpIt); // Remove the power-up
        }
        else {
            ++powerUpIt;
        }
    }
}
*/
void handlePowerUpCollection(Player& player, GameState& gameState) {
    for (auto powerUpIt = powerUps.begin(); powerUpIt != powerUps.end(); ) {
        if (checkCollision(player.rect, powerUpIt->rect)) {
            player.health -= 10; // Decrease health when a power-up is collected
            powerUpIt = powerUps.erase(powerUpIt); // Remove the power-up
        }
        else {
            ++powerUpIt;
        }
    }
}

void renderBullets(SDL_Renderer* renderer) {
    for (const auto& bullet : bullets) {
        SDL_RenderCopy(renderer, bulletTexture, nullptr, &bullet.rect);
    }
}

void renderEnemies(SDL_Renderer* renderer) {
    for (const auto& enemy : enemies) {
        SDL_RenderCopy(renderer, enemyTexture, nullptr, &enemy.rect);
    }
}

void renderPowerUps(SDL_Renderer* renderer) {
    for (const auto& powerUp : powerUps) {
        SDL_RenderCopy(renderer, powerUpTextures[powerUp.type], nullptr, &powerUp.rect);
    }
}

void renderHealthBar(SDL_Renderer* renderer, Player& player) {
    SDL_Rect healthBarBack = { 10, 50, 200, 20 }; // Background of the health bar
    SDL_Rect healthBarFront = { 10, 50, static_cast<int>(200.0 * player.health / player.maxHealth), 20 }; // Front of the health bar based on player's health

    // Render the background of the health bar (red)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBarBack);

    // Render the front of the health bar (green)
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthBarFront);
}


void renderText(SDL_Renderer* renderer, const std::string& text, int x, int y, TTF_Font* font, int fontSize = 28) {
    SDL_Color textColor = { 255, 255, 255, 255 }; // White color
    TTF_SetFontSize(font, fontSize); // Set the font size

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect renderQuad = { x - textSurface->w / 2, y - textSurface->h / 2, textSurface->w, textSurface->h }; // Center the text
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

bool checkCollision(SDL_Rect a, SDL_Rect b) {
    return SDL_HasIntersection(&a, &b);
}

void renderGame(SDL_Renderer* renderer, GameState gameState, Player& player) {
    SDL_RenderClear(renderer);

    // Render background
    SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);

    // Render player
    SDL_RenderCopy(renderer, playerTexture, nullptr, &player.rect);

    // Render other elements based on game state
    if (gameState == TITLE_SCREEN) {
        renderText(renderer, " STELIUM", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 50, font, 80); // Adjust font size 
        renderText(renderer, " PLAY", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 50 - 25, font, 28); // Adjust position
        renderText(renderer, " QUIT", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100 - 25, font, 28); // Adjust position
    }
    else if (gameState == GAME_OVER) {
        renderText(renderer, "     GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, font, 70);
        renderText(renderer, "   " + std::to_string(player.points), SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT / 2, font, 50);
        renderText(renderer, "PLAY AGAIN", SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 70, font, 28);
        renderText(renderer, "QUIT", SCREEN_WIDTH / 2 + 10, SCREEN_HEIGHT / 2 + 120, font, 28);
    }
    else {
        // Render bullets, enemies, power-ups, health bar, and points
        renderBullets(renderer);
        renderEnemies(renderer);
        renderPowerUps(renderer);
        renderHealthBar(renderer, player);
        renderText(renderer, "POINTS: " + std::to_string(player.points), 100, 20, font, 28);
    }

    SDL_RenderPresent(renderer);
}
