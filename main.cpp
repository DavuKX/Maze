#include <iostream>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <vector>
#include <string>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

using namespace std;

int nGood = 0;
int locX = 1, locY = 1;

// print the maze
void printGrid(vector<vector<char>> grid, int mazeSize)
{
    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; j++)
        {
            cout << grid[i][j];
        }
        cout << endl;
    }
}

// move in the x-axis
int moveEW(int direction, int x)
{
    if (direction == EAST)
        return x + 1;
    else if (direction == WEST)
        return x - 1;
    else
        return x;
}

// move in the y-axis
int moveNS(int direction, int y)
{
    if (direction == SOUTH)
        return y + 1;
    else if (direction == NORTH)
        return y - 1;
    else
        return y;
}

// check if is a valid position
bool isValid(int x, int y, int direction, vector<vector<char>> grid, int mazeSize)
{
    x = moveEW(direction, x);
    y = moveNS(direction, y);

    // if the position is taken, or x and y are out of the grid then is not valid
    if (
            grid[y][x] == ' '
            || x >= (mazeSize - 1)
            || x <= 0
            || y <= 0
            || y >= (mazeSize - 1)
        )
        return false;

    // check cardinal directions
    if (direction == NORTH)
    {
        if (
                grid[y][x-1] != ' '
                &&  grid[y-1][x] != ' '
                && grid[y][x+1] != ' '
                &&  grid[y-1][x-1] != ' '
                && grid[y-1][x+1] != ' '
            )
        {
            return true;
        }
    }
    if (direction == SOUTH)
    {
        if (
                grid[y][x-1] != ' '
                &&  grid[y+1][x] != ' '
                && grid[y][x+1] != ' '
                && grid[y+1][x-1] != ' '
                && grid[y+1][x+1] != ' '
            )
        {
            return true;
        }
    }
    if (direction == EAST){
        if (
                grid[y][x+1] != ' '
                &&  grid[y-1][x] != ' '
                && grid[y+1][x] != ' '
                && grid[y-1][x+1] != ' '
                && grid[y+1][x+1] != ' '
            )
        {
            return true;
        }
    }
    if (direction == WEST)
    {
        if (
                grid[y][x-1] != ' '
                &&  grid[y-1][x] != ' '
                && grid[y+1][x] != ' '
                && grid[y-1][x-1] != ' '
                && grid[y+1][x-1] != ' '
            )
        {
            return true;
        }
    }
    return false;
}

vector<vector<int>> generateRandomEntryAndExit(int mazeSize, vector<vector<char>> grid)
{
    vector<vector<int>> coords(mazeSize, vector<int>(mazeSize, 0));

    // select the wall
    srand(time(0));

    // get random coordinates making sure that to entry and exit are different
    do {

        for (int i = 0; i < 2; ++i) {

            int wall = rand() % 4;

            if (wall == 0)
            {
                do {
                    coords[i][0] = 0;
                    coords[i][1] = rand() % mazeSize;
                } while (grid[coords[i][0] + 1][coords[i][1]] != ' ');
            }
            else if (wall == 1)
            {
                do {
                    coords[i][0] = mazeSize - 1;
                    coords[i][1] = rand() % mazeSize;
                } while (grid[coords[i][0] - 1][coords[i][1]] != ' ');
            }
            else if (wall == 2)
            {
                do {
                    coords[i][0] = rand() % mazeSize;
                    coords[i][1] = 0;
                } while (grid[coords[i][0]][coords[i][1] + 1] != ' ');
            }
            else if (wall == 3)
            {
                do {
                    coords[i][0] = rand() % mazeSize;
                    coords[i][1] = mazeSize - 1;
                } while (grid[coords[i][0]][coords[i][1] - 1] != ' ');
            }
        }
    } while (coords[0] == coords[1]);

    return coords;
}

int main()
{
    // define the grid
    int mazeSize;
    cout << "Type the maze size: ";
    cin >> mazeSize;
    cout << endl;
    vector<vector<char>> grid(mazeSize, vector<char>(mazeSize, 0));

    // init grid
    for (int i = 0; i < mazeSize; i++)
    {
        for (int j = 0; j < mazeSize; ++j)
        {
            grid[i][j] = '#';
        }
    }

    // init rand
    srand(time(0));

    // init stacks for xy coords
    stack<int> xValues;
    stack<int> yValues;

    nGood = 0;
    int direction;

    do {
        // find n good moves
        for (int i = 0; i < 4; i++){
            if (isValid(locX, locY, i, grid, mazeSize))
                nGood++;
        }
        if (nGood == 1)
        {
            if (isValid(locX, locY, NORTH, grid, mazeSize))
                locY = moveNS(NORTH, locY);
            else if (isValid(locX, locY, SOUTH, grid, mazeSize))
                locY = moveNS(SOUTH, locY);
            else if (isValid(locX, locY, EAST, grid, mazeSize))
                locX = moveEW(EAST, locX);
            else if (isValid(locX, locY, WEST, grid, mazeSize))
                locX = moveEW(WEST, locX);
        }
        else if (nGood == 0)
        {
            locX = xValues.top();
            locY = yValues.top();
            xValues.pop();
            yValues.pop();
        }

            //if more than 1 good move, push stack
        else if (nGood > 1)
        {
            xValues.push(locX);
            yValues.push(locY);

            //direction to move randomly chosen
            do {
                direction = rand() % 4;
            } while (!isValid(locX, locY, direction, grid, mazeSize));

            locX = moveEW(direction,locX);
            locY = moveNS(direction,locY);
        }

        // set grid
        grid[locY][locX] = ' ';

//        //output grid to show creation
//        printGrid(grid);

        //reset nGood value
        nGood = 0;
    } while (!xValues.empty());

    // selecting a random entry and exit
    vector<vector<int>> entryAndExitCoords(2, vector<int>(2, 0));
    entryAndExitCoords = generateRandomEntryAndExit(mazeSize, grid);
    grid[entryAndExitCoords[0][0]][entryAndExitCoords[0][1]] = 'I';
    grid[entryAndExitCoords[1][0]][entryAndExitCoords[1][1]] = 'O';

    // final maze output
    printGrid(grid, mazeSize);
    return 0;
}
