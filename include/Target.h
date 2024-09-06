#pragma once 

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include <functional>


class Target
{
public:
	Target(sf::Texture& t_texture, TargetData& t_data);
	virtual void update(double& dt, sf::Vector2f& t_tankPosition) = 0;
	virtual void render(sf::RenderWindow& t_window) = 0;
	virtual sf::Sprite& getTargetSprite();
	virtual void applyDamage1(int t_damage);
	virtual void init() = 0;
	virtual bool isActivated();
	virtual bool isAlive();

protected:
	sf::Sprite m_targetSprite;
	sf::Vector2f m_position;
	float m_appearTime;
	bool m_activated{ false };
	int m_health{ 1 };
};