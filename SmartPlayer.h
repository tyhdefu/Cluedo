#pragma once
#include "Player.h"
#include "table.h"
#include "SmartNotes.h"
#include <vector>
#include <iostream>

class SmartPlayer : public Player
{
public:
	SmartPlayer();

	virtual void DoTurn() override;
	virtual void Decide() override;

	virtual void CardExchanged(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards) override;
	virtual void CardNotExchanged(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards) override;

	virtual CardSet OnAsked(CardSet Selection, std::shared_ptr<Player> Asker) override;

	virtual void SetPlayers(std::vector<std::shared_ptr<Player>> players, int self) override;

	bool CheckNotes();

	void Reset() override;

	SmartNotes smartnotes;

private:
	

	// Used in decide to try to figure out information
	void FigureOutCards();

	

	// Call when discovered a card
	void FoundCard(CardSet card, std::shared_ptr<Player> owner);
	
	bool CheckCard(CardSet cardset);
};