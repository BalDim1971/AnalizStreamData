#include <QTime>

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

  ui->s_IntervalRandom->setTickPosition(QSlider::TicksBothSides);
  ui->s_IntervalRandom->setValue(1000);

  ui->textEdit->append("Random STD  - ");
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::update_time()
{
  ui->l_timer->setText(QTime::currentTime().toString());
}


void MainWindow::on_pushButton_clicked()
{
  close();
}

void MainWindow::add_random()
{
  ui->textEdit->append(QString::number(Random::get(15, 43)));
}


void MainWindow::on_s_IntervalRandom_valueChanged(int value)
{
  QString s_value = QString::number(value);
  ui->l_valueSlider->setText(s_value.rightJustified(7, QChar('0')));
  timer_random->setInterval(value);
}

