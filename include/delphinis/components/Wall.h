#pragma once

namespace delphinis {

enum class WallType {
    Top,
    Bottom,
    LeftGoal,
    RightGoal
};

struct Wall {
    WallType type;

    Wall() : type(WallType::Top) {}
    Wall(WallType t) : type(t) {}
};

} // namespace delphinis
