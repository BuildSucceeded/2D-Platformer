#include "framework.h"
#include "Settings.h"
#include "EngineBase.h"
#include "Engine.h"
#include "Level.h"
#include "HUD.h"

double Engine::offset = 0;

Engine::Engine()
{
    // Initilize level
    level = new Level();
    AddGameObject(level);

    // Initialize character
    character = new Character();
    AddGameObject(character);

    // Initialize enemies
    noEnemies = 3;
    enemies[0] = new Enemy(425, 350, 625, 0);
    AddGameObject(enemies[0]);
    enemies[1] = new Enemy(1475, 150, 1525, 7);
    AddGameObject(enemies[1]);
    enemies[2] = new Enemy(3275, 300, 3475, 11);
    AddGameObject(enemies[2]);

    // Initialize HUD
    hud = new HUD();
    AddGameObject(hud);

    leftPressed = false;
    rightPressed = false;
    spacePressed = false;
}

Engine::~Engine()
{
}

void Engine::KeyUp(WPARAM wParam)
{
    // sets and resets key presses
    if (wParam == VK_LEFT)
        leftPressed = false;
    if (wParam == VK_RIGHT)
        rightPressed = false;
    if (wParam == VK_SPACE)
        spacePressed = false;
}

void Engine::KeyDown(WPARAM wParam)
{
    // sets and resets key presses
    if (wParam == VK_LEFT)
        leftPressed = true;
    if (wParam == VK_RIGHT)
        rightPressed = true;
    if (wParam == VK_SPACE)
        spacePressed = true;
}

void Engine::Logic(double elapsedTime)
{
    EngineBase::Logic(elapsedTime);

    // This is the logic part of the engine. Add your game logic here.
    // It runs every frame and it receives the elapsed time between the last frame and this one, in seconds.
    // Use this value for a smooth and consistent movement, regardless of the CPU or graphics speed

    if (!character->IsDead()) // all the logic below is happening only if the character is not dead
    {
        // key inputs are sent to character
        character->GoesLeft = leftPressed;
        character->GoesRight = rightPressed;

        // If we hit any coins, we add them to the HUD
        hud->AddCoins(level->PickUpCollectibles(character));

        // If we collide with the tiles of the level
        CollisionDistances cd = level->CharacterCollides(character);
        if (cd.bottom > 0)
        {
            // if it's a bottom collision, we stop the character from falling
            character->StopFalling(cd.bottom);
            if (spacePressed)
            { // if the user pressed the jump key, the character jumps
                character->Jump(true);
                spacePressed = false;
            }
        }
        if (cd.left > 0)
        {
            // if it's a left collision
            character->StopMovingLeft(cd.left);
        }
        if (cd.right > 0)
        {
            // if it's a right collision
            character->StopMovingRight(cd.right);
        }
        if (cd.top > 0)
        {
            // if it's a top collision
            character->BounceTop();
        }

        for (int i = 0; i < noEnemies; i++)
        { // check for collisions with enemies
            CollisionDistances cd = enemies[i]->CharacterCollides(character);
            if (cd.bottom > 0)
            {
                // if we hit the enemy from above, remove enemy
                Enemy* enemy = enemies[i];
                for (int k = i; k < noEnemies - 1; k++)
                {
                    enemies[k] = enemies[k + 1];
                }
                enemies[noEnemies - 1] = NULL;
                RemoveGameObject(enemy);
                delete enemy;
                noEnemies--;
                character->Jump(false);
            }
            else if (cd.left > 0 || cd.right > 0 || cd.top > 0)
            {
                // if we hit the enemy from any other direction, die
                character->Die();
                hud->RemoveLife();
            }
        }

        if (character->GetPosition().y > RESOLUTION_Y)
        {
            // If the character falls below the level, it dies
            character->Die();
            hud->RemoveLife();
        }

        if (level->LevelExit(character))
        {
            // if the character hit the exit point of the level, game over, you win
            hud->FinishedLevel();
        }

        // Change the display offset based on character position, but clamp it to the limits
        Engine::offset = character->GetPosition().x - RESOLUTION_X / 2;
        if (Engine::offset < 0)
            Engine::offset = 0;
        if (Engine::offset > LEVEL_WIDTH* TILE_WIDTH - RESOLUTION_X)
            Engine::offset = LEVEL_WIDTH * TILE_WIDTH - RESOLUTION_X;
    }
    else
    {
        // if the character is dead (floating up) and it hits the top of the screen, we can reset (if we still have lives)
        if (character->GetPosition().y < 0 && hud->HasLives())
        {
            character->Reset();
        }
    }
}