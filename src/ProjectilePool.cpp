#include "ProjectilePool.h"

////////////////////////////////////////////////////////////
void ProjectilePool::create(sf::Texture const & t_texture, double t_x, double t_y, double t_rotation)
{
	// If no projectiles available, simply re-use the next in sequence.
	if (m_poolFull)
	{
		m_nextAvailable = (m_nextAvailable + 1) % s_POOL_SIZE;
	}
	
	m_projectiles.at(m_nextAvailable).init(t_texture, t_x, t_y, t_rotation);
}

////////////////////////////////////////////////////////////t_
void ProjectilePool::update(double t_dt, std::vector<sf::Sprite> & t_wallSprites, sf::Sprite& t_tank, std::function<void(int)> t_damageFunc)
{	
	// The number of active projectiles.
	int activeCount = 0;
	// Assume the pool is not full initially.
	m_poolFull = false;
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		// If m_projectiles.at(i).update() returns true, then this projectile is onscreen
		// Note how the condition is negated (i.e. checking if projectile is not in use)
		if( !m_projectiles.at(i).update(t_dt, t_wallSprites, t_tank, t_damageFunc))
		{
			// If this projectile has expired, make it the next available.
			m_nextAvailable = i;
		}
		else
		{
			// So we know how many projectiles are active.
			activeCount++;
		}
	}
	// If no projectiles available, set a flag.
	if (s_POOL_SIZE == activeCount)
	{		
		m_poolFull = true;
	}
}

bool ProjectilePool::update(double t_dt, std::vector<sf::Sprite>& t_wallSprites, std::vector<sf::Sprite*> t_targets, std::vector<std::function<void(int)>> t_damageFunc)
{
	// The number of active projectiles.
	int activeCount = 0;
	// Assume the pool is not full initially.
	m_poolFull = false;
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		// If m_projectiles.at(i).update() returns true, then this projectile is onscreen
		// Note how the condition is negated (i.e. checking if projectile is not in use)
		if (!m_projectiles.at(i).update(t_dt, t_wallSprites, t_targets, t_damageFunc))
		{
			// If this projectile has expired, make it the next available.
			m_nextAvailable = i;
			if (m_projectiles.at(i).collidedWithTarget())
			{
				return false;
			}
		}
		else
		{
			// So we know how many projectiles are active.
			activeCount++;
		}
	}
	// If no projectiles available, set a flag.
	if (s_POOL_SIZE == activeCount)
	{
		m_poolFull = true;
	}
	return true;
}

////////////////////////////////////////////////////////////
void ProjectilePool::render(sf::RenderWindow & t_window)
{
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		// If projectile is active...
		if (m_projectiles.at(i).inUse())
		{
			t_window.draw(m_projectiles.at(i).m_projectile);
		}
		m_projectiles.at(i).draw(t_window);
	}
}

void ProjectilePool::updateParticles()
{
	for (int i = 0; i < s_POOL_SIZE; i++)
	{
		m_projectiles.at(i).m_particles.update();
	}
}