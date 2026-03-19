
CHESS ENGINE IN C WITH SDL2 UI <br>

~13 million nodes per second raw <br>

Initial Position: <br>
Raw Search: <br>
EXECUTION TIME: 0.404377 seconds <br>
NODES SEARCHED 5072213 <br>

Alpha Beta Search with MVV LVA Move Ordering <br>
EXECUTION TIME: 0.011796 seconds <br>
NODES SEARCHED 40213 <br>

Alpha Beta TT improved move ordering <br>
EXECUTION TIME: 0.020208 seconds <br>
NODES SEARCHED 19007 <br>
TT Hits: 517 <br>
TT Probes: 2894 <br>
TT HIT RATE: 5.597679 <br>
TT CUTOFFS: 512 <br>
TT cuttoff rate: 0.176918 <br>

Chess Programming Wiki Perft Results Position 2 depth 5: <br>
Raw Search: <br>
EXECUTION TIME: 17.244235 seconds <br>
NODES SEARCHED 197740573 <br>

Alpha Beta Search with MVV LVA Move Ordering <br>
EXECUTION TIME: 0.308834 seconds <br>
NODES SEARCHED 592356 <br>

Alpha Beta TT improved move ordering <br>
EXECUTION TIME: 0.074908 seconds <br>
NODES SEARCHED 87350 <br>
TT Hits: 1253 <br>
TT Probes: 7836 <br>
TT HIT RATE: 6.253791 <br>
TT CUTOFFS: 985 <br>
TT cuttoff rate: 0.125702 <br>

----------------------------------------------------------------------------<br>

FEATURES: <br>
Full bitboard engine<br>
Full legal move generation<br>
Attack bitboard generation for both players in any position<br>
Full special move handling promotions, castling, enpassant<br>
Concept of check and checkmate, stalemate<br>
Full perft suite testing positions from chess programming wiki perft result<br>
Openings book (Titans.bin)<br>
Benchmarking search and perft <br>
Transposition table 64MB <br>

Negamax opponent algorithm with alpha beta pruning and move ordering<br>
Full zobrist hashing and incremental updating<br>

----------------------------------------------------------------------------<br>

