#include "Settings.h"
#include "Enemy.h"
//#include "Level.h"
#include "Engine.h"

Enemy::Enemy(double initialX, double initialY, double maxX, int type)
{
	// initializes the enemy position and maximums
	position.x = initialX;
	position.y = initialY;

	min = initialX;
	max = maxX;

	enemyType = type;

	forward = true;
}

void Enemy::Logic(double elapsedTime)
{
	// moves the enemy and turns it around if it gets to the limits
	position.x += (forward ? 25 : -25) * elapsedTime;

	if (position.x >= max)
	{
		forward = false;
	}
	if (position.x <= min)
	{
		forward = true;
	}
}

CollisionDistances Enemy::CharacterCollides(Character* character)
{ // returns collision details if the enemy collides with the character

	double charTop = character->GetPosition().y - CHARACTER_HEIGHT;
	double charBottom = character->GetPosition().y;
	double charLeft = character->GetPosition().x - CHARACTER_WIDTH / 2 + 3;
	double charRight = character->GetPosition().x + CHARACTER_WIDTH / 2 - 4;

	double enemyTop = position.y - ENEMY_HEIGHT;
	double enemyBottom = position.y;
	double enemyLeft = position.x - ENEMY_WIDTH / 2 + 3;
	double enemyRight = position.x + ENEMY_WIDTH / 2 - 4;

	CollisionDistances collision;
	collision.top = 0;
	collision.bottom= 0;
	collision.left = 0;
	collision.right = 0;

	// If it's a collision
	if (charTop < enemyBottom && charBottom > enemyTop&& charRight > enemyLeft&& charLeft < enemyRight)
	{
		collision.top = abs(charTop - enemyBottom);
		collision.bottom = abs(charBottom - enemyTop);
		collision.left = abs(charLeft - enemyRight);
		collision.right = abs(charRight - enemyLeft);

		collision.keepSmallest();
	}

	return collision;

}

void Enemy::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	if (enemySetImg == NULL) // loads the tileset if needed
	{
		enemySetImg = engine->LoadImageW(L"enemies.png");
	}

	D2D1_RECT_F rectangle1 = D2D1::RectF(
		position.x - Engine::offset - ENEMY_WIDTH / 2, position.y - ENEMY_HEIGHT,
		position.x - Engine::offset + ENEMY_WIDTH / 2 - 1, position.y
	);

	int posX = (int)position.x % 20 / 10 + (forward ? 2 : 0);
	int posY = enemyType;

	D2D1_RECT_F rectangle2 = D2D1::RectF(
		posX * ENEMY_WIDTH, posY * ENEMY_HEIGHT,
		(posX + 1) * ENEMY_WIDTH - 1, (posY + 1) * ENEMY_HEIGHT - 1
	);

	// drawing the enemy
	m_pRenderTarget->DrawBitmap(enemySetImg, rectangle1, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangle2);
}