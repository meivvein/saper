#include <iostream>
#include <cstdlib>
#include <time.h>

using namespace std;

// Windows XP difficulties:
// Beginner - 8x8 map, 10 mines
// Advanced - 16x16 map, 40 mines
// Expert - 16x36 map, 99 mines

int map_size_x = 8;
int map_size_y = 8;

int map[8][8];
bool map_revealed[8][8];

int number_of_mines = 10;

int score_to_win;
bool game_continues = true;


bool is_mine(int x, int y);
void reveal_tile(int x, int y);
void print_map();
string color_number(int n);


int main()
{
    system("title Saper");

    srand(time(NULL));

    // Calculate score required to win
    score_to_win = map_size_x * map_size_y - number_of_mines;

    // Populate map & map_revealed arrays
    for (int i = 0; i < map_size_x; i++) {
        for (int j = 0; j < map_size_y; j++) {
            map[i][j] = 0;
            map_revealed[i][j] = false;
        }
    }

    // Place mines
    int mines_to_place = number_of_mines;
    while (mines_to_place > 0) {

        // Pick random tile on the map
        int x, y;
        x = rand() % map_size_x;
        y = rand() % map_size_y;

        // Check if there is a mine on the tile
        if (!is_mine(x, y)) {
            // Place new mine
            map[x][y] = -99;
            mines_to_place--;

            // Increment adjacent tiles
            for (int k = -1; k <= 1; k++) {
                if (x + k >= 0 && x + k < map_size_x) {
                    for (int l = -1; l <= 1; l++) {
                        if (y + l >= 0 && y + l < map_size_y) {
                            map[x + k][y + l]++;
                        }
                    }
                }
            }
        }
    }
    
    // Game loop
    while (game_continues) {
        // Clear screen
        system("cls");

        print_map();

        while (true) {
            // Get user input
            int x, y;
            cout<<endl<<"Enter coordinates x & y: ";
            // Reordered x & y for more intuitive control
            cin>>y>>x;

            // Check user input
            if ((x < 1 || x >map_size_x) || (y < 1 || y > map_size_y)) {
                cout<<"Invalid coordinates."<<endl;
            } else {
                x--;
                y--;

                if (map_revealed[x][y]) {
                    cout<<"Already revealed."<<endl;
                } else {
                    reveal_tile(x, y);
                    break;
                }
            }
        }
    }

    // End of the game
    system("cls");
    print_map();

    // Win
    if (score_to_win == 0) {
        // Green
        cout<<endl<<"\u001b[32m"<<"YOU WON!"<<"\u001b[0m"<<endl;
    }

    // Lost
    else {
        // Red
        cout<<endl<<"\u001b[31m"<<"YOU LOST!"<<"\u001b[0m"<<endl;
    }

    cout<<endl<<"Press any key to exit . . ."<<endl;
    system("pause>nul");

    return 0;
}


// Checks if given tile has a mine on it.
bool is_mine(int x, int y)
{
    return map[x][y] < 0;
}


// Reveals given tile on the map.
void reveal_tile(int x, int y)
{
    map_revealed[x][y] = true;

    // Loss
    if (is_mine(x, y)) {
        game_continues = false;
    }
    
    else {
        // Update score
        score_to_win--;

        // Win
        if (score_to_win == 0) {
            game_continues = false;
            return;
        }

        // Reveal adjacent tiles for 0
        if (map[x][y] == 0) {
            for (int k = -1; k <= 1; k++) {
                if (x + k >= 0 && x + k < map_size_x) {
                    for (int l = -1; l <= 1; l++) {
                        if (y + l >= 0 && y + l < map_size_y) {
                            if (!map_revealed[x + k][y + l]) {
                                reveal_tile(x + k, y + l);
                            }
                        }
                    }
                }
            }
        }
    }
}


// Prints map to the user.
void print_map()
{
    for (int i = -2; i <= map_size_x; i++) {
        for (int j = -2; j <= map_size_y; j++) {

            // Tiles
            if (i >= 0 && i < map_size_x && j >= 0 && j < map_size_y) {
                // Revealed tiles
                if (map_revealed[i][j]) {

                    // Mine
                    if (is_mine(i, j)) {
                        cout<<"\u001b[41m"<<(char)207<<"\u001b[0m";
                    }

                    // Number
                    else {
                        cout<<color_number(map[i][j]);
                    }
                }

                // Not yet revealed tiles
                else {

                    // After lost game print mines positions
                    if (!game_continues && is_mine(i, j)) {
                        cout<<(char)207;
                    }
                    
                    else {
                        cout<<"@";
                    }
                }

                cout<<" ";
            }

            // Frame - top-left corner piece
            else if (i == -1 && j == -1) {
                cout<<(char)218;
                cout<<(char)196;
            }

            // Frame - top-right corner piece
            else if (j == map_size_y && i == -1) {
                cout<<(char)191;
            }

            // Frame - bottom-left corner piece
            else if (j == -1 && i == map_size_x) {
                cout<<(char)192;
                cout<<(char)196;
            }

            // Frame - bottom-right corner piece
            else if (j == map_size_y && i == map_size_x) {
                cout<<(char)217;
            }

            // Frame - top & bottom piece
            else if ((i == -1 || i == map_size_x) && j >= 0 && j < map_size_y) {
                cout<<(char)196;
                cout<<(char)196;
            }

            // Frame - left & right piece
            else if ((j == -1 || j == map_size_y) && i >= 0 && i < map_size_x) {
                cout<<(char)179;
                cout<<" ";
            }
            
            // Numbers - top
            else if (i == -2 && j >= 0 && j < map_size_y) {
                cout<<(j + 1) % 10;
                cout<<" ";
            }

            // Numbers - left
            else if (j == -2 && i >= 0 && i < map_size_x) {
                cout<<(i + 1) % 10;
                cout<<" ";
            }

            
            else {
                cout<<"  ";
            }

        }

        cout<<endl;
    }
}


// Colors given number.
string color_number(int n)
{
    string reset = "\u001b[0m";
    string color;

    switch (n) {
        case 1:
            // Bright blue
            color = "\u001b[34;1m";
            break;
        case 2:
            // Green
            color = "\u001b[32m";
            break;
        case 3:
            // Bright red
            color = "\u001b[31;1m";
            break;
        case 4:
            // Blue
            color = "\u001b[34m";
            break;
        case 5:
            // Red
            color = "\u001b[31m";
            break;
        case 6:
            // Cyan
            color = "\u001b[36m";
            break;
        case 7:
            // Black
            color = "\u001b[30m";
            break;
        case 8:
            // White
            color = "\u001b[37m";
            break;
        default:
            // Empty field
            return " ";
    }

    return color + to_string(n) + reset;
}