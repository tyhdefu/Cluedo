#include "pch.h"
#include "GameClass.h"
#include <iostream>

Game::Game(int logLevel) : loglevel(logLevel)
{
	for (int i = 0; i < 6; i++)
	{
		this->Players.push_back(nullptr);
	}
}

Game::~Game()
{
	delete this->Players[0]->CurrentGame;
}

CardSet Game::Ask(std::shared_ptr<Player> Asker, CardSet Selection)
{
	int i;
	for (i = 0; i < this->players; i++)
	{
		if (this->Players[i] == Asker)
		{
			if (this->loglevel >= 2)
			{
				std::cout << "Player " << i + 1 << " asked" << std::endl;
				if (loglevel >= 3)
					Selection.printCardSet();
			}
			break;
		}
	}
	// go to player after the asker
	if (i == (this->players - 1))
		i = 0;
	else
		i++;

	while (this->Players[i] != Asker)
	{
		CardSet Answer = this->Players[i]->OnAsked(Selection, Asker);
		if (Answer != CardSet())
		{
			// If not null then contains answer
			if (this->loglevel >= 2)
			{
				std::cout << "Player " << i + 1 << " answered" << std::endl;
				if (this->loglevel >= 3)
				{
					Answer.printCardSet();
				}
				std::cout << std::endl;
			}
			this->CardShown(this->Players[i], Asker, Selection, true);
			return Answer;
		}
		else
		{
			this->CardShown(this->Players[i], Asker, Selection, false);
		}

		// Go to first player if we have reached the last player
		if (i == (this->players - 1))
			i = 0;
		else
			i++;
	}
	return CardSet();
}

void Game::StartGame()
{
	int rounds = 0;
	// Each game shouldnt ever last for more than 500 rounds
	while (this->GameEnded == false && rounds < 100)
	{
		// Cycle through players until game ends
		for (int i = 0; i < this->players; i++)
		{
			if (this->GameEnded == true)
				break;
			else if (this->Players[i]->bHasLost == false)
			{
				this->Players[i]->DoTurn();
			}
				
		}
		rounds++;
	}
}

void Game::DealCards()
{
	int r = 0;
	// Pick three random cards
	
	PersonCard* persons[6];
	WeaponCard* weapons[6];
	RoomCard* rooms[9];

	//Fill arrays
	for (int i = 0; i < 6; i++)
		persons[i] = new PersonCard(i);
	for (int i = 0; i < 6; i++)
		weapons[i] = new WeaponCard(i);
	for (int i = 0; i < 9; i++)
		rooms[i] = new RoomCard(i);

	int personcards = 6;
	int weaponcards = 6;
	int roomcards = 9;

	// Create SecretCards

	CardSet secretcards;

	r = rand() % 6;
	secretcards.Person = std::make_shared<PersonCard>(persons[r]->Getobject());
	for (;r < 5; r++)
		persons[r] = persons[(r + 1)];
	personcards--;
	persons[r] = nullptr;

	r = rand() % 6;
	secretcards.Weapon = std::make_shared<WeaponCard>(weapons[r]->Getobject());
	for (; r < 5; r++)
		weapons[r] = weapons[(r + 1)];
	weaponcards--;
	weapons[r] = nullptr;

	r = rand() % 9;
	secretcards.Room = std::make_shared<RoomCard>(rooms[r]->Getobject());;
	for (;  r < 8; r++)
		rooms[r] = rooms[(r + 1)];
	roomcards--;
	rooms[r] = nullptr;

	this->SecretCards = secretcards;

	// round of dealing
	int i = 0;
	//Deal cards while there are enough cards to deal one to each player
	while ((personcards + weaponcards + roomcards) >= this->players)
	{
		for (int j = 0; j < players; j++)
		{
			//Generate a number between the amount of cards left
			r = rand() % (personcards + weaponcards + roomcards);

			if (r < personcards)
			{
				// within personcards amount
				this->Players[j]->AddStartingCard(new CardSet(*persons[r]), i);
				for (; r < (personcards - 1); ++r)
					persons[r] = persons[(r + 1)];
				persons[r] = nullptr;
				personcards--;
			}
			else if (r < (personcards + weaponcards))
			{
				r -= personcards;
				// within weaponcards amount
				this->Players[j]->AddStartingCard(new CardSet(*weapons[r]), i);
				for (; r < (weaponcards - 1); ++r)
					weapons[r] = weapons[(r + 1)];
				weapons[r] = nullptr;
				weaponcards--;
			}
			else if (r < (personcards + weaponcards + roomcards))
			{
				r -= (personcards + weaponcards);
				// within roomcards amount
				this->Players[j]->AddStartingCard(new CardSet(*rooms[r]), i);
				for (; r < (roomcards - 1); ++r)
					rooms[r] = rooms[(r + 1)];
				rooms[r] = nullptr;
				roomcards--;
			}
		}
		i++;
	}
	for (int i = 0; i < 6; i++)
	{
		delete persons[i];
	}
	for (int i = 0; i < 6; i++)
	{
		delete weapons[i];
	}
	for (int i = 0; i < 9; i++)
	{
		delete rooms[i];
	}
}

bool Game::Convict(CardSet secretcards, std::shared_ptr<Player> Acuser)
{
	if (secretcards == this->SecretCards)
	{
		EndGame(Acuser);
		return true;
	}
	else
	{
		if (this->loglevel >= 2)
		{
			std::cout << "removed player" << std::endl;
		}
		//RemovePlayer(Acuser);
		return false;
	}
}

void Game::RemovePlayer(std::shared_ptr<Player> player)
{
	for (int i = 0, j = 0; i < (this->players - 1); i++)
	{
		if (this->Players[i] != player)
		{
			if (j > i)
			{
				// replace current with next
				this->Players[i] = this->Players[(i + 1)];
			}
		}
		else
		{
			j = i;
			this->Players[i] = this->Players[i + 1];
		}
	}
}

void Game::EndGame(std::shared_ptr<Player> Winner)
{
	if (this->loglevel >= 1)
		std::cout << "Ending game..." << std::endl;
	for (int i = 0; i < this->players; i++)
	{
		if (this->Players[i] == Winner)
		{
			if (this->loglevel >= 1)
				std::cout << "Player " << (i + 1) << " has won" << std::endl;
		}
		else if (this->Players[i] != nullptr)
		{
			this->Players[i]->bHasLost = true;
		}
	}
	if (this->loglevel >= 1)
	{
		std::cout << "Secret Cards were" << std::endl;
		this->SecretCards.printCardSet();
	}
	this->GameEnded = true;
}

void Game::CardShown(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards, bool Cardshown)
{
	if (Cardshown == true)
	{
		for (int i = 0; i < (players - 1); i++)
		{
			this->Players[i]->CardExchanged(Shower, Receiver, AskedCards);
		}
	}
	else
	{
		for (int i = 0; i < (players - 1); i++)
		{
			this->Players[i]->CardNotExchanged(Shower ,Receiver, AskedCards);
		}
	}
}

void Game::ResetGame()
{
	this->SecretCards = CardSet();
	for (int i = 0; i < players; i++)
	{
		this->Players[i]->Reset();
	}
	
	this->GameEnded = false;
}

void Game::SetPlayers(std::vector<std::shared_ptr<Player>> players)
{

	for (int i = 0; i < 6; i++)
	{
		if (players[i] != nullptr)
		{
			this->Players[i] = players[i];
			players[i]->CurrentGame = this;
			players[i]->SetPlayers(players, i);
			// starts at 0 and needs to start at one
			this->players = i + 1;
		}
		else
			break;
	}
}