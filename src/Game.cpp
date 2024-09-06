#include "Game.h"
#include <iostream>

// Our target FPS
static double const FPS{ 60.0f };

////////////////////////////////////////////////////////////
Game::Game()
	: m_window(sf::VideoMode(ScreenSize::s_width, ScreenSize::s_height, 32), "SFML Playground", sf::Style::Default), m_tank(m_holder,m_wallSprites)
{
	init();
}

////////////////////////////////////////////////////////////
void Game::init()
{
	int currentLevel = 1;
	// Will generate an exception if level loading fails.
	try 
	{ 
		LevelLoader::load(currentLevel, m_level); 
	}
	catch (std::exception& e) 
	{ 
		std::cout << "Level Loading failure." << std::endl; std::cout << e.what() << std::endl; throw e; 
	}
	// Really only necessary is our target FPS is greater than 60.
	m_window.setVerticalSyncEnabled(true);

	if (!m_arialFont.loadFromFile("BebasNeue.otf"))
	{
		std::cout << "Error loading font file";
	}
	m_hud.setFont(m_arialFont);
	//m_holder.acquire("tankAtlas", thor::Resources::fromFile<sf::Texture>("resources/images/spritesheet.png"));
	m_holder.acquire("background", thor::Resources::fromFile<sf::Texture>("resources/images/Background.jpg"));
	m_holder.acquire("wall", thor::Resources::fromFile<sf::Texture>("resources/images/wall.png"));

	sf::Texture& background = m_holder["background"];
	m_bgSprite.setTexture(background);
	m_texture = m_holder["wall"];
	

	m_tank.setScale(m_level.m_tank.m_scale);
	m_tank.setPosition(m_level.m_tank.m_position);

	generateWalls();
	generateTargets();

	LevelLoader::load(levelNr, m_level);

	//the sprite is being drawn in the top left corner and a bit off-screen. that's because our origin, in our case the center of the tank, is being drawn at the 100,100 coordinates.
	//it is drawn off-screen because half of our tank is wider than 100 px

	//this releases the texture therefore the texture is not available for the sprite, the pointer in sf::Sprite doesnt work anymore
	//m_holder.release("tankAtlas");

#ifdef TEST_FPS
	x_updateFPS.setFont(m_arialFont);
	x_updateFPS.setPosition(20, 300);
	x_updateFPS.setCharacterSize(24);
	x_updateFPS.setFillColor(sf::Color::White);
	x_drawFPS.setFont(m_arialFont);
	x_drawFPS.setPosition(20, 350);
	x_drawFPS.setCharacterSize(24);
	x_drawFPS.setFillColor(sf::Color::White);
#endif
}

////////////////////////////////////////////////////////////
void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	
	sf::Time timePerFrame = sf::seconds(1.0f / FPS); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame.asMilliseconds()); //60 fps
#ifdef TEST_FPS
			x_secondTime += timePerFrame;
			x_updateFrameCount++;
			if (x_secondTime.asSeconds() > 1)
			{
				std::string updatesPS = "UPS " + std::to_string(x_updateFrameCount - 1);
				x_updateFPS.setString(updatesPS);
				std::string drawsPS = "DPS " + std::to_string(x_drawFrameCount);
				x_drawFPS.setString(drawsPS);
				x_updateFrameCount = 0;
				x_drawFrameCount = 0;
				x_secondTime = sf::Time::Zero;
			}
#endif
		}
		render(); // as many as possible
#ifdef TEST_FPS
		x_drawFrameCount++;
#endif
	}
}

////////////////////////////////////////////////////////////
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

////////////////////////////////////////////////////////////
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		default:
			break;
		}
	}
	
}

/// ////////////////////////////////////////////////////////
void Game::generateWalls()
{
	// Replace the ? With the actual values for the wall image
	sf::IntRect wallRect(0, 230, 32, 22);
	// Create the Walls
	for (auto const& obstacle : m_level.m_obstacles)
	{
		sf::Sprite sprite;
		sf::Texture& texture = m_holder["tankAtlas"];
		sprite.setTexture(texture);
		sprite.setTextureRect(wallRect);
		sprite.setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite.setPosition(obstacle.m_position);
		sprite.setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(sprite);
	}
}

void Game::generateTargets()
{
	using std::placeholders::_1;
	m_funcApplyDamage = std::bind(&Tank::applyDamage, &m_tank, _1);
	for (TargetData& target : m_level.m_targets)
	{
		Target* targetNew=nullptr;
		sf::Texture& texture = m_holder["tankAtlas"];
		if (target.m_type == "STATIC")
		{
			targetNew = new Static(texture, target);
		}
		else if (target.m_type == "TURRET")
		{
			targetNew = new Turret(texture, target, m_wallSprites, m_tank.getSprite(), m_funcApplyDamage);
		}
		m_targets.push_back(targetNew);
	}
	m_tank.addTarget(*m_targets.at(0)); 
	m_activeTargetIndex = m_targets.size() - 1;
}

////////////////////////////////////////////////////////////
void Game::update(double dt)
{
	switch (m_gameState)
	{
	case GameState::GamePlay:
		if (m_hud.stillTime() && m_tank.isAlive())
		{
			if (m_tank.turretCentering())
			{
				m_tank.centreTurret();
			}
			m_tank.update(dt,m_hud);
			m_hud.updateClock();
			sf::Vector2f pos = m_tank.getPosition();
			if (m_targets.at(m_activeTargetIndex)->isActivated())
			{
				m_targets.at(m_activeTargetIndex)->update(dt,pos);
			}
			else if(m_activeTargetIndex+1<m_targets.size())
			{
				if (m_targets.at(m_activeTargetIndex + 1)->isAlive())
				{
					m_activeTargetIndex++;
					m_targets.at(m_activeTargetIndex)->init();
					m_tank.addTarget(*m_targets.at(m_activeTargetIndex));
				}
				else //finds next target that was not destroyed
				{
					bool changed = false;
					for (int i = m_activeTargetIndex+1; i < m_targets.size(); i++)
					{
						if (m_targets.at(i)->isAlive())
						{
							m_activeTargetIndex = i;
							m_targets.at(m_activeTargetIndex)->init();
							m_tank.addTarget(*m_targets.at(m_activeTargetIndex));
							changed = true;
							break;
						}
					}
					if(!changed)
						m_activeTargetIndex = m_targets.size()-1;
				}
			}
			else if (m_activeTargetIndex + 1 >= m_targets.size()) //if we reached end of the vector, check if all the targets have been destroyed
			{
				bool empty = true;
				for (int i = 0; i < m_targets.size(); i++)
				{
					if (m_targets.at(i)->isAlive())
					{
						m_activeTargetIndex = i;
						m_targets.at(m_activeTargetIndex)->init();
						m_tank.addTarget(*m_targets.at(m_activeTargetIndex));
						empty = false;
						break;
					}
				}
				if (empty) //winning condition when all the targets are destroyed
				{
					m_gameState = GameState::GameWon;
					m_hud.calculateAccuracy();
				}
			}
			/*for (auto& target : m_targets)
			{
				target->update(dt, pos);
			}
			*/
		}/*
		else if(m_tank.isAlive())
		{
			m_gameState = GameState::GameWon;
			m_hud.calculateAccuracy();
		}*/
		else
		{
			m_gameState = GameState::GameLost;
			m_hud.calculateAccuracy();
		}
		break;
	case GameState::GameLost:
		break;
	case GameState::GameWon:
		break;
	default:
		break;
	}
	
}

////////////////////////////////////////////////////////////
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	//the whole spriteatlas is drawn
	//m_window.draw(m_tankSprite);
	//m_window.draw(m_turretSprite);
	switch (m_gameState)
	{
	case GameState::GamePlay:
		m_window.draw(m_bgSprite);
		m_tank.render(m_window);

		for (int i = 0; i < m_wallSprites.size(); i++)
		{
			m_window.draw(m_wallSprites.at(i));
		}
		m_hud.render(m_window, m_gameState);

		for (auto& target : m_targets)
		{
			target->render(m_window);
		}

		break;
	case GameState::GameLost:
		m_hud.render(m_window, m_gameState);
		break;
	case GameState::GameWon:
		m_hud.render(m_window, m_gameState);
		break;
	default:
		break;
	}
	

#ifdef TEST_FPS
	m_window.draw(x_updateFPS);
	m_window.draw(x_drawFPS);
#endif
	m_window.display();
}





