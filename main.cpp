/*********************************************************
*  AUTHOR        : Bighani Lometillo
*  Final Project : Battleship
*  CLASS         : CS002
*  SECTION       : MW: 7a - 8:25a TTh 7a - 9a
*  Due Date      :
*********************************************************/
#include <iostream>
#include <iomanip>
#include <vector>
#include<cctype>
using namespace std;
/**********************************************************
*
* Battleship
*_________________________________________________________
*
*_________________________________________________________
* INPUT:
*
*
* OUTPUT:
*
***********************************************************/
//GLOBAL CONSTS AND STRUCTS
const int ROWS = 10;
const int COLS = 10;
const int CARRIER_SIZE = 5;
const int BATTLESHIP_SIZE = 4;
const int CRUISER_SIZE = 3;
const int SUBMARINE_SIZE = 3;
const int DESTROYER_SIZE = 2;
const int FLEET_SIZE = 5;
struct Point{
    int row;
    int column;
};
struct Ship{
    string name;
    int size;
    int hitcount;
    vector<Point> ship_points;
};
struct PlayerBoard{
    char board[ROWS][COLS];
    Ship fleet[FLEET_SIZE];
};

//Prototype
/***********************************************************
 * displayBoards
 *  this function displays two 10x10 boards side by side.
 *
***********************************************************/
void displayBoards(char [][10], char [][10]);
/***********************************************************
 * initFleet
 *  initializes board and all ships in the fleet with proper
 *    info.
 *  THINGS TO DO:
 *      - ensure board is clear
 *      -fill fleet array
 *      -initialize each ship w/:
 *          -name
 *          -size
 *          -hitcount
 *          -ask, should vector in ship struct be initialized to anything?
 *              -or should I keep it random/empty?
 ***********************************************************/
void initFleet(PlayerBoard&);
/**********************************************************************
 * placeShip
 *  places the ship on the board assuming the spot is valid
************************************************************************/
void placeShip(PlayerBoard&, int, bool);
/**********************************************************************
 * getValidShipInfo
 *  -prompts the user for starting row and column coordinates of the ship.
 *  -converts to proper row and column index of array.
 *  -prompt for horizontal or vertical orientation
 *  -checks for error
 *      -is ship fully in the board
 *      -call spaceOccupied function to ensure ships are not overlapping
***********************************************************************/
void getValidShipInfo(int&, int&, char&, PlayerBoard&, int, bool);
/***********************************************************************
 * spaceOccupied
 *  returns true if placement of ship overlaps any pre-exisiting ships
 *  returns false if space is not occupied
************************************************************************/
bool spaceOccupied(PlayerBoard, unsigned int, unsigned int, char,
                      unsigned int);
/***********************************************************************
 * boardSetup
 *  calls the place ship function for each ship in the fleet
 ***********************************************************************/
void boardSetup(PlayerBoard&, PlayerBoard&, bool);
/***********************************************************************
 * getPlay
 *  prompts user for a play
 *  if invalid --> not in board or already played, then invalid
 *      output invalid and keep asking for valid play
 *
 * **********************************************************************/
void getPlay(int&, int&, PlayerBoard&, bool);
/***********************************************************************
 * validPlay
 *  checks if play has been previously played and if play is within bounds
 *  returns true if both criteria are met
 *
 ***********************************************************************/
bool validPlay(PlayerBoard&, int, int);

/************************************************************************
 * placeHit
 *  call getValidPlay before updating board
 *  updates char array(enemy board and inactive player's board)
 *  and outputs hit or miss
 ************************************************************************/
void placeHit(PlayerBoard&, PlayerBoard&, PlayerBoard&, bool, int = 0, int = 0);
/*************************************************************************
 * increaseHit
 *  finds the ship that was hit based on its coordinates and adds one to its
 *  hitcount
 **************************************************************************/
void increaseHit(PlayerBoard&, int, int);
/***************************************************************************
 * checkGameState
 *  loops through fleet and sums up hit counts after a hit is made. If hitcount
 *  totals to 17, then someone has one the game.
 ***************************************************************************/
bool isGameOver(PlayerBoard);
/**************************************************************************
 *
 **************************************************************************/

int main()
{
    //variables -- data table

    PlayerBoard p1;
    PlayerBoard p1_enemy;
    PlayerBoard p2;
    PlayerBoard p2_enemy;
    int game_type;     //determines if 2 player game or against CPU
    bool cpu_play;     //true if game against computer

    initFleet(p1);
    initFleet(p1_enemy);
    initFleet(p2);
    initFleet(p2_enemy);

    cout << "Enter 1 for a two player game or 2 to play against the computer.";
    cout << endl;
    cin >> game_type;

    if(game_type == 1){
        //set cpu_play to false
        cpu_play = false;
        //Prompt user 1 to set up board
        cout << "Player 1 set your board.\n";
        displayBoards(p1.board, p1_enemy.board);
        boardSetup(p1, p1_enemy, cpu_play);

        //Prompt user 2 to set up board
        cout << "Player 2 set your board.\n";
        displayBoards(p2.board, p2_enemy.board);
        boardSetup(p2, p2_enemy, cpu_play);

        //keep playing until all ships are sunk
        while(!isGameOver(p1) && !isGameOver(p2)){
            //Player1 turn
            cout << "Player 1" << endl;
            displayBoards(p1.board, p1_enemy.board);
            placeHit(p1, p1_enemy, p2, cpu_play);
            //Player2 turn
            if(!isGameOver(p1)){
                cout << "Player 2" << endl;
                displayBoards(p2.board, p2_enemy.board);
                placeHit(p2, p2_enemy, p1, cpu_play);
            }
        }

        cout << "You sunk the fleet!!! You win!!";
    }
    else if(game_type == 2){
        //let player 1 set their board
        cpu_play = false;

        //Prompt user 1 to set up board
        cout << "Player 1 set your board.\n";
        displayBoards(p1.board, p1_enemy.board);
        boardSetup(p1, p1_enemy, cpu_play);

        //Let computer set up board
        cpu_play = true;
        //Prompt computer to set up board
        cout << "Player 2(CPU) set up your board. \n";
        displayBoards(p2.board, p2_enemy.board);
        boardSetup(p2, p2_enemy, cpu_play);

        //keep playing until all ships are sunk
        while(!isGameOver(p1) && !isGameOver(p2)){
            //Player1 turn
            cpu_play = false;
            cout << "Player 1" << endl;
            displayBoards(p1.board, p1_enemy.board);
            placeHit(p1, p1_enemy, p2, cpu_play);
            //Player2 turn
            if(!isGameOver(p1)){
                cpu_play = true;
                cout << "Player 2" << endl;
                displayBoards(p2.board, p2_enemy.board);
                placeHit(p2, p2_enemy, p1, cpu_play);
            }
        }

        cout << "You sunk the fleet!!! You win!!";
    }
    else{
        cout << "invalid option" << endl;
    }

    return 0;
}

void displayBoards(char p1[][10], char p2[][10]){
    char row = 'A';         //row to print
    int row_index = 0;      // to access element in array
    string dashes(42, '-');
    string spaces(18, ' ');

    cout << setfill(' ');
    cout << spaces << "Your Board\t      " << spaces
         << spaces << "Enemy Board";
    cout << endl;

    for(int i = 0; i < 2; i++){
        cout << setfill(' ');
        cout << setw(4) << ' ';
        for(int i = 1; i < 11; i++){
            cout << left << setw(4) << i;
        }
        cout << '\t' << '\t';

    }
    cout << endl;

    for(int i = 1; i < 21; i++){
        //line with letters
        if(!(i%2)){
            cout << row << ' ' << '|';

            //player board
            for(int j = 0; j < 10; j++){
                cout << ' ' << p1[row_index][j] << ' ' << '|';
            }
            //cout <<'|';

            //enemy board
            cout << "\t\t" << row << ' ' << '|';
            for(int j = 0; j < 10; j++){
                cout << ' ' << p2[row_index][j] << ' ' << '|';
            }
            cout << endl;

            row++;
            row_index++;
        }
        //line of dashes
        else{
            cout << ' ' << dashes <<"\t\t" << ' ' << dashes << endl;
        }
    }
    cout << " " << dashes << "\t\t" << ' ' << dashes << endl;
}

void initFleet(PlayerBoard& pb){
    Point initial_point = {0,0};

    //initialize array to be filled with spaces
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            pb.board[i][j] = ' ';
        }
    }

    //initialize size, name and hitcount of each ship in fleet
    //CARRIER
    pb.fleet[0].name = "carrier";
    pb.fleet[0].size = CARRIER_SIZE;
    //BATTLESHIP
    pb.fleet[1].name = "battleship";
    pb.fleet[1].size = BATTLESHIP_SIZE;
    //CRUISER
    pb.fleet[2].name = "cruiser";
    pb.fleet[2].size = CRUISER_SIZE;
    //SUBMARINE
    pb.fleet[3].name = "submarine";
    pb.fleet[3].size = SUBMARINE_SIZE;
    //DESTROYER
    pb.fleet[4].name = "destroyer";
    pb.fleet[4].size = DESTROYER_SIZE;

    //initialize all hitcounts to 0
    for(unsigned int i = 0; i < FLEET_SIZE; i++){
        pb.fleet[i].hitcount = 0;
    }

    //initialize ship_points to be filled with all 0's
    //carrier
    for(unsigned int i = 0; i < CARRIER_SIZE; i++){
        pb.fleet[0].ship_points.push_back(initial_point);
    }
    //battleship
    for(unsigned int i = 0; i < BATTLESHIP_SIZE; i++){
        pb.fleet[1].ship_points.push_back(initial_point);
    }
    //cruiser
    for(unsigned int i = 0; i < CRUISER_SIZE; i++){
        pb.fleet[2].ship_points.push_back(initial_point);
    }
    //submarine
    for(unsigned int i = 0; i < SUBMARINE_SIZE; i++){
        pb.fleet[3].ship_points.push_back(initial_point);
    }
    //destroyer
    for(unsigned int i = 0; i < DESTROYER_SIZE; i++){
        pb.fleet[4].ship_points.push_back(initial_point);
    }
}

void placeShip(PlayerBoard& pb, int ship_index, bool cpu_play){
    Ship this_ship = pb.fleet[ship_index];
    vector<Point> ship_point;
    int row;
    int col;
    char orientation;
    getValidShipInfo(row, col, orientation, pb, ship_index, cpu_play);
    ship_point = pb.fleet[ship_index].ship_points;

        //place the ship in board object
        for(unsigned int i = 0; i < ship_point.size(); i++ ){
            pb.board[ship_point.at(i).row][ship_point.at(i).column] = 's';
        }
}

void getValidShipInfo(int& row, int& col, char& orientation, PlayerBoard& pb,
                      int ship_index, bool cpu_play)
{
   Ship this_ship = pb.fleet[ship_index];
   int size = this_ship.size;
   char user_row;
   int user_col;
   char starting_char;
   int count;
   int ship_row_end;
   int ship_col_end;
   int cpu_orientation;

   do{
       starting_char = 'A';
       count = 0;
       ship_row_end = 0;
       ship_col_end = 0;
       //INPUT: ask for row -- letter and column -- number starting index of ship
        cout << "Enter the starting coordinates of your " << this_ship.name
             << ": ";
        if(cpu_play == false){
            cin >> user_row >> user_col;


            //makesure char is uppercase
            user_row = toupper(user_row);

            //CONVERT letter and number into correct index in pb
            //convert letter to correct num
            while(user_row != starting_char){
                count++;
                starting_char++;
            }
            row = count;
            col = user_col -1;
        }

        //computer randomly generates placement
        else{
            row = rand() % 9;
            col = rand() % 9;
            cout << static_cast<char>(starting_char + row)
                 << ' ' << col + 1 << endl;
        }


        //INPUT: ask for horizontal and vertical orientation of ship
        do{
        cout << "Enter the orientation of your " << this_ship.name
             << "(horizontal (h) or vertical(v)): ";
        if(cpu_play == false){
            cin >> orientation;
        }
        else{
            cpu_orientation = rand() % 2 + 1;
            if(cpu_orientation == 1){
                orientation = 'h';
                cout << orientation << endl;
            } else{
                orientation = 'v';
                cout << orientation << endl;
            }

        }

        } while (orientation != 'h' && orientation != 'v');

        //CHECK FOR ERROR: if not in board
        if(row > 9 || col > 9 || row < 0 || col < 0){
            cout << "Error: ship placement is outside the board." << endl;
        }
        else if(orientation == 'h'){
            ship_col_end =  col + size -1;
            if(ship_col_end > 9){
                cout << "Error: ship placement is outside the board." << endl;
            }
        }
        else if(orientation == 'v'){
            ship_row_end = row + size -1;
            if(ship_row_end > 9){
                cout << "Error: ship placement is outside the board." << endl;
            }
        }
        if(spaceOccupied(pb, row, col, orientation, size)){
            cout << "Error: Space already occupied." << endl;
        }
    }while(spaceOccupied(pb, row, col, orientation, size) || ship_row_end > 9
           || ship_col_end > 9 || row > 9 || col > 9 || row < 0 || col < 0);




    //change to proper values this_ship.ship_points
    for(int i = 0; i < size; i++ ){
        if(orientation == 'h'){
            pb.fleet[ship_index].ship_points[i].row = row;
            pb.fleet[ship_index].ship_points[i].column = col;
            col++;
        }
        else if(orientation == 'v'){
            pb.fleet[ship_index].ship_points[i].row = row;
            pb.fleet[ship_index].ship_points[i].column = col;
            row++;
        }
    }
}





bool spaceOccupied(PlayerBoard pb, unsigned int row, unsigned int col,
                      char orientation, unsigned int size)
{
    //horizontal
    if(orientation == 'h'){
        for(unsigned int i = col; i < col + size; i++ ){
            if(pb.board[row][i] == 's')
                return true;
        }
    }
    //vertical
    else if(orientation == 'v'){
        for(unsigned int i = row; i < row + size; i++){
            if(pb.board[i][col] == 's'){
                return true;
            }
        }
    }
    //space is not occupied
    return false;
}

void boardSetup(PlayerBoard& p1, PlayerBoard& p2, bool cpu_play){
    //place all ships for player 1
    for(int i = 0; i < FLEET_SIZE; i++){
     placeShip(p1, i, cpu_play);
     displayBoards(p1.board, p2.board);
    }
}

void getPlay(int& row, int& col, PlayerBoard& pb, bool cpu_play){
    char user_row;       // the row the player chooses
    char starting_char;  //used to convert user row to proper index in pb.board
    int count;           //used to convert user row to proper index in pb.board

    do{
        //set count to 0 and starting_char to A
        count = 0;
        starting_char = 'A';

        //INPUT: prompt user to take a shot
        cout << "Fire a shot: ";
        if(cpu_play == false){
            cin >> user_row >> col;
        }
        else{
            row = rand() % 9;
            col = rand() % 9;
            cout << static_cast<char>(row + starting_char) << ' ' << col + 1
                 << endl;
        }

        //convert user_row to proper index
        //makesure char is uppercase
        user_row = toupper(user_row);

        //CONVERT letter and number into correct index in pb
        //convert letter to correct num

        if(cpu_play == false){
            while(user_row != starting_char){
                count++;
                starting_char++;
            }
            row = count;
            col = col -1;
        }

        if(!validPlay(pb, row, col)){
            cout << "Invalid!!" << endl;
        }
    }while(!validPlay(pb, row, col));

}

bool validPlay(PlayerBoard& pb, int row, int col){

    //Check if indices are within the bounds
    if((row >= 0 && row <= 9 && col >= 0 && col <= 9) &&
       (pb.board[row][col] != 'H' && pb.board[row][col] != 'M') )
    {
       return true;
    } else{
        return false;
    }
}

void placeHit(PlayerBoard& p1, PlayerBoard& p1_enemy, PlayerBoard& p2,
              bool cpu_play,int row, int col){
    getPlay(row, col, p2, cpu_play);

    //if ship is at index then place H
    if(p2.board[row][col] == 's'){
        p1_enemy.board[row][col] = 'H';
        p2.board[row][col] = 'H';
        cout << "HIT!!!";
        //call increaseHit
        increaseHit(p2, row, col);
    } else{
        p1_enemy.board[row][col] = 'M';
        p2.board[row][col] = 'M';
        cout << "MISS!!!";
    }
    displayBoards(p1.board, p1_enemy.board);
}

void increaseHit(PlayerBoard& inactivePlayer, int row, int col){

    for(unsigned int i = 0; i < FLEET_SIZE; i++){
        for(unsigned int j = 0; j < inactivePlayer.fleet[i].ship_points.size();
            j++){
            if(inactivePlayer.fleet[i].ship_points.at(j).row == row &&
               inactivePlayer.fleet[i].ship_points.at(j).column == col){
                inactivePlayer.fleet[i].hitcount += 1;
                //let player know if ship has been sunk
                if(inactivePlayer.fleet[i].hitcount ==
                        inactivePlayer.fleet[i].size ){
                    cout << "You have sunk the "
                         << inactivePlayer.fleet[i].name << endl;
                }
                return;
            }
        }
    }
}

bool isGameOver(PlayerBoard playerHit){
    int total_hit_count = 0;
    for(unsigned int i = 0; i < FLEET_SIZE; i++){
        total_hit_count += playerHit.fleet[i].hitcount;
    }

    if(total_hit_count == 17){
        return true;
    }
    return false;
}
