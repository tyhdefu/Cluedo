use card::PersonOptions::{Scarlet, Mustard, White, Green, Peacock, Plum};
use card::WeaponOptions::{Dagger, Wrench, Pistol, Rope, LeadPipe, Candlestick};
use card::RoomOptions::{Bedroom, Bathroom, Study, LivingRoom, DiningRoom, Kitchen, Courtyard, Garage, GamesRoom};
use game::{CluedoGame, Game, GameResult};
use deck::PWRDeckCreator;
use crate::notes::pwr_notes::PWRNotes;
use crate::notes::vec_card_type_notes::VecCardTypeNotes;
use player::RegularPlayer;
use std::time::Instant;
use card::card_store::PWRCardStore;
use rand::rngs::{SmallRng, OsRng};
use rand::{SeedableRng, RngCore};
use core::mem;

mod card;
mod notes;
mod player;
mod deck;
mod game;

fn main() {

    println!("Hello, world!");

    let persons = vec![Scarlet, Mustard, White, Green, Peacock, Plum];
    let weapons = vec![Candlestick, Dagger, LeadPipe, Pistol, Rope, Wrench];
    let rooms = vec![Bedroom, Bathroom, Study, LivingRoom, DiningRoom, Kitchen, Courtyard, Garage, GamesRoom];

    let deck_creator = PWRDeckCreator::new(
        &persons,
        &weapons,
        &rooms
    );

    let notes = PWRNotes::new(
        VecCardTypeNotes::new(&persons),
        VecCardTypeNotes::new(&weapons),
        VecCardTypeNotes::new(&rooms),
    );
    let player1 = RegularPlayer::new(PWRCardStore::new(), notes.clone());
    let player2 = RegularPlayer::new(PWRCardStore::new(), notes.clone());
    let player3 = RegularPlayer::new(PWRCardStore::new(), notes);

    let mut game = CluedoGame::new(
        deck_creator,
    );
    game.add_player(Box::new(player1));
    game.add_player(Box::new(player2));
    game.add_player(Box::new(player3));

    let mut win_counts = [0; 3];

    let start = Instant::now();

    let games = 10_000_000;
    for i in 0..games {
        let result = game.play();
        match result {
            GameResult::Win(player) => win_counts[player] += 1,
            GameResult::Lose(player) => panic!("Player {} lost", player),
        }
        if i % 100_000 == 0 {
            println!("{} games played", i);
        }
    }
    let duration = start.elapsed();
    println!("Took: {:?}", duration);
    println!("Win counts: {:?}", win_counts);
    println!("{} games / second", games as f32 / duration.as_secs_f32());
    println!("{} nanoseconds per game", duration.as_nanos() / games);
    //println!("Result: {:?}", result);
}
