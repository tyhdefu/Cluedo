use crate::card::{Card, PersonOptions, WeaponOptions, RoomOptions};
use std::hash::Hash;
use rand::{RngCore, Rng};

#[derive(Debug)]
pub struct CardTypeNotes<T : Eq + Hash + Clone> {
    possible: Vec<T>,
    cleared: Vec<T>,
}

impl<T : Eq + Hash + Clone> CardTypeNotes<T> {
    pub fn new(suspects: &Vec<T>) -> CardTypeNotes<T> {
        CardTypeNotes {
            possible: suspects.clone(),
            cleared: vec![],
        }
    }

    pub fn get_remaining(&self) -> &Vec<T> {
        &self.possible
    }

    pub fn get_random_remaining(&self, rng: &mut impl Rng) -> T {
        self.possible[(rng.next_u32() as usize) % self.possible.len()].clone()
    }

    pub fn amount_remaining(&self) -> usize {
        self.possible.len()
    }

    pub fn answer_found(&self) -> bool {
        self.possible.len() == 1
    }

    fn is_possible(&self, value: &T) -> bool {
        self.possible.contains(value)
    }

    fn mark_not(&mut self, not_answer: &T) {
        //assert!(self.possible.contains(not_answer));
        self.possible.iter().position(|n| n == not_answer)
            .map(|x| self.cleared.push(self.possible.swap_remove(x)));
    }

    fn mark_answer(&mut self, answer: &T) {
        //assert!(self.possible.contains(answer));

        for x in self.possible.drain(0..self.possible.len()) {
            if x != *answer {
                self.cleared.push(x);
            }
        }
        self.possible.clear();
        self.possible.push(answer.clone());
    }

    fn reset(&mut self) {
        for item in self.cleared.drain(..) {
            self.possible.push(item);
        }
    }
}

pub trait Notes {
    fn is_possible(&self, card: &Card) -> bool;

    fn mark_not(&mut self, card: &Card);

    fn mark_answer(&mut self, card: &Card);

    fn reset(&mut self);
}

#[derive(Debug)]
pub struct PWRNotes {
    person: CardTypeNotes<PersonOptions>,
    weapon: CardTypeNotes<WeaponOptions>,
    room: CardTypeNotes<RoomOptions>,
}

impl PWRNotes {

    pub fn new(persons: &Vec<PersonOptions>, weapons: &Vec<WeaponOptions>, rooms: &Vec<RoomOptions>) -> PWRNotes {
        PWRNotes {
            person: CardTypeNotes::new(persons),
            weapon: CardTypeNotes::new(weapons),
            room: CardTypeNotes::new(rooms),
        }
    }

    pub fn person(&self) -> &CardTypeNotes<PersonOptions> {
        &self.person
    }

    pub fn weapon(&self) -> &CardTypeNotes<WeaponOptions> {
        &self.weapon
    }

    pub fn room(&self) -> &CardTypeNotes<RoomOptions> {
        &self.room
    }

    pub fn answer_found(&self) -> bool {
        self.person.answer_found()
            && self.weapon.answer_found()
            && self.room.answer_found()
    }
}

impl Notes for PWRNotes {
    fn is_possible(&self, card: &Card) -> bool {
        match card {
            Card::Person(o) => self.person.is_possible(o),
            Card::Weapon(o) => self.weapon.is_possible(o),
            Card::Room(o) => self.room.is_possible(o),
            _ => panic!("Don't know how to deal with card type: {:?}", card)
        }
    }

    fn mark_not(&mut self, card: &Card) {
        match card {
            Card::Person(o) => self.person.mark_not(o),
            Card::Weapon(o) => self.weapon.mark_not(o),
            Card::Room(o) => self.room.mark_not(o),
            _ => panic!("Don't know how to deal with card type: {:?}", card)
        }
    }

    fn mark_answer(&mut self, card: &Card) {
        match card {
            Card::Person(o) => self.person.mark_answer(o),
            Card::Weapon(o) => self.weapon.mark_answer(o),
            Card::Room(o) => self.room.mark_answer(o),
            _ => panic!("Don't know how to deal with card type: {:?}", card)
        }
    }

    fn reset(&mut self) {
        self.person.reset();
        self.weapon.reset();
        self.room.reset();
    }
}