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
#include <QPainter>
#include <QGridLayout>
#include <iostream>

#include "authmsgwindow.h"

AuthMsgWindow::AuthMsgWindow(QWidget *parent)
        : QDialog(parent)
{
    exitButton = new QPushButton(_("&Exit"));
    miniButton = new QPushButton(_("&Minimize"));
    reauthButton = new QPushButton(_("Re-&authenticate"));
    authMsg = new QTextEdit;
    args = new QStringList;
    sysTrayIcon = new QSystemTrayIcon(QIcon(":/warmth.png"));
    setAttribute(Qt::WA_DeleteOnClose,true);

    trayActions();
    createTrayMenu();

    //connect(sysTrayIcon,SIGNAL(messageClicked()),this, SLOT(show()));
    //connect(sysTrayIcon,SIGNAL(messageClicked()),sysTrayIcon, SLOT(hide()));
    connect(sysTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this,SLOT(trayIconAct(QSystemTrayIcon::ActivationReason)));

    authMsg->setReadOnly(true);

    setWindowTitle(_("Authentication"));

    msgTitle = new QLabel(_("<font size=5><b>Authentication</b></font>"));


    connect(exitButton, SIGNAL(clicked()), this, SLOT(exitClicked()));
    connect(miniButton, SIGNAL(clicked()), this, SLOT(miniClicked()));
    connect(reauthButton,SIGNAL(clicked()),this, SLOT(reAuth()));

    backendName = new QString;
    backend = new QProcess(this);//get runtime error when delete backend in destructor
    backend->setProcessChannelMode(QProcess::MergedChannels);
    connect(backend, SIGNAL(readyReadStandardOutput()), this, SLOT(readresult()));
    //show reauthenticate button when authenticate fail
    connect(backend,SIGNAL(finished(int)),this,SLOT(changeButton(int)));
    exitMTH = new QProcess(this); /*process to exit mentohust */

}

AuthMsgWindow::~AuthMsgWindow()
{
    delete layout();
    delete msgTitle;
    delete authMsg;
    delete exitButton;
    delete miniButton;
    delete reauthButton;
    delete args;
    delete minimizeAction;
    delete exitAction;
    delete restoreAction;
    delete quitAction;    
    delete trayIconMenu;
    delete sysTrayIcon;
    delete backendName;
}

void AuthMsgWindow::displayWD()
{
    delete layout();

    //#ifndef ENABLE_TRANSPARENT
    //try to use if statement instead of pre-processing
    if(!transparent)
    {

        QHBoxLayout *buttons = new QHBoxLayout;
        buttons->addStretch();
        buttons->addWidget(miniButton);
        buttons->addStretch();
        buttons->addWidget(exitButton);
        buttons->addStretch();
        //reauthenticate button's on the same position as minimize button
        buttons->insertWidget(1,reauthButton);
        reauthButton->hide();


        resize(358,250);


        QVBoxLayout *mainLayout = new QVBoxLayout(this);
        mainLayout->addWidget(authMsg);
        mainLayout->addLayout(buttons);
        setLayout(mainLayout);
    }
    //#else
    else
    {
        setAutoFillBackground(true);
        //setWindowTitle(tr("Authentication"));
        setWindowOpacity(1);
        setAttribute(Qt::WA_TranslucentBackground,true);

        QGridLayout *titleLayout = new QGridLayout;
        titleLayout->addWidget(msgTitle);
        titleLayout->setContentsMargins(50,20,100,10);

        QVBoxLayout *buttons = new QVBoxLayout;
        buttons->setContentsMargins(0,100,10,0);
        buttons->addStretch();
        buttons->addWidget(miniButton);
        buttons->addStretch();
        buttons->addWidget(exitButton);
        buttons->addStretch();
        //reauthenticate button's on the same position as minimize button
        buttons->insertWidget(1,reauthButton);
        reauthButton->hide();


        QGridLayout *msgLayout = new QGridLayout;
        //msgLayout->addStretch();
        msgLayout->addWidget(authMsg);
        msgLayout->setContentsMargins(15,5,0,20);
        //msgLayout->addStretch();

        QVBoxLayout *leftLayout = new QVBoxLayout;
        leftLayout->addLayout(titleLayout);
        leftLayout->addLayout(msgLayout);

        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->addLayout(leftLayout);
        mainLayout->addLayout(buttons);
        setLayout(mainLayout);

        // resize first and then it can be move to the center of the screen
        resize(450,360);

        setWindowFlags(Qt::FramelessWindowHint); // not show the window frame
    }

    move((QApplication::desktop()->width()-this->width())/2,(QApplication::desktop()->height()-this->height())/2);

    //#endif
}

void AuthMsgWindow::readresult()
{
    QTextCodec *data = QTextCodec::codecForName("UTF-8");
    QString result = data->toUnicode(backend->readAllStandardOutput()); 
    authMsg->append(result.trimmed()); //delete the whitespace from the start and the end
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
    exitMTH->start(*backendName,QStringList()<<"-k");

    if(exitMTH->waitForFinished(-1)&&backend->waitForFinished(-1))close();
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
    sysTrayIcon->showMessage(_("WarMtH"), message,QSystemTrayIcon::Information,2000);
}

void AuthMsgWindow::trayActions()
{
    minimizeAction = new QAction(_("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));


    exitAction = new QAction(_("&Exit"), this);
    connect(exitAction, SIGNAL(triggered()), this, SLOT(exitClicked()));

    restoreAction = new QAction(_("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));


    quitAction = new QAction(_("&Quit"), this);
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

//#ifdef	ENABLE_TRANSPARENT
void AuthMsgWindow::paintEvent(QPaintEvent *event)
{
    //trying to set a transparent background image
    //the size of image is 500x400
    QPainter painter(this);
    if(transparent)
    {
    painter.setCompositionMode(QPainter::CompositionMode_Source);
    painter.drawImage(QRectF(0, 0, 450, 360), QImage(":/bg.png"));
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.fillRect(QImage(":/bg.png").rect(), QColor(0, 0, 0, 250));
    }
}
//#endif

