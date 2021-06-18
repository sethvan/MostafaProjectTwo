#ifndef BORROWBOOKDIALOG_H
#define BORROWBOOKDIALOG_H
#include "mainwindow.h"
#include "bookDialogType.h"

#include <QDialog>

namespace Ui {
  class borrowBookDialog;
}

class borrowBookDialog : public QDialog
{
  Q_OBJECT

public:
  explicit borrowBookDialog(MainWindow *parent = nullptr);
  ~borrowBookDialog();


  int getBorrower() const;

  int getBorrowedBook() const;

private slots:

  void on_submitButton_clicked();

  void on_cancelButton_clicked();

private:
  Ui::borrowBookDialog *ui;

  int borrower;
  int borrowedBook;
};

#endif // BORROWBOOKDIALOG_H
