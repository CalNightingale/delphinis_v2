#pragma once

#include "Entity.h"
#include <vector>

namespace delphinis {

// Forward declaration
class World;

template<typename... Components>
class View {
public:
    class Iterator {
    public:
        Iterator(const World* world, const std::vector<Entity>* entities, size_t index);

        Entity operator*() const {
            return (*m_entities)[m_index];
        }

        Iterator& operator++() {
            ++m_index;
            skipToNextValid();
            return *this;
        }

        bool operator!=(const Iterator& other) const {
            return m_index != other.m_index;
        }

        bool operator==(const Iterator& other) const {
            return m_index == other.m_index;
        }

    private:
        void skipToNextValid();

        const World* m_world;
        const std::vector<Entity>* m_entities;
        size_t m_index;
    };

    View(const World& world, const std::vector<Entity>& entities)
        : m_world(&world), m_entities(&entities) {}

    Iterator begin() const {
        return Iterator(m_world, m_entities, 0);
    }

    Iterator end() const {
        return Iterator(m_world, m_entities, m_entities->size());
    }

private:
    const World* m_world;
    const std::vector<Entity>* m_entities;
};

} // namespace delphinis

// Include World to implement methods that depend on it
#include "World.h"

namespace delphinis {

template<typename... Components>
View<Components...>::Iterator::Iterator(const World* world, const std::vector<Entity>* entities, size_t index)
    : m_world(world), m_entities(entities), m_index(index) {
    skipToNextValid();
}

template<typename... Components>
void View<Components...>::Iterator::skipToNextValid() {
    while (m_index < m_entities->size()) {
        Entity entity = (*m_entities)[m_index];
        if (m_world->hasComponents<Components...>(entity)) {
            break;
        }
        ++m_index;
    }
}

} // namespace delphinis
