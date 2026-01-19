#pragma once

#include <delphinis/ecs/System.h>
#include <delphinis/ecs/Entity.h>
#include <delphinis/math/Vec2.h>
#include <delphinis/audio/AudioManager.h>

namespace delphinis {

struct Transform;
struct BoxCollider;
struct PaddleCollider;

class PaddleCollisionSystem : public System {
public:
    PaddleCollisionSystem() = default;
    PaddleCollisionSystem(AudioManager& audioManager, SoundId paddleSound);

    void update(World& world, float deltaTime) override;

private:
    AudioManager* m_audioManager = nullptr;
    SoundId m_paddleSound = INVALID_SOUND_ID;

    bool checkAABB(const Transform& t1, const BoxCollider& c1,
                   const Transform& t2, const PaddleCollider& c2) const;

    Vec2 calculatePaddleReflection(const Vec2& ballVelocity,
                                   const Vec2& ballPosition,
                                   const Transform& paddleTransform,
                                   const PaddleCollider& paddleCollider,
                                   float maxAngleDegrees) const;
};

} // namespace delphinis
