#pragma once

#include "resource.h"
#include "framework.h"
#include "EngineBase.h"
#include "GameObjectBase.h"

class HUD : public GameObjectBase
{
public:
    HUD();

    void Draw(ID2D1HwndRenderTarget* m_pRenderTarget) override;

    void AddCoins(int addCoins);
    void RemoveLife();
    bool HasLives();
    void FinishedLevel();

private:
    ID2D1Bitmap* tileSetImg;
    ID2D1Bitmap* charSetImg;

    IDWriteFactory* m_pDWriteFactory;
    IDWriteTextFormat* m_pTextFormat1;
    IDWriteTextFormat* m_pTextFormat2;
    ID2D1SolidColorBrush* m_pWhiteBrush;

    int lives;
    int coins;
    bool success;
};

