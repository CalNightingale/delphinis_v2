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

    // Remove all components from this entity
    // Solution: Iterate through all registered component types and remove if present
    // This is O(T) where T = total component types in the game (typically 10-30)
    //
    // Alternative approach (if this becomes a bottleneck):
    // Track components per-entity in a std::unordered_map<Entity, std::unordered_set<std::type_index>>
    // This would be O(C) where C = components on this entity (typically 2-6)
    // Trade-off: ~32-64 bytes overhead per entity, additional bookkeeping complexity
    for (const auto& typeIndex : m_componentTypes) {
        auto it = m_componentPools.find(typeIndex);
        if (it != m_componentPools.end()) {
            it->second->removeEntity(it->second->pool, entity);
        }
    }
}

} // namespace delphinis
