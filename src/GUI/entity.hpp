#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <string>
#include <type_traits>

/**
 * @brief Entity class that describes an entity in the game world (wall, enemy, pickup).
 * It holds an unordered map of components that have type indexes as their keys. 
 */
class Entity {
    private:
        std::string name;
        std::unordered_map<std::type_index, std::shared_ptr<void>> components;

    public:
        Entity(std::string nam) : name(nam) {}

        Entity() = default;

        /**
         * @param component component struct to be added to components map.
         */

        template <typename T>
        void AddComponent(const T& component) {
            components[std::type_index(typeid(T))] = std::make_shared<T>(component);
        }

        /**
         * @param args arguments that are forwarded to component constructor,
         */
        
        template <typename T, typename ... Args>
        void CreateAddComponent(Args&&... args) {
            components[std::type_index(typeid(T))] = std::make_shared<T>(std::forward<Args>(args)...);
        }

        /**
         * @return component of the entity of given type or nullptr if it does not have it
         */

        template <typename T>
        std::shared_ptr<T> GetComponent() {
            auto it = components.find(std::type_index(typeid(T)));
            if (it != components.end()) {
                return std::static_pointer_cast<T>(it->second);
            }
            return nullptr;
        }


        /**
         * @return true if Entity has component, false otherwise
         */
        template<typename T>
        bool HasComponent() const {
            return components.find(std::type_index(typeid(T))) != components.end();
        }

        const std::string& GetName() const { return name; }
        void ChangeName(const std::string& newName) { name = newName; }


};

#endif

