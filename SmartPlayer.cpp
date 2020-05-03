#include "pch.h"
#include "GameClass.h"
#include "SmartPlayer.h"

SmartPlayer::SmartPlayer()
{
}

void SmartPlayer::DoTurn()
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
			if (this->CurrentGame->loglevel >= 2)
			{
				std::cout << "no one answered" << "\n";

				if (this->CurrentGame->loglevel >= 3)
					this->decisions.AskSelection.printCardSet();

				std::cout << "\n";
			}
			// Check if player hasnt asked about cards they have
			if (this->CheckCard(CardSet(*decisions.AskSelection.Person)) == false)
				this->notes.WriteCulprit(*this->decisions.AskSelection.Person);
			if (this->CheckCard(CardSet(*decisions.AskSelection.Weapon)) == false)
				this->notes.WriteCulprit(*this->decisions.AskSelection.Weapon);
			if (this->CheckCard(CardSet(*decisions.AskSelection.Room)) == false)
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

void SmartPlayer::Decide()
{
	/// todo
	// Try to figure out new info
	FigureOutCards();

	/// The same as normal player, does it need to be different??
	/// Room will need to change as we start using a real board.


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
		if (CheckNotes() == false)
		{
			std::cout << "Checknotes was false: Notes have been incorrectly made" << std::endl;
			throw;
		}
		

		RoomCard room = this->CurrentRoom;
		// Change room if needed
		if (this->notes.ReadNote(this->CurrentRoom) != false)
		{
			// Need to move
			do
			{
				//pick random room, checked if not proven innocent
				room = RoomCard((rand() % 9));
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
		} while (this->notes.ReadNote(person) != false);

		WeaponCard weapon;
		do
		{
			//pick random room, checked if not proven innocent
			weapon = WeaponCard(static_cast<WeaponCard::Object>(rand() % 6));
		} while (this->notes.ReadNote(weapon) != false);

		this->decisions.AskSelection = CardSet(person, weapon, room);
	}
}

void SmartPlayer::CardExchanged(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards)
{
	if (Shower != this->self)
	{
		if (smartnotes.SuspectedCards.CheckArray(Shower, AskedCards) == false) // check if not already in array
		{
			smartnotes.SuspectedCards.Addtoarray(Shower, AskedCards);
		}
	}
}

void SmartPlayer::CardNotExchanged(std::shared_ptr<Player> Shower, std::shared_ptr<Player> Receiver, CardSet AskedCards)
{	
	if (Shower != this->self)
	{
		if (smartnotes.NotOwnedCards.CheckArray(Shower, CardSet(AskedCards.Person)) == false) // check if not already in array
		{
			smartnotes.NotOwnedCards.Addtoarray(Shower, CardSet(AskedCards.Person));
		}
		if (smartnotes.NotOwnedCards.CheckArray(Shower, CardSet(AskedCards.Weapon)) == false) // check if not already in array
		{
			smartnotes.NotOwnedCards.Addtoarray(Shower, CardSet(AskedCards.Weapon));
		}
		if (smartnotes.NotOwnedCards.CheckArray(Shower, CardSet(AskedCards.Room)) == false) // check if not already in array
		{
			smartnotes.NotOwnedCards.Addtoarray(Shower, CardSet(AskedCards.Room));
		}
	}
}

CardSet SmartPlayer::OnAsked(CardSet Selection, std::shared_ptr<Player> Asker)
{	
	int answer = -1;

	for (int i = 0; i < 9; i++)
	{
		if (StartingCards[i] == nullptr)
			break;
		else if (StartingCards[i]->Person != nullptr)
		{
			if (*StartingCards[i]->Person == *Selection.Person)
			{
				if (smartnotes.ShownCards.CheckArray(Asker, *StartingCards[i]))
				{
					return *StartingCards[i];
				}
				else
					answer = i;
			}
		}
		else if (StartingCards[i]->Weapon != nullptr)
		{
			if (*StartingCards[i]->Weapon == *Selection.Weapon)
			{
				if (smartnotes.ShownCards.CheckArray(Asker, *StartingCards[i]))
				{
					return *StartingCards[i];
				}
				else
					answer = i;
			}
		}
		else if (StartingCards[i]->Room != nullptr)
		{
			if (*StartingCards[i]->Room == *Selection.Room)
			{
				if (smartnotes.ShownCards.CheckArray(Asker, *StartingCards[i]))
				{
					return *StartingCards[i];
				}
				else
					answer = i;
			}
		}
	}
	if (answer == -1)
	{
		return CardSet();
	}
	else
	{
		this->smartnotes.ShownCards.Addtoarray(Asker, *StartingCards[answer]);
		return *StartingCards[answer];
	}
}

void SmartPlayer::SetPlayers(std::vector<std::shared_ptr<Player>> players, int self)
{
	this->self = players[self];
	for (int i = 0; i < 6; i++)
	{
		if (players[i] == nullptr)
			break;
		else
		{
			this->smartnotes.Players.push_back(players[i]);
			this->smartnotes.ShownCards.Addkey(players[i]);
			this->smartnotes.KnownCards.Addkey(players[i]);
			this->smartnotes.SuspectedCards.Addkey(players[i]);
			this->smartnotes.NotOwnedCards.Addkey(players[i]);
		}
	}
}

bool SmartPlayer::CheckNotes()
{
	int num = 0;
	for (int i = 0; i < 6; i++)
	{
		if (notes.ReadNote(PersonCard(i)) == false)
		{
			num++;
			break;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (notes.ReadNote(WeaponCard(i)) == false)
		{
			num++;
			break;
		}
	}
	for (int i = 0; i < 9; i++)
	{
		if (notes.ReadNote(RoomCard(i)) == false)
		{
			num++;
			break;
		}
	}
	if (num == 3)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SmartPlayer::Reset()
{
	//std::cout << "In Smartplayer Reset" << std::endl;
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
	
	this->smartnotes.ShownCards.Clear();
	this->smartnotes.KnownCards.Clear();
	this->smartnotes.SuspectedCards.Clear();
	this->smartnotes.NotOwnedCards.Clear();
}

void SmartPlayer::FigureOutCards()
{
	// Cards figured out that could affect previous players
	table2D<std::shared_ptr<Player>, CardSet> NewCards;
	for (auto i = smartnotes.Players.begin(); i != smartnotes.Players.end(); i++)
	{
		NewCards.Addkey(*i);
	}

	
	
	/// TODO: make it so that changedcards does something. Changedcards needs to be new values in the notowned 
	bool HasChanged = true; // allow it to go inside first time
	// Add changed cards (Known cards of a player) to the notowned of other players
	while (HasChanged == true)
	{
		HasChanged = false;
		for (auto i = smartnotes.Players.begin(); i != smartnotes.Players.end(); i++)
		{
			// for each player
			if (*i != this->self)
			{
				for (auto j = smartnotes.SuspectedCards.begin(*i); j != smartnotes.SuspectedCards.end(*i); j++)
				{
					for (auto k = smartnotes.NotOwnedCards.begin(*i); k != smartnotes.NotOwnedCards.end(*i); k++)
					{
						if (k->Person != nullptr && j->Person != nullptr)
						{
							if (*k->Person == *j->Person) // if person card in notowned matches one in suspected
							{
								j->Person = nullptr; // set to nullptr, no longer a possible option
								if (j->ContainsOneCard() == true)
								{
									if (j->Weapon != nullptr)
									{
										smartnotes.KnownCards.Addtoarray(*i, CardSet(j->Weapon));
										notes.WriteNote(*j->Weapon);
										HasChanged = true;
										NewCards.Addtoarray(*i, j->Weapon);
									}
									else
									{
										smartnotes.KnownCards.Addtoarray(*i, CardSet(j->Room));
										notes.WriteNote(*j->Room);
										HasChanged = true;
										NewCards.Addtoarray(*i, j->Room);
									}
								}
							}
						}
						else if (k->Weapon != nullptr && j->Weapon != nullptr)
						{
							if (*k->Weapon == *j->Weapon) // if person card in notowned matches one in suspected
							{
								j->Weapon = nullptr; // set to nullptr, no longer a possible option
								if (j->ContainsOneCard() == true)
								{
									if (j->Person != nullptr)
									{
										smartnotes.KnownCards.Addtoarray(*i, CardSet(j->Person));
										notes.WriteNote(*j->Person);
										HasChanged = true;
										NewCards.Addtoarray(*i, j->Person);
									}
									else
									{
										smartnotes.KnownCards.Addtoarray(*i, CardSet(j->Room));
										notes.WriteNote(*j->Room);
										HasChanged = true;
										NewCards.Addtoarray(*i, j->Room);
									}
								}
							}
						}
						else if (k->Room != nullptr && j->Room != nullptr)
						{
							if (*k->Room == *j->Room) // if person card in notowned matches one in suspected
							{
								j->Room = nullptr; // set to nullptr, no longer a possible option
								if (j->ContainsOneCard() == true)
								{
									if (j->Person != nullptr)
									{
										smartnotes.KnownCards.Addtoarray(*i, CardSet(j->Person));
										notes.WriteNote(*j->Person);
										HasChanged = true;
										NewCards.Addtoarray(*i, j->Person);
									}
									else
									{
										smartnotes.KnownCards.Addtoarray(*i, CardSet(j->Weapon));
										notes.WriteNote(*j->Weapon);
										HasChanged = true;
										NewCards.Addtoarray(*i, j->Weapon);
									}
								}
							}
						}
					}
				}
			}
		}

		for (auto i = smartnotes.Players.begin(); i != smartnotes.Players.end(); i++)
		{
			for (auto j = NewCards.begin(*i); j != NewCards.end(*i); j++)
			{
				for (auto k = smartnotes.Players.begin(); k != smartnotes.Players.end(); k++)
				{
					if (*k != *i && *i != this->self)
					{
						if (j->Person != nullptr)
						{
							if (smartnotes.NotOwnedCards.CheckArray(*k, *j->Person) == false)
								smartnotes.NotOwnedCards.Addtoarray(*k, *j->Person);
						}
						else if (j->Weapon != nullptr)
						{
							if (smartnotes.NotOwnedCards.CheckArray(*k, *j->Weapon) == false)
								smartnotes.NotOwnedCards.Addtoarray(*k, *j->Weapon);
						}
						else if (j->Room != nullptr)
						{
							if (smartnotes.NotOwnedCards.CheckArray(*k, *j->Room) == false)
								smartnotes.NotOwnedCards.Addtoarray(*k, *j->Room);
						}
					}
				}
			}
		}
		NewCards.Clear();
	}
	
}

void SmartPlayer::FoundCard(CardSet card, std::shared_ptr<Player> owner)
{
	for (auto i = smartnotes.Players.begin(); i != smartnotes.Players.end(); i++)
	{
		if (*i != owner)
		{
			if (!(smartnotes.NotOwnedCards.CheckArray(*i, card)))
			{
				this->smartnotes.NotOwnedCards.Addtoarray(*i, card);
			}
		}
	}

	this->smartnotes.KnownCards.Addtoarray(owner, card);
	if (card.Person != nullptr)
		notes.WriteNote(*card.Person);
	else if (card.Weapon != nullptr)
		notes.WriteNote(*card.Weapon);
	else if (card.Room != nullptr)
		notes.WriteNote(*card.Room);
	else
		throw;
}

bool SmartPlayer::CheckCard(CardSet cardset)
{
	for (int i = 0; i < 9; i++)
	{
		if (this->StartingCards[i] == nullptr)
			return false;
		else
		{
			if (*this->StartingCards[i] == cardset)
				return true;
		}
	}
	return false;
}