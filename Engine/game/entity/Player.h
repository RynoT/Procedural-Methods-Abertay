#pragma once

#include "entity.h"

class Player : public Entity
{
public:
	Player();

	virtual void Update(const float& delta) override;

};
