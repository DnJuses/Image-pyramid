#include "pyramid.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QPalette>
#include <QMessageBox>
#include <QScrollArea>


pyramid::pyramid(QWidget *parent) : QMainWindow(parent)
{
    this->resize(550, 550);
    createAll();
}

QWidget *pyramid::createCentral()
{
    centralWidget = new QWidget(this);
    centralWidget->setLayout(createCLayout());
    return centralWidget;
}

QVBoxLayout *pyramid::createCLayout()
{
    centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->addLayout(createBoxLay());
    centralLayout->addWidget(createScroll());
    centralLayout->addLayout(createSpawner());
    return centralLayout;
}

QWidget *pyramid::createImage()
{
    imageWdg = new QWidget(centralWidget);
    img = new QPalette;
    imageWdg->setAutoFillBackground(true);
    return imageWdg;
}

QScrollArea *pyramid::createScroll()
{
    QScrollArea *imageScroll = new QScrollArea(centralWidget);
    imageScroll->setFixedSize(500, 500);
    imageScroll->setAlignment(Qt::AlignCenter);
    imageScroll->setWidget(createImage());
    return imageScroll;
}

QComboBox *pyramid::createFilesBox()
{
    files = new QComboBox(centralWidget);
    return files;
}

QComboBox *pyramid::createLayersBox()
{
    layers = new QComboBox(centralWidget);
    return layers;
}

QHBoxLayout *pyramid::createBoxLay()
{
    fileTip = new QLabel(tr("File:"), centralWidget);
    layerTip = new QLabel(tr("Layer:"), centralWidget);
    sizeTip = new QLabel(tr("Size: 0x0"), centralWidget);
    boxLayout = new QHBoxLayout;
    boxLayout->addWidget(fileTip);
    boxLayout->addSpacing(-110);
    boxLayout->addWidget(createFilesBox());
    boxLayout->addSpacing(40);
    boxLayout->addWidget(layerTip);
    boxLayout->addSpacing(-100);
    boxLayout->addWidget(createLayersBox());
    boxLayout->addWidget(sizeTip);
    return boxLayout;
}

QHBoxLayout *pyramid::createSpawner()
{
    spawnLayers = new QPushButton(tr("Create layers"), centralWidget);
    QHBoxLayout *lowerEnd = new QHBoxLayout;
    lowerEnd->addStretch();
    lowerEnd->addWidget(spawnLayers);
    return lowerEnd;
}

void pyramid::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *openFile = new QAction(tr("&Open image"), this);
    openFile->setShortcuts(QKeySequence::Open);
    connect(openFile, QAction::triggered, this, pyramid::openImage);
    fileMenu->addAction(openFile);
}

void pyramid::createAll()
{
    createMenu();
    this->setCentralWidget(createCentral());
}

void pyramid::setSizeTip(QSize imageSize)
{
    sizeTip->setText("Size: " + QString::number(imageSize.width()) + "x" + QString::number(imageSize.height()));
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
        this->setSizeTip(newImage->size());
        img->setBrush(imageWdg->backgroundRole(), QBrush(*newImage));
        openedImages.push_back(newImage);
        imageWdg->resize(newImage->size());
        imageWdg->setPalette(*img);
        return true;
    }
    QMessageBox cantOpen;
    cantOpen.setWindowTitle("File opening");
    cantOpen.setText("Cannot open an image.");
    cantOpen.setIcon(QMessageBox::Critical);
    cantOpen.exec();
    return false;
}
