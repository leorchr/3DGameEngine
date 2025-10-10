#include "Actor.h"
#include "AkComponent.h"
#include "Utilities.h"
#include <AK/SoundEngine/Common/AkSoundEngine.h>

AkComponent::AkComponent(class Actor* owner, int updateOrder) : Component(owner, updateOrder)
{
    gameObjectId = getNewAkGameObjectId();
    AK::SoundEngine::RegisterGameObj(gameObjectId, "GameObject");
}

AkComponent::~AkComponent()
{
    AK::SoundEngine::UnregisterGameObj(gameObjectId);
}

void AkComponent::setAsListener() const
{
    updatePosition();
    AK::SoundEngine::SetDefaultListeners(&gameObjectId, 1);
}

void AkComponent::updatePosition() const
{
    Vector3 actorPosition = owner.getPosition();
    Vector3 actorFront = owner.getForward();
    Vector3 actorTop = owner.getUp();
    AkVector pos = { actorPosition.x, -actorPosition.z, actorPosition.y };
    AkVector front = { actorFront.x, -actorFront.z, actorFront.y };
    AkVector top = { actorTop.x, -actorTop.z, actorTop.y };
    
    AkTransform position {};
    position.Set(pos, front, top);

    AK::SoundEngine::SetPosition(gameObjectId, position);
}

void AkComponent::PostEvent(AkUniqueID eventId) const
{
    updatePosition();
    AK::SoundEngine::PostEvent(eventId, gameObjectId);
}
