#include "addbookdialog.h"
#include "ui_addbookdialog.h"
#include <QMessageBox>


addBookDialog::addBookDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::addBookDialog)
{
  ui->setupUi(this);
}

addBookDialog::~addBookDialog()
{
  delete ui;
}

void addBookDialog::on_submitButton_clicked()
{
  QString bookTitle = ui->titlelineEdit->text();
  int bookQty = ui->spinBox->value();

  if(ui->titlelineEdit->text().isEmpty())
    {
      QMessageBox::question(this, "Incomplete", "Title missing", QMessageBox::Ok);
    }
  else
    {
      title = bookTitle;
      quantity = bookQty;
      accept();
    }
}

int addBookDialog::getQuantity() const
{
  return quantity;
}

QString addBookDialog::getTitle() const
{
  return title;
}

void addBookDialog::on_cancelButton_clicked()
{
    reject();
}
