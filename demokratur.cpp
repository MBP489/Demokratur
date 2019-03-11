#include <iostream>
#include <random>

using namespace std;

int* initField(unsigned int fieldsize, int parties);
int* moveRel(int stepsX, int stepsY, int *position);
int* moveAbs(int stepsX, int stepsY, int *position);
int* left(int* position);
int* right(int* position);
int* up(int* position);
int* down(int* position);
int* getRandNeighbour(int* candidate);

random_device *const rd = new random_device();
mt19937 *const engine = new mt19937(rd);

uniform_int_distribution<int> *const randNeighbour = new uniform_int_distribution<int>(0, 3);

int main() {

    const unsigned int fieldX = 20;
    const unsigned int fieldY = 20;
    const unsigned int parties = 2;
    const unsigned int steps = 40000;

    const unsigned int fieldsize = fieldX * fieldY;
    int *const field = initField(fieldsize, parties);
    int *position = field;
    uniform_int_distribution<int> *const randCandidate = new uniform_int_distribution<int>(0, fieldsize-1);

    for (unsigned int i = 0; i < steps; i++) {
        int* candidate = field + (*randCandidate)(engine);
        int* neighbour = getRandNeighbour(candidate);
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

int* left(int *position) {
    return moveRel(-1, 0, position);
}

int* right(int *position) {
    return moveRel(1, 0, position);
}

int* up(int *position) {
    return moveRel(0, -1, position);
}

int* down(int *position) {
    return moveRel(0, 1, position);
}

int* moveRel(int stepsX, int stepsY, int *position) {

}

int* moveAbs(int stepsX, int stepsY, int *position) {

}

int* initField(unsigned int fieldsize, int parties) {
    uniform_int_distribution<int> randInit(0, parties - 1);
    int *field = new int[fieldsize];
    for (unsigned int i = 0; i < fieldsize; i++) {
        int *p = field + i;
        *p = randInit(engine);
    }
    return field;
}
