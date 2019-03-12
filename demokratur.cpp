#include <iostream>
#include <random>

using namespace std;

void init(int X, int Y, int part, int st);
int* getRandNeighbour(int* candidate);
int* pos(int x, int y);
int x(int *position);
int y(int *position);
int* left(int *position);
int* right(int *position);
int* up(int *position);
int* down(int *position);
int* initField(int fieldsize, int parties);

random_device *const rd = new random_device();
mt19937 *const engine = new mt19937(rd);
uniform_int_distribution<int> *const randNeighbour = new uniform_int_distribution<int>(0, 3);
uniform_int_distribution<int> *randField;

int parties;
int steps;

int *field;
int fieldX;
int fieldY;
int fieldsize;

void init(int X, int Y, int part, int st) {
    fieldX = X;
    fieldY = Y;
    fieldsize = fieldX * fieldY;
    parties = part;
    steps = st;

    field = initField(fieldsize, parties);

    randField = new uniform_int_distribution<int>(0, fieldsize - 1);
}

int main() {
    init(20, 20, 2, 40000);
    int *candidate;
    int *neighbour;

    for (int i = 0; i < steps; i++) {
        candidate = field[(*randField)(engine)];
        neighbour = getRandNeighbour(candidate);
    }
}

int* getRandNeighbour(int* candidate){
    int* position = candidate;
    int choosen = (*randNeighbour)(engine);
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
    if(field <= position && position <= (field + fieldsize)) {
        return position;
    }
    return nullptr;
}

int x(int *position) {
    int count = position - field;
    if(count < 0 || count > fieldsize) {
        return nullptr;
    }
    return count / fieldX;
}

int y(int *position) {
    int count = position - field;
    if(count < 0 || count > fieldsize) {
        return nullptr;
    }
    int y = count % fieldX;
}

int* left(int *position) {
    int x = x(position);
    position = (x - 1 >= 0) ? position-- : position + fieldX - 1;
    return position;
}


int* right(int *position) {
    int x = x(position);
    position = (x + 1 < fieldX) ? position++ : position - fieldX + 1;
    return position;
}

int* up(int *position) {
    int y = y(position);
    position = (y - fieldX >= 0) ? position - fieldX : position - fieldX + fieldsize;
    return position;
}

int* down(int *position) {
    int y = y(position);
    position = (y + fieldX < fieldY) ? position + fieldX : position + fieldX - fieldsize;
    return position;
}

int* initField(int fieldsize, int parties) {
    uniform_int_distribution<int> randInit(0, parties - 1);
    int *field = new int[fieldsize];
    for (unsigned int i = 0; i < fieldsize; i++) {
        int *p = field + i;
        *p = randInit(engine);
    }
    return field;
}
