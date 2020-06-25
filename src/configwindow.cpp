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

#include "configwindow.h"

ConfigWindow::ConfigWindow(QWidget *parent)
    : QDialog(parent)
{
    setAttribute(Qt::WA_DeleteOnClose,true);
    // settings
    QSettings setting("WarMtH","warmth");

    ////**Initialize the tab page**////
    configTabs = new QTabWidget;
    argsPage = new QWidget;
    appPage = new QWidget;

    configTabs->addTab(argsPage,_("&Arguments"));
    configTabs->addTab(appPage,_("A&pplication"));

    ////**Widgets on configuration window**////
    /* 1. Confirm and Exit buttons */
    confirmButton = new QPushButton(_("Confir&m"));
    cancelButton = new QPushButton(_("Canc&el"));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(cancelClicked()));
    connect(confirmButton, SIGNAL(clicked()), this, SLOT(confirmClicked()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(confirmButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addStretch();

    //**Widgets on arguments page**//
    /* 2. Net cards selection */
    CVNetCard = new QString;
    netCardName = new QLabel(_("Netcard:"));
    netCardSelect = new QComboBox;
    netCardArg = new QStringList;
    netCardName->setToolTip(_("Select the network card you used"));
    QHBoxLayout *netCardLayout = new QHBoxLayout;
    netCardLayout->addWidget(netCardName);
    netCardLayout->addWidget(netCardSelect);

    //netCardSelect->setEditable(true);
    //QLineEdit *edit = netCardSelect->lineEdit();

    //get active net card from system, it is a simple and stupid implementation,
    //it should be rewriten some day.
    getNetCard= new QProcess;
    getNetCard->start("/bin/ls -1 /proc/net/dev_snmp6");
    connect(getNetCard, SIGNAL(readyReadStandardOutput()), this, SLOT(showNetCards()));

    //set default netcard read from setting file
    if(!setting.value("netcard").isNull())
    {
        *netCardArg = QStringList()<<"-n"<<setting.value("netcard","").toString();
        *CVNetCard = setting.value("netcard","").toString();
    }
    netCardSelect->addItem(setting.value("netcard","").toString());
    connect(netCardSelect,SIGNAL(currentIndexChanged(QString)), this, SLOT(saveNetCard(QString)));


    /* 3. Authentication type selection */
    //CVAuthType=1;
    authTypeName = new QLabel(_("Auth. Type:"));
    authTypeName->setToolTip(_("Authentication type"));
    authType = new QComboBox;
    authTypeArg = new QStringList;
    //the index for Authentication Type is: 0. wireless 1. wired
    authType->insertItems( 0,QStringList()<<_("wireless")<<_("wired"));

    QHBoxLayout *authTypeLayout = new QHBoxLayout;
    authTypeLayout->addWidget(authTypeName);
    authTypeLayout->addWidget(authType);

    //set default mulcast read from setting file, if no value read, set 1(wired)
    authType->setCurrentIndex(setting.value("authtype",1).toInt());
    *authTypeArg=QStringList()<<"-a"<<setting.value("authtype",1).toString();
    CVAuthType = setting.value("authtype",1).toInt();
    connect(authType,SIGNAL(currentIndexChanged(int)), this, SLOT(saveAuthType(int)));

    /* 5. DHCP type */
    //CVDhcpType=0;
    dhcpTypeName = new QLabel(_("DHCP type:"));
    dhcpTypeName->setToolTip(_("Set the type of DHCP"));
    dhcpType = new QComboBox;
    dhcpTypeArg = new QStringList;
    //the index for DHCP type is: 0(not in used) 1(in used)
    dhcpType->insertItems( 0,QStringList()<<_("not in used")<<_("in used"));

    QHBoxLayout *dhcpTypeLayout = new QHBoxLayout;
    dhcpTypeLayout->addWidget(dhcpTypeName);
    dhcpTypeLayout->addWidget(dhcpType);

    //set default dhcp type read from setting file, if no value read, set 0(not in used)
    dhcpType->setCurrentIndex(setting.value("dhcptype",0).toInt());
    *dhcpTypeArg=QStringList()<<"-d"<<setting.value("dhcptype",0).toString();
    CVDhcpType = setting.value("dhcptype",0).toInt();
    connect(dhcpType,SIGNAL(currentIndexChanged(int)), this, SLOT(saveDhcpType(int)));

    //**Widgets on application page**//
    /* 4. time to display notification */
    CVDispNotif = new QString;
    dispNotif = new QLabel(_("System Notification:"));
    dispNotif->setToolTip(_("Seconds to display system notification, letters to no show"));
    dispNotifTime = new QLineEdit;
    QLabel *unitdis = new QLabel(_("s [0(no) 1-20(yes)]"));

    //line edit width
    dispNotifTime->setMaximumWidth(22);
    dispNotifTime->setMaxLength(2);
    //set default value
    dispNotifTime->setText(setting.value("displaynotification",5).toString());
    *CVDispNotif = setting.value("displaynotification",5).toString();

    //set layout
    QHBoxLayout *dispNotifLayout = new QHBoxLayout;
    dispNotifLayout->addWidget(dispNotif);
    dispNotifLayout->addStretch();
    dispNotifLayout->addWidget(dispNotifTime);
    dispNotifLayout->addWidget(unitdis);

    connect(dispNotifTime,SIGNAL(textChanged(QString)), this, SLOT(saveDispNotif(QString)));

    /* 11. auto show system tray message */
    autoTrayMsg = new QCheckBox(_("Show balloon message on system tray when minimized"));
    autoTrayMsg->setChecked((setting.value("traymsg",1).toInt())==1);
    CVTrayMsg=setting.value("traymsg",1).toInt();
    connect(autoTrayMsg,SIGNAL(clicked()),this,SLOT(saveTrayMsg()));

    /* 12. enable transparent authentication message window or not */
    transAuthWD = new QCheckBox(_("Enable transparent authentication window"));
    transAuthWD->setChecked((setting.value("transparentauthwd",1).toInt())==1);
    CVTransparent=setting.value("transparentauthwd",1).toInt();
    connect(transAuthWD,SIGNAL(clicked()),this,SLOT(saveTransAuthWD()));

    ////** set the whole layout of configure window **////
    QVBoxLayout *argsPageLayout = new QVBoxLayout;
    argsPageLayout->addLayout(dhcpTypeLayout);
    argsPageLayout->addLayout(authTypeLayout);
    argsPageLayout->addLayout(netCardLayout);
    argsPage->setLayout(argsPageLayout);

    QVBoxLayout *appPageLayout = new QVBoxLayout;
    appPageLayout->addWidget(transAuthWD);
    appPageLayout->addWidget(autoTrayMsg);
    appPageLayout->addLayout(dispNotifLayout);
    appPageLayout->addStretch();
    appPage->setLayout(appPageLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(configTabs);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    setWindowTitle(_("Configuration"));
    setFixedHeight(sizeHint().height());

    args=new QStringList;

}

ConfigWindow::~ConfigWindow()
{
    delete layout();
    delete args;

    delete netCardArg;
    delete CVNetCard;
    delete authTypeArg;
    delete CVDispNotif;
    delete dhcpTypeArg;

    //when the configTabs deleted, the widgets included in it will be deleted too.
    //so I don't need to delete them
    delete configTabs;
    delete confirmButton;
    delete cancelButton;

}



void ConfigWindow::showNetCards()
{
    //QString result = QString(getNetCard->readAllStandardOutput());
    QStringList netCards=QString(getNetCard->readAllStandardOutput())
            .remove(QRegExp("lo")).split("\n",QString::SkipEmptyParts);
    netCardSelect->addItems(netCards);
}

void ConfigWindow::confirmClicked()
{

    QSettings setting("WarMtH","warmth");
    CVNetCard->isEmpty()?*netCardArg=QStringList():*netCardArg = QStringList()<<"-n"<<*CVNetCard;
    setting.setValue("netcard",*CVNetCard);

    *authTypeArg = QStringList()<<"-a"<<QString::number(CVAuthType);
    setting.setValue("authtype",CVAuthType);

    *dhcpTypeArg = QStringList()<<"-d"<<QString::number(CVDhcpType);
    setting.setValue("dhcptype",CVDhcpType);

    setting.setValue("traymsg",CVTrayMsg);

    setting.setValue("transparentauthwd",CVTransparent);

    close();
}

void ConfigWindow::cancelClicked()
{
    close();
}

void ConfigWindow::saveNetCard(const QString name)
{
    *CVNetCard = name;
}

void ConfigWindow::saveAuthType(const int index)
{
    CVAuthType = index;
}

void ConfigWindow::saveDhcpType(const int index)
{
    CVDhcpType = index;
}

void ConfigWindow::saveDispNotif(const QString time)
{
    if(time.toInt()>20)
    {
        QMessageBox::warning(this,_("Out of Range"),_("The second value should be 0-20."));
    }
    else
    {
        *CVDispNotif = time;
    }
}

void ConfigWindow::saveTrayMsg()
{
    autoTrayMsg->isChecked()?CVTrayMsg=1:CVTrayMsg=0;
}

void ConfigWindow::saveTransAuthWD()
{
    transAuthWD->isChecked()?CVTransparent=1:CVTransparent=0;
}

void ConfigWindow::setArgs()
{
    *args = QStringList()<<netCardArg->join("")
            <<authTypeArg->join("")
            <<dhcpTypeArg->join("");
}
