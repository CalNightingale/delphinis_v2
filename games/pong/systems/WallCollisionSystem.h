#pragma once

#include <delphinis/ecs/System.h>
#include <delphinis/audio/AudioManager.h>

namespace delphinis {

class WallCollisionSystem : public System {
public:
    WallCollisionSystem(AudioManager& audioManager, SoundId wallSound);

    void update(World& world, float deltaTime) override;

private:
    AudioManager& m_audioManager;
    SoundId m_wallSound;
};

} // namespace delphinis
