#pragma once
#include <string>

class Card
{
public:
	enum Type { Person = 0, Weapon, Room };

	Type GetType();
	
protected:
	Type CardType;
};



class PersonCard : public Card
{
public:
	enum Object { Scarlett = 0, Green, Mustard, Plum, Peacock, White};
	static const std::string ObjectNames[];

	PersonCard() {};
	PersonCard(int object);
	PersonCard(Object object);
	~PersonCard();

	PersonCard::Object Getobject() const;

	bool operator==(const PersonCard& rhs) const
	{
		if (this->Getobject() == rhs.Getobject())
		{
			return true;
		}
		else
			return false;
	}
	bool operator!=(const PersonCard& rhs) const
	{
		if (*this == rhs)
			return false;
		else
			return true;
	}
private:
	static const Type CardType = Type::Person;
	Object object;
};




class WeaponCard : public Card
{
public:
	enum Object {Candlestick = 0, Dagger, Leadpipe, Revolver, Rope, Wrench};
	static const std::string ObjectNames[];

	WeaponCard() {};
	WeaponCard(int object);
	WeaponCard(Object object);
	~WeaponCard();

	WeaponCard::Object Getobject() const;

	bool operator==(const WeaponCard& rhs) const
	{
		if (this->Getobject() == rhs.Getobject())
		{
			return true;
		}
		else
			return false;
	}
	bool operator!=(const WeaponCard& rhs) const
	{
		if (*this == rhs)
			return false;
		else
			return true;
	}
private:
	static const Type CardType = Type::Weapon;
	Object object;
};

class RoomCard : public Card
{
public:
	enum Object {Bedroom = 0, Bathroom, Study, LivingRoom, DiningRoom, Kitchen, Courtyard, Garage, GamesRoom};
	static const std::string ObjectNames[];

	RoomCard() {};
	RoomCard(int object);
	RoomCard(Object object);
	~RoomCard();

	RoomCard::Object Getobject() const;

	bool operator==(const RoomCard& rhs) const
	{
		if (this->Getobject() == rhs.Getobject())
		{
			return true;
		}
		else
			return false;
	}
	bool operator!=(const RoomCard& rhs) const
	{
		if (*this == rhs)
			return false;
		else
			return true;
	}
private:
	static const Type CardType = Type::Room;
	Object object;
};

// One of each kind of card
class CardSet 
{
public:
	
	CardSet();

	CardSet(PersonCard Person);
	CardSet(WeaponCard Weapon);
	CardSet(RoomCard Room);

	CardSet(std::shared_ptr<PersonCard> Person);
	CardSet(std::shared_ptr<WeaponCard> Weapon);
	CardSet(std::shared_ptr<RoomCard> Room);

	CardSet(PersonCard Person, WeaponCard Weapon, RoomCard Room);

	std::shared_ptr<PersonCard> Person = nullptr;
	std::shared_ptr<WeaponCard> Weapon = nullptr;
	std::shared_ptr<RoomCard> Room = nullptr;

	void printCardSet() const;
	bool ContainsOneCard() const;

	bool operator==(const CardSet& rhs) const;
	bool operator!=(const CardSet& rhs) const
	{
		if (*this == rhs)
			return false;
		else
			return true;
	}
};


