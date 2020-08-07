#pragma once

class Character;

#include "resource.h"
#include "framework.h"
#include "CollisionDistances.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

#define ENEMY_WIDTH 50
#define ENEMY_HEIGHT 50

class Enemy : public GameObjectBase
{
public:
	Enemy(double initialX, double initialY, double maxX, int type);

	void Logic(double elapsedTime) override;
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	CollisionDistances CharacterCollides(Character* character);

private:
	ID2D1Bitmap* enemySetImg;

	double min, max;

	int enemyType;

	bool forward;
};

