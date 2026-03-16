
CHESS ENGINE IN C WITH SDL2 UI<br>

13 million nodes per second in perft<br>

Example on initial position: <br>

Pure Search <br>
EXECUTION TIME: 0.404377 seconds <br>
NODES SEARCHED 5072213 <br>

Alpha Beta Search with Move Ordering <br>
EXECUTION TIME: 0.011796 seconds <br>
NODES SEARCHED 40213 <br>

----------------------------------------------------------------------------<br>

FEATURES: <br>
full bitboard engine<br>
full legal move generation (normal sliding pieces)<br>
attack bitboard generation for both players in any position<br>
full special move handling promotions, castling, enpassant<br>
concept of check and checkmate, stalemate<br>
full perft suite according to chess programming wiki perft result<br>
openings book (Titans.bin)<br>
benchmarking search and perft <br>

negamax opponent algorithm with alpha beta pruning and move ordering<br>
full zobrist hashing and incremental updating<br>

----------------------------------------------------------------------------<br>

TODO implement:<br>

transposition tables<br>
threefold repetition draw<br>

magic bitboards<br>
positional play, king safety etc.<br>
insufficient material draw<br>

