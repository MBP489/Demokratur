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
int getRand(int inclusiveUpperBound);
void printField();

static int *parties;
static int *field;
static int fieldX;
static int fieldY;
static int steps;
static int counter;

void init(int X, int Y, int *part, int partsize, int st) {
    fieldX = X;
    fieldY = Y;
    steps = st;
    counter = 0;

    parties = new int[partsize];
    for(int i = 0; i < partsize; i++){
        int *p = parties + i;
        *p = part[i];
    }

    field = initField(fieldX * fieldY, partsize);
}

int main() {
    int parties[] = {50, 50};
    init(20, 20, parties, 2, 1000000);
    printField();
    int *candidate;
    int *neighbour;
    int limit = 0;
    int fieldsize = fieldX * fieldY;
    for (int i = 0; i < steps; i++) {
        candidate = field + getRand(fieldsize);
        neighbour = getRandNeighbour(candidate);
        if(*candidate != *neighbour) {
            talk(candidate, neighbour);
        }
        limit++;
        counter++;
        if(limit > 10000){
            printField();
            limit = 0;
        }
    }
}

void talk(int *candidate, int *neighbour) {
    int partyCandidate = parties[*candidate];
    int partyNeighbour = parties[*neighbour];
    if(getRand(partyCandidate + partyNeighbour) >= partyCandidate) {
        *candidate = *neighbour;
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

int* initField(int fieldsize, int parties) {
    if(fieldsize > 0 && parties > 0) {
        int *field = new int[unsigned(fieldsize)];
        for (int i = 0; i < fieldsize; i++) {
            int *p = field + i;
            *p = getRand(parties);
        }
        return field;
    }
    return nullptr;
}

void printField() {
    system("cls");
    int *pos = field;
    for (int y = 0; y < fieldY; y++) {
        for (int x = 0; x < fieldX; x++) {
            cout << *pos;
            pos++;
        }
        cout << endl;
    }
    cout << "steps: " << counter << endl;
}

int getRand(int exclusiveUpperBound) {
    return rand() % exclusiveUpperBound;
}
