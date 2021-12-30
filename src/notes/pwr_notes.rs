use crate::card::{Card, PersonOptions, RoomOptions, WeaponOptions};

use super::{CardTypeNoteTracker, Notes};

#[derive(Debug)]
pub struct PWRNotes<P, W, R>
where
    P: CardTypeNoteTracker<PersonOptions>,
    W: CardTypeNoteTracker<WeaponOptions>,
    R: CardTypeNoteTracker<RoomOptions>,
{
    person: P,
    weapon: W,
    room: R,
}

impl<P, W, R> PWRNotes<P, W, R>
where
    P: CardTypeNoteTracker<PersonOptions>,
    W: CardTypeNoteTracker<WeaponOptions>,
    R: CardTypeNoteTracker<RoomOptions>,
{
    pub fn new(persons: P, weapons: W, rooms: R) -> Self {
        PWRNotes {
            person: persons,
            weapon: weapons,
            room: rooms,
        }
    }

    pub fn person(&self) -> &P {
        &self.person
    }

    pub fn weapon(&self) -> &W {
        &self.weapon
    }

    pub fn room(&self) -> &R {
        &self.room
    }

    pub fn answer_found(&self) -> bool {
        self.person.answer_found() && self.weapon.answer_found() && self.room.answer_found()
    }
}

impl<P, W, R> Notes for PWRNotes<P, W, R>
where
    P: CardTypeNoteTracker<PersonOptions>,
    W: CardTypeNoteTracker<WeaponOptions>,
    R: CardTypeNoteTracker<RoomOptions>,
{
    fn is_possible(&self, card: &Card) -> bool {
        match card {
            Card::Person(o) => self.person.is_possible(o),
            Card::Weapon(o) => self.weapon.is_possible(o),
            Card::Room(o) => self.room.is_possible(o),
            _ => panic!("Don't know how to deal with card type: {:?}", card),
        }
    }

    fn mark_not(&mut self, card: &Card) {
        match card {
            Card::Person(o) => self.person.mark_not(o),
            Card::Weapon(o) => self.weapon.mark_not(o),
            Card::Room(o) => self.room.mark_not(o),
            _ => panic!("Don't know how to deal with card type: {:?}", card),
        }
    }

    fn mark_answer(&mut self, card: &Card) {
        match card {
            Card::Person(o) => self.person.mark_answer(o),
            Card::Weapon(o) => self.weapon.mark_answer(o),
            Card::Room(o) => self.room.mark_answer(o),
            _ => panic!("Don't know how to deal with card type: {:?}", card),
        }
    }

    fn reset(&mut self) {
        self.person.reset();
        self.weapon.reset();
        self.room.reset();
    }
}

impl<P, W, R> Clone for PWRNotes<P, W, R>
where
    P: CardTypeNoteTracker<PersonOptions> + Clone,
    W: CardTypeNoteTracker<WeaponOptions> + Clone,
    R: CardTypeNoteTracker<RoomOptions> + Clone,
{
    fn clone(&self) -> Self {
        PWRNotes::new(self.person.clone(), self.weapon.clone(), self.room.clone())
    }
}
