#pragma once
#include "CubeActor.h"

class KeyActor : public CubeActor
{
public:
	KeyActor();
	~KeyActor();
	void setKey(int keyCode);
	int getKey() { return keyCode; }
	void onHit();
private:
	int keyCode;
};