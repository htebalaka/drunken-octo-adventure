
#include <ncurses.h>
#include <panel.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../../headers/BoardGUI.h"
#include "../../headers/GUI_Globals.h"
#include "../../headers/BoardGUI_hof.h"

using namespace GUI_Globals;
using namespace BoardGUI_hof;
using namespace std;

int main(int argc, char** argv)
{
    /* TEST SPECIFIC */
    // initialize files for dummy data and data recording
    ifstream run_macro(argv[0]);
    ofstream record_macro(argv[1]);
    ofstream record_stdout(argv[2]);
    /* TEST SPECIFIC */

    init_gui();
    BoardGUI gui = smart_init_board();

    // in a non-test context isBottomPlayer would be set depending on
    // whether the player created or joined the game
    bool isBottomPlayer;

    /* TEST SPECIFIC */
    run_macro >> isBottomPlayer;
    gui.set_debug_flags(&run_macro, &record_macro, &record_stdout);
    /* TEST SPECIFIC */

    auto starting_board = gui.new_game(true);

    // run tests for flattenVec here

    // update the board in response to the other players starting pieces.
    // because we are in testing mode the provided callbacks are not used;
    // their output is overrided to the results from reading run_macro
    gui.refresh_board(
            [](int y, int x) {
                bool r;
                return r;},
            [](int y, int x) {
                bool r;
                return r;},
            [](int y, int x) {
                char r;
                return r;});

    bool ourTurn = isBottomPlayer;
    while (!run_macro.eof())
    {
        if (ourTurn)
        {
            gui.wait_for_player(
                    [](int y, int x) {
                        bool r;
                        return r;
                        },
                    [](int ty, int tx, int fy, int fx) {
                        bool r;
                        return r;
                        },
                    [](int ty, int tx, int fy, int fx) {
                        return;
                        });
            ourTurn = false;
        }
        else
        {
            gui.refresh_board(
                    [](int y, int x) {
                        bool r;
                        return r;
                        },
                    [](int y, int x) {
                        bool r;
                        return r;
                        },
                    [](int y, int x) {
                        char r;
                        return r;
                        });
            ourTurn = true;
        }
    }
    exit_gui_quietly();
    return 0;
}

