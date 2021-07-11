#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
#include "secondwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    ,ui(new Ui::MainWindow)
    ,rightLogin("admin")
    ,rightPassword("1234")
{
    ui->setupUi(this);
    ui->passwordLine->setEchoMode(QLineEdit::Password);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loginButton_clicked()
{
    QString login = ui->loginLine->text();
    QString pass = ui->passwordLine->text();
    if(login == rightLogin && pass == rightPassword) {
        hide();
        SecondWindow window;
        window.setModal(true);
        window.exec();
    } else if( login == "" || pass == "") {
        QMessageBox::warning(this , "Error" , "Input login and password");
    } else {
         QMessageBox::critical(this , "Error" , "Wrong login or password!");
    }
}



