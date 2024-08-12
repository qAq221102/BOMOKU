#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#define boardsize 15
void modeselect(); // choose NORMAL or BOMB
void showboard();  // display the current board
void coordinate(); // create a correct coordinate(row*column)
void setbomb();    // create the bomb randomly
void boom();       // deal with the bombs
void combo();      // deal with combo arround the bomb by recursion
int horizon();     // check "--", if won return 1, or 0
int vertical();    // check "|", if won return 1, or 0
int slash();       // check "/", if won return 1, or 0
int backslash();   // check "\", if won return 1, or 0
int judge();       // consist of each continuous directions, if won return 1, or 0
int endproject();  // depends on returned win's value and return 0 to default or replay
int main()
{
    HWND hwnd = GetConsoleWindow(); // get the handle
    ShowWindow(hwnd, SW_MAXIMIZE);  // maximum the window
    int board[boardsize][boardsize] = {0};
    char playerpiece[2]; //'O'=>-1, 'X'=>1
    int roundcounter = 0;
    int win = 0;     // will be assigned by judge(),0 to nobody won, 1 to someone won
    int key = 0;     // will be assigned by endproject(), 0 to default or replay
    int modekey = 0; // will be assigned by modeselect(), 0 to NORMAL, 1 to FIXED BOMB , 2 to INCREASING BOMB
    while (key == 0)
    {
        modeselect(playerpiece, &modekey, roundcounter); // change modekey
        setbomb(board, modekey, roundcounter);
        showboard(board, playerpiece, roundcounter);
        coordinate(board, playerpiece, roundcounter);
        boom(board);
        win = judge(board);
        key = endproject(board, &roundcounter, win);
        system("cls");
    }
}
void modeselect(char(*mplayerpiece), int *mmodekey, int mroundcounter) // choose NORMAL or BOMB
{
    if (mroundcounter < 1)
    {
        printf("--------------------------\n> FOREWORD <\n--------------------------\n");
        printf("Hello players, this is bomoku game made by \"B11207026\"\n");
        printf("BUT THIS IS NOT THE SAME AS THE OTHERS!!\n");
        printf("You can choose NORMAL mode or BOMB mode to play.\n");
        printf("And BOMB mode has two kinds: one is FIXED, another is INCREASING change over round.\n");
        printf("The new rules are as follows:\n");
        printf("If \"B\" (which means the bomb on the board) is surrounded by pieces, the \"B\" will clear all the spaces around it.\n");
        printf("But \"B\" will be replaced depending on which kind of pieces are around \"B\" more.\n");
        printf("If the number of pieces around \"B\" is equal, then \"B\" will be replaced with zero.\n");
        printf("If the original \"B\" boomed, then the new \"B\" nearby the original one will also change.\n");
        printf("---------------------------------------------------------------------------------------------------------\n");
        while (1) // choose NORMAL mode or BOMB
        {
            printf("choose \"N\" to play NORMAL mode or \"B\" to play BOMB mode :");
            char modeselect;
            scanf(" %c", &modeselect);
            if (modeselect == 'N' || modeselect == 'B')
            {
                if (getchar() != '\n') // make sure the absolute input
                {
                    while (getchar() != '\n')
                    {
                    }
                    continue;
                }
                switch (modeselect)
                {
                case 'N': // NORMAL
                    *mmodekey = 0;
                    break;
                case 'B':     // BOMB
                    while (1) // choose FIXED quantity or INCREASING
                    {
                        printf("please choose \"F\" to set quantity FIXED or \"I\" to set INCREASING :");
                        scanf(" %c", &modeselect);
                        if (modeselect == 'F' || modeselect == 'I')
                        {
                            if (getchar() != '\n') // make sure the absolute input
                            {
                                while (getchar() != '\n')
                                {
                                }
                                continue;
                            }
                            switch (modeselect)
                            {
                            case 'F': // FIXED
                                *mmodekey = 1;
                                break;
                            case 'I': // INCREASING
                                *mmodekey = 2;
                                break;
                            }
                            break;
                        }
                        else
                        {
                            while (getchar() != '\n')
                            {
                            }
                            continue;
                        }
                    }
                    break;
                }
                break;
            }
            else
            {
                while (getchar() != '\n')
                {
                }
                continue;
            }
        }
        while (1) // default the players's piece into array
        {
            printf("Player1, please choose your chess piece(O or X) :");
            scanf(" %c", &mplayerpiece[0]); // the " " is important
            if (mplayerpiece[0] == 'O' || mplayerpiece[0] == 'X')
            {
                if (getchar() != '\n') // make sure the absolute input
                {
                    while (getchar() != '\n')
                    {
                    }
                    continue;
                }
                mplayerpiece[1] = (mplayerpiece[0] == 'O') ? 'X' : 'O';
                printf("---------------\n> GAME START <\n---------------\n\n");
                break;
            }
            else
            {
                while (getchar() != '\n')
                {
                }
                continue;
            }
        }
    }
}
void showboard(int (*mboard)[boardsize], char(*mplayerpiece), int mroundcounter) // display the current board
{
    int i, j;
    int index;
    printf("Player1 is %2c\n", mplayerpiece[0]);
    printf("Player2 is %2c\n", mplayerpiece[1]);
    printf("Current round is %2d\n", mroundcounter + 1);
    printf("It's %s turn\n", (mroundcounter % 2 == 0) ? "Player1" : "Player2");
    printf("-----------------------\n\n     ");
    for (index = 1; index <= boardsize; index++) // +index print
    {
        printf("%5d", index);
    }
    printf("    COLUMN v\n\n");
    for (j = 0, index = -1; j < boardsize; j++, index--) // j(column) print & -index print & show the board
    {
        printf("%5d", index);
        for (i = 0; i < boardsize; i++) // i(row) print
        {
            if (mboard[j][i] == -1)
            {
                printf("%5c", 'O');
            }
            else if (mboard[j][i] == 1)
            {
                printf("%5c", 'X');
            }
            else if (mboard[j][i] == 8)
            {
                printf("%5c", 'B');
            }
            else
            {
                printf("%5c", '.');
            }
        }
        printf("\n\n");
    }
    printf("  ROW >\n\n");
}
void coordinate(int (*mboard)[boardsize], char(*mplayerpiece), int mroundcounter) // obtain the correct coordinate(row*column)
{
    int gamerow = 0, gamecolumn = 0;
    char checker;
regret:
    while (1)
    {
        printf("please choose a available coordinate(row*column) to set your piece :");
        scanf(" %d*%d", &gamerow, &gamecolumn);
        if (gamerow < 1 || gamerow > boardsize || gamecolumn > -1 || gamecolumn < -boardsize ||
            mboard[-(gamecolumn + 1)][gamerow - 1] != 0) // out of board or something is set already
        {
            printf("invalid row or column !!\n");
            while (getchar() != '\n')
            {
            }
            continue;
        }
        else if (mboard[-(gamecolumn + 1)][gamerow - 1] == 0) // vaild coordinate
        {
            if (getchar() != '\n') // make sure the absolute input
            {
                while (getchar() != '\n')
                {
                }
                continue;
            }
            break;
        }
    }
    printf("set piece on (row: %2d, column: %2d)\n", gamerow, gamecolumn);
    while (1) // to check the coordinate was chosen correctly
    {
        printf("input \"C\" to confirm the coordinate(row*column) you chose or \"R\" to choose again :");
        scanf(" %c", &checker);
        if (checker == 'C' || checker == 'R') // confirm or regret
        {
            if (getchar() != '\n') // make sure the absolute input
            {
                while (getchar() != '\n')
                {
                }
                continue;
            }
            switch (checker)
            {
            case 'C': // confirm
                break;
            case 'R':        // regret
                goto regret; // regret system
                break;
            }
            break;
        }
        else
        {
            while (getchar() != '\n')
            {
            }
            continue;
        }
    }
    int i = 0, j = 0;
    for (j = 0; j < boardsize; j++) // change the board
    {
        for (i = 0; i < boardsize; i++)
        {
            if (i + 1 == gamerow && j + 1 == -gamecolumn)
            {
                mboard[j][i] = (mplayerpiece[mroundcounter % 2] == 'O') ? -1 : 1;
            }
        }
    }
    printf("\n\n");
}
void setbomb(int (*mboard)[boardsize], int mmodekey, int mroundcounter) // create the bomb randomly
{
    srand(time(NULL));              // change random seed
    int quantity = rand() % 11 + 8; // quantity will within 8~18
    int randrow = 0;
    int randcolumn = 0;
    if (mmodekey == 1 && mroundcounter == 0) // FIXED BOMB
    {
        while (quantity != 0)
        {
            randrow = rand() % 15;
            randcolumn = rand() % 15;
            if (mboard[randcolumn][randrow] == 0)
            {
                mboard[randcolumn][randrow] = 8;
                quantity--;
            }
            else
            {
                continue;
            }
        }
    }
    if (mmodekey == 2 && mroundcounter % 2 == 0) // INCREASING BOMB, increase 3 bombs per 2 rounds
    {
        quantity = 0;
        while (quantity < 3)
        {
            randrow = rand() % 15;
            randcolumn = rand() % 15;
            if (mboard[randcolumn][randrow] == 0)
            {
                mboard[randcolumn][randrow] = 8;
                quantity++;
            }
            else
            {
                continue;
            }
        }
    }
}
void boom(int (*mboard)[boardsize]) // deal with the bombs
{
    int row, column;
    for (column = 0; column < boardsize; column++)
    {
        for (row = 0; row < boardsize; row++)
        {
            if (mboard[column][row] == 8) // locate one bomb
            {
                int x = 0, y = 0;
                int surroundcount = 0;
                int valuecount = 0;
                int edgetop = 0, edgebottom = 0, edgeleft = 0, edgeright = 0;
                for (y = -1; y <= 1; y++) // analyse arround the bomb
                {
                    for (x = -1; x <= 1; x++)
                    {
                        int newcolumn = column + y;
                        int newrow = row + x;
                        if (x == 0 && y == 0)
                        {
                            continue;
                        }
                        if (newcolumn >= 0 && newcolumn < boardsize && newrow >= 0 && newrow < boardsize)
                        // count objects and piece's value arround the bomb
                        {
                            if (mboard[newcolumn][newrow] == 1 || mboard[newcolumn][newrow] == -1 || mboard[newcolumn][newrow] == 8)
                            {
                                surroundcount++;
                            }
                            if (mboard[newcolumn][newrow] == 1 || mboard[newcolumn][newrow] == -1)
                            {
                                valuecount += mboard[newcolumn][newrow];
                            }
                        }
                        if (newcolumn < 0) // on the top of board
                        {
                            edgetop = 1;
                        }
                        if (newcolumn == boardsize) // on the bottom of board
                        {
                            edgebottom = 1;
                        }
                        if (newrow < 0) // on the left of board
                        {
                            edgeleft = 1;
                        }
                        if (newrow == boardsize) // on the right of board
                        {
                            edgeright = 1;
                        }
                    }
                }
                int edgesum = edgetop + edgebottom + edgeleft + edgeright; // judge if on the edge
                if ((surroundcount == 8) || (edgesum == 1 && surroundcount == 5) || (edgesum == 2 && surroundcount == 3))
                // inside, edgesum=1 is on edge, edgesum=2 is on corner
                {
                    for (y = -1; y <= 1; y++) // change board arround the bomb
                    {
                        for (x = -1; x <= 1; x++)
                        {
                            int newcolumn = column + y;
                            int newrow = row + x;
                            if (newcolumn >= 0 && newcolumn < boardsize && newrow >= 0 && newrow < boardsize)
                            {
                                if (mboard[newcolumn][newrow] == 1 || mboard[newcolumn][newrow] == -1)
                                {
                                    mboard[newcolumn][newrow] = 0;
                                }
                            }
                        }
                    }
                    combo(mboard, valuecount, column, row); // original bomb's position
                }
            }
        }
    }
}
void combo(int (*mboard)[15], int valuecount, int column, int row) // deal with combo arround the bomb by recursion
{
    int x = 0, y = 0;
    for (y = -1; y <= 1; y++)
    {
        for (x = -1; x <= 1; x++)
        {
            int newcolumn = column + y;
            int newrow = row + x;
            if (valuecount > 0)
            {
                mboard[column][row] = 1;
            }
            else if (valuecount < 0)
            {
                mboard[column][row] = -1;
            }
            else
            {
                mboard[column][row] = 0;
            }
            if (newcolumn >= 0 && newcolumn < boardsize && newrow >= 0 && newrow < boardsize)
            {
                if (mboard[newcolumn][newrow] == 8)
                {
                    combo(mboard, valuecount, newcolumn, newrow);
                }
            }
        }
    }
}
int horizon(int (*mboard)[boardsize]) // check "--", if won return 1, or 0
{
    int i = 0, start_row = 0, start_col = 0;
    for (start_col = 0; start_col < boardsize; start_col++)
    {
        for (start_row = 0; start_row < 11; start_row++)
        {
            int wincount = 0;
            for (i = 0; i < 5; i++) // find continuous 5 pieces
            {
                if (mboard[start_col][start_row + i] == 1 || mboard[start_col][start_row + i] == -1)
                {
                    wincount += mboard[start_col][start_row + i];
                }
            }
            if (start_row - 1 >= 0 && abs(wincount) == 5) // check continuous 5 pieces forward
            {
                if (mboard[start_col][start_row - 1] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (start_row + 5 < boardsize && abs(wincount) == 5) // check continuous 5 pieces backward
            {
                if (mboard[start_col][start_row + 5] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (abs(wincount) == 5) // only continuous 5 pieces
            {
                printf("%d\n", wincount);
                return 1;
            }
        }
    }
    return 0;
}
int vertical(int (*mboard)[boardsize]) // check "|", if won return 1, or 0
{
    int i = 0, start_row = 0, start_col = 0;
    for (start_row = 0; start_row < boardsize; start_row++)
    {
        for (start_col = 0; start_col < 11; start_col++)
        {
            int wincount = 0;
            for (i = 0; i < 5; i++) // find continuous 5 pieces
            {
                if (mboard[start_col + i][start_row] == 1 || mboard[start_col + i][start_row] == -1)
                {
                    wincount += mboard[start_col + i][start_row];
                }
            }
            if (start_col - 1 >= 0 && abs(wincount) == 5) // check continuous 5 pieces forward
            {
                if (mboard[start_col - 1][start_row] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (start_col + 5 < boardsize && abs(wincount) == 5) // check continuous 5 pieces backward
            {
                if (mboard[start_col + 5][start_row] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (abs(wincount) == 5) // only continuous 5 pieces
            {
                return 1;
            }
        }
    }
    return 0;
}
int slash(int (*mboard)[boardsize]) // check "/", if won return 1, or 0
{
    int i = 0, start_row = 0, start_col = 0;
    for (start_row = 4; start_row < boardsize; start_row++)
    {
        for (start_col = 0; start_col <= boardsize - 5; start_col++)
        {
            int wincount = 0;
            for (i = 0; i < 5; i++) // find continuous 5 pieces
            {
                if (mboard[start_row - i][start_col + i] == 1 || mboard[start_row - i][start_col + i] == -1)
                {
                    wincount += mboard[start_row - i][start_col + i];
                }
            }
            if (start_row + 1 < boardsize && start_col - 1 >= 0 && abs(wincount) == 5) // check continuous 5 pieces forward
            {
                if (mboard[start_col - 1][start_row + 1] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (start_row - 5 >= 0 && start_col + 5 < boardsize && abs(wincount) == 5) // check continuous 5 pieces backward
            {
                if (mboard[start_col + 5][start_row - 5] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (abs(wincount) == 5) // only continuous 5 pieces
            {
                return 1;
            }
        }
    }
    return 0;
}
int backslash(int (*mboard)[boardsize]) // check "\", if won return 1, or 0
{
    int i = 0, start_row = 0, start_col = 0;
    for (start_col = 0; start_col <= boardsize - 5; start_col++)
    {
        for (start_row = 0; start_row <= boardsize - 5; start_row++)
        {
            int wincount = 0;
            for (i = 0; i < 5; i++)
            {
                if (mboard[start_row + i][start_col + i] == 1 || mboard[start_row + i][start_col + i] == -1)
                {
                    wincount += mboard[start_row + i][start_col + i];
                }
            }
            if (start_row - 1 >= 0 && start_col - 1 >= 0 && abs(wincount) == 5) // check continuous 5 pieces forward
            {
                if (mboard[start_col - 1][start_row - 1] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (start_row + 5 < boardsize && start_col + 5 < boardsize && abs(wincount) == 5) // check continuous 5 pieces backward
            {
                if (mboard[start_col + 5][start_row + 5] == ((wincount == 5) ? 1 : -1))
                {
                    return 0;
                }
            }
            if (abs(wincount) == 5) // only continuous 5 pieces
            {
                return 1;
            }
        }
    }
    return 0;
}
int judge(int (*mboard)[boardsize]) // consist of each continuous directions, if won return 1, or 0
{
    if (horizon(mboard) + vertical(mboard) + slash(mboard) + backslash(mboard) > 0)
    {
        return 1;
    }
    return 0;
}
int endproject(int (*mboard)[boardsize], int *mroundcounter, int mwin)
// depends on returned win's value and return 0 to default or replay, 1 to close
{
    int j = 0, i = 0, tie = 0;
    char checker;
    for (j = 0; j < boardsize; j++) // check tie
    {
        for (i = 0; i < boardsize; i++)
        {
            if (mboard[j][i] != 0)
            {
                tie++;
            }
        }
    }
    if (mwin == 1 || (tie == 225 && mwin == 0))
    {
        printf("---------------\n> GAME OVER <\n---------------\n\n");
        printf("Total round: %d\n", (*mroundcounter) + 1);
        if (mwin == 1) // someone won
        {
            printf(">**  The winner is %s  **<\n", (*mroundcounter % 2 == 0) ? "Player 1" : "Player 2");
        }
        else if (tie == 225 && mwin == 0) // nobody won
        {
            printf("!! Nobody wins !!\n");
        }
    }
    while ((mwin == 1) || (tie == 225 && mwin == 0)) // close or replay
    {
        printf("input \"Q\" to close the project or \"R\" to play again :");
        scanf(" %c", &checker); // the " " is important
        if (checker == 'Q' || checker == 'R')
        {
            if (getchar() != '\n') // make sure the absolute input
            {
                while (getchar() != '\n')
                {
                }
                continue;
            }
            switch (checker)
            {
            case 'Q':
                printf("---THANKS FOR PLAYING---\n");
                return 1;
            case 'R':
                for (j = 0; j < boardsize; j++)
                {
                    for (i = 0; i < boardsize; i++)
                    {
                        mboard[j][i] = 0;
                    }
                }
                *mroundcounter = 0;
                system("cls");
                return 0;
            }
        }
        else
        {
            while (getchar() != '\n')
            {
            }
            continue;
        }
    }
    *mroundcounter = (*mroundcounter) + 1;
    return 0;
}