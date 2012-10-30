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
#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QProcess>
#include <QSettings>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QTabWidget>

class ConfigWindow : public QDialog
{
    Q_OBJECT
public:
    ConfigWindow(QWidget *parent = 0);
    ~ConfigWindow();
    friend class MainWindow;

private slots:
    void cancelClicked();
    void confirmClicked();
    void showNetCards();
    void saveNetCard(const QString name);
    void saveMulAdr(const int index);
    void saveDispNotif(const QString time);
    void saveDhcpType(const int index);
    void saveAuthTimeOut(const QString time);
    void saveMaxFailTimes(const QString times);
    void saveWaitFailTimeOut(const QString time);
    void saveHeartbeatTimeOut(const QString time);
    void saveClientVersion(const QString version);
    void saveTrayMsg();

private:
    void setArgs();
    QStringList *args;
    QTabWidget *configTabs;
    QWidget *argsPage;
    QWidget *appPage;


    QPushButton *confirmButton;
    QPushButton *cancelButton;

    QLabel *netCardName;
    QProcess *getNetCard;
    QComboBox *netCardSelect;
    QStringList *netCardArg;
    QString *CVNetCard;         // save current of net card

    QLabel *mulCastAdrName;
    QComboBox *mulCastAdr;
    QStringList *mulCastAdrArg;
    int CVMulAdr;               // save current combobox index of mulcast address

    QLabel *dispNotif;
    QLineEdit *dispNotifTime;
    QStringList *dispNotifArg;
    QString *CVDispNotif;       // save current value of display notification

    QLabel *dhcpTypeName;
    QComboBox *dhcpType;
    QStringList *dhcpTypeArg;
    int CVDhcpType;             // save current combobox index of DHCP type

    QLabel *authTimeOutName;
    QLineEdit *authTimeOut;
    QStringList *authTimeOutArg;
    QString *CVAuthTO;          // save current value of authenticate timeout

    QLabel *maxFailTimesName;
    QLineEdit *maxFailTimes;
    QStringList *maxFailTimesArg;
    QString *CVMaxFT;           // save current value of max failure times

    QLabel *waitFailTimeOutName;
    QLineEdit *waitFailTimeOut;
    QStringList *waitFailTimeOutArg;
    QString *CVWaitFTO;         // save current value of waiting on failure timeout

    QLabel *heartbeatTimeOutName;
    QLineEdit *heartbeatTimeOut;
    QStringList *heartbeatTimeOutArg;
    QString *CVHeatBTO;         // save current value of heartbeat timeout

    QLabel *clientVersionName;
    QLineEdit *clientVersion;
    QStringList *clientVersionArg;
    QString *CVClientVer;       // save current value of imitated client version

    QCheckBox *autoTrayMsg;
    int CVTrayMsg;              // save current state of the checkbox
};

#endif // CONFIGWINDOW_H
