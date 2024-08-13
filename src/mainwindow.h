#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTime>

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

    void on_PB_Close_clicked();

    void add_random();
    QString randString(int len);

    void on_S_IntervalRandom_valueChanged(int value);

    void on_TE_MinInterval_userTimeChanged(const QTime &time);

    void load();
    void setInterval(const int &interval);
    QTime setTimeFromInterval(const int &interval) const;

  private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTimer *timer_random;
};

#endif // MAINWINDOW_H
