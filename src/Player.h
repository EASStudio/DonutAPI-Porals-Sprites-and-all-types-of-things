#pragma once

#include "System.h"

class Player : public System
{
public:
    void update(float dt) override;
};