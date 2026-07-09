#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    const QString pB_StartStop[2] = {"Start", "Stop"};

    QTimer *timer = new QTimer(this);       // для отрисовки окна по таймеру
    long mSPrev=0;                          // мсек при предыдущей прорисовке
    double dt=0;                            // физическое время (сек) от предыдущей прорисовки
    QLineEdit **lE_T;           // Кол-во


private:
    Ui::MainWindow *ui;

protected:
    void paintEvent(QPaintEvent *event);    // paintEvent - будет вызываться из обработчика сигнала
                                      // (события) update(), который будет создаваться по таймеру
private slots:  // функции - приемники событий
    void pB_StartStopAction();
};
#endif // MAINWINDOW_H
