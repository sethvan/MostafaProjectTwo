#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QList>
#include <QString>
#include <QChar>
#include <algorithm>
#include <QKeyEvent>
#include <iterator>
#include <memory>
#include "borrowbookdialog.h"
#include <QMessageBox>
#include <addbookdialog.h>
#include <fstream>
#include <iostream>


MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  app = new libraryApp();

  loadPreviousSession();

  addUserReady = false;
  searchByPrefixReady = false;
}

MainWindow::~MainWindow()
{
  saveSession();
  delete ui;
}

void MainWindow::on_lineEdit_cursorPositionChanged(int arg1, int arg2)
{
  if(searchByPrefixReady)
  {
    ui->textEdit->setText("");

    QString input = ui->lineEdit->text();
    int size = input.size();

    for(size_t i {0}; i < app->library.size(); ++i)
      {
        if(size > app->library.at(i).title.size())
          continue;

        for(int j {0}; j < size; ++j)
          {
            if(input.at(j).toUpper() != app->library.at(i).title.at(j).toUpper())
              break;

            if(j == size-1)
              ui->textEdit->append(app->library.at(i).title);
          }
      }
  }
}

void MainWindow::on_actionPrint_Library_by_ID_triggered()
{
  searchByPrefixReady = false;
  addUserReady = false;
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("");

  if(app->library.size() == 0)
    {
      ui->textEdit->setText("There are no books in the library at this time.");
    }
  else
    {
      std::sort(app->library.begin(), app->library.end(), [](Book &a, Book& b) {
                  return a.id < b.id;
          });
      ui->textEdit->setText("");
      ui->textEdit->append("Printout of Library sorted per ID:\n");

      for(auto book : app->library)
        {
          ui->textEdit->append("ID = " + QString::number(book.id) + ", Title = \"" + book.title +
                               "\", Quantity = " + QString::number(book.qty));
        }
      ui->textEdit->scrollToAnchor("Printout of Library sorted per Title:\n");
    }
}

void MainWindow::on_actionPrit_Library_by_Name_triggered()
{
  searchByPrefixReady = false;
  addUserReady = false;
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("");

  if(app->library.size() == 0)
    {
      ui->textEdit->setText("There are no books in the library at this time.");
    }
  else
    {
      std::sort(app->library.begin(), app->library.end(), [](Book &a, Book& b) {
                  return a.title.toUpper() < b.title.toUpper();
          });

      ui->textEdit->setText("");
      ui->textEdit->append("Printout of Library sorted per Title:\n");

      for(auto book : app->library)
        {
          ui->textEdit->append("Title = \"" + book.title + "\", ID = " + QString::number(book.id) +
                               ", Quantity = " + QString::number(book.qty));
        }
      ui->textEdit->scrollToAnchor("Printout of Library sorted per Title:\n");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if( event->key() == Qt::Key_Enter ||  event->key() == Qt::Key_Return)
    {
      if(searchByPrefixReady)
        {
          searchByPrefixReady = false;
          ui->lineEdit->setText("");
          ui->lineEdit->setPlaceholderText("");
        }

      if(addUserReady)
        {
          QString  text = ui->lineEdit->text();
          if( text.isEmpty())
            {
              ui->textEdit->setText("No name was captured, no User added....");
              ui->lineEdit->setText("");
              ui->lineEdit->setPlaceholderText("");
            }
          else
            {
              int alreadyExists {0};
              std::for_each(app->users.begin(), app->users.end(), [&](User &user){
              if(user.name.toUpper() == ui->lineEdit->text().toUpper())
                alreadyExists += 1;
                });

              if(alreadyExists)
                {
                  ui->textEdit->setText("User " + ui->lineEdit->text() + " already exists");
                }
              else
                {
                  app->users.emplace_back(ui->lineEdit->text());
                  ui->textEdit->setText("New user " + ui->lineEdit->text() + " has been added");
                  addUserReady = false;
                  ui->lineEdit->setText("");
                  ui->lineEdit->setPlaceholderText("");
                }
            }
        }
    }
}

void MainWindow::on_actionAdd_User_triggered()
{
    ui->lineEdit->setText("");
    ui->lineEdit->setPlaceholderText("Enter the user's name here and press enter..");
    addUserReady = true;
    searchByPrefixReady = false;
}

void MainWindow::on_actionPrint_Usrs_List_triggered()
{
    searchByPrefixReady = false;
    addUserReady = false;
    ui->lineEdit->setText("");
    ui->lineEdit->setPlaceholderText("");

    ui->textEdit->setText("");
    ui->textEdit->append("Current Users in the System are: ");
    ui->textEdit->append("");

    for(auto user : app->users)
      ui->textEdit->append(user.name);

    ui->textEdit->scrollToAnchor("Current Users in the System are: ");

}

void MainWindow::on_actionSearch_Book_by_prefix_triggered()
{
    ui->lineEdit->setPlaceholderText("Search by prefix here, when finished hit enter");
    searchByPrefixReady = true;
    addUserReady = false;
}

void MainWindow::on_actionBorrow_Book_triggered()
{
  searchByPrefixReady = false;
  addUserReady = false;
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("");

  borrowOrReturn = (bookDialogType::borrowing);
  borrowBookDialog* dialog = new borrowBookDialog(this);

  int ret = dialog->exec();//this returns an enum of 1(accepted dialog) or 0(rejected dialog)

  if(ret == 1)
    {
      if(app->library.at(dialog->getBorrowedBook()).qty == 0)
        {
          QMessageBox::information(this, "Out of Stock", "Sorry, this book is currently out of stock. "
                                             "Please check back later.", QMessageBox::Ok);
        }
      else if( app->users.at(dialog->getBorrower()).borrowedList.size() >= 3 )
        {
          QMessageBox::information(this, "Borrowed Limit reached", "Sorry, this User has reached their limit of 3 books borrowed. "
                                   "Please return one or more books first." );
        }
      else
        {
          --app->library.at(dialog->getBorrowedBook()).qty;

          app->users.at(dialog->getBorrower()).borrowedList.
              emplace_back(app->library.at(dialog->getBorrowedBook()).id,
                                           app->library.at(dialog->getBorrowedBook()).title);
        }
    }
}

void MainWindow::on_actionReturn_Book_triggered()
{
  searchByPrefixReady = false;
  addUserReady = false;
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("");

  borrowOrReturn = (bookDialogType::returning);
  borrowBookDialog* dialog = new borrowBookDialog(this);

  int ret = dialog->exec();
  if(ret == 1)
    {
      BorrowedBook temp {app->library.at(dialog->getBorrowedBook()).id,
                         app->library.at(dialog->getBorrowedBook()).title};

      auto itBook = std::find(app->users.at(dialog->getBorrower()).borrowedList.begin(),
                    app->users.at(dialog->getBorrower()).borrowedList.end(), temp);

      if( itBook ==  app->users.at(dialog->getBorrower()).borrowedList.end() )
        {
          QMessageBox::information(this, "Invalid Choice", "Chosen User has not borrowed the Book selected.");
        }
      else
        {
          itBook = app->users.at(dialog->getBorrower()).borrowedList.erase(itBook);
          ++app->library.at(dialog->getBorrowedBook()).qty;
          ui->textEdit->setText("Thank you for returning " + app->library.at(dialog->getBorrowedBook()).title );
        }
    }
}

void MainWindow::on_actionAdd_Book_triggered()
{
  searchByPrefixReady = false;
  addUserReady = false;
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("");

  addBookDialog* dialog = new addBookDialog(this);

  int ret = dialog->exec();
  if(ret == 1)
    {
      QString title = dialog->getTitle();
      int qty = dialog->getQuantity();
      Book temp {1, title, qty};

      auto itBook = std::find(app->library.begin(), app->library.end(), temp);

      if( itBook != app->library.end() )
        {
          itBook->qty += qty;
          ui->textEdit->setText("The book \"" + title + "\" already exists in the Library"
                                ". Quantity " + qty + (qty == 1? " has": " have") + " been added to"
                                " it's inventory.");
        }
      else
        {
          int id = app->library.size()+101;
          app->library.emplace_back(id, title, qty);
        }
    }
}

void MainWindow::on_actionPrint_who_borrowed_book_by_name_triggered()
{
  searchByPrefixReady = false;
  addUserReady = false;
  ui->lineEdit->setText("");
  ui->lineEdit->setPlaceholderText("");

  ui->textEdit->setText("");
  ui->textEdit->append("Current Users borrowing books: \n");

  for(int i {0}; i < app->users.size(); ++i)
    {
      if(app->users.at(i).borrowedList.size() != 0)
        {
         ui->textEdit->append(app->users.at(i).name + " is borrowing: ");

         auto it = app->users.at(i).borrowedList.begin();
         while(it != app->users.at(i).borrowedList.end())
           {
             ui->textEdit->insertPlainText(" \"" + it->title + "\"  ");
             ++it;
           }
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
  saveSession();
  QApplication::quit();
}

void MainWindow::saveSession()
{
  std::ofstream out_file{"session.txt"};
  if(!out_file)
  {
     qDebug() << "file failed to be created.";
  }
  else
  {
     out_file << app->library.size() << std::endl;
     for(size_t i {0}; i < app->library.size(); ++i)
        {
           out_file << app->library.at(i).title.toStdString() << std::endl;
           out_file << app->library.at(i).id << " " << app->library.at(i).qty << std::endl;
        }

     out_file << app->users.size() << std::endl;
     for(size_t i {0}; i < app->users.size(); ++i)
        {
           out_file << app->users.at(i).name.toStdString() << std::endl;
           out_file << app->users.at(i).borrowedList.size() << std::endl;

           if(app->users.at(i).borrowedList.size() > 0)
           {
              auto it = app->users.at(i).borrowedList.begin();
              for(size_t j{0}; j < app->users.at(i).borrowedList.size(); ++j)
              {
                 out_file << it->title.toStdString() << std::endl;
                 out_file << it->id << std::endl;
                 ++it;
              }
           }
        }
    }
}

void MainWindow::loadPreviousSession()
{
  std::ifstream in_file {"session.txt"};
  if(in_file)
     {
        qDebug() << "Opened file \"session.txt\" successfully";

        int id, qty, loops, innerLoops;
        std::string title {}, name {}, data {};

        in_file >> loops;
        in_file.ignore(120,'\n');

        for(int i {0}; i < loops; ++i)
           {
              std::getline(in_file, title);
              in_file >> id >> qty;
              app->library.emplace_back(id, title, qty);
              in_file.ignore(120,'\n');
           }

        in_file >> loops;
        in_file.ignore(120,'\n');

        for(int i {0}; i < loops; ++i)
           {
              std::getline(in_file, name);
              app->users.emplace_back(name);

              in_file >> innerLoops;
              in_file.ignore(120,'\n');

              if(innerLoops > 0)
              {
                 for(int i {0}; i < innerLoops; ++i)
                    {
                       std::getline(in_file, title);
                       in_file >> id;
                       app->users.back().borrowedList.emplace_back(id, title);
                       in_file.ignore(120,'\n');
                    }
              }
           }
     }
     else
     {
        qDebug() << "Could not open file \"session.txt\"";
        QList<QString> testTitles;
        testTitles << "C++ essentials" << "C++ concurrency" << "C++20 for the modern programmer"
                   << "California Dreamin" << "Biography of Luis xiv" << "Big trouble in litte China"
                   << "Bigots in the deomocratic party" << "Biology 101" << "Aphrodite" << "Apocolypse now"
                   << "All along the watchtower" << "Delta blues" << "Devil's triangle" << "Damn thema all"
                   << "Fredrik not nice" << "Fillmore west" << "Great Expectations" << "Great Gatsby"
                   << "Zootopia" << "Led Zeppelin" << "Leo Tolstoy" << "Lemmy's story" << "lowercase book";
        for(int i{0}; i < testTitles.size(); ++i)
          {
            app->library.emplace_back(i+101, testTitles.at(i));
          }
        app->library.at(0).qty = 0;
     }
}
