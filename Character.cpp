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

	forward = true;
	dead = false;
}

void Character::Logic(double elapsedTime)
{
	if (!dead) // We do all this logic only if it's not dead
	{
		// save previous position
		prevPosition = position;

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

		// set forward, to be used for drawing the character in the right direction
		if (speed.x > 0)
			forward = true;
		if (speed.x < 0)
			forward = false;
	}
	else
	{
		// if it's dead, just slowly float up
		position.y -= 200 * elapsedTime;
	}
}

void Character::StopFalling(double collisionSize)
{
	// this is called when we have a bottom collision and we need to stop falling
	if (!dead)
	{
		speed.y = 0;
		position.y -= collisionSize;
	}
}

void Character::Jump(bool fullJump)
{
	// this is called when the user presses the jump key
	if (!dead)
	{
		if (fullJump)
			speed.y = -700;
		else
			speed.y = -400;
	}
}

void Character::BounceTop()
{
	// this is called when we have a top collision and we need to bounce back
	if (!dead)
	{
		speed.y = abs(speed.y);
	}
}

void Character::StopMovingLeft(double collisionSize)
{
	// this is called when we have a left collision
	if (!dead)
	{
		speed.x = 0;
		position.x += collisionSize;
	}
}

void Character::StopMovingRight(double collisionSize)
{
	// this is called when we have a right collision
	if (!dead)
	{
		speed.x = 0;
		position.x -= collisionSize;
	}
}

void Character::Die()
{
	// kills the character
	dead = true;
}

bool Character::IsDead()
{
	// returns true if the character is dead
	return dead;
}

void Character::Reset()
{
	// resets the character at the beginning of the level
	position.x = 150;
	position.y = RESOLUTION_Y - 250;

	speed.x = 0;
	speed.y = 0;

	GoesLeft = false;
	GoesRight = false;

	forward = true;
	dead = false;
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

	// this is to decide which tile in the tileset to draw, by default just sitting
	int posX = forward ? 3 : 4;
	int posY = 0;

	if (position.y != prevPosition.y) // if the character is in the middle of the jump
	{
		posX = forward ? 1 : 3;
		posY = 2;
	}
	else if (position.x != prevPosition.x) // if the character moves on ground
	{
		int step = (int)position.x % 100 / 25;
		posY = 1;
		posX = (forward ? step : 7 - step);
	}

	if (dead) // if it's dead
	{
		posX = forward ? 4 : 5;
		posY = 3;
	}

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		posX * CHARACTER_WIDTH, posY * CHARACTER_TILE_HEIGHT,
		(posX + 1) * CHARACTER_WIDTH - 1, (posY + 1) * CHARACTER_TILE_HEIGHT - 1
	);

	// actually drawing
	m_pRenderTarget->DrawBitmap(tileSetImg, rectangle1, dead ? 0.6f : 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangle2);
}