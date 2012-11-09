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
#include <QTranslator>
#include <QLibraryInfo>

#include "mainwindow.h"
#include "l10n.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/warmth.png"));
    app.setQuitOnLastWindowClosed(false);
    QTranslator qtTranslator;
    qtTranslator.load("qt_" + QLocale::system().name(),
                      QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    app.installTranslator(&qtTranslator);

    //QTranslator warmthTranslator;
    //warmthTranslator.load("warmth_" + QLocale::system().name(),"/usr/share/warmth/l10n");
    //app.installTranslator(&warmthTranslator);

    setlocale(LC_ALL,"");
    bindtextdomain(PACKAGE, LOCALEDIR);
    textdomain (PACKAGE);
    
    QCoreApplication::setOrganizationName("WarMtH");
    QCoreApplication::setApplicationName("warmth");
    MainWindow *mainWD = new MainWindow;
    mainWD->show();

    return app.exec();
}
