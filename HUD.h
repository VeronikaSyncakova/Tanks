#pragma once
#include <SFML/Graphics.hpp>
#include "ScreenSize.h"
#include "yaml-cpp\yaml.h"
#include <iostream>
#include <fstream>

/// <summary>
/// will display any game information needed for player while playing
/// </summary>
class HUD
{
public:
	HUD();
	void readHighScore(const YAML::Node& t_highScoreNode);
	void init(); //initialize members
	void updateClock(); //updates the timer
	void render(sf::RenderWindow& t_window, GameState& t_gameState);
	bool stillTime(); //if there is still time to play the game
	void increaseScore();
	void setFont(sf::Font& t_font);
	void increaseShots();
	void calculateAccuracy();
	void saveData();

private:
	sf::RectangleShape m_timeBar; //decreases
	sf::RectangleShape m_timerBackground;
	sf::Clock m_gameTimer; //clock
	float m_gameTime{ 60 }; //60 seconds to play the game
	sf::Vector2f m_position{ m_gameTime,m_gameTime/2.f };
	bool m_enoughTime{ true }; //control flag to switch screen to gameOver
	int m_score{ 0 };
	sf::Text m_scoreText;
	sf::Text m_stateText;
	int m_totalShots{ 0 };
	int m_accuracy{ 0 };
	int m_highScore;
};

