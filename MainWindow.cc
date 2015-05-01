#include "MainWindow.hh"
#include "RenderWidget.hh"

MainWindow::MainWindow()
  : _renderWidget( new RenderWidget( this ) )
{
  this->setCentralWidget( _renderWidget );
}
