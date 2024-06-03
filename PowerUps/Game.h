#pragma once
#include "PowerUp.h"
#include "SoundEffect.h"
#include <vector>
#include <SDL_ttf.h>
#include "Texture.h"

class Game final
{
public:
    enum class GameState
    {
        Start,
        Playing,
        Restart
    };

    explicit Game(const Window& window);
    Game(const Game& other) = delete;
    Game& operator=(const Game& other) = delete;
    Game(Game&& other) = delete;
    Game& operator=(Game&& other) = delete;
    ~Game();

    void Update(float elapsedSec);
    void Draw() const;

    // Event handling
    void ProcessKeyDownEvent(const SDL_KeyboardEvent& e);
    void ProcessKeyUpEvent(const SDL_KeyboardEvent& e);
    void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e);
    void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e);
    void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e);

private:
    // DATA MEMBERS
    const Window m_Window;

    Point2f m_MousePos;
    static const int m_InitialNrPowerUps{ 10 }; 
    std::vector<PowerUp*> m_pPowerUps;
    std::vector<Point2f> m_PowerUpsCenters;
    std::vector<PowerUp::Type> m_PowerUpTypes;
    std::vector<bool> m_PowerUpsDestroyed;
    float m_PowerUpRadius;
    int m_ActNrPowerUps;
    Rectf m_Destroyer;
    SoundEffect* m_KillMusic;
    int m_CurrentLevel;
    int m_Mistakes;
  

    GameState m_GameState; 

    // Textures
    Texture* m_pTextureStart;
    Texture* m_pTextureFight;
    Texture* m_pTextureEnd;

    // Timer
    float m_LevelTimer;
    const float m_MaxLevelTime = 20.0f;

    // Font and text
    Color4f m_TextColor{ 1.0f, 1.0f, 1.0f, 1.0f };
    TTF_Font* m_pFont{ TTF_OpenFont("Resources/Fonts/Speedy.ttf", 32) };
    Texture* m_pTextTexture{ new Texture("Level: " + std::to_string(m_CurrentLevel) + "                     Time: " + std::to_string(m_LevelTimer) + "             Mistakes: " + std::to_string(m_Mistakes) , m_pFont, m_TextColor) };
    
    // FUNCTIONS
    void Initialize();
    void Cleanup();
    void ClearBackground() const;

    void ShowTestMessage() const;
    void ShowNrPowerUps() const;
    void CreatePowerUps(int number = m_InitialNrPowerUps); 
    void UpdatePowerUps(float elapsedSec);
    void DrawPowerUps() const;
    void DeletePowerUps();

    void MoveDestroyer(const Point2f& newCenter);
    void VerifyOverlapping();
    void DrawDestroyer() const;
    void Reset();
    void LevelUp();
    void DeleteGreenPowerUps();
    void RegenerateGreenPowerUps();
   
};
