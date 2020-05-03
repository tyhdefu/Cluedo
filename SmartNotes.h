#pragma once
#include "Card.h"
#include "table.h"
#include "Player.h"

class SmartNotes
{
public:

	SmartNotes() {};
	
	void printShownCards();
	void printKnownCards();
	void printSuspectedCards();
	void printNotOwnedCards();

	std::vector<std::shared_ptr<Player>> Players;

	// Contains cards that have been showed to other players
	table2D<std::shared_ptr<Player>, CardSet> ShownCards;

	// Contains known cards of players
	table2D<std::shared_ptr<Player>, CardSet> KnownCards;

	// Contains cards that other players might have. CardSet contains the three possible cards
	table2D<std::shared_ptr<Player>, CardSet> SuspectedCards;

	// Contains cardSets that players could not show
	table2D<std::shared_ptr<Player>, CardSet> NotOwnedCards;
};