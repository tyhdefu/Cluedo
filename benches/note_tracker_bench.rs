use rust_cluedo::card::PersonOptions;
use rust_cluedo::{card::PersonOptions::*, notes::CardTypeNoteTracker};
use rust_cluedo::notes::{vec_card_type_notes::VecCardTypeNotes, hashmap_card_type_notes::HashMapCardTypeNotes};
use criterion::{black_box, criterion_group, criterion_main, Criterion};

fn test_mark_not<F,N>(note_creator: F)
    where F: Fn(&Vec<PersonOptions>) -> N,
        N: CardTypeNoteTracker<PersonOptions> {
    let suspects = vec![Scarlet, Mustard, White, Green, Peacock, Plum];

    {
        let mut notes = note_creator(&suspects);
        for suspect in suspects[..suspects.len() - 1].iter() {
            notes.mark_not(suspect);
        }
        assert!(notes.answer_found());
    }

    {
        let mut notes = note_creator(&suspects);
        for suspect in suspects.iter().skip(1).rev() {
            notes.mark_not(&suspect);
        }
        assert!(notes.answer_found());
    }
}

fn benchmark_vec(c: &mut Criterion) {
    c.bench_function("vec_notes", 
    |b| b.iter(|| test_mark_not(|suspects| black_box(VecCardTypeNotes::new(suspects)))));
}

fn benchmark_hashmap(c: &mut Criterion) {
    c.bench_function("hashmap_notes", 
    |b| b.iter(|| test_mark_not(|suspects| black_box(HashMapCardTypeNotes::new(suspects.clone())))));
}

criterion_group!(benches, benchmark_vec, benchmark_hashmap);
criterion_main!(benches);