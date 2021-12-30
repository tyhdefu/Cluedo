use super::CardTypeNoteTracker;

use std::{collections::HashMap, hash::Hash};

pub struct HashMapCardTypeNotes<T>
where
    T: Hash + Eq,
{
    map: HashMap<T, bool>,
}

impl<T> HashMapCardTypeNotes<T>
    where T: Hash + Eq {
    
    pub fn new(suspects: Vec<T>) -> Self {
        HashMapCardTypeNotes {
            map: suspects.into_iter().map(|k| (k, false)).collect(),
        }
    }
}

impl<T> CardTypeNoteTracker<T> for HashMapCardTypeNotes<T>
where
    T: Hash + Eq + Clone,
{
    fn get_remaining(&self) -> Vec<T> {
        self.map.iter().filter(|(k,v)| **v == false).map(|(k,_v)| k.clone()).collect()
    }

    fn get_random_remaining(&self, rng: &mut impl rand::Rng) -> T {
        let vec = self.get_remaining();
        vec[(rng.next_u32() as usize) % vec.len()].clone()
    }

    fn amount_remaining(&self) -> usize {
        self.map.iter().filter(|(_k,v)| **v == false).count()
    }

    fn answer_found(&self) -> bool {
        self.amount_remaining() == 1
    }

    fn is_possible(&self, value: &T) -> bool {
        self.map[value] == false
    }

    fn mark_not(&mut self, not_answer: &T) {
        *self.map.get_mut(not_answer).unwrap() = true;
    }

    fn mark_answer(&mut self, answer: &T) {
        self.map.iter_mut().for_each(|(k,v)| if *k != *answer { *v = true });
    }

    fn reset(&mut self) {
        self.map.iter_mut().for_each(|(k,v)| *v = false);
    }
}
