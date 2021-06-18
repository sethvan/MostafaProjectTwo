#include "borrowbookdialog.h"
#include "ui_borrowbookdialog.h"
#include "mainwindow.h"
#include <algorithm>
#include <QMessageBox>
#include <QString>
#include "bookDialogType.h"
#include <QDebug>


borrowBookDialog::borrowBookDialog(MainWindow *parent) :
  QDialog(parent),
  ui(new Ui::borrowBookDialog)
{
  ui->setupUi(this);

  switch (parent->borrowOrReturn)
    {
      case bookDialogType::borrowing :
        setWindowTitle("Borrowing Book");
        ui->dialogInstructionLabel->setText("Please select the User and the Book being borrowed ");
        break;

      case bookDialogType::returning :
        setWindowTitle("Returning Book");
        ui->dialogInstructionLabel->setText("Please select the User and the Book being returned ");
      break;
    }

  std::sort(parent->app->users.begin(), parent->app->users.end(), [](User &a, User &b){
    return a.name < b.name;
  });
  ui->userComboBox->addItem("");
  for(auto const& user : parent->app->users)
    ui->userComboBox->addItem(user.name);

  std::sort(parent->app->library.begin(), parent->app->library.end(), [](Book &a, Book &b){
    return a.title.toUpper() < b.title.toUpper();
  });
  ui->bookComboBox->addItem("");
  for(auto const& book : parent->app->library)
    ui->bookComboBox->addItem(book.title);
}

borrowBookDialog::~borrowBookDialog()
{
  delete ui;
}

int borrowBookDialog::getBorrower() const
{
  return borrower;
}

int borrowBookDialog::getBorrowedBook() const
{
  return borrowedBook;
}


void borrowBookDialog::on_submitButton_clicked()
{
  int borrowerIndex = ui->userComboBox->currentIndex()-1;
  int bookIndex = ui->bookComboBox->currentIndex()-1;

  if(ui->userComboBox->currentText().isEmpty() || ui->bookComboBox->currentText().isEmpty())
    QMessageBox::question(this, "Incomplete", "Field(s) missing", QMessageBox::Ok);
  else
    {
      borrower = borrowerIndex;
      borrowedBook = bookIndex;

      accept();
    }
}

void borrowBookDialog::on_cancelButton_clicked()
{
    reject();
}
