#include "pch.h"
#include "SmartNotes.h"
#include <Windows.h>

void SmartNotes::printShownCards()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 3); // light blue
	std::cout << "ShownCards vector:";
	int i = 0;
	for (auto j = ShownCards.valuearray.begin(); j != ShownCards.valuearray.end(); i++, j++)
	{
		SetConsoleTextAttribute(hConsole, 2); // green
		std::cout << std::endl  << "Player " << (i + 1) << ":";
		SetConsoleTextAttribute(hConsole, 7); // normal
		for (auto k = j->begin(); k != j->end(); k++)
		{
			if (k->ContainsOneCard())
			{
				if (k->Person != nullptr)
				{
					std::cout << "  " << PersonCard::ObjectNames[k->Person->Getobject()];
				}
				else if (k->Weapon != nullptr)
				{
					std::cout << "  " << WeaponCard::ObjectNames[k->Weapon->Getobject()];
				}
				else
				{
					std::cout << "  " << RoomCard::ObjectNames[k->Room->Getobject()];
				}
			}
		}
	}
	std::cout << std::endl << std::endl;
}

void SmartNotes::printKnownCards()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 3); // light blue
	std::cout << "KnownCards vector:";
	int i = 0;
	for (auto j = KnownCards.valuearray.begin(); j != KnownCards.valuearray.end(); i++, j++)
	{
		SetConsoleTextAttribute(hConsole, 2); // green
		std::cout << std::endl << "Player " << (i + 1) << ":";
		SetConsoleTextAttribute(hConsole, 7); // normal
		for (auto k = j->begin(); k != j->end(); k++)
		{
			if (k->ContainsOneCard())
			{
				if (k->Person != nullptr)
				{
					std::cout << "  " << PersonCard::ObjectNames[k->Person->Getobject()];
				}
				else if (k->Weapon != nullptr)
				{
					std::cout << "  " << WeaponCard::ObjectNames[k->Weapon->Getobject()];
				}
				else
				{
					std::cout << "  " << RoomCard::ObjectNames[k->Room->Getobject()];
				}
			}
		}
	}
	std::cout << std::endl << std::endl;
}

void SmartNotes::printSuspectedCards()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 3); // light blue
	std::cout << "SuspectedCards vector:";
	int i = 0;
	for (auto j = SuspectedCards.valuearray.begin(); j != SuspectedCards.valuearray.end(); i++, j++)
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN); // green
		std::cout << std::endl << "Player " << (i + 1) << ":" << std::endl;
		SetConsoleTextAttribute(hConsole, 7); // normal

		for (auto k = j->begin(); k != j->end(); k++)
		{
			
			if (k->Person != nullptr)
			{
				std::cout << "    " << PersonCard::ObjectNames[k->Person->Getobject()] << " OR";
			}
			if (k->Weapon != nullptr)
			{
				std::cout << "    " << WeaponCard::ObjectNames[k->Weapon->Getobject()];
				if (k->Room != nullptr)
					std::cout << " OR";

			}
			if (k->Room != nullptr)
			{
				std::cout << "    " << RoomCard::ObjectNames[k->Room->Getobject()];
			}

			std::cout << ";"<< std::endl;
		}
	}
	std::cout << std::endl << std::endl;
}

void SmartNotes::printNotOwnedCards()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 3); // light blue
	std::cout << "NotOwnedCards vector:";
	int i = 0;
	for (auto j = NotOwnedCards.valuearray.begin(); j != NotOwnedCards.valuearray.end(); i++, j++)
	{
		SetConsoleTextAttribute(hConsole, 2); // green
		std::cout << std::endl << "Player " << (i + 1) << ":" << std::endl;
		SetConsoleTextAttribute(hConsole, 7); // normal
		for (auto k = j->begin(); k != j->end(); k++)
		{
				if (k->Person != nullptr)
				{
					std::cout << "  " << PersonCard::ObjectNames[k->Person->Getobject()] << std::endl;
				}
				else if (k->Weapon != nullptr)
				{
					std::cout << "  " << WeaponCard::ObjectNames[k->Weapon->Getobject()] << std::endl;
				}
				else
				{
					std::cout << "  " << RoomCard::ObjectNames[k->Room->Getobject()] << std::endl;
				}
		}
	}
	std::cout << std::endl << std::endl;
}
