#include "Tank.h"

Tank::Tank(thor::ResourceHolder<sf::Texture, std::string> & t_holder, std::vector<sf::Sprite>& t_wallSprites)
: m_holder(t_holder), m_wallSprites(t_wallSprites)
{
	initSprites();
}

void Tank::update(double dt, HUD& t_hud)
{	
	if (checkWallCollision())
	{
		m_state = TankState::COLLIDING;
	}
	else
	{
		m_state = TankState::NORMAL;
	}
	switch (m_state)
	{
	case TankState::NORMAL:
		handleKeyInput();
		tankMovement(dt);
		break;
	case TankState::COLLIDING:
		deflect(dt);
		break;
	default:
		break;
	}

	/*
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles.at(i).move(dt);
		m_projectiles.at(i).wallCollision(m_wallSprites);
	}
	*/

	//decrement cooldown for shooting
	if (m_fireRequested)
	{
		m_shootTimer -= dt;
		if (m_shootTimer <= 0)
		{
			m_shootTimer = s_TIME_BETWEEN_SHOTS;
			m_fireRequested = false;
			t_hud.increaseShots();
		}
	}
	
	if (!m_pool.update(dt, m_wallSprites, m_targetSprites, m_applyDamageToTarget))//true if projectile collided with target
	{
		t_hud.increaseScore();
	}
	m_pool.updateParticles();
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
	m_pool.render(window);
	/*
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles.at(i).render(window);
	}
	*/
}

void Tank::setPosition(sf::Vector2f t_position)
{
	m_tankBase.setPosition(t_position);
	m_turret.setPosition(t_position);
}

sf::Vector2f Tank::getPosition()
{
	return m_tankBase.getPosition();
}


void Tank::setScale(sf::Vector2f t_scale)
{
	m_tankBase.setScale(t_scale);
	m_turret.setScale(t_scale);
}

sf::Sprite& Tank::getSprite()
{
	return m_tankBase;
}

void Tank::increaseSpeed()
{
	m_speed += 1;
}

void Tank::decreaseSpeed()
{
	m_speed -= 1;
}

void Tank::increaseRotation()
{
	m_rotation += 1.0+2*(5-m_health); 
	if (m_rotation >= 360.0) 
	{ 
		m_rotation = 0; 
	}
}


void Tank::decreaseRotation()
{
	m_rotation -= 1.0+2*(5 - m_health);
	if (m_rotation <= 0.0) 
	{ 
		m_rotation = 359.0; 
	}
}

void Tank::increaseTurretRotation()
{
	m_turretRotation += 1;
	if (m_turretRotation >= 360.0)
	{
		m_turretRotation = 0;
	}
}

void Tank::decreaseTurretRotation()
{
	m_turretRotation -= 1;
	if (m_turretRotation <= 0.0)
	{
		m_turretRotation = 359.0;
	}
}

void Tank::centreTurret()
{
	float rotationTurret = m_turretRotation+m_rotation;
	if (rotationTurret < m_rotation)
	{
		m_turretRotation--;
	}
	else if(rotationTurret > m_rotation )
	{
		m_turretRotation++;
	}

	if (abs(rotationTurret - (m_rotation)) < 180) //decides the shortest direstion
	{
		m_turretRotation-=2; //2 because i am adding one so if it is just 1 it will stand in place
	}
	//sets the flag to false when rotate to the place
	if (m_turretRotation <= 0 || m_turretRotation >= 360)
	{
		m_turretRotation = 1;
		m_centering = false;
	}
}

bool Tank::turretCentering()
{
	return m_centering;
}

void Tank::handleKeyInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		increaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		decreaseSpeed();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		decreaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		increaseRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		decreaseTurretRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
	{
		increaseTurretRotation();
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
	{
		m_centering = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		/*//shooting timer
		if (m_fireTimer.getElapsedTime().asSeconds() > 0.5f)
		{
			m_fireTimer.restart();
			fireProjectile();
		}
		*/
		requestFire();
	}
}

bool Tank::checkWallCollision()
{
	//the collision of turret is less accuret because there is more empty space around turret, we are checking the bounding box of the sprite 
	//and not just the pixels
	//to fix that we need pixel perfect collision check
	for (sf::Sprite const& sprite : m_wallSprites)
	{
		// Checks if either the tank base or turret has collided
		// with the current wall sprite.
		if (CollisionDetector::collision(m_turret, sprite))
		{
			//pixel perfect collision check (checks each pixel)
			if (CollisionDetector::pixelPerfectTest(m_turret, sprite))
			{
				// Get contact normal vector between the turret and the wall
				m_contactNormal = m_turret.getPosition() - sprite.getPosition();
				return true;
			}
		}
		else if (CollisionDetector::collision(m_tankBase, sprite))
		{
			//pixel perfect collision check (checks each pixel)
			if (CollisionDetector::pixelPerfectTest(m_tankBase, sprite))
			{
				// Get contact normal vector between the tankBase and the wall
				m_contactNormal = m_tankBase.getPosition() - sprite.getPosition();
				return true;
			}
		}
	}
	return false;

	//observations about pixel perfect collision, more accurate and i see why
	//are checking it only when previous collision was detected, since it checks every pixel, it takes longer
	//there is issue when your speed is less than the amount of pixels that you are ovelapping, then you get stuck because eventually your speed will be 0
	//and there will be no bounce back
	//i fixed that issue
}

void Tank::deflect(double dt)
{
	// Calculate impulse based on speed
	float impulseMagnitude = std::abs(m_speed);
	//this is to prevent getting stuck if your speed is 0 and you are still stuck
	/*
	if (impulseMagnitude < 1.f)
	{
		impulseMagnitude = 1;
	}
	*/
	// Normalise the contact vector
	m_contactNormal = thor::unitVector(m_contactNormal);
	// Calculate deflection amount
	sf::Vector2f deflectVector =
	{
	impulseMagnitude * m_contactNormal.x * (static_cast<float>(dt) / 1000),
	impulseMagnitude * m_contactNormal.y * (static_cast<float>(dt) / 1000)
	};
	// Apply impulse along the contact normal
	m_tankBase.move(deflectVector.x, deflectVector.y);
	m_turret.move(deflectVector.x, deflectVector.y);
	
}

void Tank::tankMovement(double dt)
{
		m_speed = std::clamp(m_speed, MAX_REVERSE_SPEED, MAX_FORWARD_SPEED);
		float currentX = m_tankBase.getPosition().x;
		float currentY = m_tankBase.getPosition().y;
		float x = currentX + cos(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);
		float y = currentY + sin(m_rotation * MathUtility::DEG_TO_RAD) * m_speed * (dt / 1000);
		m_speed *= 0.99f; //friction
		m_tankBase.setRotation(m_rotation);
		m_tankBase.setPosition(x, y);
		m_turret.setRotation(m_rotation);
		m_turret.rotate(m_turretRotation);
		m_turret.setPosition(x, y);
}

/*
void Tank::fireProjectile()
{
	sf::Vector2f startPos = m_turret.getPosition(); //origin position
	double rotation = m_rotation + m_turretRotation; //final rotation
	float originToEnd = 149.5f*m_turret.getScale().x; //lenght to the turret end from origin
	float x = originToEnd * cos(rotation * MathUtility::DEG_TO_RAD) + startPos.x;
	float y = originToEnd * sin(rotation * MathUtility::DEG_TO_RAD) + startPos.y;
	sf::Vector2f endPos={x, y}; //position of the end of the turret with rotation
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles.at(i).isOffScreen())
		{
			m_projectiles[i].fire(startPos, endPos);
			break;
		}
	}
}
*/

void Tank::requestFire()
{
	//m_fireRequested = true;
	if (m_shootTimer == s_TIME_BETWEEN_SHOTS)
	{
		m_fireRequested = true;
		sf::Vector2f tipOfTurret(m_turret.getPosition().x, m_turret.getPosition().y);
		tipOfTurret.x += std::cos(MathUtility::DEG_TO_RAD * m_turret.getRotation()) *
			(m_turret.getLocalBounds().top + m_turret.getLocalBounds().height);
		tipOfTurret.y += std::sin(MathUtility::DEG_TO_RAD * m_turret.getRotation()) *
			(m_turret.getLocalBounds().top + m_turret.getLocalBounds().height);
		m_pool.create(m_holder["tankAtlas"], tipOfTurret.x, tipOfTurret.y, m_turret.getRotation());
	}
}

void Tank::applyDamage(int t_damage)
{
	if (m_health > 0)
	{
		m_health -= t_damage;
		m_turretRotation += 45;
	}
}

void Tank::addTarget(Target& t_target)
{
	m_targets.push_back(&t_target);
	std::function<void(int)> func = std::bind(&Target::applyDamage1, &t_target, std::placeholders::_1);
	m_applyDamageToTarget.push_back(func);
	m_targetSprites.push_back(&t_target.getTargetSprite());
}

bool Tank::isAlive()
{
	if (m_health <= 0)
	{
		return false;
	}
	return true;
}

void Tank::initSprites()
{
	m_holder.acquire("tankAtlas", thor::Resources::fromFile<sf::Texture>("resources/images/spritesheet.png"));
	m_holder.acquire("projectile", thor::Resources::fromFile<sf::Texture>("resources/images/Projectile.png"));
	sf::Texture& texture = m_holder["tankAtlas"];
	sf::Texture& projectileTexture = m_holder["projectile"];
	m_tankBase.setTextureRect(sf::IntRect(0, 115, 246, 114));
	m_tankBase.setOrigin(89.0f, 57.0f); //origin in the black hole
	m_tankBase.setTexture(texture);
	m_turret.setTexture(texture);
	m_turret.setTextureRect(sf::IntRect(247, 0, 191, 94));
	m_turret.setOrigin(41.5f, 47.0f);
	/*
	for (int i = 0; i < NUM_PROJECTILES; i++)
	{
		Projectile projectile;
		m_projectiles.push_back(projectile);
		m_projectiles.at(i).setTexture(projectileTexture);
	}
	*/
}