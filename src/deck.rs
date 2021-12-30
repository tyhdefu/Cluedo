use super::card::{CardSet, PWRCardSet, Card, PersonOptions, WeaponOptions, RoomOptions};
use rand::rngs::{OsRng, SmallRng};
use rand::{RngCore, Rng};
use rand::seq::SliceRandom;
use super::card::Card::{Person, Room, Weapon};

// Deck Creator

pub trait DeckCreator<CS : CardSet, D : PreDeck<CS>> {
    fn create(&self) -> D;
}

pub struct PWRDeckCreator {
    persons: Vec<PersonOptions>,
    weapons: Vec<WeaponOptions>,
    rooms:   Vec<RoomOptions>,
}

impl PWRDeckCreator {
    pub fn new(persons: &Vec<PersonOptions>, weapons: &Vec<WeaponOptions>, rooms: &Vec<RoomOptions>) -> PWRDeckCreator {
        PWRDeckCreator {
            persons: persons.clone(),
            weapons: weapons.clone(),
            rooms: rooms.clone(),
        }
    }
}

impl DeckCreator<PWRCardSet, PWRPreDeck> for PWRDeckCreator {
    fn create(&self) -> PWRPreDeck {
        PWRPreDeck {
            persons: self.persons.clone(),
            weapons: self.weapons.clone(),
            rooms:   self.rooms.clone(),
        }
    }
}

// Pre Deck

pub trait PreDeck<CS : CardSet> {
    fn take_secret_cards(&mut self, rng: &mut impl Rng) -> CS;

    fn next_step(self, rng: &mut impl Rng) -> Deck;
}

pub struct PWRPreDeck {
    persons: Vec<PersonOptions>,
    weapons: Vec<WeaponOptions>,
    rooms:   Vec<RoomOptions>,
}

impl PreDeck<PWRCardSet> for PWRPreDeck {
    fn take_secret_cards(&mut self, rng: &mut impl Rng) -> PWRCardSet {
        PWRCardSet::new(
            self.persons.swap_remove((rng.next_u32() as usize) % self.persons.len()),
            self.weapons.swap_remove((rng.next_u32() as usize) % self.weapons.len()),
            self.rooms.swap_remove((rng.next_u32() as usize) % self.rooms.len()),
        )
    }

    fn next_step(self, rng: &mut impl Rng) -> Deck {
        let mut combined : Vec<Card> = self.persons.into_iter().map(|x| Person(x))
            .chain(self.weapons.into_iter().map(|x| Weapon(x)))
            .chain(self.rooms.into_iter().map(|x| Room(x)))
            .collect();

        combined.shuffle(rng);
        Deck {
            cards: combined
        }
    }
}

// Deck

pub struct Deck {
    cards: Vec<Card>
}

impl Deck {

    pub fn take(&mut self) -> Option<Card> {
        self.cards.pop()
    }

    pub fn len(&self) -> usize {
        self.cards.len()
    }
}