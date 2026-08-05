#pragma once

#include "System.h"

class Item : public System
{
public:
	void update(float dt) override;
};