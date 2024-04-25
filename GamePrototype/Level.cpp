#include "pch.h"
#include "Level.h"
#include "Texture.h"
#include "Platform.h"
#include "SVGParser.h"
#include "utils.h"

Level::Level()
	:m_LevelTexture{ new Texture{"Resources/Level.png" } }

	, m_BackgroundTexture{ new Texture{"Resources/Background.png"} }
	, m_Vertices{ }
	, m_Boundaries{ }
{
	SVGParser::GetVerticesFromSvgFile("Resources/Level.svg", m_Vertices);

	m_Boundaries.width = m_LevelTexture->GetWidth();
	m_Boundaries.height = 500.f;

	InitializePlatforms();
}

Level::~Level()
{
	delete m_LevelTexture;
	delete m_BackgroundTexture;

	for (Platform* platform : m_Platforms)
	{
		delete platform;
	}
}

void Level::DrawForeground() const
{
	glPushMatrix();


	glPopMatrix();
}

void Level::DrawBackground(const Point2f& translation) const
{
	glPushMatrix();
	glTranslatef(translation.x, translation.y, 0);
	m_BackgroundTexture->Draw();
	glPopMatrix();

	m_LevelTexture->Draw();
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	for (Platform* platform : m_Platforms)
	{
		platform->HandleCollision(actorShape, actorVelocity);
	}

	Point2f rayStartPoint{ actorShape.left + actorShape.width / 2.f,
						   actorShape.bottom + actorShape.height };
	Point2f rayEndPoint{ actorShape.left + actorShape.width / 2.f,
						 actorShape.bottom };

	utils::HitInfo hitInfo{};

	if (utils::Raycast(m_Vertices.back(), rayStartPoint, rayEndPoint, hitInfo))
	{
		actorShape.bottom = hitInfo.intersectPoint.y;
		actorVelocity.y = 0.f;
	}
}

bool Level::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
	for (Platform* platform : m_Platforms)
	{
		if (platform->IsOnGround(actorShape, actorVelocity))
		{
			return true;
		}
	}

	Point2f playerStartPoint{ actorShape.left + actorShape.width / 2.f,
						   actorShape.bottom + actorShape.height };
	Point2f playerEndPoint{ actorShape.left + actorShape.width / 2.f,
						 actorShape.bottom - 1.f };

	utils::HitInfo hitInfo{};

	if (utils::Raycast(m_Vertices.back(), playerStartPoint, playerEndPoint, hitInfo))
	{
		return true;
	}

	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void InitializePlatforms()
{

}