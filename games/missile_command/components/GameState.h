#pragma once

namespace delphinis {

struct GameState {
    int score = 0;
    int wave = 1;
    int citiesRemaining = 6;
    bool gameOver = false;
    bool waveComplete = false;
    int enemiesRemaining = 0;
    float spawnTimer = 0.0f;
    int enemiesSpawned = 0;
};

} // namespace delphinis
