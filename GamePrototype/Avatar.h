#pragma once
#include <vector>
#include "Vector2f.h"


class Level;
class SoundEffect;

class Avatar final
{
public:
	explicit Avatar();

	~Avatar();
	Avatar(const Avatar& avatar) = delete;
	Avatar& operator=(const Avatar& rhs) = delete;
	Avatar(Avatar&& avatar) = delete;
	Avatar& operator=(Avatar&& rhs) = delete;

	void Update(float elapsedSec, const Level& level);
	void Draw() const;
	void Hit();
	void Restart();
	Rectf GetShape() const;


private:
	enum class ActionState
	{
		idle,
		walking,
		sliding,
		jumping,
		shoot,
		hit,
		dead
	};

	ActionState m_ActionState;
	Rectf m_Shape;
	Vector2f m_Velocity;
	const float m_HorizontalSpeed;
	const Vector2f m_Acceleration;
	SoundEffect* m_LostLifeSound;
	SoundEffect* m_ShootSound;

	void UpdateXPos(float elapsedSec);
	void UpdateYPos(float elapsedSec);
	void StayInLevelBoundaries(const Level& level);
	void HandleInput(const Level& level);
	void DrawAvatar() const;
};