#include "Star.h"
#include "constants.h"
#include <cmath>


const double borderMass[] = {borderMassC * massVenus, borderMassC * massEarth, borderMassC * massUran,
                             borderMassC * massJup, borderMassC * massSun};
const int starColors[] = {0x7FFFD4,
                          0x78DBE2,
                          0xED3CCA,
                          0x9966CC,
                          0xEFD334,
                          0xFF0033};
const int nBorders = sizeof(borderMass) / sizeof(borderMass[0]); // кол-во границ, чтобв не выйти за границы

Star::Star(const double *coord, const double *velocity, double mass){ // Создание звезды. Подаем координаты, скорости и массу
    for (int k = 0; k < dim; ++k) {
        x[k] = coord[k];
        v[k] = velocity[k];
    }
    m = mass;
    updateColor();
    updateSize();
}

Star& Star::operator+=(const Star &rhs) { // оформление оператора +=
    for (int i = 0; i < dim;++i) {  // сумма импульсов звезд при столкновении
        v[i] = (v[i]*m + rhs.v[i]*rhs.m) / (m + rhs.m); // закон сохранения импульса: U2*(M1+M2)=V1*M1+V2*M2
    }
    m += rhs.m; // складываем массы звезд
    updateColor(); // меняем цвет в зависимости от градуировки
    updateSize(); // меняем размер
    return *this; // возвращаем ссылку на левый (левый += правый -> Возвращает новый левый)
}

void Star::updateColor() { // меняем цвет
    int i = 0;
    while (i + 1 < nBorders && borderMass[i] < m ) ++i; // когда перешагиваем границу массы m, меняем цвет на правый, не превыщающий массу
    col = starColors[i];
}

void Star::updateSize() { // меняем размер
    int i = 0;
    while (i + 1 < nBorders && borderMass[i] < m) ++i;
    if (floor(m / massSun) > 3) i += 3;
    else i += (int)(m / massSun);
    size = i + 1;
}
