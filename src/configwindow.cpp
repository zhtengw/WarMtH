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
    setAttribute(Qt::WA_DeleteOnClose);
    // settings
    QSettings setting("WarMtH","warmth");

    ////**Initialize the tab page**////
    configTabs = new QTabWidget;
    argsPage = new QWidget;
    appPage = new QWidget;

    configTabs->addTab(argsPage,tr("&Arguments"));
    configTabs->addTab(appPage,tr("A&pplication"));

    ////**Widgets on configuration window**////
    /* 1. Confirm and Exit buttons */
    confirmButton = new QPushButton(tr("Confir&m"));
    cancelButton = new QPushButton(tr("Canc&el"));
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
    netCardName = new QLabel(tr("Netcard:"));
    netCardSelect = new QComboBox;
    netCardArg = new QStringList;
    netCardName->setToolTip(tr("Select the network card you used"));
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


    /* 3. Mulcast address selection */
    //CVMulAdr=0;
    mulCastAdrName = new QLabel(tr("Mulcast Address:"));
    mulCastAdrName->setToolTip(tr("Mulcast Address"));
    mulCastAdr = new QComboBox;
    mulCastAdrArg = new QStringList;
    //the index for Mulcast Address is: 0. standard 1. ruijie 2. saier
    mulCastAdr->insertItems( 0,QStringList()<<tr("standard")<<tr("ruijie")<< tr("saier"));

    QHBoxLayout *mulCastLayout = new QHBoxLayout;
    mulCastLayout->addWidget(mulCastAdrName);
    mulCastLayout->addWidget(mulCastAdr);

    //set default mulcast read from setting file, if no value read, set 0(standard)
    mulCastAdr->setCurrentIndex(setting.value("mulcastaddress",0).toInt());
    *mulCastAdrArg=QStringList()<<"-a"<<setting.value("mulcastaddress",0).toString();
    CVMulAdr = setting.value("mulcastaddress",0).toInt();
    connect(mulCastAdr,SIGNAL(currentIndexChanged(int)), this, SLOT(saveMulAdr(int)));

    /* 5. DHCP type */
    //CVDhcpType=0;
    dhcpTypeName = new QLabel(tr("DHCP type:"));
    dhcpTypeName->setToolTip(tr("Set the type of DHCP"));
    dhcpType = new QComboBox;
    dhcpTypeArg = new QStringList;
    //the index for DHCP type is: 0(not in used) 1(secondary authenticate) 2(post authenticate) 3(pre authenticate)
    dhcpType->insertItems( 0,QStringList()<<tr("not in used")<<tr("secondary authenticate")<< tr("post authenticate")<<tr("pre authenticate"));

    QHBoxLayout *dhcpTypeLayout = new QHBoxLayout;
    dhcpTypeLayout->addWidget(dhcpTypeName);
    dhcpTypeLayout->addWidget(dhcpType);

    //set default dhcp type read from setting file, if no value read, set 0(not in used)
    dhcpType->setCurrentIndex(setting.value("dhcptype",0).toInt());
    *dhcpTypeArg=QStringList()<<"-d"<<setting.value("dhcptype",0).toString();
    CVDhcpType = setting.value("dhcptype",0).toInt();
    connect(dhcpType,SIGNAL(currentIndexChanged(int)), this, SLOT(saveDhcpType(int)));

    /* 6. authenticate timeout */
    CVAuthTO = new QString;
    authTimeOutName= new QLabel(tr("Authenticate timeout:"));
    authTimeOutName->setToolTip(tr("Seconds to wait for authentication"));
    authTimeOut = new QLineEdit;
    authTimeOutArg = new QStringList;
    QLabel *unitTO = new QLabel(tr("s"));

    //line edit width
    authTimeOut->setMaximumWidth(40);
    //set default value
    authTimeOut->setText(setting.value("authtimeout",8).toString());
    *authTimeOutArg = QStringList()<<"-t"<<setting.value("authtimeout",8).toString();
    *CVAuthTO = setting.value("authtimeout",8).toString();

    //set layout
    QHBoxLayout *authTimeOutLayout = new QHBoxLayout;
    authTimeOutLayout->addWidget(authTimeOutName);
    authTimeOutLayout->addStretch();
    authTimeOutLayout->addWidget(authTimeOut);
    authTimeOutLayout->addWidget(unitTO);

    connect(authTimeOut,SIGNAL(textChanged(QString)), this, SLOT(saveAuthTimeOut(QString)));

    /* 7. max failure times */
    CVMaxFT = new QString;
    maxFailTimesName= new QLabel(tr("Max failure times:"));
    maxFailTimesName->setToolTip(tr("Times limit for failure[0 means no limit]"));
    maxFailTimes = new QLineEdit;
    maxFailTimesArg = new QStringList;

    //line edit width
    maxFailTimes->setMaximumWidth(57);
    //set default value
    maxFailTimes->setText(setting.value("maxfailtimes",8).toString());
    *maxFailTimesArg = QStringList()<<"-l"<<setting.value("maxfailtimes",8).toString();
    *CVMaxFT = setting.value("maxfailtimes",8).toString();
    connect(maxFailTimes,SIGNAL(textChanged(QString)), this, SLOT(saveMaxFailTimes(QString)));

    //set layout
    QHBoxLayout *maxFailTimesLayout = new QHBoxLayout;
    maxFailTimesLayout->addWidget(maxFailTimesName);
    maxFailTimesLayout->addStretch();
    maxFailTimesLayout->addWidget(maxFailTimes);

    /* 8. wait on failure timeout */
    CVWaitFTO = new QString;
    waitFailTimeOutName= new QLabel(tr("Wait on failure timeout:"));
    waitFailTimeOutName->setToolTip(tr("Seconds to wait on failure"));
    waitFailTimeOut = new QLineEdit;
    waitFailTimeOutArg = new QStringList;
    QLabel *unitFTO = new QLabel(tr("s"));

    //line edit width
    waitFailTimeOut->setMaximumWidth(40);
    //set default value
    waitFailTimeOut->setText(setting.value("failtimeout",15).toString());
    *waitFailTimeOutArg = QStringList()<<"-r"<<setting.value("failtimeout",15).toString();
    *CVWaitFTO = setting.value("failtimeout",15).toString();

    //set layout
    QHBoxLayout *waitFailTimeOutLayout = new QHBoxLayout;
    waitFailTimeOutLayout->addWidget(waitFailTimeOutName);
    waitFailTimeOutLayout->addStretch();
    waitFailTimeOutLayout->addWidget(waitFailTimeOut);
    waitFailTimeOutLayout->addWidget(unitFTO);

    connect(waitFailTimeOut,SIGNAL(textChanged(QString)), this, SLOT(saveWaitFailTimeOut(QString)));

    /* 9. heartbeat timeout */
    CVHeatBTO = new QString;
    heartbeatTimeOutName= new QLabel(tr("Heartbeat timeout:"));
    heartbeatTimeOutName->setToolTip(tr("Interval between sending two heartbeat packages"));
    heartbeatTimeOut = new QLineEdit;
    heartbeatTimeOutArg = new QStringList;
    QLabel *unitHTO = new QLabel(tr("s"));

    //line edit width
    heartbeatTimeOut->setMaximumWidth(40);
    //set default value
    heartbeatTimeOut->setText(setting.value("heartbeattimeout",30).toString());
    *heartbeatTimeOutArg = QStringList()<<"-e"<<setting.value("heartbeattimeout",30).toString();
    *CVHeatBTO = setting.value("heartbeattimeout",30).toString();

    //set layout
    QHBoxLayout *heartbeatTimeOutLayout = new QHBoxLayout;
    heartbeatTimeOutLayout->addWidget(heartbeatTimeOutName);
    heartbeatTimeOutLayout->addStretch();
    heartbeatTimeOutLayout->addWidget(heartbeatTimeOut);
    heartbeatTimeOutLayout->addWidget(unitHTO);

    connect(waitFailTimeOut,SIGNAL(textChanged(QString)), this, SLOT(saveHeartbeatTimeOut(QString)));

    /* 10. imitated client version */
    CVClientVer= new QString;
    clientVersionName= new QLabel(tr("Client Version:"));
    clientVersionName->setToolTip(tr("The version of authentification client to imitate[default to 0.00, compatible with xrgsu]"));
    clientVersion = new QLineEdit;
    clientVersionArg = new QStringList;

    //line edit width
    clientVersion->setMaxLength(4);
    clientVersion->setMaximumWidth(57);
    //set default value
    clientVersion->setText(setting.value("clientversion","0.00").toString());
    *clientVersionArg = QStringList()<<"-v"<<setting.value("clientversion","0.00").toString();
    *CVClientVer = setting.value("clientversion","0.00").toString();

    //set layout
    QHBoxLayout *clientVersionLayout = new QHBoxLayout;
    clientVersionLayout->addWidget(clientVersionName);
    clientVersionLayout->addStretch();
    clientVersionLayout->addWidget(clientVersion);

    connect(clientVersion,SIGNAL(textChanged(QString)), this, SLOT(saveClientVersion(QString)));

    //**Widgets on application page**//
    /* 4. time to display notification */
    CVDispNotif = new QString;
    dispNotif = new QLabel(tr("System Notification:"));
    dispNotif->setToolTip(tr("Seconds to display system notification, letters to no show"));
    dispNotifTime = new QLineEdit;
    dispNotifArg = new QStringList;
    QLabel *unitdis = new QLabel(tr("s [0(no) 1-20(yes)]"));

    //line edit width
    dispNotifTime->setMaximumWidth(22);
    dispNotifTime->setMaxLength(2);
    //set default value
    dispNotifTime->setText(setting.value("displaynotification",5).toString());
    *dispNotifArg = QStringList()<<"-y"<<setting.value("displaynotification",5).toString();
    *CVDispNotif = setting.value("displaynotification",5).toString();

    //set layout
    QHBoxLayout *dispNotifLayout = new QHBoxLayout;
    dispNotifLayout->addWidget(dispNotif);
    dispNotifLayout->addStretch();
    dispNotifLayout->addWidget(dispNotifTime);
    dispNotifLayout->addWidget(unitdis);

    connect(dispNotifTime,SIGNAL(textChanged(QString)), this, SLOT(saveDispNotif(QString)));

    /* 11. auto show system tray message */
    autoTrayMsg = new QCheckBox(tr("Show balloon message on system tray when minimized"));
    autoTrayMsg->setChecked((setting.value("traymsg",1).toInt())==1);
    CVTrayMsg=setting.value("traymsg",1).toInt();
    connect(autoTrayMsg,SIGNAL(clicked()),this,SLOT(saveTrayMsg()));

    /* 12. enable transparent authentication message window or not */
    transAuthWD = new QCheckBox(tr("Enable transparent authentication window"));
    transAuthWD->setChecked((setting.value("transparentauthwd",1).toInt())==1);
    CVTransparent=setting.value("transparentauthwd",1).toInt();
    connect(transAuthWD,SIGNAL(clicked()),this,SLOT(saveTransAuthWD()));

    ////** set the whole layout of configure window **////
    QVBoxLayout *argsPageLayout = new QVBoxLayout;
    argsPageLayout->addLayout(clientVersionLayout);
    argsPageLayout->addLayout(heartbeatTimeOutLayout);
    argsPageLayout->addLayout(waitFailTimeOutLayout);
    argsPageLayout->addLayout(maxFailTimesLayout);
    argsPageLayout->addLayout(authTimeOutLayout);
    argsPageLayout->addLayout(dhcpTypeLayout);
    argsPageLayout->addLayout(mulCastLayout);
    argsPageLayout->addLayout(netCardLayout);
    argsPage->setLayout(argsPageLayout);

    QVBoxLayout *appPageLayout = new QVBoxLayout;
    appPageLayout->addWidget(transAuthWD);
    appPageLayout->addLayout(dispNotifLayout);
    appPageLayout->addWidget(autoTrayMsg);
    appPageLayout->addStretch();
    appPage->setLayout(appPageLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(configTabs);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    setWindowTitle(tr("Configuration"));
    setFixedHeight(sizeHint().height());

    args=new QStringList;


}

ConfigWindow::~ConfigWindow()
{
    delete layout();
    delete args;

    delete netCardArg;
    delete CVNetCard;
    delete mulCastAdrArg;
    delete dispNotifArg;
    delete CVDispNotif;
    delete dhcpTypeArg;
    delete authTimeOutArg;
    delete CVAuthTO;
    delete maxFailTimesArg;
    delete CVMaxFT;
    delete waitFailTimeOutArg;
    delete CVWaitFTO;
    delete heartbeatTimeOutArg;
    delete CVHeatBTO;
    delete clientVersionArg;
    delete CVClientVer;

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

    *mulCastAdrArg = QStringList()<<"-a"<<QString::number(CVMulAdr);
    setting.setValue("mulcastaddress",CVMulAdr);

    *dispNotifArg=QStringList()<<"-y"<<*CVDispNotif;
    setting.setValue("displaynotification",CVDispNotif->toInt());

    *dhcpTypeArg = QStringList()<<"-d"<<QString::number(CVDhcpType);
    setting.setValue("dhcptype",CVDhcpType);

    *authTimeOutArg=QStringList()<<"-t"<<*CVAuthTO;
    setting.setValue("authtimeout",CVAuthTO->toInt());

    *maxFailTimesArg=QStringList()<<"-l"<<*CVMaxFT;
    setting.setValue("maxfailtimes",CVMaxFT->toInt());

    *waitFailTimeOutArg=QStringList()<<"-r"<<*CVWaitFTO;
    setting.setValue("failtimeout",CVWaitFTO->toInt());

    *heartbeatTimeOutArg=QStringList()<<"-e"<<*CVHeatBTO;
    setting.setValue("heartbeattimeout",CVHeatBTO->toInt());

    *clientVersionArg=QStringList()<<"-v"<<*CVClientVer;
    setting.setValue("clientversion",*CVClientVer);

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

void ConfigWindow::saveMulAdr(const int index)
{
    CVMulAdr = index;
}

void ConfigWindow::saveDhcpType(const int index)
{
    CVDhcpType = index;
}

void ConfigWindow::saveDispNotif(const QString time)
{
    if(time.toInt()>20)
    {
        QMessageBox::warning(this,tr("Out of Range"),tr("The second value should be 0-20."));
    }
    else
    {
        *CVDispNotif = time;
    }
}

void ConfigWindow::saveAuthTimeOut(const QString time)
{
    *CVAuthTO = time;
}

void ConfigWindow::saveMaxFailTimes(const QString times)
{
    *CVMaxFT = times;
}

void ConfigWindow::saveWaitFailTimeOut(const QString time)
{
    *CVWaitFTO = time;
}

void ConfigWindow::saveHeartbeatTimeOut(const QString time)
{
    *CVHeatBTO = time;
}

void ConfigWindow::saveClientVersion(const QString version)
{
    *CVClientVer = version;
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
            <<mulCastAdrArg->join("")
            <<dispNotifArg->join("")
            <<dhcpTypeArg->join("")
            <<authTimeOutArg->join("")
            <<maxFailTimesArg->join("")
            <<waitFailTimeOutArg->join("")
            <<heartbeatTimeOutArg->join("")
            <<clientVersionArg->join("");
}
