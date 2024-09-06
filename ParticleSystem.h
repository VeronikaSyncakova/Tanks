#pragma once
#include <SFML/Graphics.hpp>

class Particle
{
public:
	Particle() {}
	void init(sf::Vector2f t_pos, sf::Vector2f t_velocity)
	{
		position = t_pos;
		velocity = t_velocity;
		body.setSize(size);
		body.setPosition(position);
		body.setFillColor(sf::Color::Red);
		timeAlive = rand() % 100;
	}

	bool update()
	{
		if (timeAlive > 0)
		{
			timeAlive -= 1;
			body.move(velocity);
			return true;
		}
		return false;
	}
	void draw(sf::RenderWindow& t_window)
	{
		if (timeAlive > 0)
		{
			t_window.draw(body);
		}
	}

private:
	sf::RectangleShape body;
	sf::Vector2f size{ 5.f,5.f };
	sf::Vector2f position;
	sf::Vector2f velocity;
	float timeAlive;
};

class ParticleSystem
{

public:
	ParticleSystem() {}
	void init(sf::Vector2f t_pos);
	void update();
	void draw(sf::RenderWindow& t_window);

private:
	static const int MAX_PARTICLES{ 50 };
	std::vector<Particle> m_particles;
	sf::Vector2f m_position;
};