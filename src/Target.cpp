#include "Target.h"

Target::Target(sf::Texture& t_texture, TargetData& t_data)
{
	m_targetSprite.setTexture(t_texture);
	float x = (rand() % static_cast<int>(2 * t_data.m_randomOffset)) - t_data.m_randomOffset;//random offset x between -randomoffset and +randomoffset
	float y = (rand() % static_cast<int>(2 * t_data.m_randomOffset)) - t_data.m_randomOffset;//random offset y between +/-randomOffset
	m_position = sf::Vector2f(t_data.m_x+x, t_data.m_y+y);
	m_appearTime = static_cast<float>(t_data.m_appearTime);
	m_targetSprite.setScale(0.f, 0.f);
}

sf::Sprite& Target::getTargetSprite()
{
	return m_targetSprite;
}

void Target::applyDamage1(int t_damage)
{
	m_health -= t_damage;
	m_targetSprite.setScale(0.f, 0.f);
	m_activated = false;
}

bool Target::isActivated()
{
	return m_activated;
}

bool Target::isAlive()
{
	if (m_health > 0)
		return true;
	return false;
}
