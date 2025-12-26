#include "delphinis/ecs/World.h"
#include <algorithm>

namespace delphinis {

World::World() : m_nextEntityId(1) {
}

World::~World() = default;

Entity World::createEntity() {
    Entity entity = m_nextEntityId++;
    m_entities.push_back(entity);
    return entity;
}

void World::destroyEntity(Entity entity) {
    // Remove from entity list
    auto it = std::find(m_entities.begin(), m_entities.end(), entity);
    if (it != m_entities.end()) {
        m_entities.erase(it);
    }

    // Note: Components are not automatically removed
    // For Pong, entities are created at startup and never destroyed
    // If needed, could iterate through all component pools and remove components
}

} // namespace delphinis
