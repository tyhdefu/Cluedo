use super::card::{CardSet, Card};
use super::player::Player;
use crate::player::player_move::{Move, MoveResult};
use super::deck::{DeckCreator, PreDeck};
use std::marker::PhantomData;
use rand::rngs::{SmallRng, OsRng};
use rand::SeedableRng;

pub trait Game<T> {
    fn play(&mut self) -> GameResult;

    fn add_player(&mut self, player: Box<dyn Player<T>>);
}

pub struct CluedoGame<CS : CardSet, PD : PreDeck<CS>, DC : DeckCreator<CS, PD>> {
    players: Vec<Box<dyn Player<CS>>>,
    deck_creator: DC,
    phantom: PhantomData<PD>,
    rng: SmallRng
}

impl<CS : CardSet, PD : PreDeck<CS>, DC : DeckCreator<CS, PD>> CluedoGame<CS, PD, DC> {
    pub fn new(deck_creator: DC) -> CluedoGame<CS, PD, DC> {
        CluedoGame {
            players: vec![],
            deck_creator,
            phantom: Default::default(),
            rng: SmallRng::from_rng(OsRng).unwrap()
        }
    }

    fn reset(&mut self) {
        for i in 0..self.players.len() {
            self.players[i].reset();
        }
    }
}

impl<CS : CardSet, PD: PreDeck<CS>, DC : DeckCreator<CS, PD>> Game<CS> for CluedoGame<CS, PD, DC> {

    fn play(&mut self) -> GameResult {

        // Setup
        let mut pre_deck = self.deck_creator.create();

        let mut state = GameState {
            secret: pre_deck.take_secret_cards(&mut self.rng),
        };

        let mut deck = pre_deck.next_step(&mut self.rng);

        // Deal.
        
        while deck.len() >= self.players.len() {
            for i in 0..self.players.len() {
                let card = deck.take().unwrap();
                //state.player_cards[i].insert(card.clone());
                self.players[i].give_card(card)
            }
        }

        // Play

        let mut rounds = 0;
        let player_count = self.players.len();
        loop {
            //println!("Round: {}", rounds);
            for i in 0..player_count {
                //println!("Player: {}", i);
                match self.players[i].do_turn() {
                    Move::Ask(question) => {
                        let mut answered = false;
                        for j in 0..(player_count - 1) {
                            let to_ask_index = (i + j + 1) % player_count;
                            let response = self.players[to_ask_index].ask(&question);
                            match response {
                                Some(x) => {
                                    //assert!(state.player_cards[to_ask_index].contains(&x));
                                    let result = MoveResult::Ask(question.clone(), Some(x));
                                    self.players[i].receive_turn_result(&result);
                                    answered = true;
                                    break;
                                }
                                None => {
                                    for card in question.clone().into_iter() {
                                        //assert!(!state.player_cards[to_ask_index].contains(&card), "Player {} had card {:?} but didn't show anything when asked", j, &card)
                                    }
                                }
                            }
                        }
                        if !answered {
                            let result = MoveResult::Ask(question.clone(), None);
                            self.players[i].receive_turn_result(&result);
                            break;
                        }
                    }
                    Move::Accuse(accusation) => {
                        if accusation == state.secret {
                            //println!("Player {} won", i);
                            self.reset();
                            return GameResult::Win(i);
                        }
                        //println!("Player {} guessed incorrectly and lost!\nGuess:({:?}\nSecret:{:?}", i, accusation, state.secret);
                        self.reset();
                        return GameResult::Lose(i)
                    }
                }
            }
            rounds += 1;
            if rounds > 10_000 {
                panic!("More than 10k rounds have passed and no one has won")
            }
        }
    }

    fn add_player(&mut self, player: Box<dyn Player<CS>>) {
        self.players.push(player);
    }
}

/*impl Game for CluedoGame<PWRCardSet> {

    fn play(&self) {
        let secret = PWRCardSet::new(Person(Peacock), Weapon(Dagger), Room(Living));
        let state = GameState {
            secret
        };

    }

    fn add_player(&mut self) {
        todo!()
    }
}*/

pub struct GameState<CS : CardSet> {
    secret: CS,
//    player_cards: Vec<HashSet<Card>>
}

#[derive(Debug)]
pub enum GameResult {
    Win(usize),
    Lose(usize),
}