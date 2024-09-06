#pragma once

// If VS Debug build is enabled, then any block of code enclosed within 
//  the preprocessor directive #ifdef TEST_FPS is compiled and executed.
#ifdef _DEBUG
#define TEST_FPS
#endif // _DEBUG

#include <SFML/Graphics.hpp>
#include <string>
#include "ScreenSize.h"
#include <Thor/Resources.hpp>
#include "LevelLoader.h"
#include "Tank.h"
#include "HUD.h"
#include "Turret.h"
#include "Static.h"


/// <summary>
/// @brief Main class for the SFML Playground project.
/// 
/// This will be a single class framework for learning about SFML. 
/// Example usage:
///		Game game;
///		game.run();
/// </summary>

class Game
{
public:
	/// <summary>
	/// @brief Default constructor that initialises the SFML window, 
	///   and sets vertical sync enabled. 
	/// </summary>
	Game();

	/// <summary>
	/// @brief the main game loop.
	/// 
	/// A complete loop involves processing SFML events, updating and drawing all game objects.
	/// The actual elapsed time for a single game loop is calculated. If this value is 
	///  greater than the target time for one loop (1 / 60), then (and only then) is an update 
	///  operation performed.
	/// The target is at least one update and one render cycle per game loop, but typically 
	///  more render than update operations will be performed as we expect our game loop to
	///  complete in less than the target time.
	/// </summary>
	void run();

protected:
	/// <summary>
	/// @brief Once-off game initialisation code
	/// </summary>	
	void init();
	/// <summary>
	/// @brief Placeholder to perform updates to all game objects.
	/// </summary>
	/// <param name="time">update delta time</param>
	void update(double dt);

	/// <summary>
	/// @brief Draws the background and foreground game objects in the SFML window.
	/// The render window is always cleared to black before anything is drawn.
	/// </summary>
	void render();

	/// <summary>
	/// @brief Checks for events.
	/// Allows window to function and exit. 
	/// Events are passed on to the Game::processGameEvents() method.
	/// </summary>	
	void processEvents();

	/// <summary>
	/// @brief Handles all user input.
	/// </summary>
	/// <param name="event">system event</param>
	void processGameEvents(sf::Event&);

	/// <summary>
	/// @brief Creates the wall sprites and loads them into a vector.
	/// Note that sf::Sprite is considered a light weight class, so
	/// storing copies (instead of pointers to sf::Sprite) in std::vector
	/// is acceptable.
	/// </summary>
	void generateWalls();

	/// <summary>
	/// takes data from m_level and creates targets accordingly and pushes them into a vector
	/// </summary>
	void generateTargets();

	std::function<void(int)> m_funcApplyDamage;
	std::function<void(int)> m_funcApplyDamageTarget;

	// Font used for all text
	sf::Font m_arialFont;
	// main window
	sf::RenderWindow m_window;

	thor::ResourceHolder<sf::Texture, std::string> m_holder;
	//sf::Sprite m_tankSprite;
	//sf::Sprite m_turretSprite;
	Tank m_tank;
	sf::Sprite m_bgSprite;

	sf::Texture m_texture; //wall texture

	std::vector<sf::Sprite> m_tankSprites;
	std::vector<sf::Sprite> m_wallSprites;

	LevelData m_level;
	int levelNr = 1; //level number 

	HUD m_hud;
	GameState m_gameState{ GameState::GamePlay }; //current game state
	std::vector<Target*> m_targets; 
	int m_activeTargetIndex;

#ifdef TEST_FPS
	sf::Text x_updateFPS;					// text used to display updates per second.
	sf::Text x_drawFPS;						// text used to display draw calls per second.
	sf::Time x_secondTime {sf::Time::Zero};	// counter used to establish when a second has passed.
	int x_updateFrameCount{ 0 };			// updates per second counter.
	int x_drawFrameCount{ 0 };				// draws per second counter.
#endif // TEST_FPS

};
