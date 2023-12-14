#include "Dialog.h"
#include "./ui_Dialog.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMessageBox>

Dialog::Dialog(QWidget *parent):
  QDialog(parent),
  ui(new Ui::Dialog)
{
  ui->setupUi(this);
}

Dialog::~Dialog()
{
  delete ui;
}

std::string  Dialog::openForReadFile()
{
  QString  result = QFileDialog::getOpenFileName(this,
                                                 tr("Open csv"), "/home", tr("csv Files (*.csv)"));

  return result.toStdString();
}

QList<int>  Dialog::doTask(QList<double> task)
{
  int            numberOfGroup = ui->lineEdit->text().toInt();
  double         min           = *std::min_element(task.begin(), task.end());
  double         max           = *std::max_element(task.begin(), task.end());
  int            countClown    = task.size();
  int            numberInGroup = countClown / numberOfGroup;
  double         rangeDouble   = max - min;
  double         rangeinGroup  = rangeDouble / numberOfGroup;
  int            ip            = 0;
  QList<int>     listCountTask;
  QList<double>  range;

  for (int i = 0; i < numberOfGroup; ++i)
  {
    listCountTask.append(0);
  }

  for (int i = 0; i < numberOfGroup + 1; ++i)
  {
    double  end = i * rangeinGroup + min;

    if (i == numberOfGroup)
    {
      end = max;
    }

    range.append(end);
  }

  for (int groupNumber = 1; groupNumber <= numberOfGroup; ++groupNumber)
  {
    for (int h = 1; h <= numberInGroup; ++h)
    {
      ////every item height.at(ip)

      for (int i = 0; i < range.size() - 1; ++i)
      {
        if ((task.at(ip) >= range.at(i)) && (task.at(ip) < range.at(i + 1)))
        {
          listCountTask[i]++;
        }
      }

      ip++;

      if (ip == (countClown - 1))
      {
        listCountTask[range.size() - 2]++;
      }
    }
  }

  return listCountTask;
}

std::string  Dialog::saveForReadFile()
{
  QString  result = QFileDialog::getSaveFileName(this,
                                                 tr("save as..."));

  return result.toStdString();
}

QList<double>  Dialog::readFileDouble(int index, std::string pth)
{
  ///
  QFile  file(QString::fromStdString(pth));
  int    countClouwn = 0;

  if (file.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QTextStream    in(&file);
    QString        string    = "";
    QString        allString = "";
    QList<double>  result;

    do
    {
      string = in.readLine();
      ++countClouwn;

      if (!string.isEmpty())
      {
        QList<QString>  myvec = string.split(',');

        result.append(myvec.at(index).toDouble());
      }
    } while (!string.isEmpty());

    file.close();

    return result;
  }
}

void  Dialog::startPlot(QList<int> list, int index, QList<double> task)
{
  QCustomPlot   *customPlot;
  double         min           = *std::min_element(task.begin(), task.end());
  double         max           = *std::max_element(task.begin(), task.end());
  double         rangeDouble   = max - min;
  int            numberOfGroup = ui->lineEdit->text().toInt();
  double         rangeinGroup  = rangeDouble / numberOfGroup;
  QList<double>  range;

  for (int i = 0; i < numberOfGroup + 1; ++i)
  {
    double  end = i * rangeinGroup + min;

    if (i == numberOfGroup)
    {
      end = max;
    }

    range.append(end);
  }

  qInfo() << range;

  if (index == 1)
  {
    customPlot = ui->widget_2;
  }
  else
  {
    customPlot = ui->widget;
  }

// set dark background gradient:
  QLinearGradient  gradient(0, 0, 0, 400);
  gradient.setColorAt(0, QColor(90, 90, 90));
  gradient.setColorAt(0.38, QColor(105, 105, 105));
  gradient.setColorAt(1, QColor(70, 70, 70));
  customPlot->setBackground(QBrush(gradient));

  // create empty bar chart objects:
  QCPBars *regen = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  // QCPBars *nuclear = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  // QCPBars *fossil  = new QCPBars(customPlot->xAxis, customPlot->yAxis);
  regen->setAntialiased(false); // gives more crisp, pixel aligned bar borders
  // nuclear->setAntialiased(false);
  // fossil->setAntialiased(false);
  regen->setStackingGap(1);
  // nuclear->setStackingGap(1);
  // fossil->setStackingGap(1);
  // set names and colors:
  // fossil->setName("Fossil fuels");
  // fossil->setPen(QPen(QColor(111, 9, 176).lighter(170)));
  // fossil->setBrush(QColor(111, 9, 176));
  // nuclear->setName("Nuclear");
  // nuclear->setPen(QPen(QColor(250, 170, 20).lighter(150)));
  // nuclear->setBrush(QColor(250, 170, 20));
  regen->setName("Regenerative");
  regen->setPen(QPen(QColor(0, 168, 140).lighter(130)));
  regen->setBrush(QColor(0, 168, 140));
  // stack bars on top of each other:
  // nuclear->moveAbove(fossil);
  // regen->moveAbove(nuclear);

  // prepare x axis with country labels:
  QVector<double>   ticks;
  QVector<QString>  labels;

  for (int i = 1; i <= list.size(); ++i)
  {
    ticks.append(i);
    labels.append(QString::number(range.at(i - 1)) + " to " + QString::number(range.at(i)));
  }

  QSharedPointer<QCPAxisTickerText>  textTicker(new QCPAxisTickerText);
  textTicker->addTicks(ticks, labels);

  customPlot->xAxis->setTicker(textTicker);
  customPlot->xAxis->setTickLabelRotation(60);
  customPlot->xAxis->setSubTicks(false);
  customPlot->xAxis->setTickLength(0, 4);

  customPlot->xAxis->setRange(0, list.size());
  customPlot->xAxis->setBasePen(QPen(Qt::white));
  customPlot->xAxis->setTickPen(QPen(Qt::white));
  customPlot->xAxis->grid()->setVisible(true);
  customPlot->xAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));
  customPlot->xAxis->setTickLabelColor(Qt::white);
  customPlot->xAxis->setLabelColor(Qt::white);

  // prepare y axis:
  int  maxrange = *std::max_element(list.begin(), list.end());
  customPlot->yAxis->setRange(0, maxrange);
  customPlot->yAxis->setPadding(5); // a bit more space to the left border

  if (index == 0)
  {
    customPlot->yAxis->setLabel("Height result");
  }
  else
  {
    customPlot->yAxis->setLabel("Weight result");
  }

  customPlot->yAxis->setBasePen(QPen(Qt::white));
  customPlot->yAxis->setTickPen(QPen(Qt::white));
  customPlot->yAxis->setSubTickPen(QPen(Qt::white));
  customPlot->yAxis->grid()->setSubGridVisible(true);
  customPlot->yAxis->setTickLabelColor(Qt::white);
  customPlot->yAxis->setLabelColor(Qt::white);
  customPlot->yAxis->grid()->setPen(QPen(QColor(130, 130, 130), 0, Qt::SolidLine));
  customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(130, 130, 130), 0, Qt::DotLine));

  // Add data:
  QVector<double>  regenData;

  for (int i = 0; i < list.size(); ++i)
  {
    regenData.append(list.at(i));
  }

  regen->setData(ticks, regenData);

  // setup legend:
  customPlot->legend->setVisible(true);
  customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignHCenter);
  customPlot->legend->setBrush(QColor(255, 255, 255, 100));
  customPlot->legend->setBorderPen(Qt::NoPen);
  QFont  legendFont = font();
  legendFont.setPointSize(10);
  customPlot->legend->setFont(legendFont);
  customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
  customPlot->replot();
}

void  Dialog::writeFileDouble(std::string pth, QList<int> height, QList<int> weight, QList<double> task1, QList<double> task2)
{
  QFile  file(QString::fromStdString(pth));

  if (file.open(QIODevice::WriteOnly | QIODevice::Text))
  {
    QDataStream  out(&file);
    QByteArray   byte;
    QString      string1("height: ");
    QString      string2("weight: ");

    out << string1;
    out << height;
    out << string2;
    out << weight;

    file.close();
  }

  QFile  file1(QString::fromStdString(pth));

  if (file1.open(QIODevice::ReadOnly | QIODevice::Text))
  {
    QDataStream  in(&file1);
    QString      index;
    QString      height;
    QString      weight;
    QList<int>   listheight;
    QList<int>   listWeight;
    QString      string1("");
    QString      string2("");

    in >> string1;
    in >> listheight;
    in >> string2;
    in >> listWeight;

    // period as decimal separator and comma as thousand separator
    /////////////////////////////////////////////////////////////////////////////////////////////

    // by default, the legend is in the inset layout of the main axis rect. So this is how we access it to change legend placement:

    // setup for graph 0: key axis left, value axis bottom
    // will contain left maxwell-like function
    // setup for graph 1: key axis bottom, value axis left (those are the default axes)
    // will contain bottom maxwell-like function with error bars
    startPlot(listheight, 0, task1);
    startPlot(listWeight, 1, task2);

    file.close();
  }
}

///
/// \brief Dialog::click buttom
///
void  Dialog::on_pushButton_clicked()
{
  std::string  pthopen = openForReadFile();
  // reading....
  QList<double>  height = readFileDouble(1, pthopen);
  QList<double>  weight = readFileDouble(2, pthopen);
  /// getting....
  QList<int>  resultHeight = doTask(height);
  QList<int>  resultWeight = doTask(weight);

// writing....
  std::string  pthsave = saveForReadFile();

  writeFileDouble(pthsave, resultHeight, resultWeight, height, weight);
}
