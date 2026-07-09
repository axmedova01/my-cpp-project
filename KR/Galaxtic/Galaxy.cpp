#include "Galaxy.h"
#include <iostream>
#include <cmath>
#include <bits/stdc++.h>


void Galaxy::mergeStars(int a, int b) {
    *at(a) += *at(b); // прибавляем к звезде а звезду b
    // удаляем звезду b(т.к мы ее прибавили к а):
    if (at(b) == central_star) central_star = at(a); // Если b - центральная звезда, то меняем центральную звезду на а
    delete at(b); // удаляем звезду b
    at(b) = nullptr; // в массиве помечаем b как удаленную
    --star_cnt; // уменьшаем кол-во звезд
}

Galaxy::Galaxy(int n): star_cnt(n) { // ссоздание галактики
    reserve(n);    // выделяем место под n звезд
    double pos[dim] = {0}, // position(координаты)
           v[dim] = {0}; // velocity (скорости)
    std::fill_n(maxMasses, 3, 0);

    // Creating central object
    central_star = new Star(pos, v, massSun);
    push_back(central_star); // добавляем нулевую звезду

    // Creating other objects
    for (int i = 1; i < n; ++i) {
        // генерируем полярную СК
        double R  = rand() * systemRadius / RAND_MAX, // радиус полярной СК
               fi = rand() * 2 * M_PI / RAND_MAX; // угол полярной СК (от 0 до 2П)
        pos[0] = R * cos(fi);
        pos[1] = R * sin(fi);

        // вторая космическая скорость
        double absV = sqrt(G * central_star->m / R);
        v[0] = absV * sin(fi);
        v[1] = -absV * cos(fi); // скорость направлена вдоль окружности с центром в начале координат
        push_back(new Star(pos, v, massEarth / (double) n * (double) (6 * i))); // создаем звезду
    }
}

void Galaxy::update() { // наслденик от вектора. Работаем с ним как с вектором
    for (Star *star: *this) { // remove all previous forces
        if (!star) continue;
        std::fill_n(star->f, dim, 0); // просто заполняет массив 0, можно через for
    }

    for (int i = 0; i < size(); ++i) { // Выбираем опорную звезду
        if (!at(i)) continue;  // Пропускаем удаленные i звезды
        for (int j = i + 1; j < size(); ++j) { // (1,2), но уже не будет (2,1)
            if (!at(j)) continue; // пропускаем удаленные J звезды
            // Calculate distance (Вычисиление дистанции между парой хдвет i и j)
            double sqrDist = 0; // Квадрат расстояние между зведами
            double dCoord[dim]; // вектор расстояния по каждой координате
            for (int k = 0; k < dim; ++k) {
                dCoord[k] = at(i)->x[k] - at(j)->x[k]; // x = x2 - x1 or y = y2 - y1
                sqrDist += dCoord[k] * dCoord[k]; // r^2 = x^2 + y^2
            }

            // Connect if near enough(соединяем, если сблизились на  sqrDistConnect = 1e9 * 1e9 * 4
            if (sqrDist < sqrDistConnect) {
                mergeStars(i, j); // объединяем звезды i и j
                continue;
            }

            // Update forces
            double inv_dist = 1/sqrt(sqrDist); // ускорение(уможаем его на обратный корень)
            for (int k = 0; k < dim; ++k) {
                double tmp = G * at(i)->m * at(j)->m / sqrDist; // гравитационная постоянная(g)
                at(i)->f[k] -= tmp * dCoord[k] * inv_dist; // убрали силу тяготения
                at(j)->f[k] += tmp * dCoord[k] * inv_dist; // добавиди силу тяготения
            }
        }
        if (size() >= 3){ //находим три максимальные звезды
            double masses[size()];
            for (int i = 0; i < size(); ++i){ //создаем массив масс
                if (!at(i))continue;
                masses[i] = at(i)->m;
            }
            std::sort(masses,masses+size()); //сортируем этот массив
            maxMasses[0] = masses[size()-3];
            maxMasses[1] = masses[size()-2];
            maxMasses[2] = masses[size()-1];
        }
        // Update velocity and coords
        for (int k = 0; k < dim; ++k) {
            at(i)->v[k] += dt * at(i)->f[k] / at(i)->m; // обновляем скорости
            at(i)->x[k] += dt * at(i)->v[k]; // обновляем координаты
        }
    }
}
