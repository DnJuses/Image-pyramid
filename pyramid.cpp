#include "pyramid.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QPalette>
#include <QDebug>
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
    connect(files, SIGNAL(currentIndexChanged(int)), this, SLOT(updateStats(int)));
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

void pyramid::setSizeTip(QString imageSize)
{
    sizeTip->setText("Size: " + imageSize);
}

bool pyramid::isDuplicate(QString checkPath)
{
    for(int i = 0; i < openedImages.size(); i++)
    {
        if(checkPath == openedImages[i]->getPath())
        {
            return true;
        }
    }
    return false;
}

void pyramid::sortAndRefill()
{
    int lastId = openedImages.size() - 1;
    // Сортировка вставками по параметру диагонали изображения
    for (int i = 1; i < openedImages.size(); i++)
    {
        int j = i;
        while (j > 0)
        {
            if(openedImages[j - 1]->getDiag() > openedImages[j]->getDiag())
            {
                if(lastId == j) lastId--;
                PyramidPixmap *temp = openedImages[j];
                openedImages[j] = openedImages[j - 1];
                openedImages[j - 1] = temp;
                j--;
            }
            else break;
        }
    }
    files->blockSignals(true);
    files->clear(); // Дважды посылает сигнал, из-за чего слот updateStats получает невалидные значения и программа крашится
    files->blockSignals(false);
    // Очистка и перезаполнение бокса
    for (int i = 0; i < openedImages.size(); i++)
    {
        int pos = openedImages[i]->getPath().lastIndexOf("/");
        if(pos != -1)
        {
            files->addItem(openedImages[i]->getPath().remove(0, pos + 1) + "   " + openedImages[i]->getImgSizeTip());
        }
        else
        {
            files->addItem(openedImages[i]->getPath() + "   " + openedImages[i]->getImgSizeTip());
        }
    }
    files->setCurrentIndex(lastId);
}

bool pyramid::openImage()
{
    QString openFilePath = QFileDialog::getOpenFileName(nullptr, nullptr, nullptr, tr("Any files (*.*);;Image files (*.jpg *.png)"));
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
    if(isDuplicate(openFilePath))
    {
        QMessageBox::StandardButton conf;
        conf = QMessageBox::question(0,
                                     "File opening",
                                     "This file is already opened! Open anyways?",
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);
                if(conf == QMessageBox::No)
                {
                    return true;
                }
    }
    PyramidPixmap *newImage = new PyramidPixmap(openFilePath);
    if(newImage->isLoaded())
    {
        openedImages.push_back(newImage);
        this->sortAndRefill();
        return true;
    }
    else
    {
        delete newImage;
    }
    QMessageBox cantOpen;
    cantOpen.setWindowTitle("File opening");
    cantOpen.setText("Cannot open an image.");
    cantOpen.setIcon(QMessageBox::Critical);
    cantOpen.exec();
    return false;
}

void pyramid::updateStats(int id)
{

    qDebug() << "Second - " << id;
    this->setSizeTip(openedImages[id]->getImgSizeTip());
    img->setBrush(imageWdg->backgroundRole(), QBrush(openedImages[id]->getImage()));
    imageWdg->resize(openedImages[id]->getImgSize());
    imageWdg->setPalette(*img);
}
