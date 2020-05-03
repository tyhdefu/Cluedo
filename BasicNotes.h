#pragma once
#include "Card.h"
#include "table.h"
#include <string>
#include <iomanip>
#include <Windows.h>



class BasicNotes
{
public:	
	BasicNotes();
	~BasicNotes() {};
	

	void WriteNote(PersonCard Object);
	void WriteNote(WeaponCard Object);
	void WriteNote(RoomCard Object);

	void WriteCulprit(PersonCard Object);
	void WriteCulprit(WeaponCard Object);
	void WriteCulprit(RoomCard Object);

	bool ReadNote(PersonCard Object);
	bool ReadNote(WeaponCard Object);
	bool ReadNote(RoomCard Object);

	void Reset();

	static void printnote(BasicNotes note);

	CardSet Solution();

private:
	mytable<PersonCard, 6> Persons;
	mytable<WeaponCard, 6> Weapons;
	mytable<RoomCard, 9> Rooms;
};

