#include <QDebug>
#include <QRandomGenerator>

#include <QJsonObject>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "random.hpp"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow)
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
  name_file = QString("random_data.json");
  load_json();
  load();
}

MainWindow::~MainWindow()
{
  if (file.isOpen())
    file.close();
  delete ui;
}

void MainWindow::load()
{
  // Прочитать файл и вывести в окно.
  // Если нет, создать файл и открыть на дозапись

  QTime min_time(setTimeFromInterval(ui->S_IntervalRandom->minimum())),
        max_time(setTimeFromInterval(ui->S_IntervalRandom->maximum()));

  ui->TE_MinInterval->setTime(min_time);
  ui->TE_MaxInterval->setTime(max_time);

  setInterval(ui->S_IntervalRandom->value());
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
        str[s] = QChar('A' + char(QRandomGenerator::global()->generate() % ('Z' - 'A')));

    return str;
}

void MainWindow::add_random()
{
  int rand_bool = Random::get(0, 1);
  QString rand_string;
  QJsonObject json_object;
  if (rand_bool)
  {
    int rand_int = Random::get(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());
    rand_string = QString::number(rand_int);
    json_object.insert("int", rand_int);
  }
  else
  {
    int length_rand=Random::get(10, 100);
    rand_string = randString(length_rand);
    json_object.insert("string", rand_string);
  }
  ui->TE_Data->append(rand_string);
  QTextStream out(&file);
  out << rand_string << "\n";
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
  if (current_interval>=maximum || current_interval<=minimum)
  {
    ui->S_IntervalRandom->setValue(minimum+diff_msecs/2);
  }
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

void MainWindow::on_TE_MaxInterval_userTimeChanged(const QTime &time)
{
  if (!ui->TE_MaxInterval->hasFocus())
    return;

  QTime min_time(ui->TE_MinInterval->time());
  int minimum((min_time.minute()*60+min_time.second())*1000+min_time.msec()),
      maximum((time.minute()*60+time.second())*1000+time.msec()),
      diff_msecs=maximum - minimum;

  if (diff_msecs <= 0)
  {
    diff_msecs = 1000;
    minimum = maximum - diff_msecs;
    if (minimum <= 0)
    {
      minimum = 1;
      maximum = minimum + diff_msecs;
    }
  }

  ui->TE_MaxInterval->setTime(setTimeFromInterval(maximum));
  ui->TE_MinInterval->setTime(setTimeFromInterval(minimum));

  ui->S_IntervalRandom->setMinimum(minimum);
  ui->S_IntervalRandom->setMaximum(maximum);

  int current_interval(ui->S_IntervalRandom->value());
  if (current_interval>=maximum || current_interval<=minimum)
  {
    ui->S_IntervalRandom->setValue(minimum+diff_msecs/2);
  }
}

void MainWindow::save_json()
{
  QJsonObject jsonObject = json_document.object();
  jsonObject["city"] = "Los Angeles";

  QJsonDocument newJsonDocument(jsonObject);

  file.open(QIODevice::WriteOnly);
  file.write(newJsonDocument.toJson());
  file.close();
}

void MainWindow::load_json()
{
  QFile file(name_file);
  file.open(QIODevice::ReadOnly);
  QByteArray jsonData = file.readAll();
  file.close();

  QJsonDocument json_document = QJsonDocument::fromJson(jsonData);

//  file.setFileName(name_file);
//  if (file.exists())
//  {
//    file.open(QIODevice::ReadOnly | QIODevice::Text);
//    QTextStream in(&file);
//    while (!in.atEnd())
//    {
//      QString line = in.readLine();
//      ui->TE_Data->append(line);
//    }
//    file.close();
//    file.open(QIODevice::Append | QIODevice::Text);
//  }
//  else
//    file.open(QIODevice::WriteOnly | QIODevice::Text);
}

