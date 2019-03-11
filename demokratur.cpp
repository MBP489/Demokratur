#include <iostream>
#include <random>

using namespace std;

void initField();
int* move(int stepsX, int stepsY, int *position);
int* left(int* position);
int* right(int* position);
int* up(int* position);
int* down(int* position);
int* getRandNeighbour(int* candidate);

const int fieldX = 20;
const int fieldY = 20;
const int parties = 2;
const int steps = 40000;

const int fieldsize = fieldX * fieldY;
int *const field = new int[fieldsize];

random_device *const rd = new random_device();
mt19937 *const engine = new mt19937(rd);

uniform_int_distribution<int> *const randCandidate = new uniform_int_distribution<int>(0, fieldsize-1);
uniform_int_distribution<int> *const randNeighbour = new uniform_int_distribution<int>(0, 3);

int main() {

    initField();

    int* candidate = field + (*randCandidate)(engine);
    int* neighbour = getRandNeighbour(candidate);

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
    return move(-1, 0, position);
}

int* right(int *position) {
    return move(1, 0, position);
}

int* up(int *position) {
    return move(0, -1, position);
}

int* down(int *position) {
    return move(0, 1, position);
}

int* move(int stepsX, int stepsY, int *position) {

}

void initField() {
    uniform_int_distribution<int> randInit(0, parties - 1);
    for (int i = 0; i < fieldsize; i++) {
        int *p = field + i;
        *p = randInit(engine);
    }
}
