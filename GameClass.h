#pragma once

#include "Player.h"

class Game
{
public:
	Game(int loglevel);
	~Game();

	void SetPlayers(std::vector<std::shared_ptr<Player>> players);

	// Asks all players about questions
	CardSet Ask(std::shared_ptr<Player> Asker, CardSet Selection);
	//Show card to asking player
	
	void StartGame();

	void DealCards();

	bool Convict(CardSet, std::shared_ptr<Player> Acuser);

	void RemovePlayer(std::shared_ptr<Player> player);

	void EndGame(std::shared_ptr<Player> Winner);

	void CardShown(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards, bool Cardshown);
	
	int players = 0;

	void ResetGame();

	const int loglevel;

	std::vector<std::shared_ptr<Player>> Players;

private:
	
	CardSet SecretCards;

	bool GameEnded = false;
};