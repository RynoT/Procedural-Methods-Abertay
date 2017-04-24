#pragma once

#include "entity.h"

#define PLAYER_HEIGHT 10.0f

class Player : public Entity
{
public:
	Player();

	virtual void Update(const float& delta) override;
};
