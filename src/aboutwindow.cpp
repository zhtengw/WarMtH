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
#include <QApplication>
#include <QScrollArea>
#include <QLabel>
#include "aboutwindow.h"

#define VERSION "1.2.1"

AboutWindow::AboutWindow(QWidget *parent)
    :QDialog(parent)
{
    //setFixedSize(299,360);
    setWindowTitle(_("About WarMtH"));
    //a QLabel can only set one pixmap or text, so I use two QLabel
    QPixmap icon(":/warmth.png");
    icon=icon.scaled(64,64);
    QLabel *iconLabel = new QLabel(this);
    iconLabel->setPixmap(icon);
    //iconLabel->setScaledContents(true);
    QString a;
    a=_("<p><big><b>WarMtH</b></big> Version ");
    a.append(VERSION);
    a.append(_("</p>Run in Qt "));
    a.append(qVersion());
    a.append(_(" (Compiled in Qt "));
    a.append(QT_VERSION_STR);
    a.append(")");
    QLabel *name = new QLabel(this);
    name->setText(a);
    name->setWordWrap(true);
    okButton = new QPushButton(_("&OK"));
    aboutTabs = new QTabWidget;
    aboutTabs->addTab(new WarmthPage(this),_("&About"));
    aboutTabs->addTab(new MentohustPage(this),_("About &MentoHust"));

    //setAttribute(Qt::WA_DeleteOnClose,true);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
    QGridLayout *topLayout = new QGridLayout;
    topLayout->addWidget(iconLabel,1,1);
    topLayout->addWidget(name,1,2);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addStretch();
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(aboutTabs);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

}

AboutWindow::~AboutWindow()
{
    delete aboutTabs;
    delete okButton;

}

MentohustPage::MentohustPage(QWidget *parent)
{
    QLabel *aboutMentohust = new QLabel(this);
    aboutMentohust->setWordWrap(true);
    aboutMentohust->setText(_("<p><big><b>MentoHust</b></big> Version 0.3.1</p>"
                                    "<p>&nbsp;&nbsp;a ruijie authentication client</p>"
                                    "<p><b>Authors: </b></p>"
                               "<p><address>&nbsp;&nbsp;HustMoon</address> &lt;mentohust@ehust.co.cc&gt;</p>"
                               "<p><address>&nbsp;&nbsp;kkHAIKE</address> &lt;kkHAIKE@gmail.com&gt;</p>"));
    QScrollArea *scrollText = new QScrollArea(this);
    scrollText->setWidget(aboutMentohust);

    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(aboutMentohust);
    setLayout(mainLayout);
    setAttribute(Qt::WA_DeleteOnClose,true);
}
MentohustPage::~MentohustPage()
{
}

WarmthPage::WarmthPage(QWidget *parent)
{
    QLabel *about = new QLabel(this);
    about->setWordWrap(true);
    about->setText(_("<p><big>A GUI front end for MentoHust</big></p>"
                      "<p>Copyright &copy; 2012 <address>Aten Zhang</address></p>"
                      "<p>Homepage: </p>"
                      "<p><a href=\"http://code.google.com/p/warmth\">http://code.google.com/p/warmth</a></p>"
                      "<br />"
                      "License: "
                      "<p><a href=\"http://www.gnu.org/licenses/gpl.txt\">GNU General Public License Version 3</a></p>"));
    QVBoxLayout *mainLayout=new QVBoxLayout(this);
    mainLayout->addWidget(about);
    setLayout(mainLayout);
    setAttribute(Qt::WA_DeleteOnClose,true);
}

WarmthPage::~WarmthPage()
{

}
