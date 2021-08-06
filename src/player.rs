use crate::player_move::{Move, MoveResult};
use crate::card::{CardSet, PWRCardSet, Card};
use crate::notes::{Notes, PWRNotes};
use crate::card::Card::{Person, Weapon, Room};
use rand::rngs::{SmallRng, OsRng};
use rand::SeedableRng;
use crate::card_store::{CardStore, PWRCardStore};
use std::marker::PhantomData;

pub trait Player<T : CardSet> {
    fn give_card(&mut self, card: Card);

    fn do_turn(&mut self) -> Move<T>;

    fn ask(&self, question: &T) -> Option<Card>;

    fn receive_turn_result(&mut self, result: &MoveResult<T>);

    fn reset(&mut self);
}

pub struct RegularPlayer<CS : CardSet, C : CardStore<CS>, N : Notes> {
    starting_cards: C,
    notes: N,
    rng: SmallRng,
    phantom: PhantomData<CS>,
}

impl<CS : CardSet, C : CardStore<CS>, T : Notes> RegularPlayer<CS, C, T> {
    pub fn new(card_store: C, notes: T) -> RegularPlayer<CS, C, T> {
        RegularPlayer {
            starting_cards: card_store,
            notes,
            rng: SmallRng::from_rng(OsRng).unwrap(),
            phantom: PhantomData::default(),
        }
    }
}

impl Player<PWRCardSet> for RegularPlayer<PWRCardSet, PWRCardStore, PWRNotes> {

    fn give_card(&mut self, card: Card) {
        self.notes.mark_not(&card);
        self.starting_cards.push(card);
    }

    fn do_turn(&mut self) -> Move<PWRCardSet> {
        let person = self.notes.person().get_random_remaining(&mut self.rng);
        let weapon = self.notes.weapon().get_random_remaining(&mut self.rng);
        let room   = self.notes.room().get_random_remaining(&mut self.rng);
        let card_set = PWRCardSet::new(person, weapon, room);

        //println!("----");
        //println!("Player cards: {:?}", self.starting_cards);
        //println!("Player notes: {:?}", self.notes);
        //println!("Asking about: {:?}", card_set);

        if self.notes.answer_found() {
            Move::Accuse(card_set)
        }
        else {
            Move::Ask(card_set)
        }
    }

    fn ask(&self, question: &PWRCardSet) -> Option<Card> {
        self.starting_cards.has_any_from_card_set(question)
    }

    fn receive_turn_result(&mut self, result: &MoveResult<PWRCardSet>) {
        //println!("Received response: {:?}", result);
        match result {
            MoveResult::Ask(question, answer) => {
                match answer {
                    Some(card) => self.notes.mark_not(&card),
                    None => {
                        for x in question.clone() {
                            self.notes.mark_answer(&x);
                        }
                    }
                }
            }
            MoveResult::Accuse(_) => {}
        }
    }

    fn reset(&mut self) {
        self.starting_cards.clear();
        self.notes.reset();
    }
}