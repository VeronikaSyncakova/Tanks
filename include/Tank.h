#pragma once
#include <SFML/Graphics.hpp>
#include <Thor/Resources.hpp>
#include "MathUtility.h"
#include "CollisionDetector.h"
#include "ProjectilePool.h"
#include <vector>
#include "Target.h"
#include "HUD.h"

//enum referencing the state of a tank
enum class TankState { NORMAL, COLLIDING };

/// <summary>
/// @brief A simple tank controller.
/// 
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	/// <summary>
	/// @brief Constructor that stores drawable state (texture, sprite) for the tank
	/// Stores references to the resource holder and container of wall sprites.
	/// Creates sprites for the tank base and turret from the texture in the resource holder
	/// </summary>
	/// <param name="t_holder">A reference to the resource holder</param>
	/// < param name="t_wallSprites">A reference to the container of wall sprites </param>
	Tank(thor::ResourceHolder <sf::Texture, std::string> & t_holder, std::vector<sf::Sprite>& t_wallSprites);
	void update(double dt, HUD& t_hud);
	void render(sf::RenderWindow & window);
	void setPosition(sf::Vector2f t_position);
	sf::Vector2f getPosition();
	void setScale(sf::Vector2f t_scale);
	sf::Sprite& getSprite() ;
	
	/// <summary>
	/// @brief Increases the speed by 1.
	/// </summary>
	void increaseSpeed();

	/// <summary>
	/// @brief Decreases the speed by 1.
	/// </summary>
	void decreaseSpeed();

	/// <summary>
	/// @brief Increases the rotation by 1 degree, wraps to 0 degrees after 359
	/// </summary>
	void increaseRotation();

	/// <summary>
	/// @brief Decreases the rotation by 1 degree, wraps to 359 degrees after 0.
	/// </summary>
	void decreaseRotation();

	/// <summary>
	/// @briaf handle key input
	/// </summary>
	void handleKeyInput();

	/// <summary>
	/// increase turrest rotation by 1 degree
	/// </summary>
	void increaseTurretRotation(); 
	
	/// <summary>
	/// decrease turret rotation by 1 degree
	/// </summary>
	void decreaseTurretRotation();

	/// <summary>
	/// turret moving towards the center of the tank by 1 degree in the shortest route
	/// </summary>
	void centreTurret();
	
	/// <summary>
	/// returns if the turret is currently centering or not
	/// </summary>
	/// <returns>m_turretCentering</returns>
	bool turretCentering();

	/// <summary>
	/// @brief Checks for collisions between the tank and the walls.
	/// </summary>
	/// <returns>True if collision detected between tank and wall.</returns>
	bool checkWallCollision();

	/// <summary>
	/// @brief Basic collision response algorithm. Uses a pre-calculated
	/// contact normal to deflect the tank in the event of a base/wall or turret/wall collision
	/// </summary>
	void deflect(double dt);

	/// <summary>
	/// moves and rotates the tank and turret
	/// </summary>
	/// <param name="dt">delta time</param>
	void tankMovement(double dt);

	/*
	/// <summary>
	/// fires projectiles
	/// </summary>
	void fireProjectile();
	*/

	/// <summary>
	/// @brief generates a request to fire a projectile
	/// a projectile will only be fired from the tip of the turret if the time limit has expired
	/// fire requests are not queued
	/// </summary>
	void requestFire();

	/// <summary>
	/// decreases tanks health when hit by a projectile
	/// </summary>
	/// <param name="t_damage"> damage of a projectile </param>
	void applyDamage(int t_damage);

	/// <summary>
	/// adds sprite to the vector of target sprites
	/// </summary>
	/// <param name="t_sprite"> target sprite</param>
	void addTarget(Target& t_target);

	/// <summary>
	/// checks if the tanks health is not 0
	/// </summary>
	/// <returns> if tanks health is 0 then it returns false</returns>
	bool isAlive();


private:
	void initSprites();
	std::vector<std::function<void(int)>> m_applyDamageToTarget;

	sf::Sprite m_tankBase;
	sf::Sprite m_turret;
	thor::ResourceHolder<sf::Texture, std::string> & m_holder;
	// The tank speed. 
	double m_speed{ 0.0 }; 
	// The current rotation as applied to tank base. 
	double m_rotation{ 0.0 };
	//the current turret rotation plus the rotation of the base
	double m_turretRotation{ 0.0 };

	//flag for centering the turret
	bool m_centering{ false };

	//reference to the container of wall sprites
	std::vector<sf::Sprite>& m_wallSprites;

	//contact normal vector between the turret and the wall
	sf::Vector2f m_contactNormal;
	//state of a tank
	TankState m_state{ TankState::NORMAL };

	double const MAX_REVERSE_SPEED{ -100 };
	double const MAX_FORWARD_SPEED{ 100 };

	//std::vector<Projectile> m_projectiles;

	ProjectilePool m_pool;

	bool m_fireRequested{ false };
	static int const s_TIME_BETWEEN_SHOTS{ 200 };
	int m_shootTimer{ 200 };

	int m_health{ 5 };
	std::vector<Target*> m_targets;
	std::vector<sf::Sprite*> m_targetSprites;

	//sf::Clock m_fireTimer; //timer for fireing projectiles
};
