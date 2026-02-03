COMPILED AND TESTED ON WINDOWS

First thing I did was to include my Logger class. I also made some QOL updates to make it easier to throw it into future projects (putting Logger UI calls all in a member function of the class). 

Next, I followed Graeme's comments to implement all of the necessary functions for a human vs. human game of Tic Tac Toe. Then, I made a (bad) AI that just picks a random empty holder to place its bit in. 

I wanted the AI player to be toggleable in-game, so I made some changes to the Game class and Application.cpp so that the player could toggle an AI opponent between matched. I added a setter in Game that alters _gameOptions.AIPlaying, and setUpBoard() now checks that bool before setting an AI player. I added a UI checkbox that is disabled during active games.

BETTER AI (negamax)
Since Tic Tac Toe is a zero-sum game, we can use a simplified version of minimax that presumes the current *player's best position* is the *opponent's worst position* ([min(a, b) = -max(-b, -a)](https://en.wikipedia.org/wiki/Negamax#:~:text=Negamax%20search%20is%20a%20variant,the%20value%20to%20player%20B.)).

For my implementation, I added a function to the TicTacToe class called `getNextMove()`, which in turn calls my `negamax()` function. I did it this way instead of embedding the next move logic directly into `updateAI()` so I could switch to the random heuristic, or any other heuristics I want to add in the future (for comparison's sake). 

`getNextMove()` cycles through all of the AI player's possible next moves, evaluating them using negamax. Whichever move results in the human player's worse possible outcome becomes the AI player's best possible move. `negamax()` is recursive, searching to all terminal states (draw or win) in the possibility tree.

Next, I'll try to tackle alpha-beta pruning. Most likely, I'll make an additional function and also add a selector to the the game control panel where the player may choose which AI they want to use. Doing it this way out of curiosity; I want to compare the different heuristics. 
