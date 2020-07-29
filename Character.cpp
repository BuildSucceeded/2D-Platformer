#include "Settings.h"
#include "Character.h"
#include "Level.h"
#include "Engine.h"

Character::Character()
{
	position.x = 150;
	position.y = RESOLUTION_Y - 250;

	speed.x = 0;
	speed.y = 0;

	GoesLeft = false;
	GoesRight = false;
}

void Character::Logic(double elapsedTime)
{
	// update x position based on speed, but make sure we don't go outside the level
	position.x += speed.x * elapsedTime;
	if (position.x < CHARACTER_WIDTH / 2)
	{
		position.x = CHARACTER_WIDTH / 2;
		speed.x = 0;
	}
	if (position.x > LEVEL_WIDTH* TILE_WIDTH - CHARACTER_WIDTH / 2)
	{
		position.x = LEVEL_WIDTH * TILE_WIDTH - CHARACTER_WIDTH / 2;
		speed.x = 0;
	}
	if (GoesLeft) // if left is pressed, accelerate left
	{
		speed.x -= 1500 * elapsedTime;
		if (speed.x < -300)
			speed.x = -300;
	}
	if (GoesRight) // if right is pressed, accelerate right
	{
		speed.x += 1500 * elapsedTime;
		if (speed.x > 300)
			speed.x = 300;
	}
	if (!GoesLeft && !GoesRight) // if no key is pressed, and we're on ground, create drag so the character will stop
	{
		if (speed.y == 0)
		{
			if (speed.x > 0)
			{
				speed.x -= 1500 * elapsedTime;
				if (speed.x < 10)
					speed.x = 0;
			}
			if (speed.x < 0)
			{
				speed.x += 1500 * elapsedTime;
				if (speed.x > -10)
					speed.x = 0;
			}
		}
	}

	// update y position based on speed
	position.y += speed.y * elapsedTime;
	// apply gravity
	speed.y += 1500 * elapsedTime;
}

void Character::StopFalling(double collisionSize)
{
	// this is called when we have a bottom collision and we need to stop falling
	speed.y = 0;
	position.y -= collisionSize;
}

void Character::Jump(bool fullJump)
{
	// this is called when the user presses the jump key
	if (fullJump)
		speed.y = -700;
	else
		speed.y = -400;
}

void Character::BounceTop()
{
	// this is called when we have a top collision and we need to bounce back
	speed.y = abs(speed.y);
}

void Character::StopMovingLeft(double collisionSize)
{
	// this is called when we have a left collision
	speed.x = 0;
	position.x += collisionSize;
}

void Character::StopMovingRight(double collisionSize)
{
	// this is called when we have a right collision
	speed.x = 0;
	position.x -= collisionSize;
}

void Character::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (tileSetImg == NULL) // loads the character image if not already loaded
	{
		tileSetImg = engine->LoadImageW(L"characters.png");
	}

	// destination for draing
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		position.x - Engine::offset - CHARACTER_WIDTH / 2, position.y - CHARACTER_TILE_HEIGHT,
		position.x - Engine::offset + CHARACTER_WIDTH / 2 - 1, position.y
	);

	// just choose a random tile to draw for now
	int posX = 0;
	int posY = 0;

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		posX * CHARACTER_WIDTH, posY * CHARACTER_TILE_HEIGHT,
		(posX + 1) * CHARACTER_WIDTH - 1, (posY + 1) * CHARACTER_TILE_HEIGHT - 1
	);

	// actually drawing
	m_pRenderTarget->DrawBitmap(tileSetImg, rectangle1, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangle2);
}