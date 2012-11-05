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
#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>
#include <QtGlobal>
#include <QMessageBox>
#include <QTextEdit>
#include <QProcess>
#include <QSettings>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QCheckBox>
#include <QTabWidget>

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    AboutWindow(QWidget *parent = 0);
    ~AboutWindow();


    friend class MainWindow;

private:
    QTabWidget *aboutTabs;
    QPushButton *okButton;
};

class MentohustPage : public QWidget
{
    Q_OBJECT

public:
    MentohustPage(QWidget *parent = 0);
    ~MentohustPage();
};

class WarmthPage : public QWidget
{
public:
    WarmthPage(QWidget *parent = 0);
    ~WarmthPage();
};

#endif // ABOUTWINDOW_H
