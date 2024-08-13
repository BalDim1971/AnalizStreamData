#include <QTime>
#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "random.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  update_time();
  timer = new QTimer();
  timer->setInterval(1000);
  connect(timer, SIGNAL(timeout()), this, SLOT(update_time()));
  timer->start();

  timer_random = new QTimer();
  timer_random->setInterval(1000);
  connect(timer_random, SIGNAL(timeout()), this, SLOT(add_random()));
  timer_random->start();

  ui->TE_Data->append("Случайные данные");
  load();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::update_time()
{
  ui->L_Timer->setText(QTime::currentTime().toString());
}

void MainWindow::on_PB_Close_clicked()
{
  close();
}

QString MainWindow::randString(int len)
{
    QString str;
    str.resize(len);
    for (int s = 0; s < len ; ++s)
        str[s] = QChar('A' + char(qrand() % ('Z' - 'A')));

    return str;
}

void MainWindow::add_random()
{
  int rand_bool = Random::get(0, 1);
  if (rand_bool)
  {
    int rand_int = Random::get(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    ui->TE_Data->append(QString::number(rand_int));
  }
  else
  {
    int length_rand=Random::get(10, 100);
    ui->TE_Data->append(randString(length_rand));
  }
}

void MainWindow::on_S_IntervalRandom_valueChanged(int value)
{
  setInterval(value);
  timer_random->setInterval(value);
}

void MainWindow::on_TE_MinInterval_userTimeChanged(const QTime &time)
{
  if (!ui->TE_MinInterval->hasFocus())
    return;

  QTime max_time(ui->TE_MaxInterval->time());
  int minimum((time.minute()*60+time.second())*1000+time.msec()),
      maximum((max_time.minute()*60+max_time.second())*1000+max_time.msec()),
      diff_msecs=maximum - minimum;
  if (diff_msecs <= 0)
  {
    maximum = minimum + 1000;
    diff_msecs = 1000;
  }

  ui->TE_MaxInterval->setTime(time.addMSecs(diff_msecs));

  ui->S_IntervalRandom->setMinimum(minimum);
  ui->S_IntervalRandom->setMaximum(maximum);
  int current_interval(ui->S_IntervalRandom->value());
  if (current_interval>maximum || current_interval<minimum)
  {
    ui->S_IntervalRandom->setValue(minimum+diff_msecs/2);
  }
}

void MainWindow::load()
{
  QTime min_time(setTimeFromInterval(ui->S_IntervalRandom->minimum())),
        max_time(setTimeFromInterval(ui->S_IntervalRandom->maximum()));

  ui->TE_MinInterval->setTime(min_time);
  ui->TE_MaxInterval->setTime(max_time);

  setInterval(ui->S_IntervalRandom->value());
}

void MainWindow::setInterval(const int &interval)
{
  QTime time_int(setTimeFromInterval(interval));
  ui->L_ValueSlider->setText(QString("Интервал:") + time_int.toString("hh:mm:ss.zzz"));
}

QTime MainWindow::setTimeFromInterval(const int &interval) const
{
  int hh(interval / 1000 / 3600),
      mm(interval / 1000 % 3600 / 60),
      ss(interval / 1000 % 3600 % 60),
      zzz(interval % 1000);
  return QTime(hh, mm, ss, zzz);
}

