#pragma once
#include "Component.h"
#include "AABB.h"
#include "OBB.h"

class BoxComponent : public Component
{
public:
	BoxComponent(class Actor* ownerP, int updateOrderP = 100);
	virtual ~BoxComponent();

	const AABB& getWorldAABB() const { return worldAABB; }
	const AABB& getBaseObjectAABB() const { return baseObjectAABB; }
	const AABB& getObjectAABB() const { return objectAABB; }
	const OBB& getWorldOBB() const { return worldOBB; }
	void setObjectBox(const AABB& objectBoxP);
	void setShouldRotate(bool shouldRotateP);

	void onUpdateWorldTransform() override;


private:
	AABB baseObjectAABB;
	AABB objectAABB;
	AABB worldAABB;
	OBB worldOBB;
	bool shouldRotate;
};

