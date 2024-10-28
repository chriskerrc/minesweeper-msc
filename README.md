Project for Programming in C MSc module. Solve a minesweeper game given two rules:

- Rule 1 (Count the Mines): If we’ve discovered all the mines on the board already, then any
unknown cell can simply be numbered with the count of mines in its Moore neighbourhood.
- Rule 2 (Unknowns are Mines): For a known square having the number n, with u unknown
and m known mines in its Moore neighbourhood, if n = m + u and u > 0 then all u unknown
squares should be made mines. 
