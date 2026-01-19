#pragma once

#include <delphinis/ecs/System.h>
#include <delphinis/audio/AudioManager.h>

namespace delphinis {

class BallSystem : public System {
public:
    BallSystem(float viewWidth)
        : m_viewWidth(viewWidth), m_leftScore(0), m_rightScore(0) {}

    BallSystem(float viewWidth, AudioManager& audioManager, SoundId scoreSound)
        : m_viewWidth(viewWidth), m_leftScore(0), m_rightScore(0),
          m_audioManager(&audioManager), m_scoreSound(scoreSound) {}

    void update(World& world, float deltaTime) override;

    int getLeftScore() const { return m_leftScore; }
    int getRightScore() const { return m_rightScore; }
    bool isGameOver() const { return m_leftScore >= 5 || m_rightScore >= 5; }
    bool didPlayerWin() const { return m_leftScore >= 5; }

    void resetScores() { m_leftScore = 0; m_rightScore = 0; }

private:
    float m_viewWidth;
    int m_leftScore;
    int m_rightScore;
    AudioManager* m_audioManager = nullptr;
    SoundId m_scoreSound = INVALID_SOUND_ID;
};

} // namespace delphinis
