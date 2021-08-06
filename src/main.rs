use crate::card::PersonOptions::{Scarlet, Mustard, White, Green, Peacock, Plum};
use crate::card::WeaponOptions::{Dagger, Wrench, Pistol, Rope, LeadPipe, Candlestick};
use crate::card::RoomOptions::{Bedroom, Bathroom, Study, LivingRoom, DiningRoom, Kitchen, Courtyard, Garage, GamesRoom};
use crate::game::{CluedoGame, Game, GameResult};
use crate::deck::PWRDeckCreator;
use crate::notes::PWRNotes;
use crate::player::RegularPlayer;
use std::time::Instant;
use crate::card_store::PWRCardStore;
use rand::rngs::{SmallRng, OsRng};
use rand::{SeedableRng, RngCore};
use core::mem;

mod card;
mod game;
mod player_move;
mod player;
mod notes;
mod deck;
mod card_store;

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

    let player1 = RegularPlayer::new(PWRCardStore::new(), PWRNotes::new(&persons, &weapons, &rooms));
    let player2 = RegularPlayer::new(PWRCardStore::new(), PWRNotes::new(&persons, &weapons, &rooms));
    let player3 = RegularPlayer::new(PWRCardStore::new(), PWRNotes::new(&persons, &weapons, &rooms));

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
