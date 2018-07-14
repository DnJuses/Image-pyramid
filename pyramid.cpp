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
    this->setFixedSize(518, 594); // Оптимальный размер основного окна
    this->createAll();
    /* Все connect'ы приложения и их расположение (функция:строка):
     * connect(filesBox,  static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &pyramid::updateStats); (createFilesBox:76)
     * connect(layersBox,  static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &pyramid::updateLayers); (createLayersBox:86)
     * connect(spawnLayers, &QPushButton::clicked, this, &pyramid::startLayersCreation); (createSpawnerButton:115)
     * connect(multiplier, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &pyramid::calculateRecommend); (createMultiplier:128)
     * connect(openFile, &QAction::triggered, this, &pyramid::openImage); (createMenu:177)
     * connect(switchMode, &QAction::triggered, this, &pyramid::switchViewMode); (createMenu:183)
     */
}

pyramid::~pyramid()
{
    disconnect(filesBox,  static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &pyramid::updateStats);
    disconnect(layersBox,  static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &pyramid::updateLayers);
    disconnect(spawnLayers, &QPushButton::clicked, this, &pyramid::startLayersCreation);
    disconnect(multiplier, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &pyramid::calculateRecommend);
    disconnect(openFile, &QAction::triggered, this, &pyramid::openImage);
    disconnect(switchMode, &QAction::triggered, this, &pyramid::switchViewMode);
    for(QVector<PyramidPixmap*>::iterator i = openedImages.begin(); i != openedImages.end(); i++)
    {
        delete *i;
    }
    this->deleteLater();
}

// Создает виджет, который будет содержать в себе все layout'ы и будет центральным виджетом основного окна.
QWidget *pyramid::createCentral()
{
    centralWidget = new QWidget;
    centralWidget->setLayout(this->createCLayout());
    return centralWidget;
}

// Создает центральный layout, который содержит в себе все виджеты и другие layout'ы программы.
// Крепится к centralWidget
QVBoxLayout *pyramid::createCLayout()
{
    centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->addLayout(this->createBoxLay());
    centralLayout->addWidget(this->createScroll());
    centralLayout->addLayout(this->createLowerEnd());
    return centralLayout;
}

// Создает виджет, отвечающий за отображение изображений.
// Виджет всегда подстраивается под размер изображения.
// Встроен в imageScroll, чтобы его можно было прокручивать.
QWidget *pyramid::createImage()
{
    imageWdg = new QWidget;
    img = new QPalette;
    imageWdg->setAutoFillBackground(true);
    return imageWdg;
}

// Создает область прокрутки 500x500.
// Прокручивает imageWdg.
QScrollArea *pyramid::createScroll()
{
    QScrollArea *imageScroll = new QScrollArea;
    imageScroll->setFixedSize(500, 500);
    imageScroll->setAlignment(Qt::AlignCenter);
    imageScroll->setWidget(this->createImage());
    return imageScroll;
}

// Создает ComboBox, который хранит в себе все открытые файлы.
// Получает файлы из вектора openedImages. Обновляется каждый раз при открытие нового файла.
// Хранит файлы отсортированными по увеличению диагонали изображения.
// Располагается в layout'е boxLayout.
QComboBox *pyramid::createFilesBox()
{
    filesBox = new QComboBox;
    filesBox->setMinimumWidth(135);
    connect(filesBox, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &pyramid::updateStats);
    return filesBox;
}

// Создает ComboBox, который хранит в себе все слои изображения.
// Получает слои из вектора imageLayers, который находится в классе PyramidPixmap. Доступ осуществляется через вектор openedImages.
// Располагается в layout'е boxLayout.
QComboBox *pyramid::createLayersBox()
{
    layersBox = new QComboBox();
    connect(layersBox,  static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &pyramid::updateLayers);
    return layersBox;
}

// Создает верхний layout, где располагаются все ComboBox'ы и подсказки к ним.
// Входит в состав центрального layout'а.
QHBoxLayout *pyramid::createBoxLay()
{
    fileTip = new QLabel(tr("File:"));
    layerTip = new QLabel(tr("Layer:"));
    sizeTip = new QLabel(tr("Size: 0x0"));
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

// Создает кнопку, которая отправляет запрос на создание слоев.
// За создание слоев отвечает слот startLayersCreation().
// Располагается в layout'е lowerEnd.
QPushButton *pyramid::createSpawnerButton()
{
    spawnLayers = new QPushButton(tr("Create layers"));
    connect(spawnLayers, &QPushButton::clicked, this, &pyramid::startLayersCreation);
    return spawnLayers;
}

// Создает DoubleSpinBox, который устанавливает коэффициент для уменьшения изображения.
// Располагается в layout'е lowerEnd.
QDoubleSpinBox *pyramid::createMultiplier()
{
    multiplier = new QDoubleSpinBox;
    multiplier->setValue(2.0);
    multiplier->setSingleStep(0.01);
    multiplier->setMinimum(0.01);
    multiplier->setMaximum(99);
    connect(multiplier, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, &pyramid::calculateRecommend);
    return multiplier;
}

// Создает SpinBox, который устанавливает количество создаваемых слоев.
// Располагается в layout'е lowerEnd.
QSpinBox *pyramid::createLayersAmount()
{
    layersAmount = new QSpinBox;
    layersAmount->setMinimum(1);
    layersAmount->setMaximum(99);
    layersAmount->setValue(3);
    return layersAmount;
}

// Создает Label, подсказывающий максимальное количество создаваемых с текущим коэффициентом слоев.
// Располагается в layout'е lowerEnd.
QLabel *pyramid::createRecommendTip()
{
    recommendTip = new QLabel("Max layers possible: 0");
    return recommendTip;
}

// Создает нижний layout, где располагаются кнопка создания слоев, все спинбоксы и подсказки к ним.
// Также содержит в себе label подсказывающий максимальное количество создаваемых с текущим коэффициентом слоев.
QHBoxLayout *pyramid::createLowerEnd()
{
    multiplierTip = new QLabel(tr("Multiplier:"));
    amountTip = new QLabel(tr("Layers:"));
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

// Создает меню-бар со следующими вкладками:
// File - содержит в себе действие по открытию изображений.
// View - содержит в себе действие по смене режима отображения.
void pyramid::createMenu()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    openFile = new QAction(tr("&Open image"), this);
    openFile->setShortcuts(QKeySequence::Open);
    connect(openFile, &QAction::triggered, this, &pyramid::openImage);
    fileMenu->addAction(openFile);
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    switchMode = new QAction(tr("&Switch view mode"));
    switchMode->setCheckable(true);
    switchMode->setChecked(0);
    connect(switchMode, &QAction::triggered, this, &pyramid::switchViewMode);
    viewMenu->addAction(switchMode);
}

// Создает весь интерфейс изображения.
void pyramid::createAll()
{
    this->createMenu();
    this->setCentralWidget(createCentral());
}

// Функция устанавливает размер изображения в виде QString в label sizeTip.
void pyramid::setSizeTip(const QString &imageSize)
{
    sizeTip->setText("Size: " + imageSize);
}

// Слот подсчитывает максимально возможное к созданию количество слоев.
void pyramid::calculateRecommend(double mult)  // СЛОТ |==================================================================|
{
    if(filesBox->count() == 0 || mult == 0) return; // Нет открытых изображений или множитель - это 0
    // Получение размеров последнего слоя текущего изображения.
    QSizeF lastLayerSize = openedImages[filesBox->currentIndex()]->getLayerSize(layersBox->count() - 1);
    // Получение размеров текущего изображения.
    const QSize originalSize = openedImages[filesBox->currentIndex()]->getImgSize();
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
    recommendTip->setText("Max layers possible: " + QString::number(i));
}

// Функция проверяет открываемые файлы на дупликаты.
// Если дупликат найден - оповещает об этом пользователя и дает ему выбор - открывать файл или не открывать его.
bool pyramid::isDuplicate(const QFileInfo &checkPath)
{
    for(int i = 0; i < openedImages.size(); i++)
    {
        const QFileInfo dupePath(openedImages[i]->getPath());
        // От полного пути файла оставляем только название файла и его формат. Потом сравниваем.
        if(checkPath.fileName() == dupePath.fileName())
        {
            return true;
        }
    }
    return false;
}

// Сортирует файлы в ComboBox'е filesBox.
// Тип сортировки - сортировка вставками с перебором каждого элемента.
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
    // Очистка и перезаполнение бокса
    filesBox->blockSignals(true);
    // Метод очистки комбобокса дважды посылает сигнал, из-за чего слот updateStats получает невалидные значения (а именно -1 и 0) и программа крашится.
    // Поэтому на время очистки отключаем отсылку сигналов данным виджетом.
    filesBox->clear();
    filesBox->blockSignals(false);
    for (int i = 0; i < openedImages.size(); i++)
    {
        // Обрезаем полный путь файла, оставляя только его имя, формат и размер в пикселах.
        const QString imgFileName = QFileInfo(openedImages[i]->getPath()).fileName();
        filesBox->addItem(imgFileName + "   " + openedImages[i]->getImgSizeTip(0));
    }
    // Устанавливаем к просмотру только что открытый файл.
    filesBox->setCurrentIndex(lastId);
}

// Функция обновляет содержимое ComboBox'а layersBox, выводя новые изменения.
void pyramid::updateLayersBox()
{
    layersBox->blockSignals(true);
    layersBox->clear();
    layersBox->blockSignals(false);
    int id = filesBox->currentIndex();
    for(int i = 0; i < openedImages[id]->getVectorSize(); i++)
    {
        layersBox->addItem(openedImages[id]->getLayerName(i) + "                                                  " + openedImages[id]->getImgSizeTip(i));
    }
    this->calculateRecommend(multiplier->value());
}

// Используется только при консольном вводе.
// Функция открывает файлы с поступившего в него в качестве аргумента fileList, попутно проверяя пути на валидность.
// После успешного открытия, создает указанное в amountList количество слоев с множителем в multiplierList в режиме switcher.
void pyramid::openImagesFromList(const QStringList &fileList, const QStringList &amountList, const QStringList &multiplierList, bool switcher)
{
    for(int i = 0; i < fileList.length(); i++)
    {

       const QString filePath = fileList.at(i);
       const int createAmount = QString(amountList.at(i)).toInt();
       const double multi = QString(multiplierList.at(i)).toDouble();
       // Проверка пути на валидность.
       if(filePath.endsWith(".png") || filePath.endsWith(".jpg"))
       {
           PyramidPixmap *newImage = new PyramidPixmap(filePath);
           // Дальнейшая проверка пути на валидность.
           if(newImage->isLoaded())
           {
               openedImages.push_back(newImage);
               this->sortAndRefill();
               if(openedImages.size() >= 1 && switcher)
               {
                   switchMode->setChecked(1);
                   this->switchViewMode(1);
               }
               layersAmount->setValue(createAmount);
               multiplier->setValue(multi);
               startLayersCreation();
           }
           else
           {
               delete newImage;
           }
       }
    }
    // Вывести статистику по файлам, которые не удалось открыть.
    QMessageBox corrupted;
    corrupted.setWindowTitle("File opening");
    corrupted.setText("Invalid filepaths: " + QString::number(fileList.length() - filesBox->count()));
    corrupted.setIcon(QMessageBox::Warning);
    corrupted.exec();
}

// Слот отвечает за создание слоев изображения.
// Создает количество слоев, зависящее от значения в SpinBox'е layersAmount
// Каждый последующий слой пирамиды уменьшается на значение из DoubleSpinBox'а multiplier
// После уменьшения изображения оно вновь растягивается до размеров оригинального изображения из-за чего оно размыливается.
bool pyramid::startLayersCreation() // СЛОТ |==================================================================|
{
    // Получение максимального возможного к созданию кол-ва слоев в отдельную int переменную.
    QString tipText = recommendTip->text();
    int tipNum = tipText.remove(0, tipText.indexOf(QRegExp("[0-9]"), 0)).toInt();
    // В данный момент нет загруженных изображений и невозможно создать слои.
    if(filesBox->count() == 0)
    {
        QMessageBox cantCreate;
        cantCreate.setWindowTitle("Layer creation");
        cantCreate.setText("No images loaded.");
        cantCreate.setIcon(QMessageBox::Critical);
        cantCreate.exec();
        return false;
    }
    // Предупреждение пользователя о возможных последствиях, если он решит превысить лимит на создание слоев.
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
                    return false;
                }
    }
    // Создаем слои функцией createLayers из класса PyramidPixmap.
    openedImages[filesBox->currentIndex()]->createLayers(layersAmount->value(), multiplier->value());
    // Обновляем бокс со слоями.
    this->updateLayersBox();
    recommendTip->setText("Max layers possible: " + QString::number(tipNum < layersAmount->value() ?
                                                                                 0 :
                                                                                 tipNum - layersAmount->value()));
    QMessageBox createSuccess;
    createSuccess.setWindowTitle("Layer creation");
    createSuccess.setText(QString::number(layersAmount->value()) + " layers created succesfully!");
    createSuccess.exec();
    return true;
}

// Функция создает слои в зависимости от режима, в котором сейчас находится программа.
// 1-ый - изменяет размер изображения, затем возвращает его в оригинальное состояние. Получается эффект "размыливания"
// 2-ой - изменяет размер изображения, делая его меньше или больше оригинала.
void pyramid::transformByMode(bool mode, QPixmap *image, size_t id)
{
    const QSizeF multipliedSize = openedImages[filesBox->currentIndex()]->getLayerSize(id);
    const QSize originalSize = openedImages[filesBox->currentIndex()]->getImgSize();
    if(mode == 0)
    {
        *image = image->scaled(multipliedSize.toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        *image = image->scaled(originalSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else
    {
        *image = image->scaled(multipliedSize.toSize(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
        imageWdg->resize(multipliedSize.toSize());
    }
}

// Действие, которое используется в меню-баре
// Слот переключает режим отображения программы.
// За переключение отвечает действие switchMode.
void pyramid::switchViewMode(bool mode) // СЛОТ |=====================================================|
{
    if(filesBox->count() > 0)
    {
        for(int i = 0; i < filesBox->count(); i++)
        {
            openedImages[i]->setOpVector(mode);
        }
        imageWdg->resize(openedImages[filesBox->currentIndex()]->getImgSize());
        this->updateLayersBox();
    }
    else
    {
        switchMode->setChecked(0);
    }
}

// Действие, которое используется в меню-баре.
// Слот вызывает файловый менеджер для открытия файлов с изображениями.
// После открытия - проверяет путь на валидность и добавляет файл в ComboBox filesBox, попутно его отсортировав и проверив на дупликаты.
// В случае неудачи - оповещает пользователя о невозможности открытия файла.
bool pyramid::openImage() // СЛОТ |==================================================================|
{
    const QString openFilePath = QFileDialog::getOpenFileName(nullptr, nullptr, nullptr, tr("Any files (*.*);;Image files (*.jpg *.png)"));
    if(openFilePath == "" || openFilePath == NULL) return true; // Пользователь закрыл файловый диалог и не выбрал файл.
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
    // Проверка на дупликаты.
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
    // Дальнейшая проверка на валидность.
    if(newImage->isLoaded())
    {
        // Тест на валидность пройден - помещаем изображение в ComboBox и сортируем его.
        newImage->setOpVector(switchMode->isChecked());
        openedImages.push_back(newImage);
        this->sortAndRefill();
        return true;
    }
    else
    {
        // Файл не прошел проверку на валидность.
        delete newImage;
    }
    // Оповещаем пользователя о невозможности открытия файла.
    QMessageBox cantOpen;
    cantOpen.setWindowTitle("File opening");
    cantOpen.setText("Cannot open an image.");
    cantOpen.setIcon(QMessageBox::Critical);
    cantOpen.exec();
    return false;
}

// Слот обновляет imageWdg и sizeTip после смены файла.
void pyramid::updateStats(size_t id) // СЛОТ |==================================================================|
{
    this->setSizeTip(openedImages[id]->getImgSizeTip(0));
    img->setBrush(imageWdg->backgroundRole(), QBrush(*(openedImages[id]->getImage(0))));
    imageWdg->resize(openedImages[id]->getImgSize());
    imageWdg->setPalette(*img);
    this->updateLayersBox();
}

// Слот обновляет imageWdg и sizeTip после смены слоя.
void pyramid::updateLayers(size_t id) // СЛОТ |==================================================================|
{
    static QPixmap *generatedImage;
    if(generatedImage != nullptr) delete generatedImage;
    generatedImage = new QPixmap(*(openedImages[filesBox->currentIndex()]->getImage(0)));
    this->transformByMode(switchMode->isChecked(), generatedImage, id);
    this->setSizeTip(openedImages[filesBox->currentIndex()]->getImgSizeTip(id));
    this->calculateRecommend(multiplier->value());
    img->setBrush(imageWdg->backgroundRole(), QBrush(*generatedImage));

    imageWdg->setPalette(*img);
}
