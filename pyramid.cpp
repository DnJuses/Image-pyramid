#include "pyramid.h"
#include <QFileDialog>
#include <QMenuBar>
#include <QMenu>
#include <QPalette>
#include <QDebug>
#include <QtMath>
#include <QMessageBox>
#include <QScrollArea>


pyramid::pyramid(QWidget *parent) : QMainWindow(parent)
{
    this->resize(518, 594);
    this->createAll();
}

QWidget *pyramid::createCentral()
{
    centralWidget = new QWidget(this);
    centralWidget->setLayout(this->createCLayout());
    return centralWidget;
}

QVBoxLayout *pyramid::createCLayout()
{
    centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->addLayout(this->createBoxLay());
    centralLayout->addWidget(this->createScroll());
    centralLayout->addLayout(this->createLowerEnd());
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
    imageScroll->setWidget(this->createImage());
    return imageScroll;
}

QComboBox *pyramid::createFilesBox()
{
    filesBox = new QComboBox(centralWidget);
    connect(filesBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateStats(int)));
    return filesBox;
}

QComboBox *pyramid::createLayersBox()
{
    layersBox = new QComboBox(centralWidget);
    connect(layersBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLayers(int)));
    return layersBox;
}

QHBoxLayout *pyramid::createBoxLay()
{
    fileTip = new QLabel(tr("File:"), centralWidget);
    layerTip = new QLabel(tr("Layer:"), centralWidget);
    sizeTip = new QLabel(tr("Size: 0x0"), centralWidget);
    boxLayout = new QHBoxLayout;
    boxLayout->addWidget(fileTip);
    boxLayout->addSpacing(-110);
    boxLayout->addWidget(this->createFilesBox());
    boxLayout->addSpacing(40);
    boxLayout->addWidget(layerTip);
    boxLayout->addSpacing(-100);
    boxLayout->addWidget(this->createLayersBox());
    boxLayout->addWidget(sizeTip);
    return boxLayout;
}

QPushButton *pyramid::createSpawnerButton()
{
    spawnLayers = new QPushButton(tr("Create layers"), centralWidget);
    connect(spawnLayers, SIGNAL(clicked()), this, SLOT(startLayersCreation()));
    return spawnLayers;
}

QDoubleSpinBox *pyramid::createMultiplier()
{
    multiplier = new QDoubleSpinBox(centralWidget);
    multiplier->setValue(2.0);
    multiplier->setMinimum(0.01);
    multiplier->setMaximum(99);
    connect(multiplier, SIGNAL(valueChanged(double)), this, SLOT(calculateRecommend(double)));
    return multiplier;
}

QSpinBox *pyramid::createLayersAmount()
{
    layersAmount = new QSpinBox(centralWidget);
    layersAmount->setMinimum(1);
    layersAmount->setMaximum(99);
    layersAmount->setValue(3);
    return layersAmount;
}

QHBoxLayout *pyramid::createLowerEnd()
{
    multiplierTip = new QLabel(tr("Multiplier:"), centralWidget);
    amountTip = new QLabel(tr("Layers:"), centralWidget);
    lowerEnd = new QHBoxLayout;
    lowerEnd->addWidget(createRecommendTip());
    lowerEnd->addStretch();
    lowerEnd->addWidget(multiplierTip);
    lowerEnd->addWidget(this->createMultiplier());
    lowerEnd->addWidget(amountTip);
    lowerEnd->addWidget(this->createLayersAmount());
    lowerEnd->addSpacing(15);
    lowerEnd->addWidget(this->createSpawnerButton());
    return lowerEnd;
}

QLabel *pyramid::createRecommendTip()
{
    recommendTip = new QLabel("Recommended number of layers: 0", centralWidget);
    return recommendTip;
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
    this->createMenu();
    this->setCentralWidget(createCentral());
}

void pyramid::setSizeTip(QString imageSize)
{
    sizeTip->setText("Size: " + imageSize);
}

void pyramid::calculateRecommend(double mult)
{
    if(filesBox->count() == 0 || mult == 0) return;
    QSizeF lastLayerSize = openedImages[filesBox->currentIndex()]->getLayerSize(layersBox->count() - 1);
    QSize originalSize = openedImages[filesBox->currentIndex()]->getImgSize();
    int i = 0;
    for(i; ; i++)
    {
        lastLayerSize /= mult;
        if(mult > 1){
            if(floor(lastLayerSize.width()) == 0 ||
               floor(lastLayerSize.height()) == 0)
            {
                break;
            }
        }
        else if(mult < 1){
            if(lastLayerSize.width() > originalSize.width() &&
                    lastLayerSize.height() > originalSize.height())
            {
                break;
            }
        }
        if((lastLayerSize / mult) == lastLayerSize)
        {
            break;
        }
    }
    recommendTip->setText("Recommended number of layers: " + QString::number(i));
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
    filesBox->blockSignals(true);
    // Метод очистки комбобокса дважды посылает сигнал, из-за чего слот updateStats получает невалидные значения и программа крашится
    // Поэтому на время очистки отключаем отсылку сигналов данным виджетом.
    filesBox->clear();
    filesBox->blockSignals(false);
    // Очистка и перезаполнение бокса
    for (int i = 0; i < openedImages.size(); i++)
    {
        int pos = openedImages[i]->getPath().lastIndexOf("/");
        if(pos != -1)
        {
            filesBox->addItem(openedImages[i]->getPath().remove(0, pos + 1) + "   " + openedImages[i]->getImgSizeTip());
        }
        else
        {
            filesBox->addItem(openedImages[i]->getPath() + "   " + openedImages[i]->getImgSizeTip());
        }
    }
    filesBox->setCurrentIndex(lastId);
}

void pyramid::updateLayersBox()
{
    layersBox->blockSignals(true);
    layersBox->clear();
    layersBox->blockSignals(false);
    int id = filesBox->currentIndex();
    for(int i = 0; i < openedImages[id]->getVectorSize(); i++)
    {
        layersBox->addItem(openedImages[id]->getLayerName(i));
    }
}

bool pyramid::startLayersCreation()
{
    QString tipText = recommendTip->text();
    int tipNum = tipText.remove(0, tipText.indexOf(QRegExp("[0-9]"), 0)).toInt();
    if(filesBox->count() == 0)
    {
        QMessageBox cantCreate;
        cantCreate.setWindowTitle("Layer creation");
        cantCreate.setText("No images loaded.");
        cantCreate.setIcon(QMessageBox::Critical);
        cantCreate.exec();
        return false;
    }
    else if(tipNum < layersAmount->value())
    {
        QMessageBox::StandardButton conf;
        conf = QMessageBox::question(0,
                                     "Layer creation",
                                     "The number of layers that you want to create is greater than recommended number. Program can crash. Continue?",
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::No);
                if(conf == QMessageBox::No)
                {
                    return true;
                }
    }
    if(openedImages[filesBox->currentIndex()]->createLayers(layersAmount->value(), multiplier->value()))
    {
        this->updateLayersBox();
        recommendTip->setText("Recommended number of layers: " + QString::number(tipNum < layersAmount->value() ?
                                                                                     0 :
                                                                                     tipNum - layersAmount->value()));
        QMessageBox createSuccess;
        createSuccess.setWindowTitle("Layer creation");
        createSuccess.setText(QString::number(layersAmount->value()) + " layers created succesfully!");
        createSuccess.exec();
        return true;
    }


    return true;
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
    this->setSizeTip(openedImages[id]->getImgSizeTip());
    img->setBrush(imageWdg->backgroundRole(), QBrush(*(openedImages[id]->getImage(0))));
    imageWdg->resize(openedImages[id]->getImgSize());
    imageWdg->setPalette(*img);
    this->updateLayersBox();
}

void pyramid::updateLayers(int id)
{
    static QPixmap *generatedImage;
    if(generatedImage != nullptr) delete generatedImage;
    generatedImage = new QPixmap(*(openedImages[filesBox->currentIndex()]->getImage(0)));
    QSizeF multipliedSize = openedImages[filesBox->currentIndex()]->getLayerSize(id);
    QSize originalSize = openedImages[filesBox->currentIndex()]->getImgSize();
    qDebug() << "Original size: " << originalSize;
    *generatedImage = generatedImage->scaled(multipliedSize.toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    qDebug() << "Size before: " << generatedImage->size();
    *generatedImage = generatedImage->scaled(originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    qDebug() << "Size after: " << generatedImage->size();
    qDebug() << "Multiplier: " << static_cast<double>(openedImages[filesBox->currentIndex()]->getMult(id));
    this->calculateRecommend(multiplier->value());
    img->setBrush(imageWdg->backgroundRole(), QBrush(*generatedImage));

    imageWdg->setPalette(*img);
}
