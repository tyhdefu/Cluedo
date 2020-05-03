#include "pch.h"
#include "RealPlayer.h"

void RealPlayer::DoTurn()
{
	std::string input;

	while (true)
	{
		std::cout << R"(It is your turn. What would you like to do? Choose a number or type out the full request.
	1. Ask a question
	2. Convict
	3. Check notes
	4. Check Starting cards)" << std::endl;
		std::getline(std::cin, input);

		if (input == "1" || input == "Ask a question")
		{
			Decide();
			CardSet Answer = this->CurrentGame->Ask(this->self, this->decisions.AskSelection);
			std::cout << "You were shown ";
			if (Answer.Person != nullptr)
			{
				std::cout << PersonCard::ObjectNames[Answer.Person->Getobject()] << std::endl;
			}
			else if (Answer.Weapon != nullptr)
			{
				std::cout << WeaponCard::ObjectNames[Answer.Weapon->Getobject()] << std::endl;
			}
			else if (Answer.Room != nullptr)
			{
				std::cout << RoomCard::ObjectNames[Answer.Room->Getobject()] << std::endl;
			}
			else
			{
				std::cout << "nothing" << std::endl;
			}
			WriteNote();
			std::cout << "It is the end of your turn" << std::endl;
			return;

		}
		else if (input == "2" || input == "Convict")
		{
			Decide();
			Convict();
			return;
		}
		else if (input == "3" || input == "Check notes")
		{
			BasicNotes::printnote(this->notes);
		}
		else if (input == "4" || input == "Check Starting cards")
		{
			printStartingCards();
		}
		else
		{
			std::cout << "Invalid response" << std::endl;
		}
	}
}

CardSet RealPlayer::OnAsked(CardSet Selection, std::shared_ptr<Player> Asker)
{
	std::cout << "You have been asked" << std::endl;
	Selection.printCardSet();

	bool hasanswer = false;
	for (int i = 0; i < 9; i++)
	{
		if (StartingCards[i] != nullptr)
		{   // Should only evaluate the first of the and if it is nullptr so second part should be safe
			if ((StartingCards[i]->Person != nullptr && *StartingCards[i]->Person == *Selection.Person) 
				|| StartingCards[i]->Weapon != nullptr && *StartingCards[i]->Weapon == *Selection.Weapon
				|| StartingCards[i]->Room != nullptr && *StartingCards[i]->Room == *Selection.Room)
			{
				hasanswer = true;
				break;
			}
		}
	}
	if (hasanswer == true)
	{
		std::string input;
		while (true)
		{
			std::cout << "Pick a card to show to player " << this->GetPlayernum(Asker) << std::endl << "Your cards are:" << std::endl;
			this->printStartingCards();

			std::getline(std::cin, input);

			CardSet Answer;
			bool foundcard = false;

			for (int i = 0; i < 9; i++)
			{
				if (StartingCards[i] == nullptr)
					break;
				if (StartingCards[i]->Person != nullptr)
				{
					if (input == PersonCard::ObjectNames[StartingCards[i]->Person->Getobject()])
					{
						Answer.Person = StartingCards[i]->Person;
						if (*Answer.Person == *Selection.Person)
							return Answer;
						else
						{
							std::cout << "You must pick a card that they have asked for" << std::endl;
							foundcard = true;
							break;
						}
					}
				}
				else if (StartingCards[i]->Weapon != nullptr)
				{
					if (input == WeaponCard::ObjectNames[StartingCards[i]->Weapon->Getobject()])
					{
						Answer.Weapon = StartingCards[i]->Weapon;
						if (*Answer.Weapon == *Selection.Weapon)
							return Answer;
						else
						{
							std::cout << "You must pick a card that they have asked for" << std::endl;
							foundcard = true;
							break;
						}
					}
				}
				else if (StartingCards[i]->Room != nullptr)
				{
					if (input == RoomCard::ObjectNames[StartingCards[i]->Room->Getobject()])
					{
						Answer.Room = StartingCards[i]->Room;
						if (*Answer.Room == *Selection.Room)
							return Answer;
						else
						{
							std::cout << "You must pick a card that they have asked for" << std::endl;
							foundcard = true;
							break;
						}
					}
				}
			}
			if (foundcard == false)
				std::cout << "Card not found" << std::endl;
		}
	}
	else
	{
		std::cout << "You cannot answer this question" << std::endl;
		return CardSet();
	}
}

void RealPlayer::SetPlayers(std::vector<std::shared_ptr<Player>> players, int self)
{
	for (auto i = players.begin(); i != players.end(); i++)
	{
		this->Players.push_back(*i);
	}
	this->self = players[self];
}

void RealPlayer::printStartingCards()
{
	for (int i = 0; i < 9; i++)
	{
		if (StartingCards[i] == nullptr)
		{
			return;
		}
		else
		{
			if (StartingCards[i]->Person != nullptr)
				std::cout << PersonCard::ObjectNames[StartingCards[i]->Person->Getobject()] << std::endl;
			else if (StartingCards[i]->Weapon != nullptr)
				std::cout << WeaponCard::ObjectNames[StartingCards[i]->Weapon->Getobject()] << std::endl;
			else if (StartingCards[i]->Room != nullptr)
				std::cout << RoomCard::ObjectNames[StartingCards[i]->Room->Getobject()] << std::endl;
		}
	}
}

void RealPlayer::WriteNote()
{
	std::string input;
	while (true)
	{
		std::cout << R"(Pick an option.
	1. Write note (not the truth)
	2. Write culprit (the truth)
	3. Stop writing notes)" << std::endl;

		std::getline(std::cin, input);
		
		if (input == "1" || input == "Write note")
		{
			while (true)
			{
				std::cout << "Enter a person, weapon, room or cancel" << std::endl;
				std::getline(std::cin, input);
				if (input == "cancel")
				{
					std::cout << "cancelled writing" << std::endl;
				}
				else
				{
					bool hasresponded = false;
					for (int i = 0; i < 6; i++) // Person
					{
						if (input == PersonCard::ObjectNames[i])
						{
							notes.WriteNote(PersonCard(i));
							std::cout << "Wrote " << PersonCard::ObjectNames[i] << std::endl;
							hasresponded = true;
							break;
						}
					}
					if (hasresponded == true)
						break;
					for (int i = 0; i < 6; i++) // Weapon
					{
						if (input == WeaponCard::ObjectNames[i])
						{
							notes.WriteNote(WeaponCard(i));
							std::cout << "Wrote " << WeaponCard::ObjectNames[i] << std::endl;
							hasresponded = true;
							break;
						}
					}
					if (hasresponded = true)
						break;
					for (int i = 0; i < 9; i++) // Room
					{
						if (input == RoomCard::ObjectNames[i])
						{
							notes.WriteNote(RoomCard(i));
							std::cout << "Wrote " << RoomCard::ObjectNames[i] << std::endl;
							hasresponded = true;
							break;
						}
					}
					if (hasresponded == true)
						break;
					else
						std::cout << "Invalid response" << std::endl;
				}
			}
		}
		else if (input == "2" || input == "Write culprit")
		{
			while (true)
			{
				std::cout << "Enter a person, weapon, room or cancel" << std::endl;
				std::getline(std::cin, input);
				if (input == "cancel")
				{
					std::cout << "cancelled writing" << std::endl;
				}
				else
				{
					bool hasresponded = false;
					for (int i = 0; i < 6; i++) // Person
					{
						if (input == PersonCard::ObjectNames[i])
						{
							notes.WriteCulprit(PersonCard(i));
							std::cout << "Wrote " << PersonCard::ObjectNames[i] << std::endl;
							hasresponded = true;
							break;
						}
					}
					if (hasresponded == true)
						break;
					for (int i = 0; i < 6; i++) // Weapon
					{
						if (input == WeaponCard::ObjectNames[i])
						{
							notes.WriteCulprit(WeaponCard(i));
							std::cout << "Wrote " << WeaponCard::ObjectNames[i] << std::endl;
							hasresponded = true;
							break;
						}
					}
					if (hasresponded == true)
						break;
					for (int i = 0; i < 9; i++) // Room
					{
						if (input == RoomCard::ObjectNames[i])
						{
							notes.WriteCulprit(RoomCard(i));
							std::cout << "Wrote " << RoomCard::ObjectNames[i] << std::endl;
							hasresponded = true;
							break;
						}
					}
					if (hasresponded == true)
						break;
					else
						std::cout << "Invalid response" << std::endl;
				}
			}
		}
		else if (input == "3" || input == "Stop writing notes")
		{
			std::cout << "Stopping writing" << std::endl;
			break;
		}
		else
		{
			std::cout << "Invalid response" << std::endl;
		}
	}
}

void RealPlayer::Decide()
{
	std::string input;

	while (true)
	{
		while (true)
		{
			std::cout << R"(Enter a the name of Person. Or
	1. Check notes
	2. Check Starting cards)" << std::endl;

			bool haschosen = false;
			std::getline(std::cin, input);
			for (int i = 0; i < 6; i++)
			{
				if (input == PersonCard::ObjectNames[i])
				{
					this->decisions.AskSelection.Person = CardSet(PersonCard(i)).Person;
					std::cout << "You have selected " << PersonCard::ObjectNames[i] << std::endl;
					haschosen = true;
					break;
				}
			}
			if (haschosen == true)
			{
				break;
			}
			else if (input == "1" || input == "Check notes")
			{
				BasicNotes::printnote(this->notes);
			}
			else if (input == "2" || input == "Check Starting cards")
			{
				printStartingCards();
			}
			else
			{
				std::cout << "Invalid response" << std::endl;
			}
		}

		while (true)
		{
			std::cout << R"(Enter a the name of Weapon. Or
	1. Check notes
	2. Check Starting cards)" << std::endl;

			bool haschosen = false;
			std::getline(std::cin, input);
			for (int i = 0; i < 6; i++)
			{
				if (input == WeaponCard::ObjectNames[i])
				{
					this->decisions.AskSelection.Weapon = CardSet(WeaponCard(i)).Weapon;
					std::cout << "You have selected " << WeaponCard::ObjectNames[i] << std::endl;
					haschosen = true;
					break;
				}
			}
			if (haschosen == true)
			{
				break;
			}
			else if (input == "1" || input == "Check notes")
			{
				BasicNotes::printnote(this->notes);
			}
			else if (input == "2" || input == "Check Starting cards")
			{
				printStartingCards();
			}
			else
			{
				std::cout << "Invalid response" << std::endl;
			}
		}

		while (true)
		{
			std::cout << R"(Enter a the name of Room. Or
	1. Check notes
	2. Check Starting cards)" << std::endl;

			bool haschosen = false;
			std::getline(std::cin, input);
			for (int i = 0; i < 9; i++)
			{
				if (input == RoomCard::ObjectNames[i])
				{
					this->decisions.AskSelection.Room = CardSet(RoomCard(i)).Room;
					std::cout << "You have selected " << RoomCard::ObjectNames[i] << std::endl;
					haschosen = true;
					break;
				}
			}
			if (haschosen == true)
			{
				break;
			}
			else if (input == "1" || input == "Check notes")
			{
				BasicNotes::printnote(this->notes);
			}
			else if (input == "2" || input == "Check Starting cards")
			{
				printStartingCards();
			}
			else
			{
				std::cout << "Invalid response" << std::endl;
			}
		}

		while (true)
		{ 
			std::cout << "Are you sure you want to select these? y/n" << std::endl;
			std::getline(std::cin, input);
			if (input == "y" || input == "yes")
				return;
			else if (input == "n" || input == "no")
			{
				break;
			}
		}
		
	}
	
}

void RealPlayer::Win()
{
	std::cout << "Congratulations! You have won!" << std::endl;
	wins++;
}

void RealPlayer::Lose()
{
	this->bHasLost = false;
	std::cout << "Convict was incorrect. You have lost." << std::endl;
}

int RealPlayer::GetPlayernum(std::shared_ptr<Player> player)
{
	int i = 0;
	for (auto j = this->Players.begin(); j != this->Players.end(); i++, j++)
	{
		if (*j == player)
			return (i + 1);
	}
	return -1;
}
