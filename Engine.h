#pragma once

#include "resource.h"
#include "EngineBase.h"
#include "Level.h"
#include "Character.h"

class Engine : public EngineBase
{
public:
	Engine();
	~Engine();

	void KeyUp(WPARAM wParam);
	void KeyDown(WPARAM wParam);

	void Logic(double elapsedTime) override;

	static double offset;
private:
	Level* level;
	Character* character;
	boolean leftPressed;
	boolean rightPressed;
	boolean spacePressed;
};

