#include "pch.h"
#include <iostream>
#include <memory>
#include "Card.h"


const std::string PersonCard::ObjectNames[] = { "Scarlett", "Green", "Mustard", "Plum", "Peacock", "White" };
const std::string WeaponCard::ObjectNames[] = { "Candlestick", "Dagger", "Leadpipe", "Revolver", "Rope", "Wrench"};
const std::string RoomCard::ObjectNames[] = { "Bedroom", "Bathroom", "Study", "LivingRoom", "DiningRoom", "Kitchen", "Courtyard", "Garage", "GamesRoom" };

Card::Type Card::GetType()
{
	return this->CardType;
}

PersonCard::PersonCard(int object)
{
	this->object = static_cast<PersonCard::Object>(object);
}

PersonCard::PersonCard(Object object)
{
	this->object = object;
}

PersonCard::~PersonCard()
{

}

PersonCard::Object PersonCard::Getobject() const
{
	return this->object;
}

WeaponCard::WeaponCard(int object)
{
	this->object = static_cast<WeaponCard::Object>(object);
}

WeaponCard::WeaponCard(WeaponCard::Object object)
{
	this->object = object;
}

WeaponCard::~WeaponCard()
{

}

WeaponCard::Object WeaponCard::Getobject() const
{
	return this->object;
}

RoomCard::RoomCard(int object)
{
	this->object = static_cast<RoomCard::Object>(object);
}

RoomCard::RoomCard(Object object)
{
	this->object = object;
}

RoomCard::~RoomCard()
{

}

RoomCard::Object RoomCard::Getobject() const
{
	return this->object;
}

CardSet::CardSet()
{
}

CardSet::CardSet(PersonCard Person)
{
	this->Person = std::make_shared<PersonCard>(Person.Getobject());
}

CardSet::CardSet(WeaponCard Weapon)
{
	this->Weapon = std::make_shared<WeaponCard>(WeaponCard(Weapon.Getobject()));
}

CardSet::CardSet(RoomCard Room)
{
	this->Person = nullptr;
	this->Weapon = nullptr;
	this->Room = std::make_shared<RoomCard>(RoomCard(Room.Getobject()));
}

CardSet::CardSet(std::shared_ptr<PersonCard> Person)
{
	this->Person = Person;
}

CardSet::CardSet(std::shared_ptr<WeaponCard> Weapon)
{
	this->Weapon = Weapon;
}

CardSet::CardSet(std::shared_ptr<RoomCard> Room)
{
	this->Room = Room;
}

CardSet::CardSet(PersonCard Person, WeaponCard Weapon, RoomCard Room)
{
	this->Person = std::make_shared<PersonCard>(Person.Getobject());
	this->Weapon = std::make_shared<WeaponCard>(WeaponCard(Weapon.Getobject()));
	this->Room = std::make_shared<RoomCard>(RoomCard(Room.Getobject()));
}



void CardSet::printCardSet() const
{
	std::cout << "CardSet contains" << "\n";

	if (this->Person != nullptr)
		std::cout << "Person: " << PersonCard::ObjectNames[(this->Person->Getobject())] << "\n";
	else
		std::cout << "Person: NULL" << "\n";

	if (this->Weapon != nullptr)
		std::cout << "Weapon: " << WeaponCard::ObjectNames[(this->Weapon->Getobject())] << "\n";
	else
		std::cout << "Weapon: NULL" << "\n";

	if (this->Room != nullptr)
		std::cout << "Rooms: " << RoomCard::ObjectNames[(this->Room->Getobject())] << "\n";
	else
		std::cout << "Rooms: NULL" << "\n";
}

bool CardSet::ContainsOneCard() const
{
	int i = 0;
	if (this->Person == nullptr)
		i++;
	if (this->Weapon == nullptr)
		i++;
	if (this->Room == nullptr)
		i++;

	if (i == 2)
		return true;
	else
		return false;
}

bool CardSet::operator==(const CardSet & rhs) const
{
	{
		if (this->Person == nullptr && rhs.Person == nullptr)
		{
			// they are equal
		}
		else if (this->Person == nullptr || rhs.Person == nullptr)
		{
			// one is nullptr
			return false;
		}
		else
		{
			if (*this->Person == *rhs.Person)
			{
				//equal
			}
			else
				return false;
		}
	}
	{
		if (this->Weapon == nullptr && rhs.Weapon == nullptr)
		{
			// they are equal
		}
		else if (this->Weapon == nullptr || rhs.Weapon == nullptr)
		{
			// one is nullptr
			return false;
		}
		else
		{
			if (*this->Weapon == *rhs.Weapon)
			{
				//equal
			}
			else
				return false;
		}
	}
	{
		if (this->Room == nullptr && rhs.Room == nullptr)
		{
			// they are equal
		}
		else if (this->Room == nullptr || rhs.Room == nullptr)
		{
			// one is nullptr
			return false;
		}
		else
		{
			if (*this->Room == *rhs.Room)
			{
				//equal
			}
			else
				return false;
		}
		return true;
	}
}
