#pragma once

#include "Entity.h"
#include <vector>
#include <unordered_map>
#include <cassert>

namespace delphinis {

template<typename T>
class ComponentPool {
public:
    void add(Entity entity, const T& component) {
        assert(m_entityToIndex.find(entity) == m_entityToIndex.end() && "Component already exists for entity");

        size_t index = m_dense.size();
        m_entityToIndex[entity] = index;
        m_dense.push_back(component);
        m_denseToEntity.push_back(entity);
    }

    void remove(Entity entity) {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Component does not exist for entity");

        size_t indexToRemove = m_entityToIndex[entity];
        size_t lastIndex = m_dense.size() - 1;

        // Swap with last element
        m_dense[indexToRemove] = m_dense[lastIndex];
        m_denseToEntity[indexToRemove] = m_denseToEntity[lastIndex];

        // Update the moved entity's index
        Entity movedEntity = m_denseToEntity[indexToRemove];
        m_entityToIndex[movedEntity] = indexToRemove;

        // Remove the last element
        m_dense.pop_back();
        m_denseToEntity.pop_back();
        m_entityToIndex.erase(entity);
    }

    bool has(Entity entity) const {
        return m_entityToIndex.find(entity) != m_entityToIndex.end();
    }

    T& get(Entity entity) {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Component does not exist for entity");
        return m_dense[m_entityToIndex[entity]];
    }

    const T& get(Entity entity) const {
        assert(m_entityToIndex.find(entity) != m_entityToIndex.end() && "Component does not exist for entity");
        return m_dense[m_entityToIndex.at(entity)];
    }

    // For system iteration
    size_t size() const {
        return m_dense.size();
    }

    T& getByIndex(size_t index) {
        assert(index < m_dense.size() && "Index out of bounds");
        return m_dense[index];
    }

    const T& getByIndex(size_t index) const {
        assert(index < m_dense.size() && "Index out of bounds");
        return m_dense[index];
    }

    Entity getEntityByIndex(size_t index) const {
        assert(index < m_denseToEntity.size() && "Index out of bounds");
        return m_denseToEntity[index];
    }

private:
    std::vector<T> m_dense;                          // Tightly packed components
    std::vector<Entity> m_denseToEntity;             // Entity for each component
    std::unordered_map<Entity, size_t> m_entityToIndex; // Sparse lookup
};

} // namespace delphinis
