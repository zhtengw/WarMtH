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
#include <QDesktopWidget>
#include <QPalette>
#include <QBitmap>
#include <QtDeclarative/QDeclarativeView>
#include <QGridLayout>

#include "authmsgwindow.h"
#include "mainwindow.h"

AuthMsgWindow::AuthMsgWindow(QWidget *parent)
        : QDialog(parent)
{
    msgTitle = new QLabel(tr("<font size=5><b>Authentication</b></font>"));
    exitButton = new QPushButton(tr("&Exit"));
    miniButton = new QPushButton(tr("&Minimize"));
    reauthButton = new QPushButton(tr("Re-&authenticate"));
    authMsg = new QTextEdit;
    args = new QStringList;
    sysTrayIcon = new QSystemTrayIcon(QIcon(":/warmth.png"));

    trayActions();
    createTrayMenu();

    //connect(sysTrayIcon,SIGNAL(messageClicked()),this, SLOT(show()));
    //connect(sysTrayIcon,SIGNAL(messageClicked()),sysTrayIcon, SLOT(hide()));
    connect(sysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayIconAct(QSystemTrayIcon::ActivationReason)));

    authMsg->setReadOnly(true);

    buttons = new QVBoxLayout;
    buttons->addStretch();
    buttons->addWidget(miniButton);
    buttons->addStretch();
    buttons->addWidget(exitButton);
    buttons->addStretch();
    buttons->setContentsMargins(0,100,10,0);
    //reauthenticate button's on the same position as minimize button
    buttons->insertWidget(1,reauthButton);
    reauthButton->hide();

    QGridLayout *titleLayout = new QGridLayout;
    titleLayout->addWidget(msgTitle);
    titleLayout->setContentsMargins(50,20,100,10);

    QGridLayout *msgLayout = new QGridLayout;
    //msgLayout->addStretch();
    msgLayout->addWidget(authMsg);
    msgLayout->setContentsMargins(15,5,0,20);
    //msgLayout->addStretch();

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(titleLayout);
    leftLayout->addLayout(msgLayout);

    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(buttons);

    setAutoFillBackground(true);
    //setWindowTitle(tr("Authentication"));
    setWindowFlags(Qt::FramelessWindowHint); // not show the window frame
    setWindowOpacity(1);
    setAttribute(Qt::WA_TranslucentBackground,true);
    setLayout(mainLayout);

    // resize first and then it can be move to the center of the screen
    resize(450,360);
    move((QApplication::desktop()->width()-this->width())/2,(QApplication::desktop()->height()-this->height())/2);

    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(miniButton, SIGNAL(clicked()), this, SLOT(miniClicked()));
    connect(reauthButton,SIGNAL(clicked()),this, SLOT(reAuth()));

    backendName = new QString;
    backend = new QProcess(this);
    backend->setProcessChannelMode(QProcess::MergedChannels);
    connect(backend, SIGNAL(readyReadStandardOutput()), this, SLOT(readresult()));
    //show reauthenticate button when authenticate fail
    connect(backend,SIGNAL(finished(int)),this,SLOT(changeButton(int)));

}

AuthMsgWindow::~AuthMsgWindow()
{
}

void AuthMsgWindow::readresult()
{
    QTextCodec *data = QTextCodec::codecForName("UTF-8");
    QString result = data->toUnicode(backend->readAllStandardOutput());
    authMsg->append(result);
    if(this->isHidden()&&trayMsg)showMessage();

}

void AuthMsgWindow::setArgs(const QString &id, const QString &pd)
{

    QStringList idarg = QStringList() << "-u" << id;
    QStringList pdarg = QStringList() << "-p" << pd;
    *args = QStringList() << idarg.join("") << pdarg.join("") ;
    //args = rembCheckBox->isChecked() ? args : args << "-w" ;

}

void AuthMsgWindow::exitClicked()
{
    QProcess *exitMTH = new QProcess;
    exitMTH->start("mentohust -k");
    if(!this->isHidden())this->hide();
    //emit authMWhidden();
    delete this->sysTrayIcon;
    delete this;
}

void AuthMsgWindow::miniClicked()
{
    if (sysTrayIcon->isSystemTrayAvailable())
    {
        if(sysTrayIcon->supportsMessages()&&trayMsg)
        {
            showMessage();
        }
        this->hide();
    }
    else this->showMinimized();
}

void AuthMsgWindow::reAuth()
{
    backend->start(*backendName,*args);
    reauthButton->hide();
    miniButton->show();

}

void AuthMsgWindow::changeButton(const int &num)
{
    //hide minimize button and show reauthenticate button
    miniButton->hide();
    reauthButton->show();
}

void AuthMsgWindow::trayIconAct(QSystemTrayIcon::ActivationReason reason)
{
    minimizeAction->setEnabled(!this->isHidden());
    restoreAction->setEnabled(this->isHidden());
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        this->setVisible(this->isHidden());
        break;
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void AuthMsgWindow::showMessage()
{
    //show last two lines of the message output
    /*get the message from authMsg and split them by "return",
    so that every element of the allMessage QStringList is one
    line of all message output. And the elements are enumerated
    from 0.*/
    QStringList allMessage=QString(authMsg->toPlainText()).split("\n",QString::SkipEmptyParts);
    QStringList tempmsg = QStringList() << allMessage.at(allMessage.size()-2) << allMessage.last();
    QString message = tempmsg.join("\n");
    sysTrayIcon->showMessage(tr("WarMtH"), message,QSystemTrayIcon::Information,2000);
}

void AuthMsgWindow::trayActions()
{
    minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));


    exitAction = new QAction(tr("&Exit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitClicked()));

    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));


    quitAction = new QAction(tr("&Quit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void AuthMsgWindow::createTrayMenu()
{
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(exitAction);
    trayIconMenu->addAction(quitAction);

    sysTrayIcon->setContextMenu(trayIconMenu);
}

void AuthMsgWindow::paintEvent(QPaintEvent *event)
{
    //trying to set a transparent background image
    //the size of image is 500x400
    QPainter painter(this);
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(QRectF(0, 0, 450, 360), QImage(":/bg.png"));
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(QImage(":/bg.png").rect(), QColor(0, 0, 0, 250));
}
