#include <iostream>
#include <cstdlib>
#include <stack>
#include <ctime>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

using namespace std;

int nGood = 0;
int locX = 1, locY = 1;

// print the maze
void printGrid(char grid[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
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
bool isValid(int x, int y, int direction, char grid[10][10])
{
    x = moveEW(direction, x);
    y = moveNS(direction, y);

    // if the position is taken, or x and y are out of the grid then is not valid
    if (
            grid[y][x] == ' '
            || x >= (10 - 1)
            || x <= 0
            || y <= 0
            || y >= (10 - 1)
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

int main()
{
    char grid[10][10];

    // init grid
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; ++j)
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

    grid[locY - 1][locX] = '-';

    do {
        // find n good moves
        for (int i = 0; i < 4; i++){
            if (isValid(locX,locY,i,grid))
                nGood++;
        }
        if (nGood == 1)
        {
            if (isValid(locX, locY, NORTH, grid))
                locY = moveNS(NORTH, locY);
            else if (isValid(locX, locY, SOUTH, grid))
                locY = moveNS(SOUTH, locY);
            else if (isValid(locX, locY, EAST, grid))
                locX = moveEW(EAST, locX);
            else if (isValid(locX, locY, WEST, grid))
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
            do{
                direction = rand() % 4;
            }while (!isValid(locX, locY, direction, grid));

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
    // final maze output
    printGrid(grid);
    return 0;
}
