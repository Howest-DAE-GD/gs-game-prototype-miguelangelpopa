#include "pch.h"
#include "PowerUp.h"
#include "Texture.h"
#include "utils.h"

PowerUp::PowerUp(const Point2f& center, PowerUp::Type type)
	:m_Type{ type }, m_pTexture{ new Texture{ "Resources/Images/PowerUp.png" } }, m_Angle{ 0 }
{
	const float width{ m_pTexture->GetWidth() }, height{ m_pTexture->GetHeight() };
	m_Shape = Circlef{ center, width / 2.f };
	m_TextClip = (m_Type == Type::brown ? Rectf{ 0.f,height, width, height / 2.f } : Rectf{ 0.f,height / 2.f, width, height / 2.f });
}

PowerUp::~PowerUp()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void PowerUp::Update(float elapsedSec)
{
	m_Angle += m_RotSpeed * elapsedSec;
}

void PowerUp::Draw() const
{
	glPushMatrix();

	glTranslatef(m_Shape.center.x, m_Shape.center.y, 0.f);
	glRotatef(m_Angle, 0.f, 0.f, 1.f);

	glTranslatef(-m_Shape.radius, -m_Shape.radius, 0.f);

	m_pTexture->Draw(Point2f{ 0.f,0.f }, m_TextClip);
	glPopMatrix();
}

bool PowerUp::IsOverlapping(const Rectf& rect) const
{
	return utils::IsOverlapping(rect, m_Shape);
}
