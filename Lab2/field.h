#ifndef FIELD_H
#define FIELD_H

#include <iostream>
#include <math.h>
using namespace std;

const int Kfigures=40+10;       // стандартное кол-во фигур
const int fSize=100;        // стандартный размер поля (in R's)
const int Rfigures=25;     // стандартный макисмальный размер каждой фигуры (m)
const double PROBCIRC = 0.25; // веротяность того, что будет круг
const int rMin=5;          // минимальный размер в R
const int vMax = 20;          // м/сек
const int wMax = 10;          // град/сек

class figure
{
//private:
protected:
  int ng; // количество параметров в geometry(круг-3, прямоугольник-4)

public:
  double v[2];    // скорости vx, vy центра м/сек
  double w;    // угловые скорости град/сек (+ против ЧС)
  double a;     // угол поворота  град (+ против ЧС)
  double *g;    // массив x, y, r или x, y, sx, sy
  double rSize; // размер объекта 2R или длина диагонали

  virtual void move(double dx, double dy)=0; // передвинуть(скорость)
  virtual void rotate(double da)=0; // повернуть(угловая скорость)

  int get_ng(){return ng;}; //узнаем тип фигуры/кол-во параметров

  figure(): ng(0), a(0), g(0)   // конструктор
  {
      //заполняем значения скоростей случайными числами в пределах максимальной скорости
      v[0] = rand() % vMax + -vMax;
      v[1] = rand() % vMax + -vMax;
      //аналогично с угловой скорости
      w = rand() % wMax + -wMax;
  }
  ~figure() {delete[]g;}

};

class rectangle  : public figure  // наследуется от класса figure
{
  const int rng=4;
public:
  void move(double dx, double dy); // передвинуть
  void rotate(double da); // повернуть
  rectangle(double x, double y, double sx, double sy): figure()
  {
      ng=rng;
      g=new double [ng];
      g[0]=x;    // координаты
      g[1]=y;    // центра прямоугольника
      g[2]=sx;    // длина
      g[3]=sy;    // ширина
      rSize=sqrt(sx*sx+sy*sy);
  }
};

void rectangle::move(double dx, double dy)     // передвинуть
{
    g[0]+=dx;
    g[1]+=dy;
}

void rectangle::rotate(double da)     // повернуть
{
    a+=da;
}

class circle  : public figure  // наследуется от класса figure
{
  const int rng=3;
public:
  void move(double dx, double dy); // передвинуть
  void rotate(double da); // повернуть
  circle(double x, double y, double r): figure()
  {
      ng=rng;
      g=new double [ng];
      g[0]=x;    // координаты
      g[1]=y;    // центра окружности
      g[2]=r;    // радиус
      rSize=2*r;
  }
};

void circle::move(double dx, double dy)     // передвинуть
{
    g[0]+=dx;
    g[1]+=dy;
}

void circle::rotate(double da)     // повернуть
{
    a+=da;
}

class field
{
private:
  double probCirc;

public:
  double fMax;
  double R;
  int K;
  double tSystem;
  figure **m;
  int lE_Total[2] = {0,0};
  int lE_Refl[2] = {0,0};

  field(int kPar = Kfigures,        // конструктор
        double fPar = fSize,
        double rPar = Rfigures,
        double probCircPar= PROBCIRC
       ): probCirc(probCircPar), fMax(fPar*rPar), R(rPar), K(kPar)
  {
    int rv;
    double xrv;
    double r, t, sx, sy, x, y;

    m=new figure*[K]; // создаем копию массива figure чтобы при удалении фигуры не было смещения

    for (int k=0; k<K; k++)
    {
        xrv=rand() % vMax +2;

        if (xrv <= 7)
        {
            r=rand() % Rfigures +rMin;
            x = rand() %  550 +20;
            y = rand() % 550 +20;

            m[k]=new circle(x,y,r);
            lE_Total[0]++;
            //cout << "C " << x << " " << y << " " << r << "\n";

        }
        else
        {
            sx = rand() % Rfigures +rMin;
            sy = rand() % Rfigures +rMin;
            x = rand() % 550 +20;
            y = rand() % 550 +20;

            m[k]=new rectangle(x,y,sx, sy);
            lE_Total[1]++;
            //cout << "R "  << x << " " << y << " "  << sx << " " << sy << "\n";
        }
    };
  }
  ~field(){delete[] m;};

void moveAll(double dt); // переместить объекты за время dt

};

void field::moveAll(double dt)
{
    double dx, dy, da, r2, wS;

    for (int i=0; i < K; i++)    // перемещение всех фигур
    {
      if (!(m[i] == nullptr))   // объект существует
      {
        for (int j=0;j<2;j++)   // проверки на отражение от границ
        {
        if ( ((m[i]->g[j] + m[i]->v[j] * dt) <= m[i]->rSize/2) ||
             ((m[i]->g[j] + m[i]->v[j] * dt) >= (600 - m[i]->rSize/2) ))

             {
                m[i]->v[j] *= -1;
                lE_Refl[m[i]->get_ng()==3 ? 0 : 1]++;
             }
        }

        dx = m[i]->v[0] * dt;   // перемещение центра на dx, dy метров
        dy = m[i]->v[1] * dt;
        m[i]->move(dx, dy);

        da = m[i]->w * dt;      // поворот относительно центра на da градусов (+ против ч.с.)
        m[i]->rotate(da);
      }
    }

    for (int i=0; i<K-1; i++)
        for (int j=i+1; j<K; j++){
            if ((m[i] == nullptr)||(m[j] == nullptr)) continue;
            r2=(m[i]->g[0]-m[j]->g[0])*(m[i]->g[0]-m[j]->g[0])+
                    (m[i]->g[1]-m[j]->g[1])*(m[i]->g[1]-m[j]->g[1]); //r^2 = x^2 + y^2
            if (sqrt(r2)<=(m[i]->rSize+m[j]->rSize)){       // столкнулись
                wS=m[i]->w+m[j]->w;
                if (abs(m[i]->w) < abs(m[j]->w)){
                    lE_Total[m[i]->get_ng()==3 ? 0 : 1]--;
                    m[i]=nullptr;
                    m[j]->w=wS;
                }
                else{
                    lE_Total[m[j]->get_ng()==3 ? 0 : 1]--;
                    m[j]=nullptr;
                    m[i]->w=wS;
                }
            }
        }


    tSystem += dt;
}

#endif // FIELD_H
