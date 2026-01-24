#include "TicTacToe.h"
#include "Logger.h"
// -----------------------------------------------------------------------------
// TicTacToe.cpp
// -----------------------------------------------------------------------------
// This file is intentionally *full of comments* and gentle TODOs that guide you
// through wiring up a complete Tic‑Tac‑Toe implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 3x3 grid.
//  - Players take turns; you can only place into an empty square.
//  - First player to get three-in-a-row (row, column, or diagonal) wins.
//  - If all 9 squares are filled and nobody wins, it’s a draw.
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 3x3 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

const int AI_PLAYER = 1;    // index of the AI player (O)
const int HUMAN_PLAYER = 0; // index of the human player (X)

Logger *logger = Logger::GetInstance();

TicTacToe::TicTacToe()
{
}

TicTacToe::~TicTacToe()
{
}

// -----------------------------------------------------------------------------
// make an X or an O
// -----------------------------------------------------------------------------
// DO NOT CHANGE: This returns a new Bit with the right texture and owner
Bit *TicTacToe::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "x.png" or the "o.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "x.png" : "o.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}

//
// setup the game board, this is called once at the start of the game
//
void TicTacToe::setUpBoard()
{
    // dimensions of our bit holder images (our board squares)
    float sprite_w = 100.0f;
    float sprite_h = 100.0f;

    // padding (purely aesthetic choice)
    float pad = 50.0f;

    // there are 2 players in tic tac toe
    setNumberOfPlayers(2);

    // set up our 3x3 board
    _gameOptions.rowX = 3;
    _gameOptions.rowY = 3;
    for (int i = 0; i < _gameOptions.rowX; i++)
    {
        for (int j = 0; j < _gameOptions.rowY; j++)
        {
            // draw position, where the sprite will be placed on screen
            ImVec2 pos = {i * sprite_w + pad, j * sprite_h + pad};
            _grid[i][j].initHolder(pos, "square.png", i, j);
        }
    }

    logger->Log("Board setup completed.", logger->INFO, logger->GAME);

    // blast off!
    startGame();
}

//
// about the only thing we need to actually fill out for tic-tac-toe
//
bool TicTacToe::actionForEmptyHolder(BitHolder *holder)
{
    // check that holder exists
    if (!holder)
    {
        logger->Log("Passed holder is nullptr!", logger->ERROR, logger->GAME);
        return false;
    }

    // check that holder is empty
    if (holder->bit())
    {
        logger->Log("Holder is occupied.", logger->INFO, logger->GAME);
        return false;
    }

    // place the current player's piece on this holder
    int player_id = getCurrentPlayer()->playerNumber(); // get current player ID

    Bit *new_bit = PieceForPlayer(player_id);    // make a new bit (X or O) for current player
    new_bit->setPosition(holder->getPosition()); // put bit sprite at holder position

    holder->setBit(new_bit); // put bit in holder

    // succesfully placed bit!
    std::string log = "Bit placed by player " + std::to_string(player_id);
    logger->Log(log, logger->INFO, logger->GAME);
    return true;
}

bool TicTacToe::canBitMoveFrom(Bit *bit, BitHolder *src)
{
    // you can't move anything in tic tac toe
    return false;
}

bool TicTacToe::canBitMoveFromTo(Bit *bit, BitHolder *src, BitHolder *dst)
{
    // you can't move anything in tic tac toe
    return false;
}

//
// free all the memory used by the game on the heap
//
void TicTacToe::stopGame()
{
    // clear out the board
    for (int i = 0; i < _gameOptions.rowX; i++)
    {
        for (int j = 0; j < _gameOptions.rowY; j++)
        {
            // frees up heap memory used by placed bits
            _grid[i][j].destroyBit();
        }
    }
    
    logger->Log("Board cleared!", logger->INFO, logger->GAME);
}

//
// helper function for the winner check
//
Player *TicTacToe::ownerAt(int index) const
{
    // converting 1D board coords to 2D coords
    int i = index % _gameOptions.rowX;
    int j = index / _gameOptions.rowY;

    // get bit at these coords
    Bit *b = _grid[i][j].bit();
    if (b)
    {
        return b->getOwner();   // if it exists, return it!
    }

    // no bit at index location
    return nullptr;
}

Player *TicTacToe::checkForWinner()
{
    // array of winning triplets
    // each int refers to a holder location, representing in 1D
    int wins[8][3] = {
        {0, 1, 2},
        {3, 4, 5},
        {6, 7, 8},
        {0, 3, 6},
        {1, 4, 7},
        {2, 5, 8},
        {0, 4, 8},
        {2, 4, 6}};

    // get current board state
    std::string state = stateString();

    // loop through possible win states
    for (int i = 0; i < 8; i++)
    {
        // holder locations
        int a = wins[i][0];
        int b = wins[i][1];
        int c = wins[i][2];

        // ignore empties
        if (state[a] == '0' || state[b] == '0' || state[c] == '0')
        {
            continue;
        }

        // if winning locations all have the same owner, we have a winner, baby!
        if ((state[a] == state[b]) && (state[b] == state[c]))
        {
            return ownerAt(a); // returns a Player
        }
    }

    // no winner
    return nullptr;
}

bool TicTacToe::checkForDraw()
{
    // is the board full with no winner?
    if (checkForWinner()){
        return false;
    }

    // loop through board grid
    for (int i = 0; i < _gameOptions.rowX; i++)
    {
        for (int j = 0; j < _gameOptions.rowY; j++)
        {
            // if any square is empty, there's no draw
            if (_grid[i][j].empty())
                return false;
        }
    }

    // no empties + no winners = draw
    return true;
}

//
// state strings
//
std::string TicTacToe::initialStateString()
{
    return "000000000";
}

//
// this still needs to be tied into imguis init and shutdown
// we will read the state string and store it in each turn object
//
std::string TicTacToe::stateString() const
{
    // this will hold our state
    std::string state = "";

    // loop through grid, build our state string where...
    // ...'0' = null, '1' = player 0, '2' = player 1
    for (int i = 0; i < _gameOptions.rowX; i++)
    {
        for (int j = 0; j < _gameOptions.rowY; j++)
        {
            // get bit at (i, j)
            Bit *b = _grid[j][i].bit();
            int player = 0;

            // if b exists, put {player number + 1} in string
            if (b)
            {
                player = b->getOwner()->playerNumber() + 1;
            }

            state += std::to_string(player);
        }
    }

    // all done
    return state;
}

//
// this still needs to be tied into imguis init and shutdown
// when the program starts it will load the current game from the imgui ini file and set the game state to the last saved state
//
void TicTacToe::setStateString(const std::string &s)
{
    // set the state of the board from the given string
    // the string will be 9 characters long, one for each square
    // each character will be '0' for empty, '1' for player 1 (X), and '2' for player 2 (O)
    // the order will be left-to-right, top-to-bottom
    // for example, the starting state is "000000000"
    // if player 1 has placed an X in the top-left and player 2 an O in the center, the state would be "100020000"
    // you can loop through the string and set each square in _grid accordingly
    // for example, if s[0] is '1', you would set _grid[0][0] to have player 1's piece
    // if s[4] is '2', you would set _grid[1][1] to have player 2's piece
    // if s[8] is '0', you would set _grid[2][2] to be empty
    // you can use the PieceForPlayer function to create a new piece for a player
    // remember to convert the character to an integer by subtracting '0'
    // for example, int playerNumber = s[index] - '0';
    // if playerNumber is 0, set the square to empty (nullptr)
    // if playerNumber is 1 or 2, create a piece for that player and set it in the square
    // finally, make sure to position the piece at the holder's position
    // you can get the position of a holder using holder->getPosition()
    // loop through the 3x3 array and set each square accordingly
    // the string should always be valid, so you don't need to check its length or contents
    // but you can assume it will always be 9 characters long and only contain '0', '1', or '2'
}

//
// this is the function that will be called by the AI
//
void TicTacToe::updateAI()
{
    // we will implement the AI in the next assignment!
}
