#pragma once

#include "System.h"
#include "Raycaster.h"

class MovementSystem : public System 
{
public:
	void update(float dt) override;
	bool checkPointCollision(float tx, float ty, float curFloorH);
	bool canMoveTo(float nx, float ny);
	void setRaycasterData(Entity playerEnt, Coordinator* coord);

private:
	Raycaster raycaster;
};