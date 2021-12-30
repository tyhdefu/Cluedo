use crate::card::{Card, CardSet};

pub enum Move<CS : CardSet> {
    Ask(CS),
    Accuse(CS),
}

#[derive(Debug)]
pub enum MoveResult<CS : CardSet> {
    Ask(CS, Option<Card>),
    Accuse(bool)
}