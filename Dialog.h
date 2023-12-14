#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>


QT_BEGIN_NAMESPACE
namespace Ui
{
class Dialog;
}
QT_END_NAMESPACE

class Dialog: public QDialog
{
  Q_OBJECT

public:
  Dialog(QWidget *parent = nullptr);

  ~Dialog();

  QList<double>  readFileDouble(int index, std::string pth);

  void  writeFileDouble(std::string pth, QList<int> height, QList<int> weight, QList<double> task1, QList<double> task2);

  std::string  openForReadFile();

  std::string  saveForReadFile();

  QList<int>  doTask(QList<double> );

  void  startPlot(QList<int>, int, QList<double> task);

private slots:
  void  on_pushButton_clicked();

private:
  Ui::Dialog *ui;
};

#endif // DIALOG_H
