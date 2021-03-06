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
#include <QSharedMemory>
#include <QDebug>

#include "mainwindow.h"
#include "l10n.h"
#include "qtsingleapplication/qtsingleapplication.h"

int main(int argc, char *argv[])
{
  /* usr shared memory to implement single instance
   
    QApplication app(argc, argv);app.processEvents();

    //---- Check for another instance code snippet ----

    //GUID : Generated once for your application
    // you could get one GUID here: http://www.guidgenerator.com/online-guid-generator.aspx
    QSharedMemory shared("62d60669-bb94-4a94-88bb-b964890a7e04");

    if( !shared.create( 512, QSharedMemory::ReadWrite) )
    {
      // For a GUI application, replace this by :
      // QMessageBox msgBox;
      //msgBox.setText( QObject::tr("Can't start more than one instance of the application.") );
      //msgBox.setIcon( QMessageBox::Critical );
      //msgBox.exec();

      qWarning() << "Can't start more than one instance of the application.";

      exit(0);
    }
    else {
        qDebug() << "Application started successfully.";
    }
    //---- END OF Check for another instance code snippet ----
    */
  
    QtSingleApplication app("warmth_1",argc, argv);
  
    if (app.sendMessage("raise_running_window"))
    {
      
      return 0;
      
    }
  
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
    app.setActivationWindow(mainWD,true);
    
    mainWD->show();

    return app.exec();
}
