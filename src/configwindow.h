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

#include "l10n.h"

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
    void saveAuthType(const int index);
    void saveDispNotif(const QString time);
    void saveDhcpType(const int index);
    void saveTrayMsg();
    void saveTransAuthWD();

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

    QLabel *authTypeName;
    QComboBox *authType;
    QStringList *authTypeArg;
    int CVAuthType;               // save current combobox index of authentication type

    QLabel *dispNotif;
    QLineEdit *dispNotifTime;
    QStringList *dispNotifArg;
    QString *CVDispNotif;       // save current value of display notification

    QLabel *dhcpTypeName;
    QComboBox *dhcpType;
    QStringList *dhcpTypeArg;
    int CVDhcpType;             // save current combobox index of DHCP type

    QCheckBox *autoTrayMsg;
    int CVTrayMsg;              // save current state of the checkbox

    QCheckBox *transAuthWD;
    int CVTransparent;          // save current state of the checkbox
};

#endif // CONFIGWINDOW_H
