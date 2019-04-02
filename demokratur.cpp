#include <iostream>
#include <random>
#include <windows.h>
#include <time.h>

using namespace std;

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
int* initField(int fieldsize, int parties);
int* initParties(int *parties, int partiessize);
int getRand(int inclusiveUpperBound);
bool isUniform();
void printField(int time);

static mt19937 engine;

static char signs[4] = {'.', 'X', '%', '#'};
static int *parties;
static int *field;
static int fieldX;
static int fieldY;
static int steps;
static int printSteps;
static int counter;

void init(int X, int Y, int *part, int partsize, int st, int prtSteps) {
    fieldX = X;
    fieldY = Y;
    steps = st;
    printSteps = prtSteps;
    counter = 0;

    random_device rd;
    seed_seq seed{static_cast<long unsigned int>(time(nullptr))};
    engine.seed(seed);

    parties = initParties(part, partsize);
    field = initField(fieldX * fieldY, partsize);
}

int main() {
    int parties[] = {50, 50};
    init(20, 20, parties, 2, 70000000, 100);
    int *candidate;
    int *neighbour;
    int limit = 0;
    int fieldsize = fieldX * fieldY;

    printField(0);
    while (true) {
        candidate = field + getRand(fieldsize);
        neighbour = getRandNeighbour(candidate);
        talk(candidate, neighbour);
//        swap(candidate, neighbour);
        counter++;
        if(counter > steps || isUniform()) {
            break;
        }
        limit++;
        if(limit > printSteps){
            printField(0);
            limit = 0;
        }
    }
    printField(0);
}

void swap(int *pos1, int *pos2) {
    int temp = *pos1;
    *pos1 = *pos2;
    *pos2 = temp;
}

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

int* pos(int x, int y) {
    int *position = field + x * fieldX + y;
    if(field <= position && position <= (field + fieldX * fieldY)) {
        return position;
    }
    return nullptr;
}

int getX(int *position) {
    int count = int(position - field);
    if(count < 0 || count > fieldX * fieldY) {
        return -1;
    }
    return count / fieldX;
}

int getY(int *position) {
    int count = int(position - field);
    if(count < 0 || count > fieldX * fieldY) {
        return -1;
    }
    return count % fieldX;
}

int* left(int *position) {
    int x = getX(position);
    if(x > -1) {
        position = (x - 1 >= 0) ? --position : position + fieldX - 1;
        return position;
    }
    return nullptr;
}

int* right(int *position) {
    int x = getX(position);
    if(x > -1) {
        position = (x + 1 < fieldX) ? ++position : position - fieldX + 1;
        return position;
    }
    return nullptr;
}

int* up(int *position) {
    int y = getY(position);
    if(y > -1) {
        position = (position - fieldX >= field) ? position - fieldX : position - fieldX + fieldX * fieldY;
        return position;
    }
    return nullptr;
}

int* down(int *position) {
    int y = getY(position);
    if(y > -1) {
        position = (position + fieldX < field + fieldX * fieldY) ? position + fieldX : position + fieldX - fieldX * fieldY;
        return position;
    }
    return nullptr;
}

int* initParties(int *parties, int partiessize) {
    int* ptr = new int[unsigned(partiessize)];
    for(int i = 0; i < partiessize; i++){
        int *p = ptr + i;
        *p = parties[i];
    }
    return ptr;
}

int* initField(int fieldsize, int parties) {
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

int* initFieldRand(int fieldsize, int parties) {
    int *field = new int[unsigned(fieldsize)];
    for (int i = 0; i < fieldsize; i++) {
        int *p = field + i;
        *p = getRand(parties);
    }
    return field;
}

void printField(int time) {
    system("cls");
    cout << "DEMOKRATUR" << endl << endl;
    int *pos = field;
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            cout << signs[*pos];
            pos++;
        }
        cout << endl;
    }
    cout << endl << "COUNTER: " << counter << endl;
    Sleep(time);
}

bool isUniform() {
    for(int i = 1; i < fieldX * fieldY; i++){
        if(*(field + i) != *field) {
            return false;
        }
    }
    return true;
}

int getRand(int exclusiveUpperBound) {
    uniform_int_distribution<int> random(0, exclusiveUpperBound - 1);
    return random(engine);
}
