#pragma once

#include "entity.h"

#define PLAYER_HEIGHT 1.75f

class Player : public Entity
{
public:
	Player();

	virtual void Update(const float& delta) override;
};
