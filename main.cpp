#include <iostream>
#include <cstdlib>
#include <stack>
#include <ctime>
#include <vector>
#include <fstream>

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3

using namespace std;

int nGood = 0;
int locX = 1, locY = 1;

// print the maze
void printGrid(vector<vector<char>> maze)
{
    for (int i = 0; i < maze.size(); i++)
    {
        for (int j = 0; j < maze.size(); j++)
        {
            cout << maze[i][j];
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

vector<vector<int>> generateRandomEntryAndExit(vector<vector<char>> maze)
{
    vector<vector<int>> coords(maze.size(), vector<int>(maze.size(), 0));

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
                    coords[i][1] = rand() % maze.size();
                } while (maze[coords[i][0] + 1][coords[i][1]] != ' ');
            }
            else if (wall == 1)
            {
                do {
                    coords[i][0] = maze.size() - 1;
                    coords[i][1] = rand() % maze.size();
                } while (maze[coords[i][0] - 1][coords[i][1]] != ' ');
            }
            else if (wall == 2)
            {
                do {
                    coords[i][0] = rand() % maze.size();
                    coords[i][1] = 0;
                } while (maze[coords[i][0]][coords[i][1] + 1] != ' ');
            }
            else if (wall == 3)
            {
                do {
                    coords[i][0] = rand() % maze.size();
                    coords[i][1] = maze.size() - 1;
                } while (maze[coords[i][0]][coords[i][1] - 1] != ' ');
            }
        }
    } while (coords[0] == coords[1]);

    return coords;
}

void generateMazeFile(vector<vector<char>> maze, string mazeFileName = "maze")
{
    // create file
    ofstream mazeFile(mazeFileName + ".txt");

    // fill the file with the maze
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze.size(); ++j) {
            mazeFile << maze[i][j];
        }
        mazeFile << endl;
    }
    mazeFile.close();
}

vector<vector<char>> generateMazeFromFile(string mazeFileName = "maze")
{

    // get the size of the maze with the size of one of the lines
    string line;
    getline (ifstream(mazeFileName + ".txt"), line);

    ifstream mazeFile(mazeFileName + ".txt");

    vector<vector<char>> maze(line.size(), vector<char>(line.size(), 0));

    int counter = 0;
    while (getline (mazeFile, line)) {
        for (int i = 0; i < line.size(); ++i) {
            maze[counter][i] = line[i];
        }
        counter += 1;
    }
    return maze;
}

vector<int> getMazeEntryCoords(vector<vector<char>> maze)
{
    vector<int> entryCoords(2, 0);
    for (int i = 0; i < maze.size(); ++i) {
        for (int j = 0; j < maze.size(); ++j) {
            if (maze[i][j] == 'I')
            {
                entryCoords[0] = i;
                entryCoords[1] = j;
                return entryCoords;
            }
        }
    }
}

bool validPath(int x, int y, int direction, vector<vector<char>> maze)
{
    x = moveEW(direction, x);
    y = moveNS(direction, y);

    if (
            maze[y][x] == '#'
            || x >= (maze.size() - 1)
            || x <= 0
            || y <= 0
            || y >= (maze.size() - 1)
        )
        return false;

    if (direction == NORTH)
    {
        if (maze[y+1][x] != '#')
            return true;
    }
    if (direction == SOUTH)
    {
        if (maze[y-1][x] != '#')
            return true;
    }
    if (direction == EAST) {
        if (maze[y][x+1] != '#')
            return true;
    }
    if (direction == WEST)
    {
        if (maze[y][x-1] != '#')
            return true;
    }
    return false;
}

/*void solveMaze(vector<vector<char>> maze)
{
    vector<int> entryCoords = getMazeEntryCoords(maze);
    vector<vector<char>> solution = maze;

    int x = entryCoords[0];
    int y = entryCoords[1];

    // init rand
    srand(time(0));

    // init stacks for xy coords
    stack<int> xValues;
    stack<int> yValues;

    int goodMovements = 0;
    int direction;

    do {
        // find n good movements
        for (int i = 0; i < 4; ++i) {
            if (validPath(x, y, i, solution))
                goodMovements++;
        }

        // if there is only one good move then move there
        if (nGood == 1)
        {
            if (validPath(x, y, NORTH, solution))
                y = moveNS(NORTH, y);
            else if (validPath(x, y, SOUTH, solution))
                y = moveNS(SOUTH, y);
            else if (validPath(x, y, EAST, solution))
                x = moveEW(EAST, x);
            else if (validPath(x, y, WEST, solution))
                x = moveEW(WEST, x);
        }
        else if (nGood == 0)
        {
            x = xValues.top();
            y = yValues.top();
            xValues.pop();
            yValues.pop();
        }

            //if more than 1 good move, push stack
        else if (nGood > 1)
        {
            xValues.push(x);
            yValues.push(y);

            //direction to move randomly chosen
            do {
                direction = rand() % 4;
            } while (!validPath(x, y, direction, maze));

            x = moveEW(direction,x);
            y = moveNS(direction,y);
        }

        // set maze
        solution[y][x] = '-';
        if (solution[y][x] == 'O')
            printGrid(solution);

        //reset nGood value
        nGood = 0;

    } while (!xValues.empty());
}*/

int main()
{
    // define the maze
    int mazeSize;
    cout << "Type the maze size: ";
    cin >> mazeSize;
    cout << endl;
    vector<vector<char>> maze(mazeSize, vector<char>(mazeSize, 0));

    // init maze
    for (int i = 0; i < mazeSize; i++) {
        for (int j = 0; j < mazeSize; ++j) {
            maze[i][j] = '#';
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
            if (isValid(locX, locY, i, maze, mazeSize))
                nGood++;
        }

        // if there is only one good move then move there
        if (nGood == 1)
        {
            if (isValid(locX, locY, NORTH, maze, mazeSize))
                locY = moveNS(NORTH, locY);
            else if (isValid(locX, locY, SOUTH, maze, mazeSize))
                locY = moveNS(SOUTH, locY);
            else if (isValid(locX, locY, EAST, maze, mazeSize))
                locX = moveEW(EAST, locX);
            else if (isValid(locX, locY, WEST, maze, mazeSize))
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
            } while (!isValid(locX, locY, direction, maze, mazeSize));

            locX = moveEW(direction,locX);
            locY = moveNS(direction,locY);
        }

        // set maze
        maze[locY][locX] = ' ';

        //reset nGood value
        nGood = 0;
    } while (!xValues.empty());

    // selecting a random entry and exit
    vector<vector<int>> entryAndExitCoords(2, vector<int>(2, 0));
    entryAndExitCoords = generateRandomEntryAndExit(maze);
    maze[entryAndExitCoords[0][0]][entryAndExitCoords[0][1]] = 'I';
    maze[entryAndExitCoords[1][0]][entryAndExitCoords[1][1]] = 'O';

    // final maze output
    printGrid(maze);
    generateMazeFile(maze);
    return 0;
}
