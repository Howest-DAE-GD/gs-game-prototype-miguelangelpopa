#include "pch.h"
#include "Platform.h"
#include "utils.h"

Platform::Platform(const Point2f& beginPoint, const float width)
    : m_Shape{ beginPoint.x, beginPoint.y, width, 0 }
{
}

void Platform::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
    if (actorVelocity.y <= 0)
    {
        std::vector<Point2f> vertices{};
        vertices.push_back(Point2f{ m_Shape.left, m_Shape.bottom });
        vertices.push_back(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom });

        // Modify ray start and end points to align with player's position and dimensions
        Point2f playerStartPoint{ actorShape.left + actorShape.width / 2.f,
                               actorShape.bottom + actorShape.height }; // Adjusted to player's bottom center
        Point2f playerEndPoint{ actorShape.left + actorShape.width / 2.f,
                             actorShape.bottom }; // Adjusted to player's top center

        utils::HitInfo hitInfo{};

        if (utils::Raycast(vertices, playerStartPoint, playerEndPoint, hitInfo))
        {
            actorShape.bottom = hitInfo.intersectPoint.y;
            actorVelocity.y = 0.f;
        }
    }
}

bool Platform::IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity) const
{
    if (actorVelocity.y <= 0)
    {
        std::vector<Point2f> vertices{};
        vertices.push_back(Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height });
        vertices.push_back(Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height });

        // Modify ray start and end points to align with player's position and dimensions
        Point2f playerStartPoint{ actorShape.left + actorShape.width / 2.f,
                               actorShape.bottom + actorShape.height }; // Adjusted to player's bottom center
        Point2f playerEndPoint{ actorShape.left + actorShape.width / 2.f,
                             actorShape.bottom }; // Adjusted to player's top center

        utils::HitInfo hitInfo{};

        if (utils::Raycast(vertices, playerStartPoint, playerEndPoint, hitInfo))
        {
            return true;
        }
    }

    return false;
}
