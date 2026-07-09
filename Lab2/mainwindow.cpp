#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QTime>
#include "math.h"
#include <field.h>
#include <QSpinBox>
#include <QLabel>
#include <QLineEdit>

const int fX0 = 20, fY0 = 20, fXps = 600, fYps = 600;  // положение поля fld в окне MainWindow (ui)

field *fld;

MainWindow::MainWindow(QWidget *parent)     // конструктор MainWindow
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    fld = new field;  // создать экземпляр класса field - новое поле
                      // конструктор field автоматически сгенерит объекты (окружности и прямоугольники)

    ui->pB_StartStop->setText(pB_StartStop[0]);

    lE_T = new QLineEdit* [2];
    lE_T[0] = ui->lE_CT; //кол-во кругов в данный момент времени
    lE_T[1] = ui->lE_RT; // кол-во квадратов в данный момент времени


    MainWindow::paintEvent(0);
    //MainWindow::repaint();
    connect(ui->pB_StartStop,  SIGNAL(clicked()), this, SLOT(pB_StartStopAction())); // подключваем кнопку старт/стоп

    // отправлять сигналы от таймера в QMainWindow::update (так как отсутствует MainWindow::update)
    connect(timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::update));
}

MainWindow::~MainWindow()                   // деструктор MainWindow
{
    delete fld;
    delete ui;
}

void MainWindow::pB_StartStopAction() //функция для старт/стоп
{
    if(ui->pB_StartStop->text()==pB_StartStop[0])    // циклические изменения Start / Stop
    {
        ui->pB_StartStop->setText(pB_StartStop[1]);
        timer->start(10);        // таймер будет генерировать сигналы каждую 10 миллисек
    }
    else
    {
        ui->pB_StartStop->setText(pB_StartStop[0]);
        timer->stop();
        mSPrev = 0; //обнуление времении
    }
}

void MainWindow::paintEvent(QPaintEvent *e) // вызов инициируется таймером
{
  Q_UNUSED(e);

  QTime tCurrent = QTime::currentTime();
  long mSecs = tCurrent.msecsSinceStartOfDay();

  if (mSPrev==0)                    // вызывается первый раз
  {
      mSPrev = mSecs;               // инициализация mSPrev
      dt = 0;
      return;
  }
  dt = (mSecs - mSPrev) * 0.002;    // сколько прошло секунд от предыдущего вызова
  mSPrev = mSecs;

  QPainter painter(this);
  QPen pen(Qt::black, 1, Qt::SolidLine);
  painter.setPen(pen);

  QBrush brush;
  brush.setStyle(Qt::SolidPattern);
  brush.setColor(Qt::yellow);
  painter.setBrush(brush);

  painter.drawLine(fX0, fY0, fX0+fXps, fY0);// создание рамки
  painter.drawLine(fX0, fY0, fX0, fY0+fYps);
  painter.drawLine(fX0+fXps, fY0, fX0+fXps, fY0+fYps);
  painter.drawLine(fX0, fY0+fYps, fX0+fXps, fY0+fYps);

  int xp, yp, dxp, dyp;         // координаты левых верхних углов объектов и их размеры

  pen.setColor(Qt::black);
  painter.setPen(pen);

  for (int i=0; i < fld->K; i++)    // рисование всех фигур
  {
    if (!(fld->m[i] == nullptr)) //пропускаем пустые фигуры
    {
      if ( (fld->m[i]->get_ng()) == 3)
      {
          dxp = (fld->m[i]->g[2]);   // g[2]*2-размер окружности = 2 радиуса
          dyp = dxp;
      }
      else
      {
          dxp = (fld->m[i]->g[2]);     // g[2]-ширина
          dyp = (fld->m[i]->g[3]);     // g[3]-высота
      }

      xp = fld->m[i]->g[0] ;
      yp = fld->m[i]->g[1] ;

      if ( (fld->m[i]->get_ng()) == 3)
      {
          painter.drawEllipse(fX0 + xp - dxp/2, fY0 + yp - dyp/2, dxp, dyp);
      }
      else
      {
          painter.save();                           // сохранить состояние
          painter.translate(fX0 + xp, fY0 + yp);    // перейти в центр фигуры
          painter.rotate(-fld->m[i]->a);            // повернуть на угол a (+ против Ч.С.)
          painter.fillRect(-dxp/2, -dyp/2, dxp, dyp,Qt::black);
          painter.restore();
      }
    }
  }

    if (timer->isActive()) fld->moveAll(dt);            // передвинуть объекты

    lE_T[0]->setText(QString::number(fld->lE_Total[0]));
    lE_T[1]->setText(QString::number(fld->lE_Total[1]));

}
