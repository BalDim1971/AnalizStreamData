#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void update_time();

    void on_pushButton_clicked();

    void add_random();

    void on_s_IntervalRandom_valueChanged(int value);

  private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *timer_random;
};

#endif // MAINWINDOW_H
