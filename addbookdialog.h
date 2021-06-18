#ifndef ADDBOOKDIALOG_H
#define ADDBOOKDIALOG_H

#include <QDialog>

namespace Ui {
  class addBookDialog;
}

class addBookDialog : public QDialog
{
  Q_OBJECT

public:
  explicit addBookDialog(QWidget *parent = nullptr);
  ~addBookDialog();

  QString getTitle() const;


  int getQuantity() const;

private slots:
  void on_submitButton_clicked();

  void on_cancelButton_clicked();

private:
  Ui::addBookDialog *ui;

  QString title;
  int quantity;
};

#endif // ADDBOOKDIALOG_H
