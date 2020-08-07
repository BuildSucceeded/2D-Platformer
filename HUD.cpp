#include "Settings.h"
#include "CollisionDistances.h"
#include "Character.h"
#include "HUD.h"
#include "Engine.h"

HUD::HUD()
{
	lives = 3;
	coins = 0;
	success = false;
}

void HUD::FinishedLevel()
{
	success = true;
}

void HUD::Draw(ID2D1HwndRenderTarget* m_pRenderTarget)
{
	// load any images and D2D resources if they are not already loaded
	if (tileSetImg == NULL)
	{
		tileSetImg = engine->LoadImageW(L"tileset.png");
	}
	if (charSetImg == NULL)
	{
		charSetImg = engine->LoadImageW(L"characters.png");
	}
	if (m_pTextFormat1 == NULL || m_pTextFormat2 == NULL)
	{
		DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(m_pDWriteFactory),
			reinterpret_cast<IUnknown**>(&m_pDWriteFactory)
		);

		m_pDWriteFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			20,
			L"", //locale
			&m_pTextFormat1
		);
		m_pTextFormat1->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
		m_pTextFormat1->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

		m_pDWriteFactory->CreateTextFormat(
			L"Verdana",
			NULL,
			DWRITE_FONT_WEIGHT_NORMAL,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			40,
			L"", //locale
			&m_pTextFormat2
		);
		m_pTextFormat2->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pTextFormat2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}
	if (m_pWhiteBrush == NULL)
	{
		m_pRenderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&m_pWhiteBrush
		);
	}

	// Draw number of coins
	D2D1_RECT_F rectangle1 = D2D1::RectF(
		5, 0,
		5 + TILE_WIDTH / 2, TILE_WIDTH / 2
	);
	D2D1_RECT_F rectangle2 = D2D1::RectF(
		5 * TILE_WIDTH, 7 * TILE_WIDTH,
		6 * TILE_WIDTH - 1, 8 * TILE_WIDTH - 1
	);
	m_pRenderTarget->DrawBitmap(tileSetImg, rectangle1, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangle2);

	rectangle1 = D2D1::RectF(
		5 + TILE_WIDTH / 2, 0,
		5 + 5 * TILE_WIDTH / 2, TILE_WIDTH / 2
	);
	WCHAR scoreStr[64];
	swprintf_s(scoreStr, L" x %d  ", coins);
	m_pRenderTarget->DrawText(
		scoreStr,
		6,
		m_pTextFormat1,
		rectangle1,
		m_pWhiteBrush
	);



	// Draw number of lives
	rectangle1 = D2D1::RectF(
		5, TILE_WIDTH / 2,
		5 + CHARACTER_WIDTH / 2, CHARACTER_HEIGHT / 2 + TILE_WIDTH / 2
	);
	rectangle2 = D2D1::RectF(
		0, 3 * CHARACTER_TILE_HEIGHT - CHARACTER_HEIGHT,
		TILE_WIDTH - 1, 3 * CHARACTER_TILE_HEIGHT - 1
	);
	m_pRenderTarget->DrawBitmap(charSetImg, rectangle1, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangle2);

	rectangle1 = D2D1::RectF(
		5 + TILE_WIDTH / 2, TILE_WIDTH / 2,
		5 + 4 * TILE_WIDTH / 2 + CHARACTER_WIDTH / 2, CHARACTER_HEIGHT / 2 + TILE_WIDTH / 2
	);
	swprintf_s(scoreStr, L" x %d  ", lives);
	m_pRenderTarget->DrawText(
		scoreStr,
		6,
		m_pTextFormat1,
		rectangle1,
		m_pWhiteBrush
	);


	// Draw Game Over if no more lives left
	if (lives == 0)
	{
		rectangle1 = D2D1::RectF(
			0, 0,
			RESOLUTION_X, RESOLUTION_Y
		);
		swprintf_s(scoreStr, L"GAME OVER");
		m_pRenderTarget->DrawText(
			scoreStr,
			9,
			m_pTextFormat2,
			rectangle1,
			m_pWhiteBrush
		);
	}


	// Draw Congratulations if you got out of the level
	if (success)
	{
		rectangle1 = D2D1::RectF(
			0, 0,
			RESOLUTION_X, RESOLUTION_Y
		);
		swprintf_s(scoreStr, L"CONGRATULATIONS!");
		m_pRenderTarget->DrawText(
			scoreStr,
			16,
			m_pTextFormat2,
			rectangle1,
			m_pWhiteBrush
		);
	}
}

void HUD::AddCoins(int addCoins)
{
	coins += addCoins;
}

void HUD::RemoveLife()
{
	if (lives > 0)
		lives--;
}
bool HUD::HasLives()
{
	return lives > 0;
}

