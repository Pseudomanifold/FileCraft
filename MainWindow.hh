#ifndef FILECRAFT_MAIN_WINDOW_HH
#define FILECRAFT_MAIN_WINDOW_HH

#include <QMainWindow>

class RenderWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();

private:
  RenderWidget* _renderWidget;
};

#endif
