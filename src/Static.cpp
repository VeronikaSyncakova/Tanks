#include "Static.h"

Static::Static(sf::Texture& t_texture,TargetData& t_data):Target(t_texture,t_data)
{
	m_duration = t_data.m_duration;
	m_screenDuration = m_duration;
	m_scale = 0.5f;
	m_targetSprite.setTextureRect(sf::IntRect(0, 0, 244, 114));
	m_targetSprite.setScale(m_scale, m_scale);
	m_targetSprite.setOrigin(244 / 2.f, 114 / 2.f);
	m_targetSprite.setPosition(m_position);
}

void Static::update(double& dt, sf::Vector2f& t_tankPosition)
{
	if (m_appearTime > 0.f)
	{
		m_appearTime -= dt;
	}
	else if (!m_activated)
	{
		m_activated = true;
		m_timer.restart();
	}
	if (m_activated)
	{
		if (m_timer.getElapsedTime().asSeconds() >= 1 && m_duration >= 0)
		{
			m_timer.restart();
			m_duration--;
		}
		if (m_duration < 0 && m_scale>0.f)
		{//scale down
			m_scale -= 0.01f;
			m_targetSprite.setScale(m_scale, m_scale);
		}
		else if (m_scale <= 0.f)
		{
			m_activated = false;
		}
	}
}

void Static::render(sf::RenderWindow& t_window)
{
	if (m_activated)
	{
		if (m_scale > 0.f)
			t_window.draw(m_targetSprite);
	}
}

void Static::init()
{
	m_activated = true;
	m_appearTime = 0.f;
	m_scale = 0.5f; 
	m_targetSprite.setScale(m_scale, m_scale);
	m_health = 1.f;
	m_duration = m_screenDuration;
}
