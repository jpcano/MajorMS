#include "majorUI.h"

#include <dictionary.h>
#include <qlineedit.h>

#include <QFile>
#include <QFileDialog>
#include <QFont>
#include <QFontDialog>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QTextStream>
#include <memory>
#include <vector>

#include "dict_configs.h"
#include "ui_majorUI.h"

MajorUI::MajorUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::MajorUI) {
  ui->setupUi(this);

  QRegularExpression rx("^[0-9]*$");
  QValidator *validator = new QRegularExpressionValidator(rx, this);
  ui->lineEdit->setValidator(validator);
  connect(ui->lineEdit, &QLineEdit::textChanged, this,
          &MajorUI::lineEditChanged);

  connect(ui->actionNew, &QAction::triggered, this, &MajorUI::newDocument);
  connect(ui->actionExit, &QAction::triggered, this, &QWidget::close);
  connect(ui->actionAbout, &QAction::triggered, this, &MajorUI::about);
  // connect(ui->actionAbout_Qt, &QAction::triggered, this,
  //         &QApplication::aboutQt);

  std::vector<DictConfig> configs;
  for (const auto &c : dict_configs) configs.push_back(c.second);
  major = std::make_unique<Major>(configs);
}

MajorUI::~MajorUI() { delete ui; }

void MajorUI::lineEditChanged() {
  auto n = ui->lineEdit->text();
  auto results = major->findWords(n.toStdString(), SearchType::Separated);
  std::string results_fmt = Major::printResults(results);
  ui->textEdit->setText(QString::fromStdString(results_fmt));
}
void MajorUI::newDocument() {
  currentFile.clear();
  ui->textEdit->clear();
  ui->lineEdit->clear();
}

void MajorUI::about() {
  QLabel *icon = new QLabel;
  QImage img(":/images/info.png");
  QPixmap pm;
  pm.convertFromImage(img);
  icon->setPixmap(pm);

  QLabel *text = new QLabel;
  text->setWordWrap(true);
  text->setText(
      "<h3>About MajorMS</h3>"
      "<p>Please see <a "
      "href=\"https://github.com/jpcano/MajorMS/blob/main/LICENSE\"> "
      "https://github.com/jpcano/MajorMS/blob/main/LICENSE</a> "
      "for an overview of MajorMS licensing</p>"
      "<p>Copyright (C) Jesus Cano.</p>");
  text->setOpenExternalLinks(true);

  QPushButton *quitButton = new QPushButton("OK");

  QHBoxLayout *topLayout = new QHBoxLayout;
  topLayout->setContentsMargins(10, 10, 10, 10);
  topLayout->setSpacing(10);
  topLayout->addWidget(icon);
  topLayout->addWidget(text);

  QHBoxLayout *bottomLayout = new QHBoxLayout;
  bottomLayout->addStretch();
  bottomLayout->addWidget(quitButton);
  bottomLayout->addStretch();

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(topLayout);
  mainLayout->addLayout(bottomLayout);

  QDialog about(this);
  about.setModal(true);
  about.setWindowTitle(tr("About MajorMS"));
  about.setLayout(mainLayout);

  connect(quitButton, &QPushButton::clicked, &about, &QDialog::close);

  about.exec();
}
