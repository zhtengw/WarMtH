/***************************************************************************
 *  Copyright (c) 2012 by Aten Zhang <atenzd@gmail.com>                    *
 *                                                                         *
 *  This file is part of WarMtH.                                           *
 *                                                                         *
 *  WarMtH is free software: you can redistribute it and/or modify         *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  WarMtH is distributed in the hope that it will be useful,              *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with WarMtH.  If not, see <http://www.gnu.org/licenses/>.        *
 ***************************************************************************/
#include <QTextCodec>
#include <QApplication>

#include "mainwindow.h"
#include "configwindow.h"
#include "authmsgwindow.h"
#include "aboutwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QDialog(parent)
{

     QSettings setting("WarMtH","warmth");

    netid = new QLabel(tr("&NetID:"));
    idEdit = new QLineEdit;
    netid->setBuddy(idEdit);
    idEdit->setMaximumWidth(140);
    idEdit->setText(setting.value("netid","").toString());

    passwd = new QLabel(tr("&Password:"));
    pdEdit = new QLineEdit;
    pdEdit->setEchoMode(QLineEdit::Password);
    passwd->setBuddy(pdEdit);
    pdEdit->setMaximumWidth(140);
    pdEdit->setText(getXorEncryptDecrypt(setting.value("password","").toString(),111));

    rembCheckBox = new QCheckBox(tr("&Remember for next authification"));

    authButton = new QPushButton(tr("&Authenticate"));
    //authButton->setDefault(true);
    authButton->setEnabled(false);

    closeButton = new QPushButton(tr("Clos&e"));
    configButton = new QPushButton(tr("&Configure"));
    aboutButton = new QPushButton(tr("Abou&t"));

    this->setAttribute(Qt::WA_DeleteOnClose);
    createCfgWd();
    createAuthMW();

    enableAuthButton("");
    connect(pdEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableAuthButton(const QString&)));
    connect(idEdit, SIGNAL(textChanged(const QString&)), this, SLOT(enableAuthButton(const QString&)));


    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(closeButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(aboutButton, SIGNAL(clicked()), this, SLOT(aboutMe()));

    connect(rembCheckBox, SIGNAL(stateChanged(int)), this, SLOT(saveID(int)));

    QHBoxLayout *midLayout = new QHBoxLayout;
    midLayout->addStretch();
    midLayout->addWidget(rembCheckBox);
    midLayout->addStretch();

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(authButton);
    //bottomLayout->addStretch();
    bottomLayout->addWidget(closeButton);
    //bottomLayout->addStretch();
    bottomLayout->addWidget(configButton);
    //bottomLayout->addStretch();
    bottomLayout->addWidget(aboutButton);
    bottomLayout->addStretch();

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(netid);
    leftLayout->addWidget(passwd);

    QVBoxLayout *rightLayout = new QVBoxLayout;
    //rightLayout->addStretch();
    rightLayout->addWidget(idEdit);
    rightLayout->addWidget(pdEdit);



    QHBoxLayout *topLayout = new QHBoxLayout;
    //topLayout->addStretch();
    topLayout->addLayout(leftLayout);
    topLayout->addLayout(rightLayout);
    //topLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(topLayout);
    mainLayout->addLayout(midLayout);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);

    resize(100,200);
    setWindowTitle(tr("WarMtH"));
    //setFixedHeight(sizeHint().height());

}

MainWindow::~MainWindow()
{
    delete layout();
    delete netid;
    delete passwd;
    delete idEdit;
    delete pdEdit;
    delete rembCheckBox;
    delete authButton;
    delete closeButton;
    delete configButton;
    delete aboutButton;
    //delete authMW;
    //delete confW;
}

void MainWindow::aboutMe()
{
    AboutWindow *aboutWD = new AboutWindow;
    aboutWD->show();

}

void MainWindow::createAuthMW()
{

    authMW = new AuthMsgWindow;
    connect(authButton, SIGNAL(clicked()), this, SLOT(authClicked()));
    connect(authButton, SIGNAL(clicked()), this, SLOT(hide()));
    //connect(authButton, SIGNAL(clicked()), authMW, SLOT(show()));
    connect(authMW, SIGNAL(destroyed()), this, SLOT(show()));
    connect(authMW, SIGNAL(destroyed()), this, SLOT(createAuthMW()));


}
void MainWindow::authClicked()
{
    QString tempid = idEdit->text();
    QString temppd = pdEdit->text();
    if(rembCheckBox->isChecked()){saveID(2);}

    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        authMW->sysTrayIcon->show();
    }

    authMW->trayMsg = confW->autoTrayMsg->isChecked();
    authMW->transparent = confW->transAuthWD->isChecked();
    authMW->displayWD();
    authMW->setArgs(tempid, temppd);
    confW->setArgs();
    *authMW->args=*authMW->args<<*confW->args;
    *authMW->backendName=QString("mentohust");
    authMW->backend->start(*authMW->backendName, *authMW->args);

    authMW->show();

}

void MainWindow::enableAuthButton(const QString &text)
{
    authButton->setEnabled(!(pdEdit->text().isEmpty()||idEdit->text().isEmpty()));
    //authButton->setEnabled(!text.isEmpty());
}

void MainWindow::createCfgWd()
{
    confW= new ConfigWindow;
    connect(confW,SIGNAL(destroyed()),this,SLOT(createCfgWd()));
    connect(configButton, SIGNAL(clicked()), confW, SLOT(show()));
}

void MainWindow::showAndHideIcon()
{
    this->show();
    //authMW->sysTrayIcon->hide();
}

void MainWindow::saveID(int state)
{
    if(state == 2 )
    {
        QSettings setting("WarMtH","warmth");
        setting.setValue("netid",idEdit->text());
        setting.setValue("password",getXorEncryptDecrypt(pdEdit->text(),111));
    }
}

QString MainWindow::getXorEncryptDecrypt(const QString &str, const char &key)
{
  QString result;
  QByteArray bs = qstringToByte(str);

  for(int i=0; i<bs.size(); i++){
    bs[i] = bs[i] ^ key;
  }

  result = byteToQString(bs);
  return result;
}

QString MainWindow::byteToQString(const QByteArray &byte)
{
  QString result;
  if(byte.size() > 0){
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    result = codec->toUnicode(byte);
  }

  return result;
}

QByteArray MainWindow::qstringToByte(const QString &strInfo)
{
  QByteArray result;
  if(strInfo.length() > 0){
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    result = codec->fromUnicode(strInfo);
  }

  return result;
}
