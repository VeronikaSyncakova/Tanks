#include "HUD.h"

HUD::HUD()
{
	init();
}

void HUD::readHighScore(const YAML::Node& t_highScoreNode)
{
	m_highScore = t_highScoreNode["accuracy"].as<int>();
}

void HUD::init()
{
	std::string filename = "./resources/levelData/Highscores.yaml";

	try
	{
		YAML::Node baseNode = YAML::LoadFile(filename);
		if (baseNode.IsNull())
		{
			std::string message("File: " + filename + " not found");
			throw std::exception(message.c_str());
		}
		readHighScore(baseNode["Highscores"]);
	}
	catch (YAML::ParserException& e)
	{
		std::string message(e.what());
		message = "YAML Parser Error: " + message;
		throw std::exception(message.c_str());
	}
	catch (std::exception& e)
	{
		std::string message(e.what());
		message = "Unexpected Error: " + message;
		throw std::exception(message.c_str());
	}

	m_timerBackground.setFillColor(sf::Color::Black);
	m_timerBackground.setSize(sf::Vector2f(m_gameTime, m_gameTime / 4.f));
	m_timerBackground.setOrigin(m_timerBackground.getSize() / 2.f);
	m_timerBackground.setPosition(m_position);

	m_timeBar.setFillColor(sf::Color::Green);
	m_timeBar.setSize(sf::Vector2f(m_gameTime, m_gameTime /4.f));
	m_timeBar.setOrigin(m_timerBackground.getSize() / 2.f);
	m_timeBar.setPosition(m_position);

	m_scoreText.setPosition(400.f, 10.f);
	m_scoreText.setString("score: " + std::to_string(m_score));

	m_stateText.setPosition(400.f, 200.f);
}

void HUD::updateClock()
{
	if (m_gameTime > 0)
	{
		//decrement timerBar
		if (m_gameTimer.getElapsedTime().asSeconds() >= 1)
		{
			m_gameTimer.restart();
			m_gameTime--;
			m_timeBar.setSize(sf::Vector2f(m_gameTime, m_timeBar.getSize().y));
		}
	}
	else
	{
		m_enoughTime = false;
	}
}

void HUD::render(sf::RenderWindow& t_window, GameState& t_gameState)
{
	if (t_gameState == GameState::GamePlay)
	{
		t_window.draw(m_timerBackground);
		t_window.draw(m_timeBar);
		t_window.draw(m_scoreText);
	}
	else if (t_gameState == GameState::GameLost)
	{
		m_stateText.setString("Game Lost");
		t_window.draw(m_scoreText);
		t_window.draw(m_stateText);
	}
	else if (t_gameState == GameState::GameWon)
	{
		m_stateText.setString("Game Won");
		t_window.draw(m_scoreText);
		t_window.draw(m_stateText);
	}
}

bool HUD::stillTime()
{
	return m_enoughTime;
}

void HUD::increaseScore()
{
	m_score++;
	m_scoreText.setString("score: " + std::to_string(m_score));
}

void HUD::setFont(sf::Font& t_font)
{
	m_scoreText.setFont(t_font);
	m_stateText.setFont(t_font);
}

void HUD::increaseShots()
{
	m_totalShots++;
}

void HUD::calculateAccuracy()
{
	if(m_totalShots>0)
		m_accuracy=static_cast<int>(round((float)m_score*100/(float)m_totalShots));
	m_scoreText.setPosition(400.f, 300.f);
	m_scoreText.setString("score: " + std::to_string(m_score) + "\ntotal shots: " + std::to_string(m_totalShots) + "\naccuracy: " + std::to_string(m_accuracy)+"%"+"\nhighest accuracy: "+std::to_string(m_highScore)+"%");
	if (m_highScore < m_accuracy)
	{
		m_scoreText.setString("score: " + std::to_string(m_score) + "\ntotal shots: " + std::to_string(m_totalShots) + "\naccuracy: " + std::to_string(m_accuracy) + "% (new highest accuracy!)" + "\nhighest accuracy: " + std::to_string(m_highScore) + "%");
		saveData();
	}
}

void HUD::saveData()
{
	// A YAML::Emitter acts as a YAML output stream 	
	YAML::Emitter out;
	out << YAML::BeginMap;

	// Highscores is the top level mapping
	out << YAML::Key << "Highscores";

	out << YAML::BeginMap;
	out << YAML::Key << "accuracy";
	out << YAML::Value << m_accuracy;
	out << YAML::EndMap;

	out << YAML::EndMap;
	std::cout << "Here's the raw YAML data:\n" << out.c_str() << "\n";

	// Now write it to a file..
	std::string file = "./resources/levelData/Highscores.yaml"; 
	std::ofstream fout(file); // This is the path to the YAML file

	fout << out.c_str() << std::endl;
	fout.close();
}
