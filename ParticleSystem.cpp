#include "ParticleSystem.h"


void ParticleSystem::update()
{
	if (m_particles.size() > 0)
	{
		for (int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i).update();
		}
	}
}

void ParticleSystem::init(sf::Vector2f t_pos)
{
	m_position = t_pos;
	sf::Vector2f velocity;
	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		velocity.x = rand() / double(RAND_MAX) * 4 - 2;
		velocity.y = rand() / double(RAND_MAX) * 4 - 2;
		Particle particle;
		particle.init(m_position, velocity);
		m_particles.push_back(particle);
	}
}

void ParticleSystem::draw(sf::RenderWindow& t_window)
{
	if (m_particles.size() > 0)
	{
		for (int i = 0; i < m_particles.size(); i++)
		{
			m_particles.at(i).draw(t_window);
		}
	}
}