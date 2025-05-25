#pragma once
#include <unordered_map>
#include <vector>
#include <functional>
#include <typeindex>
#include <type_traits>
#include <cassert>

using Entity = uint32_t;
const Entity MAX_ENTITIES = 10000;

struct IComponentArray {
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
public:
    void InsertData(Entity entity, T component) {
        assert(entity < MAX_ENTITIES);
        if (entityToIndex.find(entity) != entityToIndex.end()) {
            data[entityToIndex[entity]] = component;
        } else {
            size_t newIndex = size++;
            entityToIndex[entity] = newIndex;
            indexToEntity[newIndex] = entity;
            data[newIndex] = component;
        }
    }

    void RemoveData(Entity entity) {
        if (entityToIndex.find(entity) == entityToIndex.end()) return;
        size_t index = entityToIndex[entity];
        size_t lastIndex = size - 1;
        data[index] = data[lastIndex];
        Entity lastEntity = indexToEntity[lastIndex];
        entityToIndex[lastEntity] = index;
        indexToEntity[index] = lastEntity;
        entityToIndex.erase(entity);
        indexToEntity.erase(lastIndex);
        size--;
    }

    T& GetData(Entity entity) {
        assert(entityToIndex.find(entity) != entityToIndex.end());
        return data[entityToIndex[entity]];
    }

    void EntityDestroyed(Entity entity) override {
        RemoveData(entity);
    }

    std::vector<T> GetAllData() {
        std::vector<T> all;
        for (size_t i = 0; i < size; ++i) {
            all.push_back(data[i]);
        }
        return all;
    }

    std::vector<Entity> GetAllEntities() {
        std::vector<Entity> all;
        for (size_t i = 0; i < size; ++i) {
            all.push_back(indexToEntity[i]);
        }
        return all;
    }

private:
    std::array<T, MAX_ENTITIES> data;
    std::unordered_map<Entity, size_t> entityToIndex;
    std::unordered_map<size_t, Entity> indexToEntity;
    size_t size = 0;
};

class ECS {
public:
    ECS() {
        for (Entity e = 0; e < MAX_ENTITIES; ++e)
            availableEntities.push_back(e);
    }

    Entity CreateEntity() {
        assert(livingEntityCount < MAX_ENTITIES);
        Entity id = availableEntities.back();
        availableEntities.pop_back();
        livingEntityCount++;
        return id;
    }

    void DestroyEntity(Entity entity) {
        assert(entity < MAX_ENTITIES);
        for (auto const& pair : componentArrays)
            pair.second->EntityDestroyed(entity);
        availableEntities.push_back(entity);
        livingEntityCount--;
    }

    template<typename T>
    void RegisterComponent() {
        const char* typeName = typeid(T).name();
        assert(componentTypes.find(typeName) == componentTypes.end());

        componentTypes[typeName] = nextComponentType++;
        componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    template<typename T>
    void ForEach(std::function<void(Entity, T&)> func) {
        auto arr = GetComponentArray<T>();
        for (Entity e : arr->GetAllEntities()) {
            func(e, arr->GetData(e));
        }
    }

    template<typename T1, typename T2>
    void AddSystem(std::function<void(T1&, T2&)> system) {
        auto arr1 = GetComponentArray<T1>();
        auto arr2 = GetComponentArray<T2>();

        for (Entity e : arr1->GetAllEntities()) {
            if (arr2->GetAllEntities().end() != std::find(arr2->GetAllEntities().begin(), arr2->GetAllEntities().end(), e)) {
                system(arr1->GetData(e), arr2->GetData(e));
            }
        }
    }

    void Update() {
        // Placeholder if needed later
    }

private:
    std::vector<Entity> availableEntities;
    uint32_t livingEntityCount = 0;

    std::unordered_map<const char*, size_t> componentTypes;
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays;

    size_t nextComponentType = 0;

    template<typename T>
    std::shared_ptr<ComponentArray<T>> GetComponentArray() {
        const char* typeName = typeid(T).name();
        return std::static_pointer_cast<ComponentArray<T>>(componentArrays[typeName]);
    }
};
