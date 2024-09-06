#pragma once

struct ScreenSize
{
public:
	static const int s_width{ 1440 };

	static const int s_height{ 900 };

};

//different states of the game, for screen mainly
enum class GameState
{
	GamePlay,
	GameLost,
	GameWon
};
