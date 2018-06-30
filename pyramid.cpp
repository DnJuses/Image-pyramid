#include "pyramid.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>


pyramid::pyramid(QWidget *parent) : QMainWindow(parent)
{
    this->resize(550, 550);
    createAll();
}

void *pyramid::createImage()
{
    imageLab = new QLabel(this);
    imageLab->setFixedSize(500, 500);
    imageLab->move(25, 25);
}

//QVBoxLayout *pyramid::setStdLayout()
//{
//    stdLayout = new QVBoxLayout;
//    stdLayout->addWidget(createImage());
//    return stdLayout;
//}

void pyramid::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openFile = new QAction("&Open image", this);
    openFile->setShortcuts(QKeySequence::Open);
    connect(openFile, QAction::triggered, this, pyramid::openImage);
    fileMenu->addAction(openFile);
}

void pyramid::createAll()
{
    createMenu();
    createImage();
}

bool pyramid::openImage()
{
    const QString openFilePath = QFileDialog::getOpenFileName(nullptr, nullptr, nullptr, tr("Any files (*.*);;Image files (*.jpg *.png)"));
    if(openFilePath == "") return true; // Пользователь закрыл файловый диалог и не выбрал файл.
    // Проверка на соответствие формата файла.
    if(!openFilePath.endsWith(".jpg") && !openFilePath.endsWith(".png"))
    {
        QMessageBox cantOpen;
        cantOpen.setWindowTitle("File opening");
        cantOpen.setText("Not allowed file format! Only .png and .jpg formats are allowed");
        cantOpen.setIcon(QMessageBox::Critical);
        cantOpen.exec();
        return false;
    }
    QPixmap *newImage = new QPixmap();
    if(newImage->load(openFilePath))
    {
        openedImages.push_back(newImage);
        imageLab->setPixmap(*newImage);
        return true;
    }
    QMessageBox cantOpen;
    cantOpen.setWindowTitle("File opening");
    cantOpen.setText("Cannot open an image.");
    cantOpen.setIcon(QMessageBox::Critical);
    cantOpen.exec();
    return false;
}
