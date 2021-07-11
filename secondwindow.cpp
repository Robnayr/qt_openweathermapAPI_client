#include "secondwindow.h"
#include "ui_secondwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "QMessageBox"
#include <QFile>
#include <QDir>
#include <chrono>
#include <iostream>
#include <QDateTime>
#include <QTimeZone>

SecondWindow::SecondWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SecondWindow),
    keyString("0866f3bb9b5e7b6aa8019c730fc864c1"),
    infoFileDirectoryName("resources\\"),
    fileName("data.txt")
{
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
      qDebug() << jsonObject["dt"];
      this->ui->dateLabel->setText(QDateTime::fromSecsSinceEpoch(jsonObject["dt"].toInt(),
                                   QTimeZone::systemTimeZone()).toString("dddd d MMMM yyyy hh:mm:ss"));
      double temperatureKelvin = jsonObject["main"].toObject()["temp"].toDouble();
      double temperatureCelsius = temperatureKelvin-273.15;
      double temperatureFahrenheit = (temperatureCelsius*9/5)+32;  
      QString celsiusTemperatureName = QString::number(temperatureCelsius)+" °C";
      QString kelvintemperatureName = QString::number(temperatureKelvin)+" °K";
      QString fahrenheitTemperatureName = QString::number(temperatureFahrenheit)+" °F";
      this->ui->celsiusLabel->setText(celsiusTemperatureName);
      this->ui->fahrenheitLabel->setText(fahrenheitTemperatureName);
      this->ui->kelvinLabel->setText(kelvintemperatureName);

      QFile file(infoFileDirectoryName+fileName);
      if (file.exists()) {
           if(file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
                QTextStream out(&file);
                out << jsonResponse.toJson();
           }
      } else {
          QString path(infoFileDirectoryName);
          QDir dir(path);

          if (!dir.exists(path))
              dir.mkpath(path);

          QFile file(path + fileName);
          file.open(QIODevice::WriteOnly);
          file.write(jsonResponse.toJson());
      }
      file.close();
  }
  else {
      this->ui->celsiusLabel->clear();
      this->ui->fahrenheitLabel->clear();
      this->ui->kelvinLabel->clear();
      QMessageBox::warning(this , "Error" , "Input right query");
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
    if(this->ui->celsiusLabel->text() == "") {
        if(this->ui->cityLabel->text() == "") {
            QMessageBox::warning(this , "!" , "Nothing to clear!");
        }
        this->ui->cityLabel->clear();
    }
    this->ui->celsiusLabel->clear();
    this->ui->fahrenheitLabel->clear();
    this->ui->kelvinLabel->clear();
    this->ui->cityLabel->clear();
}

