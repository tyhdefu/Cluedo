#pragma once
#include "Player.h"
#include "GameClass.h"

class RealPlayer : public Player
{
public:
	void DoTurn() override;
	CardSet OnAsked(CardSet Selection, std::shared_ptr<Player> Asker) override;
	void SetPlayers(std::vector<std::shared_ptr<Player>> players, int self) override;
	void printStartingCards();
	void WriteNote();
protected:
	void Decide() override;
	std::vector<std::shared_ptr<Player>> Players;
	void Win() override;
	void Lose() override;
	int GetPlayernum(std::shared_ptr<Player> player);
};