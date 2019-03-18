#include <iostream>
#include <random>
#include <windows.h>

using namespace std;

void init(int X, int Y, int part, int st);
int* getRandNeighbour(int* candidate);
void talk(int *candidate, int *neighbour);
int* pos(int x, int y);
int  getX(int *position);
int  getY(int *position);
int* left(int *position);
int* right(int *position);
int* up(int *position);
int* down(int *position);
int* initField(int fieldsize, int parties);
void printField();

static random_device rd;
static mt19937 engine(rd());
static uniform_int_distribution<int> randNeighbour(0, 3);
static uniform_int_distribution<int> randField;

static int *parties;
static int *field;
static int fieldX;
static int fieldY;
static int steps;

void init(int X, int Y, int* part, int st) {
    fieldX = X;
    fieldY = Y;
    parties = part;
    steps = st;

    field = initField(fieldX * fieldY, sizeof(part)/sizeof(*part));

    uniform_int_distribution<int> temp(0, fieldX * fieldY - 1);
    randField = temp;
}

int main() {
    int parties[] = {50, 50};
    init(20, 20, parties, 40000);
    int *candidate;
    int *neighbour;
    int counter = 0;
    for (int i = 0; i < steps; i++) {
        candidate = field + randField(engine);
        neighbour = getRandNeighbour(candidate);
        if(*candidate != *neighbour) {
            talk(candidate, neighbour);
        }
        counter++;
        if(counter > 1000){
            system("cls");
            printField();
            Sleep(500);
            counter = 0;
        }
    }
}

void talk(int *candidate, int *neighbour) {
    int* partyCandidate = parties + *candidate;
    int* partyNeighbour = parties + *neighbour;
    uniform_int_distribution<int> randParty(1, *partyCandidate + *partyNeighbour);
    if(randParty(engine) > *partyCandidate) {
        *candidate = *neighbour;
    }
}

int* getRandNeighbour(int* candidate){
    int* position = candidate;
    int choosen = randNeighbour(engine);
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
        position = (x - 1 >= 0) ? position-- : position + fieldX - 1;
        return position;
    }
    return nullptr;
}

int* right(int *position) {
    int x = getX(position);
    if(x > -1) {
        position = (x + 1 < fieldX) ? position++ : position - fieldX + 1;
        return position;
    }
    return nullptr;
}

int* up(int *position) {
    int y = getY(position);
    if(y > -1) {
        position = (y - fieldX >= 0) ? position - fieldX : position - fieldX + fieldX * fieldY;
        return position;
    }
    return nullptr;
}

int* down(int *position) {
    int y = getY(position);
    if(y > -1) {
        position = (y + fieldX < fieldY) ? position + fieldX : position + fieldX - fieldX * fieldY;
        return position;
    }
    return nullptr;
}

int* initField(int fieldsize, int parties) {
    if(fieldsize > 0 && parties > 0) {
        uniform_int_distribution<int> randInit(0, parties - 1);
        int *field = new int[unsigned(fieldsize)];
        for (int i = 0; i < fieldsize; i++) {
            int *p = field + i;
            *p = randInit(engine);
        }
        return field;
    }
    return nullptr;
}

void printField() {
    int *pos = field;
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            cout << *pos++;
        }
        cout << endl;
    }
}
