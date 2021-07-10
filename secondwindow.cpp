#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "QMessageBox"
#include <QFile>
#include <QDir>

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow),
    keyString("0866f3bb9b5e7b6aa8019c730fc864c1") {
    ui->setupUi(this);
}

SecondWindow::~SecondWindow() {
    delete ui;
}

void SecondWindow::replyFinished(QNetworkReply *reply) {
  QString strReply = (QString)reply->readAll();
  QJsonDocument jsonResponse = QJsonDocument::fromJson(strReply.toUtf8());
  QJsonObject jsonObject = jsonResponse.object();

  qDebug() << strReply;
  if(jsonObject["cod"] == 200) {
      double temperatureCelsius = jsonObject["main"].toObject()["temp"].toDouble()-273.15;
      double temperatureFahrenheit = (temperatureCelsius*9/5)+32;
      double temperatureKelvin = jsonObject["main"].toObject()["temp"].toDouble();
      QString celsius = QString::number(temperatureCelsius)+" °C";
      QString kelvin = QString::number(temperatureKelvin)+" °K";
      QString fahrenheit = QString::number(temperatureFahrenheit)+" °F";
      this->ui->celsiusLabel->setText(celsius);
      this->ui->fahrenheitLabel->setText(fahrenheit);
      this->ui->kelvinLabel->setText(kelvin);

      QFile file("resources\\data.txt");
      if (file.exists()) {
           if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
                QTextStream out(&file);
                out << jsonResponse.toJson();
           }
           else
               qDebug() << "file not open";
      } else {
          QString path("resources\\");
          QDir dir(path) ;

          if (!dir.exists(path))
              dir.mkpath(path);

          QFile file(path + "data.txt");
          file.open(QIODevice::WriteOnly);
          file.write(jsonResponse.toJson());
      }

      file.close();
  }
  else {
      this->ui->celsiusLabel->setText("");
      this->ui->fahrenheitLabel->setText("");
      this->ui->kelvinLabel->setText("");
      QMessageBox::warning(this,"Error","Input right query");
  }
}

void SecondWindow::on_enterButton_clicked() {
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QNetworkRequest request;

    QSslConfiguration config = QSslConfiguration::defaultConfiguration();
    config.setProtocol(QSsl::TlsV1_2);
    request.setSslConfiguration(config);
    QString url = QString("https://api.openweathermap.org/data/2.5/weather?q=%1&appid=%2").arg(this->ui->cityLabel->text(), keyString);
    request.setUrl(QUrl(url));
    qDebug() << url;
    request.setHeader(QNetworkRequest::ServerHeader, "application/json");
    manager->get(request);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this,SLOT(replyFinished(QNetworkReply*)));
}

void SecondWindow::on_clearButton_clicked() {
    this->ui->celsiusLabel->clear();
    this->ui->fahrenheitLabel->clear();
    this->ui->kelvinLabel->clear();
    this->ui->cityLabel->clear();
}

