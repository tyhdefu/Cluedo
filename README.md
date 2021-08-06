# Cluedo - Rust #
A Rust simulation of the board game cluedo, without the board.
Significantly faster than the C++ version.

Started 05/08/2021

Worked on 05/08/2021

Optimised on 06/08/2021 Using WPR/WPA (Windows Performance Recorder / Windows Performance Analyser).
The largest optimisation was storing the RNG instead of using the OS rng and using a faster RNG.

## Speed ##
395k games per second. (Single threaded, on a AMD Ryzen 7 2700X Eight Core Processor)

2523 nanoseconds per game.

Benchmarked over 10 million games (25.232 seconds)