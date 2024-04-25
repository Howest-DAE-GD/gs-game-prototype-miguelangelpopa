#include "pch.h"
#include "Avatar.h"
#include "Level.h"
#include "utils.h"
#include "SoundEffect.h"

Avatar::Avatar()
	: m_ActionState{ ActionState::idle }
	, m_Shape{ }
	, m_Velocity{ 0.f, 0.f }
	, m_HorizontalSpeed{ 150.f }
	, m_Acceleration{ 0.f, -981.f }
	, m_LostLifeSound{ new SoundEffect{"Resources/Sound/LoseLife.mp3"} }
	, m_ShootSound{ new SoundEffect{"Resources/Sound/Blaster.mp3"} }
{
	m_Shape.left = 7500.f;


	m_LostLifeSound->SetVolume(20);
	m_ShootSound->SetVolume(10);


}

Avatar::~Avatar()
{
	delete m_LostLifeSound;
	delete m_ShootSound;
}

void Avatar::Update(float elapsedSec, const Level& level)
{

}


void Avatar::UpdateXPos(float elapsedSec)
{
	m_Shape.left += elapsedSec * m_Velocity.x;
}

void Avatar::UpdateYPos(float elapsedSec)
{
	m_Shape.bottom += elapsedSec * m_Velocity.y;
	m_Velocity.y += m_Acceleration.y * elapsedSec;
}
