use crate::card::Card::{Person, Weapon, Room};
use std::fmt::Debug;

pub trait CardSet: PartialEq + IntoIterator<Item = Card> + Clone + Debug
{
    fn has(&self, card: &Card) -> bool;
}

pub struct CardSetIterator<CS : CardSet> {
    count: usize,
    source: CS,
}

#[derive(PartialEq, Eq, Hash, Clone)]
#[derive(Debug)]
pub enum Card {
    Person(PersonOptions),
    Weapon(WeaponOptions),
    Room(RoomOptions)
}

#[derive(PartialEq, Eq, Hash, Clone)]
#[derive(Debug)]
pub enum PersonOptions {
    Scarlet,
    Mustard,
    White,
    Green,
    Peacock,
    Plum,
}

#[derive(PartialEq, Eq, Hash, Clone)]
#[derive(Debug)]
pub enum WeaponOptions {
    Candlestick,
    Dagger,
    LeadPipe,
    Pistol,
    Rope,
    Wrench
}

#[derive(PartialEq, Eq, Hash, Clone)]
#[derive(Debug)]
pub enum RoomOptions {
    Bedroom,
    Bathroom,
    Study,
    LivingRoom,
    DiningRoom,
    Kitchen,
    Courtyard,
    Garage,
    GamesRoom,
}

// PWR Card Set

#[derive(Debug, Clone)]
pub struct PWRCardSet {
    person: PersonOptions,
    weapon: WeaponOptions,
    room: RoomOptions,
}

impl PWRCardSet {
    pub fn new(person: PersonOptions, weapon: WeaponOptions, room: RoomOptions) -> PWRCardSet {
        PWRCardSet {
            person,
            weapon,
            room,
        }
    }

    pub fn person(&self) -> PersonOptions {
        self.person.clone()
    }

    pub fn weapon(&self) -> WeaponOptions {
        self.weapon.clone()
    }

    pub fn room(&self) -> RoomOptions {
        self.room.clone()
    }
}

impl Iterator for CardSetIterator<PWRCardSet> {
    type Item = Card;

    fn next(&mut self) -> Option<Self::Item> {
        let next = match self.count {
            0 => Option::Some(Person(self.source.person.clone())),
            1 => Option::Some(Weapon(self.source.weapon.clone())),
            2 => Option::Some(Room(self.source.room.clone())),
            _ => Option::None,
        };
        self.count += 1;
        next
    }
}

impl IntoIterator for PWRCardSet {
    type Item = Card;
    type IntoIter = CardSetIterator<PWRCardSet>;

    fn into_iter(self) -> Self::IntoIter {
        CardSetIterator {count: 0, source:self}
    }
}

impl CardSet for PWRCardSet {
    fn has(&self, card: &Card) -> bool {
        match card {
            Person(x) => x == &self.person,
            Weapon(x) => x == &self.weapon,
            Room(x) => x == &self.room,
        }
    }
}

impl PartialEq for PWRCardSet {
    fn eq(&self, other: &Self) -> bool {
        self.person == other.person
        && self.weapon == other.weapon
        && self.room == other.room
    }
}