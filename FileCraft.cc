#include <QApplication>
#include <QThread>
#include <QTime>

#include "MainWindow.hh"

int main( int argc, char** argv )
{
  QApplication application( argc, argv );
  application.setApplicationName( "FileCraft" );
  application.setApplicationVersion( "0.0.1" );

  MainWindow mainWindow;
  mainWindow.setWindowTitle( application.applicationName() );
  mainWindow.show();

  while( !application.closingDown() )
  {
    auto start = QTime::currentTime();

    application.processEvents();

    auto end   = QTime::currentTime();

    QThread::msleep( 15 + start.msecsTo( end ) );
  }

  return 0;
}
