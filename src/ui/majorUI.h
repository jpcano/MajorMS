
#ifndef MAJOR_UI_H
#define MAJOR_UI_H

#include <major.h>

#include <QMainWindow>
#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui {
class MajorUI;
}
QT_END_NAMESPACE

class MajorUI : public QMainWindow {
  Q_OBJECT

 public:
  explicit MajorUI(QWidget *parent = nullptr);
  ~MajorUI();

 private slots:
  void newDocument();
  void about();
  void lineEditChanged();

 private:
  Ui::MajorUI *ui;
  QString currentFile;
  std::unique_ptr<Major> major;
};

#endif
