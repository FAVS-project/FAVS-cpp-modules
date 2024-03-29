#include "OpenGLPG_Base.h"

#include "World.h"

#include "Assert.h"
#include "Serializer.h"

#include <utility>

World::World(const ConstructionParams& someParams)
    : myClientLoader {someParams.myClientLoader}
{
    myClientLoader.GetWorldLoader().LoadWorld(someParams.aLevelName, this);

    for (auto& entity : myEntityQueue)
    {
        entity.myUID = UID::CreateUnique();
        entity.Load({myClientLoader});
    }
}

void World::Init()
{
    SpawnQueuedEntities();

    // Temporary camera activation flow:

    // CameraActivationRequest request {{CameraType::FreeCam, glm::radians(45.f), 16.f / 9.f, 0.1f, 200.f}};
    // Keep this around in case I ever break the serialization

    CameraActivationRequest request {{myClientLoader.myAssetFolder + "Camera/freecam.camera"}};
    myGameplayCamera = request.GetCameraUID();
    myCameraManager.ActivateCamera(request);

    //
}

void World::Shutdown()
{
    myCameraManager.DeactivateCamera({myGameplayCamera});
}

void World::Update(const UpdateParams& someParams)
{
    for (auto& entity : myEntityQueue)
    {
        entity.Load({myClientLoader});
    }
    SpawnQueuedEntities();

    for (auto& [uid, entity] : myEntities)
    {
        entity.Update();
    }

    myCameraManager.Update({someParams.myGameInputData, someParams.myDeltaTime});
}

void World::SaveWorld(const std::string& aLevelName)
{
    myClientLoader.GetWorldLoader().SaveWorld(aLevelName, this);
}

bool World::IsAvailable() const
{
    return true;
}

UID World::RequestEntitySpawn(const Filepath& aTemplateFilepath)
{
    ASSERT(aTemplateFilepath.HasExtension("template"), "Wrong file extension");
    Entity newEntity;
    SerializerLoader loader(aTemplateFilepath);
    newEntity.Serialize(loader);
    const UID newUID {UID::CreateUnique()};
    newEntity.myUID = newUID;
    myEntityQueue.EmplaceBack(std::move(newEntity));
    return newEntity.myUID;
}

void World::RequestEntityUnspawn(const UID& anEntityUID)
{
    if (auto entity = myEntities.find(anEntityUID); entity != myEntities.end())
    {
        entity->second.Unspawn();
        myEntities.erase(entity);
    }
}

const CameraData& World::GetActiveCameraData() const
{
    return myCameraManager.GetActiveCameraData();
}

const Entity& World::GetEntity(const UID& anEntityUID) const
{
    ASSERT(myEntities.contains(anEntityUID), "Invalid Entity UID");
    return myEntities.at(anEntityUID);
}

Entity& World::GetEntity(const UID& anEntityUID)
{
    ASSERT(myEntities.contains(anEntityUID), "Invalid Entity UID");
    return myEntities.at(anEntityUID);
}

void World::SpawnQueuedEntities()
{
    for (auto& entity : myEntityQueue)
    {
        const UID entityUID {entity.myUID};
        auto pair {myEntities.try_emplace(entityUID, std::move(entity))};
        Entity& newEntity {pair.first->second};
        newEntity.Spawn(this);
    }
    myEntityQueue.RemoveAll();
}
