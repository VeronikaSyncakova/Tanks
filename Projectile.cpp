#include "Projectile.h"

Projectile::Projectile(float t_radius):m_radius(t_radius)
{
	initSprite();
}

void Projectile::render(sf::RenderWindow& t_window)
{
	//t_window.draw(m_body);
	t_window.draw(m_sprite);

}

void Projectile::initSprite()
{
	m_body.setRadius(m_radius);
	m_body.setOrigin(m_radius,m_radius),
	m_body.setPosition(m_position);
}

void Projectile::setTexture(sf::Texture& t_texture)
{
	m_sprite.setTexture(t_texture);
	m_sprite.setOrigin(m_radius, m_radius);
	m_sprite.setPosition(m_position);
}

void Projectile::normaliseMovementVector(sf::Vector2f& t_startPosition, sf::Vector2f& t_endPosition)
{
	float magnitude = std::sqrt(
		(t_startPosition.x - t_endPosition.x) * (t_startPosition.x - t_endPosition.x) +
		(t_startPosition.y - t_endPosition.y) * (t_startPosition.y - t_endPosition.y)
	);

	m_direction = sf::Vector2f(
		(t_endPosition.x - t_startPosition.x) / magnitude,
		(t_endPosition.y - t_startPosition.y) / magnitude
	);
}

void Projectile::fire(sf::Vector2f& t_startPosition, sf::Vector2f& t_endPosition)
{
	normaliseMovementVector(t_startPosition, t_endPosition);
	//sets projectile at the end of the turret
	m_position= t_endPosition;
	m_body.setPosition(m_position);
	m_sprite.setPosition(m_position);
}

bool Projectile::isOffScreen()
{
	if (m_position.x >= m_offScreen.x || m_position.x <= 0.f ||
		m_position.y >= m_offScreen.y || m_position.y <= 0.f)
		return true;
	return false;
}

void Projectile::wallCollision(std::vector<sf::Sprite>& t_wallSprites)
{
	for (sf::Sprite const& sprite : t_wallSprites)
	{
		// Checks if the bullet collided with wall
		if (CollisionDetector::collision(m_sprite, sprite))
		{
			//pixel perfect collision check (checks each pixel)
			if (CollisionDetector::pixelPerfectTest(m_sprite, sprite))
			{
				m_position = m_offScreen;
				m_sprite.setPosition(m_offScreen);
				m_direction = sf::Vector2f(0.f, 0.f);
			}
		}
	}
}


void Projectile::move(double& t_dt)
{
	m_position.x += m_direction.x * m_speed*(t_dt/1000);
	m_position.y += m_direction.y * m_speed*(t_dt/1000);
	//m_body.setPosition(m_position);
	m_sprite.setPosition(m_position);
}
