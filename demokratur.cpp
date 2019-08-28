#include <iostream>
#include <random>
#include <string>
#include <sstream>
#include <windows.h>
#include <time.h>

using namespace std;

/* functions */
void init(int X, int Y, int part, int st);
int* getRandNeighbour(int* candidate);
void talk(int *candidate, int *neighbour);
void swap(int *pos1, int *pos2);
int* pos(int x, int y);
int  getX(int *position);
int  getY(int *position);
int* left(int *position);
int* right(int *position);
int* up(int *position);
int* down(int *position);
int* initFieldRand(int fieldsize, int parties);
int* initFieldUniform(int fieldsize, int parties);
int* initParties(int *parties, int partiessize);
int getRand(int inclusiveUpperBound);
bool isUniform();
void out(unsigned int time);
string printField();

/* fields */
static mt19937 engine;
//static char signs[4] = {'.', 'X', '%', '#'};
static int *parties;
static int *field;
static int fieldX;
static int fieldY;
static int *candidate;
static int *neighbour;
static int steps;
static int printSteps;
static int counter;
static int limit;

//void init(int X, int Y, int *part, int partsize, int st, int prtSteps) {
//    if (partsize > 4) {
//        throw -1;
//    }
//    fieldX = X;
//    fieldY = Y;
//=======
/**
 * @brief init initializes all necessary field for the programm
 * @param x the wide of the population-field [1..x], inclusive
 * @param y the length of the population-field [1..y], inclusive
 * @param part a pointer to an array of probabilities for the different parties
 * @param partsize the size of the array of probabilities for the different parties
 * @param st the maximum calculation stepsize
 * @param prtSteps the stepsize which induce the printing of the population-field when the calculation count is
 *        crossing this border
 */
void init(int x, int y, int *part, int partsize, int st, int prtSteps) {
    fieldX = x;
    fieldY = y;

    steps = st;
    printSteps = prtSteps;
    counter = 0;

    random_device rd;
    seed_seq seed{static_cast<long unsigned int>(time(nullptr))};
    engine.seed(seed);

    parties = initParties(part, partsize);
    field = initFieldRand(fieldX * fieldY, partsize);
}

void init(int argc, char** argv) {
    for(int j = 0; j < argc; j++) {
       if(fieldX == 0 && strcmp(argv[j], "--x") != 1) {
           stringstream sstr;
           sstr << argv[j + 1];
           sstr >> fieldX;
           if(sstr.fail() || fieldX < 0) {
               cout << "failed to read, but expected a valid parameter for \"fieldX\" after (--x)" << endl
                    << "please check your input" << endl;
               throw -1;
           }
           cout << fieldX;
        } else if(fieldY == 0 && strcmp(argv[j], "--y") != 1) {
           stringstream sstr;
           sstr << argv[j + 1];
           sstr >> fieldY;
           if(sstr.fail() || fieldY < 0) {
               cout << "failed to read, but expected a valid parameter for \"fieldY\" after (--y)" << endl
                    << "please check your input" << endl;
               throw -1;
           }
           cout << fieldY;
        } else if((steps == 0) && (strcmp(argv[j], "--s") != 1)) {
           stringstream sstr;
           sstr << argv[j + 1];
           sstr >> steps;
           if(sstr.fail() || steps < 0) {
               cout << "failed to read, but expected a valid parameter for \"steps\" after (--s)" << endl
                    << "please check your input" << endl;
               throw -1;
           }
           cout << steps;
        } else if(printSteps == 0 && strcmp(argv[j], "--p") != 1) {
           stringstream sstr;
           sstr << argv[j + 1];
           sstr >> printSteps;
           if(sstr.fail() || printSteps < 0) {
               cout << "failed to read, but expected a valid parameter for \"printSteps\" after (--p)" << endl
                    << "please check your input" << endl;
               throw -1;
           }
           cout << printSteps;
        }
    }
    if(fieldX == 0) {
        fieldX = 20;
    }
    if(fieldY == 0) {
        fieldY = 20;
    }
    if(steps == 0) {
        steps = 70000000;
    }
    if(printSteps == 0) {
        printSteps = 1000;
    }

    int p[] = {50, 50};
    parties = initParties(p, sizeof (p)/sizeof (int));
    field = initFieldRand(fieldX * fieldY, 2);

    counter = 0;

    random_device rd;
    seed_seq seed{static_cast<long unsigned int>(time(nullptr))};
    engine.seed(seed);
}

/**
 * @brief main the main application
 * @return an error-value
 */
int main(int argc, char** argv) {
    init(argc, argv);
    limit = 0;
    int fieldsize = fieldX * fieldY;

    out(30);
    while (true) {
        candidate = field + getRand(fieldsize);
        neighbour = getRandNeighbour(candidate);
        talk(candidate, neighbour);
        counter++;
        if(counter > steps || isUniform()) {
            break;
        }
        limit++;
        if(limit > printSteps){
            out(30);
            limit = 0;
        }
    }
    out(30);
}

/**
 * @brief swap swaps the values of two int pointer
 * @param pos1 one int pointer
 * @param pos2 the other in pointer
 */
void swap(int *pos1, int *pos2) {
    int temp = *pos1;
    *pos1 = *pos2;
    *pos2 = temp;
}

/**
 * @brief talk the candidate tries to convinve its randomly choosen neighbour of its party(its value)
 *        by a given probability defined in in the parties array
 * @param candidate a int pointer to the value of the candidate
 * @param neighbour a int pointer to the value of the neighbour
 */
void talk(int *candidate, int *neighbour) {
    if(*candidate != *neighbour) {
        int partyCandidate = parties[*candidate];
        int partyNeighbour = parties[*neighbour];
        int rand = getRand(partyCandidate + partyNeighbour);
        if(rand >= partyCandidate) {
            *candidate = *neighbour;
        }
    }
}

/**
 * @brief randomly choses a neighbour on the left side, on the right side, above or under the position
 *        of the given candidate
 * @param candidate an int pointer to a value of the population-field
 * @return an int pointer the value of a neighbour of the given pointer of the population-field
 */
int* getRandNeighbour(int* candidate){
    int* position = candidate;
    int choosen = getRand(4);
    switch (choosen) {
    case 0: {
        return left(position);
    }
    case 1: {
        return right(position);
    }
    case 2: {
        return up(position);
    }
    case 3: {
        return down(position);
    }
    default:
        return nullptr;
    }
}

/**
 * @brief determines the pointer to a value of the population-field from the given x- and y-coordinate
 * @param x a x-coordinate of the field
 * @param y a y-coordinate of the field
 * @return a int pointer to the value indicatet by the coordinates
 */
int* pos(int x, int y) {
    int *position = field + x * fieldX + y;
    if(field <= position && position <= (field + fieldX * fieldY)) {
        return position;
    }
    return nullptr;
}

/**
 * @brief determines the x-coordinate of a given pointer to a value of the population-field
 * @param position a pointer to a value of the population-field
 * @return the x-coordinate of the given position
 */
int getX(int *position) {
    int count = int(position - field);
    if(count < 0 || count > fieldX * fieldY) {
        return -1;
    }
    return count / fieldX;
}

/**
 * @brief determines the y-coordinate of the given pointer to a value of the population-field
 * @param position a pointer to a value of the population-field
 * @return the y-coordinate of the given position
 */
int getY(int *position) {
    int count = int(position - field);
    if(count < 0 || count > fieldX * fieldY) {
        return -1;
    }
    return count % fieldX;
}

/**
 * @brief determines the positon for the field on the left side of the given position
 * @param position a int pointer to a value of the population field
 * @return a int pointer to the value of the left side from the given position
 */
int* left(int *position) {
    int x = getX(position);
    if(x > -1) {
        position = (x - 1 >= 0) ?
                    --position :
                    position + fieldX - 1;
        return position;
    }
    return nullptr;
}

/**
 * @brief determines the positon for the field on the rigth side of the given position
 * @param position a int pointer to a value of the population field
 * @return a int pointer to the value of the right side from the given position
 */
int* right(int *position) {
    int x = getX(position);
    if(x > -1) {
        position = (x + 1 < fieldX) ?
                    ++position :
                    position - fieldX + 1;
        return position;
    }
    return nullptr;
}

/**
 * @brief determines the positon for the field above the given position
 * @param position a int pointer to a value of the population field
 * @return a int pointer to the value above the given position
 */
int* up(int *position) {
    int y = getY(position);
    if(y > -1) {
        position = (position - fieldX >= field) ?
                    position - fieldX :
                    position - fieldX + fieldX * fieldY;
        return position;
    }
    return nullptr;
}

/**
 * @brief determines the positon for the field below the given position
 * @param position a int pointer to a value of the population field
 * @return a int pointer to the value below the given position
 */
int* down(int *position) {
    int y = getY(position);
    if(y > -1) {
        position = (position + fieldX < field + fieldX * fieldY) ?
                    position + fieldX :
                    position + fieldX - fieldX * fieldY;
        return position;
    }
    return nullptr;
}

/**
 * @brief initializes a array of "parties". Each party is represented by the probality for it.
 * @param parties a pointer to an existing party-array
 * @param partiessize the size of the party-array
 * @return the pointer to the new party-array
 */
int* initParties(int *parties, int partiessize) {
    int* ptr = new int[unsigned(partiessize)];
    for(int i = 0; i < partiessize; i++){
        int *p = ptr + i;
        *p = parties[i];
    }
    return ptr;
}

/**
 * @brief initialzies a new field in a uniformed way
 * @param fieldsize the size of the field
 * @param parties the count of the parties the field will be initialized with
 * @return *int: a pointer to a new field
 */
int* initFieldUniform(int fieldsize, int parties) {
    int *field = new int[unsigned(fieldsize)];
    int size = fieldsize / parties;
    int oversize = fieldsize & parties;
    for(int i = 0; i < parties; i++) {
        if(i < (parties - 1)) {
            for(int j = i * size; j < (i + 1) * size; j++) {
                int *p = field + j;
                *p = i;
            }
        } else {
            for(int j = (parties - 1) * size; j < parties * size + oversize; j++) {
                int *p = field + j;
                *p = i;
            }
        }
    }
    return field;
}

/**
 * @brief initializes a new field in a random way
 * @param fieldsize the size of the field
 * @param parties the count of the parties the field will be initializes with
 * @return *int: a pointer to a new field
 */
int* initFieldRand(int fieldsize, int parties) {
    int *field = new int[unsigned(fieldsize)];
    for (int i = 0; i < fieldsize; i++) {
        int *p = field + i;
        *p = getRand(parties);
    }
    return field;
}

void out(unsigned int time) {
    system("cls");
    cout << printField();
    Sleep(time);
}

//string printField() {
//    stringstream sstr();
//    sstr << "DEMOKRATUR" << endl << endl;
//    int *pos = field;
//    for (int y = 0; y < fieldY; y++) {
//        for (int x = 0; x < fieldX; x++) {
//            sstr << signs[*pos];
//    cout << printField() << endl;
//    Sleep(time);
//}

/**
 * @brief printField prints the population-field to the screen an holds the programm for a given time.
 * @param time a time in ms to hold the programm
 */
string printField() {
    stringstream sstr;
    sstr << "DEMOKRATUR" << endl;
    sstr << endl;
    sstr << "field x: " << fieldX << endl;
    sstr << "field y: " << fieldY << endl;
    sstr << "steps : " << steps << endl;
    sstr << "print steps : " << printSteps << endl;
    sstr << endl;
    int *pos = field;
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            sstr << *pos;
            pos++;
        }
        sstr << endl;
    }
    sstr << endl << "COUNTER: " << counter << endl;
    return sstr.str();
}

/**
 * @brief proofs if the field is filled with uniform values
 * @return true - if the field is filled with uniform values <br>
 *         false - else
 */
bool isUniform() {
    for(int i = 1; i < fieldX * fieldY; i++){
        if(*(field + i) != *field) {
            return false;
        }
    }
    return true;
}

/**
 * @brief determines a random number between 0 and the given exclusive upper bound
 * @param exclusiveUpperBound a upper bound which is exclusive
 * @return int: 0 <= randrom number < exclusiveUpperBound
 */
int getRand(int exclusiveUpperBound) {
    uniform_int_distribution<int> random(0, exclusiveUpperBound - 1);
    return random(engine);
}
