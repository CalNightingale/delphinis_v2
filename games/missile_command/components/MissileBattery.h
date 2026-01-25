#pragma once

namespace delphinis {

struct MissileBattery {
    int index = 0;          // Battery index (0=left, 1=center, 2=right)
    int missiles = 10;      // Remaining missiles
    bool isDestroyed = false;
};

} // namespace delphinis
