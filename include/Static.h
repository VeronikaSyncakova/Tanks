#include "Target.h"

class Static :public Target
{
public:
	Static(sf::Texture& t_texture,TargetData& t_data);
	void update(double& dt, sf::Vector2f& t_tankPosition) override;
	void render(sf::RenderWindow& t_window) override;
	void init() override;


private:
	float m_screenDuration; //time that it is rendered on screen in seconds, stored value
	float m_duration; //time that it is rendered on screen in seconds, changing value
	sf::Clock m_timer;
	float m_scale;
};