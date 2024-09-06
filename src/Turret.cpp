#include "Turret.h"

Turret::Turret(sf::Texture& t_texture,TargetData& t_data, std::vector<sf::Sprite>& t_walls, sf::Sprite& t_tank, std::function<void(int)> t_damageFunc)
	:Target(t_texture, t_data), m_wallSprites(t_walls), m_tankSpriteRef(t_tank),m_damageFunction(t_damageFunc)
{
	m_rotation = t_data.m_rotation;
	m_targetSprite.setTextureRect(sf::IntRect(0, 251, 212, 97));
	//m_targetSprite.setScale(0.5f,0.5f);
	m_targetSprite.setOrigin(50.f, 97 / 2.f);
	m_targetSprite.setRotation(m_rotation);
	m_targetSprite.setPosition(m_position);

	for (sf::Sprite const wallSprite : m_wallSprites)
	{
		sf::CircleShape circle(wallSprite.getTextureRect().width*0.5f );
		circle.setOrigin(circle.getRadius(), circle.getRadius());
		circle.setPosition(wallSprite.getPosition());
		circle.setFillColor(sf::Color(255, 255, 255, 100));
		m_wallObstacles.push_back(circle);
	}
}

void Turret::update(double& dt, sf::Vector2f& t_tankPosition)
{
	if (m_appearTime > 0.f )
	{
		m_appearTime -= dt;
	}
	else if(!m_activated && m_health==1)
	{
		m_activated = true;
		m_fireTimer.restart();
	}
	if (m_activated)
	{
		if (m_fireTimer.getElapsedTime().asMilliseconds() >= m_timeFire)
		{
			//calculate rotation towards the tank
			sf::Vector2f vectorToTank = sf::Vector2f((t_tankPosition.x - m_position.x), (t_tankPosition.y - m_position.y)); //distance to the tank as a vector
			float rotationToTank = atan2(vectorToTank.y, vectorToTank.x) / std::numbers::pi * 180; //rotation angle towards the tank
			m_maxSeeAhead = std::sqrt(vectorToTank.x * vectorToTank.x + vectorToTank.y * vectorToTank.y); //only see up to the player
			calculateAheadVector();
			//find shorter way (known bug: there is a line where it crosses the 0 and then it is turning the wrong way)
			if (static_cast<int>(rotationToTank - m_rotation) > 0.f)
			{
				m_rotation++;
			}
			else if (static_cast<int>(rotationToTank - m_rotation) < 0.f)
			{
				m_rotation--;
			}
			else if (static_cast<int>(rotationToTank - m_rotation) == 0.f && noObstacle()) //turret is rotated towards the player tank
			{
				//fire the projectile
				m_fireTimer.restart();
				m_rotated = false;
				m_pool.create(*m_targetSprite.getTexture(), m_position.x, m_position.y, m_rotation);
				//decrease the time before next seeking
				if (m_timeFire > 500.f)
					m_timeFire = m_timeFire / 1.1f;
			}
			m_targetSprite.setRotation(m_rotation);
		}
		m_pool.update(dt, m_wallSprites, m_tankSpriteRef, m_damageFunction);
		m_pool.updateParticles();
	}
}

void Turret::render(sf::RenderWindow& t_window)
{
	if (m_activated)
	{
		m_pool.render(t_window);
		t_window.draw(m_targetSprite);
		for (sf::CircleShape& obstacle : m_wallObstacles)
		{
			t_window.draw(obstacle);
		}
		t_window.draw(m_aheadVector);
	}
}

void Turret::init()
{
	m_activated = true;
	m_appearTime = 0.f;
	m_fireTimer.restart();
	m_targetSprite.setScale(0.5f, 0.5f);
	m_timeFire = 3000.f;
	m_rotated = false;
	m_health = 1.f;
}

void Turret::calculateAheadVector()
{
	auto headingRadians = thor::toRadian(m_rotation);
	sf::Vector2f headingVector(std::cos(headingRadians) * m_maxSeeAhead, std::sin(headingRadians) * m_maxSeeAhead); //size of the vector
	m_ahead = m_targetSprite.getPosition() + headingVector; //position the vector with the rotation
	m_halfAhead = m_targetSprite.getPosition() + (headingVector * 0.5f); //position the vector with the rotation
	//debug visualisation of ahead vectors
	m_aheadVector.setPosition(m_targetSprite.getPosition());
	m_aheadVector.setSize({ thor::length(headingVector), 1.0f });
	m_aheadVector.setFillColor(sf::Color::Red);
	m_aheadVector.setRotation(m_rotation);
}

bool Turret::noObstacle()
{
	sf::Vector2f stepVector(0.f, 0.f); //steps until collision detected 
	sf::Vector2f stepVectorHalf(0.f, 0.f);
	int step = 10; //step amount
	if (m_targetSprite.getPosition().x < m_ahead.x)
	{
		while (stepVector.x < m_ahead.x) //goes through the ahead vector length
		{
			auto headingRadians = thor::toRadian(m_rotation);
			sf::Vector2f headingVector(std::cos(headingRadians) * step, std::sin(headingRadians) * step); //size of the vector
			stepVector = m_targetSprite.getPosition() + headingVector; //position the vector with the rotation
			stepVectorHalf = m_targetSprite.getPosition() + (headingVector * 0.5f); //position the vector with the rotation
			for (sf::CircleShape& obstacle : m_wallObstacles)
			{
				if (MathUtility::lineIntersectsCircle(stepVector, stepVectorHalf, obstacle))
				{
					return false;
				}
			}
			step += 10;
		}
	}
	else if (m_targetSprite.getPosition().x > m_ahead.x)
	{
		stepVector = m_targetSprite.getPosition();
		while (stepVector.x > m_ahead.x) //goes through the ahead vector length
		{
			auto headingRadians = thor::toRadian(m_rotation);
			sf::Vector2f headingVector(std::cos(headingRadians) * step, std::sin(headingRadians) * step); //size of the vector
			stepVector = m_targetSprite.getPosition() + headingVector; //position the vector with the rotation
			stepVectorHalf = m_targetSprite.getPosition() + (headingVector * 0.5f); //position the vector with the rotation
			for (sf::CircleShape& obstacle : m_wallObstacles)
			{
				if (MathUtility::lineIntersectsCircle(stepVector, stepVectorHalf, obstacle))
				{
					return false;
				}
			}
			step += 10;
		}
	}
	return true;
}



