pub mod vec_card_type_notes;
pub mod hashmap_card_type_notes;
pub mod pwr_notes;

use crate::card::{Card, PersonOptions, WeaponOptions, RoomOptions};
use std::hash::Hash;
use rand::{RngCore, Rng};

pub trait Notes {
    fn is_possible(&self, card: &Card) -> bool;

    fn mark_not(&mut self, card: &Card);

    fn mark_answer(&mut self, card: &Card);

    fn reset(&mut self);
}

pub trait CardTypeNoteTracker<T> {

    fn get_remaining(&self) -> Vec<T>;

    fn get_random_remaining(&self, rng: &mut impl Rng) -> T;

    fn amount_remaining(&self) -> usize;

    fn answer_found(&self) -> bool;

    fn is_possible(&self, value: &T) -> bool;

    fn mark_not(&mut self, not_answer: &T);

    fn mark_answer(&mut self, answer: &T);

    fn reset(&mut self);
}