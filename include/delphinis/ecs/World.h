#pragma once

#include "Entity.h"
#include "ComponentPool.h"
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>

namespace delphinis {

// Forward declaration
template<typename... Components>
class View;

class World {
public:
    World();
    ~World();

    // Entity management
    Entity createEntity();
    void destroyEntity(Entity entity);

    // Component management
    template<typename T>
    T& addComponent(Entity entity, const T& component);

    template<typename T>
    void removeComponent(Entity entity);

    template<typename T>
    bool hasComponent(Entity entity) const;

    template<typename T>
    T& getComponent(Entity entity);

    template<typename T>
    const T& getComponent(Entity entity) const;

    // Multiple component queries
    template<typename T1, typename T2, typename... Rest>
    bool hasComponents(Entity entity) const;

    // Entity query with iterator
    template<typename... Components>
    View<Components...> query() const;

    // Component pool access
    template<typename T>
    ComponentPool<T>& getComponentPool();

    template<typename T>
    const ComponentPool<T>& getComponentPool() const;

private:
    std::vector<Entity> m_entities;
    Entity m_nextEntityId;

    // Type-erased component pool storage
    struct PoolWrapper {
        void* pool;
        void (*deleter)(void*);

        PoolWrapper(void* p, void (*d)(void*)) : pool(p), deleter(d) {}
        ~PoolWrapper() { deleter(pool); }
    };

    std::unordered_map<std::type_index, std::unique_ptr<PoolWrapper>> m_componentPools;

    template<typename T>
    void ensureComponentPool();
};

// Template implementations

template<typename T>
T& World::addComponent(Entity entity, const T& component) {
    ensureComponentPool<T>();
    auto& pool = getComponentPool<T>();
    pool.add(entity, component);
    return pool.get(entity);
}

template<typename T>
void World::removeComponent(Entity entity) {
    assert(hasComponent<T>(entity) && "Entity does not have component");
    auto& pool = getComponentPool<T>();
    pool.remove(entity);
}

template<typename T>
bool World::hasComponent(Entity entity) const {
    std::type_index typeIndex(typeid(T));
    auto it = m_componentPools.find(typeIndex);
    if (it == m_componentPools.end()) {
        return false;
    }
    auto* pool = static_cast<ComponentPool<T>*>(it->second->pool);
    return pool->has(entity);
}

template<typename T>
T& World::getComponent(Entity entity) {
    assert(hasComponent<T>(entity) && "Entity does not have component");
    return getComponentPool<T>().get(entity);
}

template<typename T>
const T& World::getComponent(Entity entity) const {
    assert(hasComponent<T>(entity) && "Entity does not have component");
    return getComponentPool<T>().get(entity);
}

template<typename T1, typename T2, typename... Rest>
bool World::hasComponents(Entity entity) const {
    if constexpr (sizeof...(Rest) == 0) {
        return hasComponent<T1>(entity) && hasComponent<T2>(entity);
    } else {
        return hasComponent<T1>(entity) && hasComponents<T2, Rest...>(entity);
    }
}

template<typename T>
ComponentPool<T>& World::getComponentPool() {
    ensureComponentPool<T>();
    std::type_index typeIndex(typeid(T));
    auto* pool = static_cast<ComponentPool<T>*>(m_componentPools[typeIndex]->pool);
    return *pool;
}

template<typename T>
const ComponentPool<T>& World::getComponentPool() const {
    std::type_index typeIndex(typeid(T));
    auto it = m_componentPools.find(typeIndex);
    assert(it != m_componentPools.end() && "Component pool does not exist");
    auto* pool = static_cast<ComponentPool<T>*>(it->second->pool);
    return *pool;
}

template<typename T>
void World::ensureComponentPool() {
    std::type_index typeIndex(typeid(T));
    if (m_componentPools.find(typeIndex) == m_componentPools.end()) {
        auto* pool = new ComponentPool<T>();
        auto deleter = [](void* p) { delete static_cast<ComponentPool<T>*>(p); };
        m_componentPools[typeIndex] = std::make_unique<PoolWrapper>(pool, deleter);
    }
}

} // namespace delphinis

// Include View here to avoid circular dependency
#include "View.h"

namespace delphinis {

template<typename... Components>
View<Components...> World::query() const {
    return View<Components...>(*this, m_entities);
}

} // namespace delphinis
