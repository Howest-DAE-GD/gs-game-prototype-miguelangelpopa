#include "pch.h"
#include "Game.h"
#include "utils.h"
#include <iostream>
#include "Texture.h"

Game::Game(const Window& window)
    : m_Window{ window }
    , m_MousePos{}
    , m_pPowerUps{}
    , m_PowerUpsCenters{}
    , m_PowerUpTypes{}
    , m_PowerUpsDestroyed{}
    , m_PowerUpRadius{}
    , m_ActNrPowerUps{ 0 }
    , m_Destroyer{ 0.0f, 0.0f, 20.0f, 20.0f }
    , m_CurrentLevel{ 1 }
    , m_Mistakes{ 0 }
    , m_GameState{ GameState::Start }
    , m_pTextureStart{ new Texture{ "Resources/Images/Begin.png" } }
    , m_pTextureFight{ new Texture{ "Resources/Images/fight.png" } }
    , m_pTextureEnd{ new Texture{ "Resources/Images/lost.png" } }
    , m_LevelTimer{ m_MaxLevelTime }
{
    Initialize();
    
}

Game::~Game()
{
    Cleanup();
    
}

void Game::Initialize()
{
    CreatePowerUps();
    if (m_GameState == GameState::Start)
    {
        std::cout << "Press any key to start the game..." << std::endl;
    }
    //ShowTestMessage();
}

void Game::Cleanup()
{
    DeletePowerUps();
    delete m_pTextureStart;
    m_pTextureStart = nullptr;
    delete m_pTextureFight;
    m_pTextureFight = nullptr;
    delete m_pTextureEnd;
    m_pTextureEnd = nullptr;
    delete m_pTextTexture;
    m_pTextTexture = nullptr;
}

void Game::Update(float elapsedSec)
{
    switch (m_GameState)
    {
    case GameState::Start:
        break;

    case GameState::Playing:
        // Actualizar el temporizador
        m_LevelTimer -= elapsedSec;
        std::cout << "Time left: " << static_cast<int>(m_LevelTimer) << " seconds" << std::endl;
        delete m_pTextTexture;
       
        m_pTextTexture = new Texture("Level: " + std::to_string(m_CurrentLevel) + "           Time: " + std::to_string(static_cast<int>(m_LevelTimer)) + "         Mistakes: " + std::to_string(m_Mistakes)+"/10", m_pFont, m_TextColor);
        if (m_LevelTimer <= 0.0f)
        {
            std::cout << "Time's up! Game Over." << std::endl;
            m_GameState = GameState::Restart;
        }

        if (m_Mistakes >= 10)
        {
            m_GameState = GameState::Restart;
        }

        UpdatePowerUps(elapsedSec);
        MoveDestroyer(m_MousePos);
        VerifyOverlapping();

        {
            bool allBrownsEliminated = true;
            for (size_t idx = 0; idx < m_PowerUpTypes.size(); ++idx)
            {
                if (m_PowerUpTypes[idx] == PowerUp::Type::brown && m_pPowerUps[idx] != nullptr)
                {
                    allBrownsEliminated = false;
                    break;
                }
            }

            if (allBrownsEliminated)
            {
                LevelUp();
            }
        }
        break;

    case GameState::Restart:
        std::cout << "Press any key to restart the game..." << std::endl;
        break;
    }
}

void Game::Draw() const
{
    ClearBackground();

    SDL_Color white = { 255, 255, 255, 255 };

    switch (m_GameState)
    {
    case GameState::Start:
        if (m_pTextureStart)
        {
            m_pTextureStart->Draw(Point2f(0, 0), Rectf(0.f, 0.f, static_cast<float>(m_pTextureStart->GetWidth()), static_cast<float>(m_pTextureStart->GetHeight())));
          
        }
       
        break;

    case GameState::Playing:
        if (m_pTextureFight)
        {
            m_pTextureFight->Draw(Point2f(0, 0), Rectf(0.f, 0.f, static_cast<float>(m_pTextureFight->GetWidth()), static_cast<float>(m_pTextureFight->GetHeight())));
            m_pTextTexture->Draw(Point2f(0,m_Window.height-m_pTextTexture->GetHeight()));
        }
        DrawPowerUps();
        DrawDestroyer();
        break;

    case GameState::Restart:
        if (m_pTextureEnd)
        {
            m_pTextureEnd->Draw(Point2f(0, 0), Rectf(0.f, 0.f, static_cast<float>(m_pTextureEnd->GetWidth()), static_cast<float>(m_pTextureEnd->GetHeight())));
        }
        break;
    }
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
    if (m_GameState == GameState::Start)
    {
        m_GameState = GameState::Playing;
        m_LevelTimer = m_MaxLevelTime; // Resetear el temporizador al inicio del juego
    }
    else if (m_GameState == GameState::Restart)
    {
        m_GameState = GameState::Start;
        Reset();
    }
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
    m_MousePos.x = float(e.x);
    m_MousePos.y = m_Window.height - e.y;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
}

void Game::ClearBackground() const
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Game::ShowTestMessage() const
{
    std::cout << "Circle test :\n";

    ShowNrPowerUps();
}

void Game::ShowNrPowerUps() const
{
    std::cout << "Number of Circle objects " << m_ActNrPowerUps << "\n";
}

void Game::CreatePowerUps(int number)
{
    // Created PowerUps
    Texture* pTexture{ new Texture{"./Resources/Images/PowerUp.png"} };
    m_PowerUpRadius = pTexture->GetWidth() / 2;
    delete pTexture;
    pTexture = nullptr;

    int min{ int(m_PowerUpRadius) };
    int maxX{ int(m_Window.width - m_PowerUpRadius) };
    int maxY{ int(m_Window.height - m_PowerUpRadius) };

    for (int idx{ 0 }; idx < number; ++idx)
    {
        Point2f center{ float(rand() % (maxX - min + 1) + min), float(rand() % (maxY - min + 1) + min) };
        PowerUp::Type type = (idx % 2 == 0) ? PowerUp::Type::green : PowerUp::Type::brown;
        m_PowerUpsCenters.push_back(center);
        m_PowerUpTypes.push_back(type);
        m_PowerUpsDestroyed.push_back(false);
        m_pPowerUps.push_back(new PowerUp(center, type));
        ++m_ActNrPowerUps;
    }
}

void Game::DeletePowerUps()
{
    for (auto& powerUp : m_pPowerUps)
    {
        delete powerUp;
    }
    m_pPowerUps.clear();
    m_PowerUpsCenters.clear();
    m_PowerUpTypes.clear();
    m_PowerUpsDestroyed.clear();
    m_ActNrPowerUps = 0;
}

void Game::DrawPowerUps() const
{
    for (size_t idx{ 0 }; idx < m_pPowerUps.size(); ++idx)
    {
        if (m_pPowerUps[idx] != nullptr)
        {
            m_pPowerUps[idx]->Draw();
            if (m_PowerUpTypes[idx] == PowerUp::Type::brown)
            {
                utils::SetColor(Color4f{ 1.0f,0.5f,0.2f,0.5f });
            }
            else
            {
                utils::SetColor(Color4f{ 0.5f,1.0f,0.5f,0.5f });
            }
            utils::FillEllipse(m_PowerUpsCenters[idx], m_PowerUpRadius, m_PowerUpRadius);
        }
    }
}

void Game::UpdatePowerUps(float elapsedSec)
{
    for (auto& powerUp : m_pPowerUps)
    {
        if (powerUp != nullptr)
        {
            powerUp->Update(elapsedSec);
        }
    }
}

void Game::MoveDestroyer(const Point2f& newCenter)
{
    m_Destroyer.left = newCenter.x - m_Destroyer.width / 2;
    m_Destroyer.bottom = newCenter.y - m_Destroyer.height / 2;
}

void Game::VerifyOverlapping()
{
    bool powerUpDeleted{ false };
    for (size_t idx{ 0 }; idx < m_pPowerUps.size(); ++idx)
    {
        if (m_pPowerUps[idx] != nullptr && m_pPowerUps[idx]->IsOverlapping(m_Destroyer))
        {
            if (m_PowerUpTypes[idx] == PowerUp::Type::green)
            {
                ++m_Mistakes;
                std::cout << "Error detected! Total mistakes: " << m_Mistakes << std::endl;
            }

            delete m_pPowerUps[idx];
            m_pPowerUps[idx] = nullptr;
            --m_ActNrPowerUps;
            powerUpDeleted = true;
        }
    }
    if (powerUpDeleted)
    {
        //ShowNrPowerUps();
    }
}

void Game::DrawDestroyer() const
{
    utils::SetColor(Color4f{ 1.0f,0.0f,0.0f,1.0f });
    utils::FillRect(m_Destroyer);
}

void Game::Reset()
{
    std::cout << "Resetting game due to too many mistakes." << std::endl;
    m_CurrentLevel = 1;
    m_Mistakes = 0;
    m_LevelTimer = m_MaxLevelTime; // Resetear el temporizador
    DeletePowerUps();
    CreatePowerUps(m_InitialNrPowerUps);
    m_GameState = GameState::Start;
}

void Game::DeleteGreenPowerUps()
{
    std::vector<size_t> indicesToRemove;
    for (size_t idx{ 0 }; idx < m_pPowerUps.size(); ++idx)
    {
        if (m_PowerUpTypes[idx] == PowerUp::Type::green && m_pPowerUps[idx] != nullptr)
        {
            delete m_pPowerUps[idx];
            m_pPowerUps[idx] = nullptr;
            --m_ActNrPowerUps;
            indicesToRemove.push_back(idx);
        }
    }


    for (auto it = indicesToRemove.rbegin(); it != indicesToRemove.rend(); ++it)
    {
        m_pPowerUps.erase(m_pPowerUps.begin() + *it);
        m_PowerUpsCenters.erase(m_PowerUpsCenters.begin() + *it);
        m_PowerUpTypes.erase(m_PowerUpTypes.begin() + *it);
        m_PowerUpsDestroyed.erase(m_PowerUpsDestroyed.begin() + *it);
    }
}

void Game::RegenerateGreenPowerUps()
{
    int min{ int(m_PowerUpRadius) };
    int maxX{ int(m_Window.width - m_PowerUpRadius) };
    int maxY{ int(m_Window.height - m_PowerUpRadius) };

    int currentPowerUpsSize = static_cast<int>(m_pPowerUps.size());
    int greenCount = (currentPowerUpsSize + m_CurrentLevel) / 2;

    for (int idx{ 0 }; idx < greenCount; ++idx)
    {
        Point2f center{ float(rand() % (maxX - min + 1) + min), float(rand() % (maxY - min + 1) + min) };
        PowerUp::Type type = PowerUp::Type::green;
        m_PowerUpsCenters.push_back(center);
        m_PowerUpTypes.push_back(type);
        m_PowerUpsDestroyed.push_back(false);
        m_pPowerUps.push_back(new PowerUp(center, type));
        ++m_ActNrPowerUps;
    }
}

void Game::LevelUp()
{
    ++m_CurrentLevel;
    std::cout << "Level up! Current level: " << m_CurrentLevel << std::endl;
    m_LevelTimer = m_MaxLevelTime; // Resetear el temporizador al subir de nivel


    DeleteGreenPowerUps();


    int additionalBrowns = m_CurrentLevel;
    int min{ int(m_PowerUpRadius) };
    int maxX{ int(m_Window.width - m_PowerUpRadius) };
    int maxY{ int(m_Window.height - m_PowerUpRadius) };

    for (int i = 0; i < additionalBrowns; ++i)
    {
        Point2f center{ float(rand() % (maxX - min + 1) + min), float(rand() % (maxY - min + 1) + min) };
        PowerUp::Type type = PowerUp::Type::brown;
        m_PowerUpsCenters.push_back(center);
        m_PowerUpTypes.push_back(type);
        m_PowerUpsDestroyed.push_back(false);
        m_pPowerUps.push_back(new PowerUp(center, type));
        ++m_ActNrPowerUps;
    }


    RegenerateGreenPowerUps();
}

