#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include"ScreenSize.h"

class Projectile
{
public:
	Projectile(float t_radius = 5.f);
	void render(sf::RenderWindow& t_window);
	void initSprite();
	void setTexture(sf::Texture& t_texture);

	/// <summary>
	/// moves the projectiles
	/// </summary>
	/// <param name="t_dt">delta time</param>
	void move(double& t_dt);

	/// <summary>
	/// calculates vector for the projectile movement
	/// </summary>
	/// /// <param name="t_startPosition">turret origin</param>
	/// <param name="t_endPosition">turret end</param>
	void normaliseMovementVector(sf::Vector2f& t_startPosition, sf::Vector2f& t_endPosition); 
	/// <summary>
	/// sets the projectile to the end of the turret
	/// </summary>
	/// <param name="t_startPosition">turret origin</param>
	/// <param name="t_endPosition">turret end</param>
	void fire(sf::Vector2f& t_startPosition, sf::Vector2f& t_endPosition);

	/// <summary>
	/// check if the projectile can be used again
	/// </summary>
	/// <returns>is the projectile is off screen or not</returns>
	bool isOffScreen();

	/// <summary>
	/// collision with the walls
	/// if so, it moves the projectile offscreen
	/// </summary>
	/// <param name="t_wallSprites">wall sprite reference</param>
	void wallCollision(std::vector<sf::Sprite>& t_wallSprites);

private:
	sf::CircleShape m_body;
	sf::Sprite m_sprite;
	float m_radius; //body radius
	double m_speed{ 1000.0 }; //movement speed
	sf::Vector2f m_offScreen{ScreenSize::s_width, ScreenSize::s_height};
	sf::Vector2f m_position{m_offScreen};
	sf::Vector2f m_direction; //movement direction vector
};
