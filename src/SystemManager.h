#pragma once

#include "Entity.h"
#include "System.h"

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <algorithm>

class SystemManager
{
public:
    template<typename T>
    std::shared_ptr<T> registerSystem()
    {
        std::type_index typeName = std::type_index(typeid(T));

        assert(systems.find(typeName) == systems.end() && "Registering system more than once.");

        auto system = std::make_shared<T>();
        systems.insert({ typeName, system });
        return system;
    }

    template<typename T>
    void setSignature(Signature signature)
    {
        std::type_index typeName = std::type_index(typeid(T));

        assert(systems.find(typeName) != systems.end() && "System not registered.");

        signatures.insert({ typeName, signature });
    }

    void entityDestroyed(Entity entity)
    {
        for (auto const& pair : systems)
        {
            auto& ents = pair.second->entities;
            ents.erase(
                std::remove(ents.begin(), ents.end(), entity),
                ents.end());
        }
    }

    void entitySignatureChanged(Entity entity, Signature entitySignature)
    {
        for (auto const& pair : systems)
        {
            auto const& systemType = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[systemType];

            if ((entitySignature & systemSignature) == systemSignature)
            {
                if (std::find(system->entities.begin(), system->entities.end(), entity) == system->entities.end())
                    system->entities.push_back(entity);
            }
            else
            {
                auto& ents = system->entities;
                ents.erase(
                    std::remove(ents.begin(), ents.end(), entity),
                    ents.end());
            }
        }
    }

private:
    std::unordered_map<std::type_index, Signature> signatures{};
    std::unordered_map<std::type_index, std::shared_ptr<System>> systems{};
};