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

void AkComponent::PostEvent(AkUniqueID eventId)
{
    AK::SoundEngine::PostEvent(eventId, gameObjectId);
}
