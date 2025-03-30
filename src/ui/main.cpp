#include <QApplication>

#include "majorUI.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MajorUI w;
  w.show();
  return a.exec();
}
