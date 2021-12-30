use std::hash::Hash;

use rand::Rng;

use super::CardTypeNoteTracker;

#[derive(Debug, Clone)]
pub struct VecCardTypeNotes<T : Eq + Hash + Clone> {
    possible: Vec<T>,
    cleared: Vec<T>,
}

impl<T : Eq + Hash + Clone> VecCardTypeNotes<T> {
    pub fn new(suspects: &Vec<T>) -> VecCardTypeNotes<T> {
        VecCardTypeNotes {
            possible: suspects.clone(),
            cleared: vec![],
        }
    }
}

impl <T: Eq + Hash + Clone> CardTypeNoteTracker<T> for VecCardTypeNotes<T> {
    
    fn get_remaining(&self) -> Vec<T> {
        self.possible.clone()
    }

    fn get_random_remaining(&self, rng: &mut impl Rng) -> T {
        self.possible[(rng.next_u32() as usize) % self.possible.len()].clone()
    }

    fn amount_remaining(&self) -> usize {
        self.possible.len()
    }

    fn answer_found(&self) -> bool {
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