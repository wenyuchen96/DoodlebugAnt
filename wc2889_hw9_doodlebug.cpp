#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class Organism
{
public:
    Organism(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge);                         // constructor for intializing
    Organism(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge, int refRow, int refCol); // constructor got breeding given the coordinates are known
    virtual ~Organism();

    virtual void move();
    virtual void breed(); // breed function call the breed constructor
    int randomInt();

    // getters and setters
    vector<vector<char>> &getWholeGrid();
    char getGrid(int getRow, int getCol);
    void setGrid(int setRow, int setCol, char setMark);

    vector<Organism *> &getObjectVec();
    // void setObjectVec();

    int getRow() const;
    void setRow(int newRow);

    int getCol() const;
    void setCol(int newCol);

    int getAge() const;
    void setAge(int newAge);
    void increaseAge();

    int getBreedAge() const;

    char getMark() const;

private:
    vector<vector<char>> &grid;
    vector<Organism *> &objectVec;
    int row;
    int col;
    int age;
    int breedAge;
    char mark;
};

class Ant : public Organism
{
public:
    Ant(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge);
    Ant(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge, int refRow, int refCol); // breed constructor
    virtual ~Ant();
    virtual void move();
    virtual void breed();

private:
};

class Doodlebug : public Organism
{
public:
    Doodlebug(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, vector<Organism *> &refAntVec, char refMark, int refBreedAge, int refStarveAge, int refStepSinceLastMeal);
    Doodlebug(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, vector<Organism *> &refAntVec, char refMark, int refBreedAge, int refStarveAge, int refStepSinceLastMeal, int refRow, int refCol); // breed constructor
    virtual ~Doodlebug();
    virtual void move();
    virtual void breed();
    void starve();

private:
    vector<Organism *> &antVec;
    int starveAge;
    int stepSinceLastMeal;
};

int main()
{
    srand(static_cast<unsigned>(time(0))); // random seed
    // create a matrix of 20 by 20
    int time{0};
    const int rows{20}, cols{20};
    vector<vector<char>> grid(rows, vector<char>(cols, '-'));
    // create a vector to hold objects
    vector<Organism *> antVec{};
    for (int i = 0; i < 100; i++)
    {
        new Ant(grid, antVec, 'o', 3);
    }
    vector<Organism *> doodlebugVec{};
    for (int i = 0; i < 5; i++)
    {
        new Doodlebug(grid, doodlebugVec, antVec, 'X', 8, 3, 0);
    }

    string input;
    // time=0
    cout << "World at time " << time << ":\n\n";
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            cout << grid[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << '\n'
         << "Press ENTER to continue";
    while (true) // press ENTER to continue, press any key to exit the loop
    {
        getline(cin, input);
        if (input.empty()) // Print the matrix
        {
            time++;
            // Loop for Doodlebugs
            for (size_t i = 0; i < doodlebugVec.size(); ++i)
            {
                if (doodlebugVec[i] != nullptr)
                {
                    doodlebugVec[i]->move();
                }
            }
            // Loop for Ants
            for (size_t i = 0; i < antVec.size(); ++i)
            {
                if (antVec[i] != nullptr)
                {
                    antVec[i]->move();
                }
            }
            cout << "\nWorld at time " << time << ":\n\n";
            for (int i = 0; i < rows; i++)
            {
                for (int j = 0; j < cols; j++)
                {
                    cout << grid[i][j] << ' ';
                }
                cout << '\n';
            }
            cout << '\n'
                 << "Press ENTER to continue";
        }
        else
        {
            break;
        }
    }
    // Clean up dynamically allocated organisms
    for (auto ant : antVec)
    {
        delete ant;
    }
    antVec.clear();

    for (auto doodlebug : doodlebugVec)
    {
        delete doodlebug;
    }
    doodlebugVec.clear();

    return 0;
}

// Organism: implementations of class functions
Organism::Organism(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge) : grid(refGrid), objectVec(refObjectVec), mark(refMark), breedAge(refBreedAge)
{
    objectVec.push_back(this);
    int tempRow{randomInt()};
    int tempCol{randomInt()};
    while (grid[tempRow][tempCol] != '-' || tempRow < 0 || tempRow >= 20 || tempCol < 0 || tempCol >= 20)
    {
        tempRow = randomInt();
        tempCol = randomInt();
    }
    row = tempRow;
    col = tempCol;
    // mark the organism in grid and age = 0
    grid[row][col] = mark;
    age = 0;
};

Organism::Organism(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge, int refRow, int refCol) : grid(refGrid), objectVec(refObjectVec), mark(refMark), breedAge(refBreedAge), row(refRow), col(refCol)
{
    objectVec.push_back(this);
    // mark the organism in grid and age = 0
    grid[row][col] = mark;
    age = 0;
}

Organism::~Organism()
{
    // No specific cleanup needed for base class
}

void Organism::breed()
{
    // four coordinates up, down, left, right to choose from
    // up: (row-1,col)
    int upRow = row - 1;
    // down: (row+1,col)
    int downRow = row + 1;
    // left: (row,col-1)
    int leftCol = col - 1;
    // right: (row,col+1)
    int rightCol = col + 1;
    if (upRow >= 0 && upRow <= 19 && col >= 0 && col <= 19 && (grid[upRow][col] == '-'))
    {
        new Organism(grid, objectVec, mark, breedAge, upRow, col);
    }
    else if (downRow >= 0 && downRow <= 19 && col >= 0 && col <= 19 && (grid[downRow][col] == '-'))
    {
        new Organism(grid, objectVec, mark, breedAge, downRow, col);
    }
    else if (row >= 0 && row <= 19 && leftCol >= 0 && leftCol <= 19 && (grid[row][leftCol] == '-'))
    {
        new Organism(grid, objectVec, mark, breedAge, row, leftCol);
    }
    else if (row >= 0 && row <= 19 && rightCol >= 0 && rightCol <= 19 && (grid[row][rightCol] == '-'))
    {
        new Organism(grid, objectVec, mark, breedAge, row, rightCol);
    }
    else
    {
    }
};

void Organism::move()
{
    int tempRow{row + (rand() % 2 * 2 - 1)};
    int tempCol{col + (rand() % 2 * 2 - 1)};
    if (tempRow < 0 || tempRow > 19 || tempCol < 0 || tempCol > 19 || (grid[tempRow][tempCol] != '-'))
    {
        return;
    }
    else
    {
        grid[row][col] = '-';
        // move and update the coordinates
        row = tempRow;
        col = tempCol;
        grid[row][col] = mark;
    }
    age++; // update time whenever there's move
    if ((age % breedAge) == 0)
    {
        breed();
    }
};

int Organism::randomInt()
{
    return (rand() % 20);
};

vector<vector<char>> &Organism::getWholeGrid() { return grid; }
char Organism::getGrid(int getRow, int getCol) { return grid[getRow][getCol]; }
void Organism::setGrid(int setRow, int setCol, char setMark) { grid[setRow][setCol] = setMark; }

vector<Organism *> &Organism::getObjectVec() { return objectVec; };

int Organism::getRow() const { return row; }
void Organism::setRow(int newRow) { row = newRow; }

int Organism::getCol() const { return col; }
void Organism::setCol(int newCol) { col = newCol; }

int Organism::getAge() const { return age; }
void Organism::setAge(int newAge) { age = newAge; }
void Organism::increaseAge() { age++; }

int Organism::getBreedAge() const { return breedAge; }

char Organism::getMark() const { return mark; }

// implementations of Ant class
Ant::Ant(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge) : Organism(refGrid, refObjectVec, refMark, refBreedAge) {}

Ant::Ant(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, char refMark, int refBreedAge, int refRow, int refCol) : Organism(refGrid, refObjectVec, refMark, refBreedAge, refRow, refCol) {} // breed constructor

Ant::~Ant()
{
    // No specific cleanup needed for ant class
}

void Ant::move()
{
    int tempRow{getRow() + (rand() % 2 * 2 - 1)};
    int tempCol{getCol() + (rand() % 2 * 2 - 1)};
    if (tempRow < 0 || tempRow > 19 || tempCol < 0 || tempCol > 19 || (getGrid(tempRow, tempCol) != '-'))
    {
        return;
    }
    else
    {
        setGrid(getRow(), getCol(), '-');
        // move and update the coordinates
        setRow(tempRow);
        setCol(tempCol);
        setGrid(getRow(), getCol(), getMark());
    }
    increaseAge(); // update time whenever there's move
    if ((getAge() % getBreedAge()) == 0)
    {
        breed();
    }
}

void Ant::breed()
{
    // four coordinates up, down, left, right to choose from
    // up: (row-1,col)
    int upRow = getRow() - 1;
    // down: (row+1,col)
    int downRow = getRow() + 1;
    // left: (row,col-1)
    int leftCol = getCol() - 1;
    // right: (row,col+1)
    int rightCol = getCol() + 1;
    if (upRow >= 0 && upRow <= 19 && getCol() >= 0 && getCol() <= 19 && (getGrid(upRow, getCol()) == '-'))
    {
        new Ant(getWholeGrid(), getObjectVec(), getMark(), getBreedAge(), upRow, getCol());
    }
    else if (downRow >= 0 && downRow <= 19 && getCol() >= 0 && getCol() <= 19 && (getGrid(downRow, getCol()) == '-'))
    {
        new Ant(getWholeGrid(), getObjectVec(), getMark(), getBreedAge(), downRow, getCol());
    }
    else if (getRow() >= 0 && getRow() <= 19 && leftCol >= 0 && leftCol <= 19 && (getGrid(getRow(), leftCol) == '-'))
    {
        new Ant(getWholeGrid(), getObjectVec(), getMark(), getBreedAge(), getRow(), leftCol);
    }
    else if (getRow() >= 0 && getRow() <= 19 && rightCol >= 0 && rightCol <= 19 && (getGrid(getRow(), rightCol) == '-'))
    {
        new Ant(getWholeGrid(), getObjectVec(), getMark(), getBreedAge(), getRow(), rightCol);
    }
    else
    {
    }
}

// implementations of Doodlebug class
Doodlebug::Doodlebug(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, vector<Organism *> &refAntVec, char refMark, int refBreedAge, int refStarveAge, int refStepSinceLastMeal) : Organism(refGrid, refObjectVec, refMark, refBreedAge), antVec(refAntVec), starveAge(refStarveAge), stepSinceLastMeal(refStepSinceLastMeal) {}

Doodlebug::Doodlebug(vector<vector<char>> &refGrid, vector<Organism *> &refObjectVec, vector<Organism *> &refAntVec, char refMark, int refBreedAge, int refStarveAge, int refStepSinceLastMeal, int refRow, int refCol) : Organism(refGrid, refObjectVec, refMark, refBreedAge, refRow, refCol), antVec(refAntVec), starveAge(refStarveAge), stepSinceLastMeal(refStepSinceLastMeal) {} // breed constructor

Doodlebug::~Doodlebug()
{
    // No specific cleanup needed for ant class
}

void Doodlebug::move()
{
    // check to move to the adjacent ant
    int upRow = getRow() - 1;
    // down: (row+1,col)
    int downRow = getRow() + 1;
    // left: (row,col-1)
    int leftCol = getCol() - 1;
    // right: (row,col+1)
    int rightCol = getCol() + 1;
    if (upRow >= 0 && upRow <= 19 && getCol() >= 0 && getCol() <= 19 && (getGrid(upRow, getCol()) == 'o'))
    {
        stepSinceLastMeal = 0;      // restore to 0
        for (auto &object : antVec) // linear search to identify the target ant in the array
        {
            if (object->getRow() == upRow && object->getCol() == getCol())
            {

                delete object; // Safely delete pointer
                object = nullptr;

                auto it = find(antVec.begin(), antVec.end(), object);
                if (it != antVec.end())
                {
                    antVec.erase(it);
                } // Remove pointer from vector}

                setGrid(getRow(), getCol(), '-');       // original coordinate becomes '-'
                setRow(upRow);                          // update the new cooridinates
                setGrid(getRow(), getCol(), getMark()); // update the graph
                break;
            }
        }
    }
    else if (downRow >= 0 && downRow <= 19 && getCol() >= 0 && getCol() <= 19 && (getGrid(downRow, getCol()) == 'o'))
    {
        stepSinceLastMeal = 0;      // restore to 0
        for (auto &object : antVec) // linear search to identify the target ant in the array
        {
            if (object->getRow() == downRow && object->getCol() == getCol())
            {

                delete object; // free the memory
                object = nullptr;

                auto it = find(antVec.begin(), antVec.end(), object);
                if (it != antVec.end())
                {
                    antVec.erase(it);
                } // Remove pointer from vector}

                setGrid(getRow(), getCol(), '-');       // original coordinate becomes '-'
                setRow(downRow);                        // update the new cooridinates
                setGrid(getRow(), getCol(), getMark()); // update the graph
                break;
            }
        }
    }
    else if (getRow() >= 0 && getRow() <= 19 && leftCol >= 0 && leftCol <= 19 && (getGrid(getRow(), leftCol) == 'o'))
    {
        stepSinceLastMeal = 0;      // restore to 0
        for (auto &object : antVec) // linear search to identify the target ant in the array
        {
            if (object->getRow() == getRow() && object->getCol() == leftCol)
            {

                delete object; // free the memory
                object = nullptr;

                auto it = find(antVec.begin(), antVec.end(), object);
                if (it != antVec.end())
                {
                    antVec.erase(it);
                } // Remove pointer from vector}

                setGrid(getRow(), getCol(), '-');       // original coordinate becomes '-'
                setCol(leftCol);                        // update the new cooridinates
                setGrid(getRow(), getCol(), getMark()); // update the graph
                break;
            }
        }
    }
    else if (getRow() >= 0 && getRow() <= 19 && rightCol >= 0 && rightCol <= 19 && (getGrid(getRow(), rightCol) == 'o'))
    {
        stepSinceLastMeal = 0;      // restore to 0
        for (auto &object : antVec) // linear search to identify the target ant in the array
        {
            if (object->getRow() == getRow() && object->getCol() == rightCol)
            {

                delete object; // free the memory
                object = nullptr;

                auto it = find(antVec.begin(), antVec.end(), object);
                if (it != antVec.end())
                {
                    antVec.erase(it);
                } // Remove pointer from vector}

                setGrid(getRow(), getCol(), '-');       // original coordinate becomes '-'
                setCol(rightCol);                       // update the new cooridinates
                setGrid(getRow(), getCol(), getMark()); // update the graph
                break;
            }
        }
    }
    else
    {
        // otherwise, move like a ant
        stepSinceLastMeal++; // didn't catch an ant
        int tempRow{getRow() + (rand() % 2 * 2 - 1)};
        int tempCol{getCol() + (rand() % 2 * 2 - 1)};
        if (tempRow < 0 || tempRow > 19 || tempCol < 0 || tempCol > 19 || (getGrid(tempRow, tempCol) != '-'))
        {
            return;
        }
        else
        {
            setGrid(getRow(), getCol(), '-');
            // move and update the coordinates
            setRow(tempRow);
            setCol(tempCol);
            setGrid(getRow(), getCol(), getMark());
        }
    }
    increaseAge();                       // update time whenever there's move
    if ((getAge() % getBreedAge()) == 0) // check breed
    {
        breed();
    }
    if (stepSinceLastMeal >= starveAge) // check starve
    {
        starve();
    }
}

void Doodlebug::breed()
{
    // four coordinates up, down, left, right to choose from
    // up: (row-1,col)
    int upRow = getRow() - 1;
    // down: (row+1,col)
    int downRow = getRow() + 1;
    // left: (row,col-1)
    int leftCol = getCol() - 1;
    // right: (row,col+1)
    int rightCol = getCol() + 1;
    if (upRow >= 0 && upRow <= 19 && getCol() >= 0 && getCol() <= 19 && (getGrid(upRow, getCol()) == '-'))
    {
        new Doodlebug(getWholeGrid(), getObjectVec(), antVec, getMark(), getBreedAge(), starveAge, 0, upRow, getCol());
    }
    else if (downRow >= 0 && downRow <= 19 && getCol() >= 0 && getCol() <= 19 && (getGrid(downRow, getCol()) == '-'))
    {
        new Doodlebug(getWholeGrid(), getObjectVec(), antVec, getMark(), getBreedAge(), starveAge, 0, downRow, getCol());
    }
    else if (getRow() >= 0 && getRow() <= 19 && leftCol >= 0 && leftCol <= 19 && (getGrid(getRow(), leftCol) == '-'))
    {
        new Doodlebug(getWholeGrid(), getObjectVec(), antVec, getMark(), getBreedAge(), starveAge, 0, getRow(), leftCol);
    }
    else if (getRow() >= 0 && getRow() <= 19 && rightCol >= 0 && rightCol <= 19 && (getGrid(getRow(), rightCol) == '-'))
    {
        new Doodlebug(getWholeGrid(), getObjectVec(), antVec, getMark(), getBreedAge(), starveAge, 0, getRow(), leftCol);
    }
    else
    {
    }
}

void Doodlebug::starve()
{
    setGrid(getRow(), getCol(), '-'); // Update the graph

    auto &objDoodlebugVec = getObjectVec(); // Obtain a reference to the vector
    auto it = std::find(objDoodlebugVec.begin(), objDoodlebugVec.end(), this);

    // Erase the pointer from the vector if found
    if (it != objDoodlebugVec.end())
    {
        objDoodlebugVec.erase(it);
    }
    delete this;
    // Find the current object in the vector
}