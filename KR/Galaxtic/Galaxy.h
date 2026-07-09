#ifndef GALAXY_H
#define GALAXY_H
#include <vector>

#include "Star.h"


class Galaxy : std::vector<Star *> {
private:
    void mergeStars(int a, int b);
public:
    using vector::begin;
    using vector::end;
    int star_cnt;
    double maxMasses[3]={0};
    Star *central_star;

    explicit Galaxy(int n = numStars);
    void update();
    ~Galaxy() { delete central_star; };
};


#endif // GALAXY_H
