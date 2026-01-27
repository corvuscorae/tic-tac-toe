#include "Application.h"
#include "imgui/imgui.h"
#include "classes/Logger.h"
#include "classes/TicTacToe.h"

namespace ClassGame
{
    //
    // our global variables
    //
    TicTacToe *game = nullptr;
    bool gameOver = false;
    int gameWinner = -1;

    //
    // our global variables
    //
    bool show_game_panel = true;
    bool show_game_log = true;
    bool show_imgui_demo = false;
    bool show_log_demo = false;

    // log options
    bool log_to_console = true;
    bool log_to_file = false;
    bool show_info = true;
    bool show_warn = true;
    bool show_error = true;

    // Initialize logging system
    Logger *logger = Logger::GetInstance();

    //
    // game starting point
    // this is called by the main render loop in main.cpp
    //
    void GameStartUp()
    {
        game = new TicTacToe();
        game->setUpBoard();

        logger->Log("Game started successfully");
        logger->Log("Application initialized", logger->INFO, logger->GAME);
    }

    //
    // game render loop
    // this is called by the main render loop in main.cpp
    //
    void RenderGame()
    {
        ImGui::DockSpaceOverViewport();

        // ImGui::ShowDemoWindow();

        // logging UI
        logger->initUI();

        if (!game)
        {
            logger->Log("Game load failed.", logger->WARN, logger->GAME);
            return;
        }
        if (!game->getCurrentPlayer())
        {
            logger->Log("Player not detected.", logger->WARN, logger->GAME);
            return;
        }
        ImGui::Begin("Settings");
        ImGui::Text("Current Player Number: %d", game->getCurrentPlayer()->playerNumber());
        ImGui::Text("Current Board State: %s", game->stateString().c_str());

        if (gameOver)
        {
            ImGui::Text("Game Over!");
            ImGui::Text("Winner: %d", gameWinner);

            
            if (gameWinner == -1)
            {
                ImGui::Text("DRAW"); // text indicating a draw
            }

            if (ImGui::Button("Reset Game"))
            {
                logger->Log("Reseting game.", logger->INFO, logger->GAME);
                game->stopGame();
                game->setUpBoard();
                gameOver = false;
                gameWinner = -1;
            }
        }
        ImGui::End();

        ImGui::Begin("GameWindow");
        game->drawFrame();
        ImGui::End();
    }

    //
    // end turn is called by the game code at the end of each turn
    // this is where we check for a winner
    //
    void EndOfTurn()
    {
        Player *winner = game->checkForWinner();
        if (winner)
        {
            gameOver = true;
            gameWinner = winner->playerNumber();
            std::string log = "Game won by player " + std::to_string(gameWinner);
            logger->Log(log + ".", logger->INFO, logger->GAME);
        }
        if (game->checkForDraw())
        {
            gameOver = true;
            gameWinner = -1;
            logger->Log("Game ended in a draw.", logger->INFO, logger->GAME);
        }
    }
}
