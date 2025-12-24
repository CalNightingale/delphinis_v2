#pragma once

#include <delphinis/ecs/System.h>

namespace delphinis {

class BallSystem : public System {
public:
    BallSystem(float viewWidth) : m_viewWidth(viewWidth), m_leftScore(0), m_rightScore(0) {}

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
};

} // namespace delphinis
