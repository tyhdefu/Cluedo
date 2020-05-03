#include "pch.h"
#include "GameClass.h"
#include "Player.h"
#include <iostream>

Player::Player()
{
	this->CurrentRoom = RoomCard(0);

	for (int i = 0; i < 9; i++)
	{
		this->StartingCards[i] = nullptr;
	}
}

Player::~Player()
{
	//for (int i = 0; i < 9; i++)
	//{
	//	delete this->StartingCards[i];
	//}
}

void Player::SetPlayers(std::vector<std::shared_ptr<Player>> players, int self)
{
	this->self = players[self];
}

void Player::DoTurn()
{
	Decide();
	if (this->decisions.bMoveRooms == true)
	{
		Move();
	}
	if (this->decisions.WillConvict == true)
	{
		Convict();
	}
	else
	{
		CardSet Answer = Ask();
		if (Answer == CardSet())
		{
			//If no one could supply any of the cards
			//With this player only asks about cards they dont have
			if (this->CurrentGame->loglevel >= 2)
			{
				std::cout << "no one answered" << "\n";

				if (this->CurrentGame->loglevel >= 3)
					this->decisions.AskSelection.printCardSet();

				std::cout << "\n";
			}

			this->notes.WriteCulprit(*this->decisions.AskSelection.Person);
			this->notes.WriteCulprit(*this->decisions.AskSelection.Weapon);
			this->notes.WriteCulprit(*this->decisions.AskSelection.Room);
		}
		else
		{
			if (Answer.Person != nullptr)
				this->notes.WriteNote(*Answer.Person);
			else if (Answer.Weapon != nullptr)
				this->notes.WriteNote(*Answer.Weapon);
			else if (Answer.Room != nullptr)
				this->notes.WriteNote(*Answer.Room);
		}
	}
}

void Player::Lose()
{
	this->bHasLost = true;
}

void Player::Win()
{
	//Win
	this->wins++;
}

void Player::Reset()
{
	//std::cout << "In player Reset" << std::endl;
	this->notes.Reset();

	this->decisions.WillConvict = false;
	this->decisions.bMoveRooms = false;
	this->decisions.AskSelection = CardSet();
	this->decisions.DestinationRoom = RoomCard();

	for (int i = 0; i < 9; i++)
	{
		this->StartingCards[i] = nullptr;
	}

	this->bHasLost = false;
}

CardSet Player::OnAsked(CardSet Selection, std::shared_ptr<Player> Asker)
{
	int i = 0;

	while (true)
	{
		if (this->StartingCards[i] == nullptr)
		{
			// Doesnt have any more starting cards
			break;
		}
		else if (this->StartingCards[i]->Person != nullptr)
		{
			if (*this->StartingCards[i]->Person == *Selection.Person)
			{
				// Has card asked about
				return *this->StartingCards[i];
			}
			else
				i++;
		}
		else if (this->StartingCards[i]->Weapon != nullptr)
		{
			if (*this->StartingCards[i]->Weapon == *Selection.Weapon)
			{
				// Has card asked about
				return *this->StartingCards[i];
			}
			else
				i++;
		}
		else if (this->StartingCards[i]->Room != nullptr)
		{
			if (*this->StartingCards[i]->Room == *Selection.Room)
			{
				// Has card asked about
				return *this->StartingCards[i];
			}
			else
				i++;
		}
	}
	return CardSet();
}

void Player::AddStartingCard(CardSet* card, int index)
{
	if (index >= 9 && index >= 0)
		throw;
	else
	{ 
		this->StartingCards[index] = card;
		// Check type and write note
		if (card->Person != nullptr)
			this->notes.WriteNote(*card->Person);
		else if (card->Weapon != nullptr)
			this->notes.WriteNote(*card->Weapon);
		else if (card->Room != nullptr)
			this->notes.WriteNote(*card->Room);
	}
}

void Player::Decide()
{
	// Check if solution is know
	if (this->notes.Solution() != CardSet())
	{
		// Solution is known
		this->decisions.AskSelection = this->notes.Solution();
		this->decisions.WillConvict = true;
	}
	//Decide
	else
	{
		int tries = 0;
		RoomCard room = this->CurrentRoom;
		// Change room if needed
		if (this->notes.ReadNote(this->CurrentRoom) != false)
		{
			// Need to move
			do
			{
				//pick random room, checked if not proven innocent
				room = RoomCard((rand() % 9));
				tries++;
				if (tries > 500)
					throw;
			} while (this->notes.ReadNote(room) != false);
			
			this->decisions.DestinationRoom = room;
			this->decisions.bMoveRooms = true;
		}
		else
		{
			this->decisions.bMoveRooms = false;
		}
		
		//pick random unknown person
		PersonCard person;
		do
		{
			//pick random room, checked if not proven innocent
			person = PersonCard((rand() % 6));
			tries++;
			if (tries > 500)
				throw;
		} while (this->notes.ReadNote(person) != false);

		WeaponCard weapon;
		do
		{
			//pick random room, checked if not proven innocent
			weapon = WeaponCard(static_cast<WeaponCard::Object>(rand() % 6));
			tries++;
			if (tries > 500)
				throw;
		} while (this->notes.ReadNote(weapon) != false);

		this->decisions.AskSelection = CardSet(person, weapon, room);
	}
}

void Player::Move()
{
	this->CurrentRoom = this->decisions.DestinationRoom;
}

void Player::Convict()
{
	if (this->CurrentGame->Convict(this->decisions.AskSelection, this->self))
	{
		// If true then won
		if (this->CurrentGame->loglevel >= 3)
			std::cout << "I have won" << std::endl;
		Win();
	}
	else
		Lose();
}

CardSet Player::Ask()
{
	return this->CurrentGame->Ask(this->self, this->decisions.AskSelection);
}