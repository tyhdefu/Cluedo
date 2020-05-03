#include "pch.h"
#include "BasicNotes.h"
#include <iostream>


BasicNotes::BasicNotes()
{
	this->Persons = false;
	this->Weapons = false;
	this->Rooms = false;
	for (int i = 0; i < 6; i++)
	{
		this->Persons.setkey(static_cast<PersonCard::Object>(i), i);
	}
	for (int i = 0; i < 6; i++)
	{
		this->Weapons.setkey(static_cast<WeaponCard::Object>(i), i);
	}
	for (int i = 0; i < 9; i++)
	{
		this->Rooms.setkey(static_cast<RoomCard::Object>(i), i);
	}
}

void BasicNotes::WriteNote(PersonCard card)
{		
	this->Persons.Write(card, true);
}

void BasicNotes::WriteNote(WeaponCard card)
{
	this->Weapons.Write(card, true);
}

void BasicNotes::WriteNote(RoomCard card)
{
	this->Rooms.Write(card, true);
}

void BasicNotes::WriteCulprit(PersonCard Object)
{
	this->Persons.setallbools(true);
	this->Persons.Write(Object, false);
}

void BasicNotes::WriteCulprit(WeaponCard Object)
{
	this->Weapons.setallbools(true);
	this->Weapons.Write(Object, false);
}

void BasicNotes::WriteCulprit(RoomCard Object)
{
	this->Rooms.setallbools(true);
	this->Rooms.Write(Object, false);
}


bool BasicNotes::ReadNote(PersonCard card)
{
	return this->Persons.Read(card);
}

bool BasicNotes::ReadNote(WeaponCard card)
{
	return this->Weapons.Read(card);
}

bool BasicNotes::ReadNote(RoomCard card)
{
	return this->Rooms.Read(card);
}

void BasicNotes::Reset()
{
	this->Persons.setallbools(false);
	this->Weapons.setallbools(false);
	this->Rooms.setallbools(false);
}

void BasicNotes::printnote(BasicNotes note)
{
	// Set booleans to output false instead of 0
	std::cout << std::boolalpha;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	std::cout << "Person Notes:\n";
	for (int i = 0; i < 6; i++)
	{
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << std::left << std::setw(14) << PersonCard::ObjectNames[i];
		if (note.ReadNote(PersonCard(i)) == true)
			SetConsoleTextAttribute(hConsole, 2);
		else
			SetConsoleTextAttribute(hConsole, 4);

		std::cout << std::setw(5) << std::fixed << note.ReadNote(PersonCard(i)) << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Weapon Notes:\n";
	for (int i = 0; i < 6; i++)
	{
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << std::left << std::setw(14) << WeaponCard::ObjectNames[i];
		if (note.ReadNote(WeaponCard(i)) == true)
			SetConsoleTextAttribute(hConsole, 2);
		else
			SetConsoleTextAttribute(hConsole, 4);

		std::cout << std::setw(5) << std::fixed << note.ReadNote(WeaponCard(i)) << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
	std::cout << "Room Notes:\n";
	for (int i = 0; i < 9; i++)
	{
		SetConsoleTextAttribute(hConsole, 3);
		std::cout << std::left << std::setw(14) << RoomCard::ObjectNames[i];
		if (note.ReadNote(RoomCard(i)) == true)
			SetConsoleTextAttribute(hConsole, 2);
		else
			SetConsoleTextAttribute(hConsole, 4);

		std::cout << std::setw(5) << std::fixed << note.ReadNote(RoomCard(i)) << std::endl;
	}
	SetConsoleTextAttribute(hConsole, 7);
}

CardSet BasicNotes::Solution()
{
	PersonCard P;
	WeaponCard W;
	RoomCard R;
	
	for (int i = 0; i < 6; i++)
	{
		if (this->Persons.Read(PersonCard(i)) == false)
			if (P == PersonCard())
				P = PersonCard(i);
			else
				return CardSet();
	}
	for (int i = 0; i < 6; i++)
	{
		if (this->Weapons.Read(WeaponCard(i)) == false)
			if (W == WeaponCard())
				W = WeaponCard(i);
			else
				return CardSet();
	}
	for (int i = 0; i < 9; i++)
	{
		if (this->Rooms.Read(RoomCard(i)) == false)
			if (R == RoomCard())
				R = RoomCard(i);
			else
				return CardSet();
	}
	// If only one of each, return the set of cards
	return CardSet(P, W, R);
}