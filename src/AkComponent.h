#pragma once
#include "Component.h"
#include <AK/SoundEngine/Common/AkTypes.h>

class AkComponent : public Component
{
public:
    AkComponent(class Actor* owner, int updateOrder = 100);
    virtual ~AkComponent();

    void PostEvent(AkUniqueID eventId);

private:
    AkGameObjectID gameObjectId;
};