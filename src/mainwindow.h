/***************************************************************************
 *  Copyright (c) 2012 by Aten Zhang <atenzd@gmail.com>                    *
 *                                                                         *
 *  This file is part of WarMtH.                                       *
 *                                                                         *
 *  WarMtH is free software: you can redistribute it and/or modify     *
 *  it under the terms of the GNU General Public License as published by   *
 *  the Free Software Foundation, either version 3 of the License, or      *
 *  (at your option) any later version.                                    *
 *                                                                         *
 *  WarMtH is distributed in the hope that it will be useful,          *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *  GNU General Public License for more details.                           *
 *                                                                         *
 *  You should have received a copy of the GNU General Public License      *
 *  along with WarMtH.  If not, see <http://www.gnu.org/licenses/>.    *
 ***************************************************************************/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QSettings>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>

#include "authmsgwindow.h"
#include "configwindow.h"


class MainWindow : public QDialog //QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    friend class AuthMsgWindow;

//public slots:
//    void authClicked();
private slots:
    void createAuthMW();
    void authClicked();
    void enableAuthButton(const QString &text);
    void createCfgWd();
    void showAndHideIcon();
    void saveID(int state);

private:
    //functions to encrypt and decrypt password
    QString getXorEncryptDecrypt(const QString &str, const char &key);
    QString byteToQString(const QByteArray &byte);
    QByteArray qstringToByte(const QString &strInfo);

    QLabel *netid;
    QLabel *passwd;
    QLineEdit *idEdit;
    QLineEdit *pdEdit;
    QCheckBox *rembCheckBox;
    QPushButton *authButton;
    QPushButton *closeButton;
    QPushButton *configButton;
    AuthMsgWindow *authMW;
    ConfigWindow *confW;
};

#endif // MAINWINDOW_H
