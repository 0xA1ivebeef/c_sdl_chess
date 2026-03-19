
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
EXECUTION TIME: 0.079480 seconds <br>
NODES SEARCHED 87350 <br>
TT Hits: 1253 <br>
TT Probes: 7836 <br>
TT HIT RATE: 6.253791 <br>
TT CUTOFFS: 985 <br>
TT cuttoff rate: 0.125702 <br>

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
transposition table 64MB <br>

negamax opponent algorithm with alpha beta pruning and move ordering<br>
full zobrist hashing and incremental updating<br>

----------------------------------------------------------------------------<br>

