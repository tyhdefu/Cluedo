#pragma once
#include "BasicNotes.h"
#include <vector>
#include <iostream>

class Game;

class Player
{
public:
	Player();
	~Player();

	// Gives players of game, and position of self
	virtual void SetPlayers(std::vector<std::shared_ptr<Player>> players, int self);

	//Called by Game to signal the player to start their move
	virtual void DoTurn();

	// Another player showed a card to another player
	virtual void CardExchanged(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards) {};
	virtual void CardNotExchanged(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards) {};

	bool bHasLost = false;

	virtual void Lose();
	virtual void Win();

	// Prepare for another game
	virtual void Reset();

	// shared pointer to self
	std::shared_ptr<Player> self = nullptr;

	// Returns Cardset or null if player does not have any of the cards
	virtual CardSet OnAsked(CardSet Selection, std::shared_ptr<Player> Asker);

	Game* CurrentGame = nullptr;

	void AddStartingCard(CardSet* card, int index);

	//Notes
	BasicNotes notes;
	int wins = 0;

	

protected:
	// 5 + 5 + 8 possible cards = 18, min 2 players
	CardSet* StartingCards[9];

	struct Decisions
	{
		bool bMoveRooms = false;
		RoomCard DestinationRoom;
		CardSet AskSelection;
		bool WillConvict;
	};
	Decisions decisions;
	RoomCard CurrentRoom;
	//Decide flags in Decisions
	virtual void Decide();
	
	// Possible actions during turn
	virtual void Move();
	virtual CardSet Ask();		
	virtual void Convict();
};

class RegularPlayer : public Player
{
public:
	RegularPlayer() {};
	
	void Reset() override
	{
		std::cout << "I am RegularPlayer" << std::endl;
	}
};