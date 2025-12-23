#pragma once

#include "../../../include/delphinis/ecs/System.h"
#include "../../../include/delphinis/ecs/Entity.h"
#include "../../../include/delphinis/math/Vec2.h"

namespace delphinis {

struct Transform;
struct BoxCollider;
struct PaddleCollider;

class PaddleCollisionSystem : public System {
public:
    void update(World& world, float deltaTime) override;

private:
    bool checkAABB(const Transform& t1, const BoxCollider& c1,
                   const Transform& t2, const PaddleCollider& c2) const;

    Vec2 calculatePaddleReflection(const Vec2& ballVelocity,
                                   const Vec2& ballPosition,
                                   const Transform& paddleTransform,
                                   const PaddleCollider& paddleCollider,
                                   float maxAngleDegrees) const;
};

} // namespace delphinis
