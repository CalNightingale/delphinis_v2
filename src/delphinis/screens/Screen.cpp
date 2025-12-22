#include "delphinis/screens/Screen.h"

namespace delphinis {

Screen::Screen()
    : m_world(std::make_unique<World>()) {
}

} // namespace delphinis
