#include "OpenGLPG_Base.h"

#include "Entity.h"

Entity::Entity()
    : myUID {UID::CreateUnique()}
{}

void Entity::Load()
{
    for (const auto& component : myComponents)
    {
        component->OnLoad();
    }
}

void Entity::Spawn()
{
    for (const auto& component : myComponents)
    {
        component->OnEnterWorld();
    }
}

void Entity::Update()
{
    for (const auto& component : myComponents)
    {
        component->Update();
    }
}

void Entity::Unspawn()
{
    for (const auto& component : myComponents)
    {
        component->OnExitWorld();
    }
}

void Entity::Unload()
{
    for (const auto& component : myComponents)
    {
        component->OnUnload();
    }
}

const UID& Entity::GetUID()
{
    return myUID;
}