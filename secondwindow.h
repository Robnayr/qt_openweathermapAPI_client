#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
namespace Ui {
class SecondWindow;
}

class SecondWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SecondWindow(QWidget *parent = nullptr);
    ~SecondWindow();

private slots:
    void on_enterButton_clicked();
    void replyFinished(QNetworkReply *reply);
    void on_clearButton_clicked();

private:
    Ui::SecondWindow *ui;
    const QString keyString;
};

#endif // SECONDWINDOW_H
