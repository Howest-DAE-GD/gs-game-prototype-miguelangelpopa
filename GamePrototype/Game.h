#pragma once
#include "BaseGame.h"
#include "SoundEffect.h"

class PowerUp;
class Game : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec) override;
	void Draw() const override;

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	// DATA MEMBERS
	const Window m_Window;

	Point2f m_MousePos;
	static const int m_NrPowerUps{ 10 };
	PowerUp* m_pPowerUps[m_NrPowerUps];
	Point2f m_PowerUpsCenters[m_NrPowerUps];
	PowerUp::Type m_PowerUpTypes[m_NrPowerUps];
	bool m_PowerUpsDestroyed[m_NrPowerUps];
	float m_PowerUpRadius;
	int m_ActNrPowerUps;
	Rectf m_Destroyer;
	SoundEffect* m_KillMusic;

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	void ShowTestMessage() const;
	void ShowNrPowerUps() const;
	void CreatePowerUps();
	void UpdatePowerUps(float elapsedSec);
	void DrawPowerUps() const;
	void DeletePowerUps();

	void MoveDestroyer(const Point2f& newCenter);
	void DrawDestroyer() const;
	void VerifyOverlapping();

};
