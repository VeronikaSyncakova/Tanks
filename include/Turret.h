#include "Target.h"
#include "ProjectilePool.h"

class Turret : public Target
{
public:
	Turret(sf::Texture& t_texture,TargetData& t_data, std::vector<sf::Sprite>& t_walls, sf::Sprite& t_tank, std::function<void(int)> t_damageFunc);
	void update(double& dt, sf::Vector2f& t_tankPosition) override;
	void render(sf::RenderWindow& t_window) override;
	void init() override;
	void calculateAheadVector();
	bool noObstacle();

private:
	std::function<void(int)> m_damageFunction;
	ProjectilePool m_pool;
	float m_rotation;
	sf::Clock m_fireTimer;
	float m_timeFire{3000.f};
	bool m_rotated{ false };
	std::vector<sf::Sprite>& m_wallSprites;
	sf::Sprite& m_tankSpriteRef;

	// The ahead vector.
	sf::Vector2f m_ahead;
	// The half-ahead vector.
	sf::Vector2f m_halfAhead;
	//debug vector
	sf::RectangleShape m_aheadVector;
	// The maximum see ahead range.
	float m_maxSeeAhead;

	std::vector<sf::CircleShape> m_wallObstacles;
};