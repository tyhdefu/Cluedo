use crate::card::{CardSet, PersonOptions, WeaponOptions, RoomOptions, Card, PWRCardSet};
use crate::card::Card::{Person, Weapon, Room};

pub trait CardStore<CS : CardSet> {
    fn has(&self, card: &Card) -> bool;

    fn has_any_from_card_set(&self, card_set: &CS) -> Option<Card>;

    fn push(&mut self, card: Card);

    fn clear(&mut self);
}

pub struct PWRCardStore {
    person: Vec<PersonOptions>,
    weapon: Vec<WeaponOptions>,
    room: Vec<RoomOptions>,
}

impl PWRCardStore {
    pub fn new() -> PWRCardStore {
        PWRCardStore {
            person: vec![],
            weapon: vec![],
            room: vec![],
        }
    }
}

impl CardStore<PWRCardSet> for PWRCardStore {
    fn has(&self, card: &Card) -> bool {
        match card {
            Person(x) => self.person.contains(x),
            Weapon(x) => self.weapon.contains(x),
            Room(x) => self.room.contains(x),
        }
    }

    fn has_any_from_card_set(&self, card_set: &PWRCardSet) -> Option<Card> {
        let person = card_set.person();
        if self.person.contains(&person) {
            return Some(Person(person))
        }
        let weapon = card_set.weapon();
        if self.weapon.contains(&weapon) {
            return Some(Weapon(weapon))
        }
        let room = card_set.room();
        if self.room.contains(&room) {
            return Some(Room(room))
        }
        return None
    }

    fn push(&mut self, card: Card) {
        match card {
            Person(x) => self.person.push(x),
            Weapon(x) => self.weapon.push(x),
            Room(x) => self.room.push(x),
            _ => panic!("Don't know how to store {:?}", card)
        }
    }

    fn clear(&mut self) {
        self.person.clear();
        self.weapon.clear();
        self.room.clear();
    }
}