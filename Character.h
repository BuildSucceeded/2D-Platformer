#pragma once
#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

#define CHARACTER_WIDTH 50
#define CHARACTER_HEIGHT 66
#define CHARACTER_TILE_HEIGHT 100

class Character : public GameObjectBase
{
public:
	Character();

	void Logic(double elapsedTime) override;
	void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

	void StopFalling(double collisionSize);
	void Jump(bool fullJump);
	void BounceTop();
	void StopMovingLeft(double collisionSize);
	void StopMovingRight(double collisionSize);
	void Die();
	bool IsDead();
	void Reset();

	bool GoesLeft;
	bool GoesRight;

private:
	ID2D1Bitmap* tileSetImg;

	Point2D speed;
	Point2D prevPosition;

	bool forward;

	bool dead;

};

