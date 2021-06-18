#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include "libraryapp.h"
#include <QKeyEvent>
#include "bookDialogType.h"
#include <memory>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

  //QList<QString> bookTitles;

  std::unique_ptr<libraryApp> app;
  void keyPressEvent(QKeyEvent* event);
  bool addUserReady;
  bool searchByPrefixReady;

  bookDialogType borrowOrReturn;

private slots:
  void on_lineEdit_cursorPositionChanged(int arg1, int arg2);

  void on_actionPrint_Library_by_ID_triggered();

  void on_actionPrit_Library_by_Name_triggered();

  void on_actionAdd_User_triggered();



  void on_actionPrint_Usrs_List_triggered();

  void on_actionSearch_Book_by_prefix_triggered();

  void on_actionBorrow_Book_triggered();

  void on_actionReturn_Book_triggered();

  void on_actionAdd_Book_triggered();

  void on_actionPrint_who_borrowed_book_by_name_triggered();

  void on_pushButton_clicked();

private:
  Ui::MainWindow *ui;

  void saveSession();
  void loadPreviousSession();

};
#endif // MAINWINDOW_H
