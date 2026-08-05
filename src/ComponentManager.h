#pragma once

#include "Entity.h"

#include <unordered_map>
#include <typeindex>
#include <array>
#include <memory>
#include <cassert>

class IComponentArray
{
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void insert(Entity entity, T component)
    {
        assert(size < MAX_ENTITIES && "Too many components!");
        size_t newIndex = size;
        entityToIndex[entity] = newIndex;
        indexToEntity[newIndex] = entity;
        components[newIndex] = component;
        ++size;
    }

    void remove(Entity entity)
    {
        assert(entityToIndex.find(entity) != entityToIndex.end() && "Removing non-existent component.");

        size_t indexOfRemoved = entityToIndex[entity];
        size_t indexOfLast = size - 1;
        components[indexOfRemoved] = components[indexOfLast];

        Entity entityOfLast = indexToEntity[indexOfLast];
        entityToIndex[entityOfLast] = indexOfRemoved;
        indexToEntity[indexOfRemoved] = entityOfLast;

        entityToIndex.erase(entity);

        --size;
    }

    T& get(Entity entity)
    {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end())
        {
            std::string msg = std::string("Retrieving non-existent component: ") + typeid(T).name();
            static T dummy{};
            return dummy;
        }
        return components[it->second];
    }

    bool hasComponent(Entity entity)
    {
        return entityToIndex.find(entity) != entityToIndex.end();
    }

    void entityDestroyed(Entity entity) override
    {
        if (entityToIndex.find(entity) != entityToIndex.end())
            remove(entity);
    }

private:
    std::array<T, MAX_ENTITIES> components;
    std::unordered_map<Entity, size_t> entityToIndex;
    std::array<Entity, MAX_ENTITIES> indexToEntity;
    size_t size = 0;
};

class ComponentManager
{
public:
    template<typename T>
    void registerComponent()
    {
        std::type_index typeName = std::type_index(typeid(T));
        assert(componentTypes.find(typeName) == componentTypes.end() && "Component type already registered.");

        componentTypes.insert({ typeName, nextComponentType });
        componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

        ++nextComponentType;
    }

    template<typename T>
    ComponentType getComponentType()
    {
        std::type_index typeName = std::type_index(typeid(T));
        if (componentTypes.find(typeName) == componentTypes.end())
        {
            std::string msg = std::string("Component not registered: ") + typeid(T).name();
        }
        return componentTypes[typeName];
    }

    
    template<typename T>
    void addComponent(Entity entity, T component)
    {
        auto componentArray = getComponentArray<T>();
        assert(componentArray && "Component not registered: call registerComponent<T>() before addComponent");
        componentArray->insert(entity, component);
    }

    template<typename T>
    void removeComponent(Entity entity)
    {
        getComponentArray<T>()->remove(entity);
    }

    template<typename T>
    T& getComponent(Entity entity)
    {
        return getComponentArray<T>()->get(entity);
    }

    template<typename T>
    bool hasComponent(Entity entity)
    {
        std::type_index typeName = std::type_index(typeid(T));
        auto it = componentTypes.find(typeName);
        if (it == componentTypes.end())
            return false;

        return getComponentArray<T>()->hasComponent(entity);
    }

    void entityDestroyed(Entity entity)
    {
        for (auto const& pair : componentArrays)
            pair.second->entityDestroyed(entity);
    }

private:
    std::unordered_map<std::type_index, ComponentType> componentTypes{};
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays{};
    ComponentType nextComponentType = 0;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> getComponentArray()
    {
        std::type_index typeName = std::type_index(typeid(T));
        if (componentTypes.find(typeName) == componentTypes.end())
        {
            std::string msg = std::string("Component not registered: ") + typeid(T).name();
        }
        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
    }
};