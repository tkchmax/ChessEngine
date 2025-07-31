Chess Engine - C/C++

A high-performance chess engine implementing modern chess programming techniques with UCI protocol support.

Features
--------

Search Algorithms:
- [Min-Max Search](https://www.chessprogramming.org/Minimax): Foundation of decision making
- [Alpha-Beta Pruning](https://www.chessprogramming.org/Alpha-Beta): Significant search space reduction
- [Principal Variation Search](https://www.chessprogramming.org/Principal_Variation_Search): Enhanced alpha-beta for PV nodes
- [Quiescent Search](https://www.chessprogramming.org/Quiescence_Search): Prevents horizon effect in tactical positions
- [Iterative Deepening](https://www.chessprogramming.org/Iterative_Deepening): Progressive depth search with time management
- [Aspiration Windows](https://www.chessprogramming.org/Aspiration_Windows): Adaptive alpha-beta bounds around expected value

Move Ordering:
- [MVV/LVA](https://www.chessprogramming.org/MVV-LVA): Most Valuable Victim / Least Valuable Aggressor prioritization
- [Killer Moves](https://www.chessprogramming.org/Killer_Heuristic): Reuse of cutoff-causing moves from same depth
- [History Heuristic](https://www.chessprogramming.org/History_Heuristic): Move success tracking across search

Optimization Techniques:
- [Transposition Table](https://www.chessprogramming.org/Transposition_Table): Position caching using Zobrist Hashing
- [Null-Move Pruning](https://www.chessprogramming.org/Null_Move_Pruning): Forward pruning via null-move trials
- [Zobrist Hashing](https://www.chessprogramming.org/Zobrist_Hashing): Efficient position hashing

Evaluation Heuristics:
- [Piece-Square Tables](https://www.chessprogramming.org/Piece-Square_Tables): Positional piece valuation
- [Pawn Structure](https://www.chessprogramming.org/Pawn_Structure): Passed pawns, isolated/doubled pawns
- [King Safety](https://www.chessprogramming.org/King_Safety): Pawn shield, castling rights, attack proximity
- [Material Evaluation](https://www.chessprogramming.org/Material): Piece value balancing
- [Mobility & Center Control](https://www.chessprogramming.org/Mobility): Space advantage metrics

Example Games
-------------
View sample games in PGN format:

Build Instructions
------------------

Requirements:
- GCC (v9.4.0+) or Clang
- cmake

Compilation:
```bash git clone https://github.com/tkchmax/ChessEngine.git master-cmake
mkdir build && cd build
cmake -A x64 -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

Running:
```bash
 ./Release/ChessEngine.exe
```

UCI Commands
------------
Currently implemented commands with examples:

1. `position [fen <fenstring> | startpos] [moves <move1> <move2> ...]`  
   Set board position via [FEN](https://en.wikipedia.org/wiki/Forsyth%E2%80%93Edwards_Notation) or starting position  
   Examples:  
   - `position startpos`  
     Reset to initial chess position  
   - `position startpos moves e2e4 e7e5`  
     Start from initial position and apply moves  
   - `position fen r1bqkbnr/pppp1ppp/2n5/4p3/4P3/5N2/PPPP1PPP/RNBQKB1R w KQkq - 2 3`  
     Set custom position via FEN

2. `d`  
   Draw current board state (debug view)  

3. `go [depth <x>] [movetime <ms>]`  
Start search with depth limit or time limit  
   Examples:  
   - `go depth 6`  
   Search to fixed depth of 6 plies  
   - `go movetime 3000`  
   Search for 3 seconds maximum 

      Search Output Format:
      - `depth`: Current search depth in plies
      - `score cp [value]`: Evaluation score in centipawns (positive = white advantage)
      - `nodes`: Total positions evaluated
      - `time`: Elapsed search time in milliseconds
      - `pv`: Principal variation (best line found)

Performance
-----------
- ~3M nodes/sec (i7-12700K)
- 2300+ [ELO](https://en.wikipedia.org/wiki/Elo_rating_system)
