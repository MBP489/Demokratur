#include <iostream>
#include <random>

using namespace std;

int* initField(unsigned int fieldsize, int parties);
int* move(int *const field, int fieldsize, int *position, int steps);
int* left(int *const field, int fieldX, int fieldY, int *position);
int* right(int *const field, int fieldX, int fieldY, int *position);
int* up(int *const field, int fieldX, int fieldY, int *position);
int* down(int *const field, int fieldX, int fieldY, int *position);
int* getRandNeighbour(int* candidate);

random_device *const rd = new random_device();
mt19937 *const engine = new mt19937(rd);

uniform_int_distribution<int> *const randNeighbour = new uniform_int_distribution<int>(0, 3);

int main() {

    const int fieldX = 20;
    const int fieldY = 20;
    const int parties = 2;
    const int steps = 40000;

    const int fieldsize = fieldX * fieldY;
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

    }
    case 1: {

    }
    case 2: {

    }
    case 3: {

    }
    default:
        return nullptr;
    }
}

int* move(const int *field, int fieldsize, int *position, int steps) {
    if(field <= position && position <= (field + fieldsize)) {
        int* newPosition = position + steps;
        if(field <= newPosition && newPosition <= (field + fieldsize)) {
            position = newPosition;
            return position;
        }
    }
    return nullptr;
}

int* left(int *const field, int fieldX, int fieldY, int *position) {

}

int* right(int *const field, int fieldX, int fieldY, int *position) {

}

int* up(int *const field, int fieldX, int fieldY, int *position) {

}

int* down(int *const field, int fieldX, int fieldY, int *position) {

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
