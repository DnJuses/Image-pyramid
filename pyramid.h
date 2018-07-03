#ifndef PYRAMID_H
#define PYRAMID_H

#include <QMainWindow>
#include <QObject>
#include <QPixmap>
#include <QScrollArea>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QSize>
#include <QPalette>
#include <QHBoxLayout>
#include <QVector>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include "pyramidpixmap.h"

class pyramid : public QMainWindow
{
    Q_OBJECT
public:  
    explicit pyramid(QWidget *parent = nullptr);
    // Используется только при консольном вводе.
    // Функция открывает файлы с поступившего в него в качестве аргумента fileList, попутно проверяя пути на валидность.
    // После успешного открытия, создает указанное в amountList количество слоев с множителем в multiplierList.
    void openImagesFromList(QStringList fileList, QStringList amountList, QStringList multiplierList);
private:
    int mode;

    QVector<PyramidPixmap*> openedImages;
    QWidget *centralWidget;                 QWidget* createCentral();
    QVBoxLayout *centralLayout;             QVBoxLayout* createCLayout();
    QWidget *imageWdg; QPalette *img;       QWidget* createImage();
    QScrollArea *imageScroll;               QScrollArea* createScroll();
    QComboBox *filesBox;                    QComboBox* createFilesBox();
    QComboBox *layersBox;                   QComboBox* createLayersBox();
    QHBoxLayout *boxLayout;                 QHBoxLayout* createBoxLay();
    QLabel *fileTip, *layerTip, *sizeTip; // Создаются в createBoxLay();
    QPushButton *spawnLayers;               QPushButton* createSpawnerButton();
    QDoubleSpinBox *multiplier;             QDoubleSpinBox* createMultiplier();
    QSpinBox *layersAmount;                 QSpinBox* createLayersAmount();
    QHBoxLayout *lowerEnd;                  QHBoxLayout* createLowerEnd();
    QLabel *multiplierTip, *amountTip; // Создаются в createLowerEnd();
    QLabel *recommendTip;                    QLabel* createRecommendTip();
                                            void createMenu();
                                            void createAll();

    // Функция устанавливает размер изображения в виде QString в label sizeTip.
    void setSizeTip(QString imageSize);
    // Функция проверяет открываемые файлы на дупликаты.
    // Если дупликат найден - оповещает об этом пользователя и дает ему выбор - открывать файл или не открывать его.
    bool isDuplicate(QString checkPath);
    // Сортирует файлы в ComboBox'е filesBox.
    // Тип сортировки - сортировка вставками с перебором каждого элемента.
    void sortAndRefill();
    // Слот обновляет содержимое ComboBox'а layersBox, выводя новые изменения.
    void updateLayersBox();
private slots:
    // Слот подсчитывает максимально возможное к созданию количество слоев.
    void calculateRecommend(double mult);
    // Слот отвечает за создание слоев изображения.
    // Создает количество слоев, зависящее от значения в SpinBox'е layersAmount
    // Каждый последующий слой пирамиды уменьшается на значение из DoubleSpinBox'а multiplier
    // После уменьшения изображения оно вновь растягивается до размеров оригинального изображения из-за чего оно размыливается.
    bool startLayersCreation();
    void transformByMode(int mode, QPixmap *image, int id);
    void switchViewMode();
    // Единственное действие, которое используется в верхнем меню.
    // Слот вызывает файловый менеджер для открытия файлов с изображениями.
    // После открытия - проверяет пути на валидность и добавляет файл в ComboBox filesBox, попутно его отсортировав и проверив на дупликаты.
    // В случае неудачи - оповещает пользователя о невозможности открытия файла.
    bool openImage();
    // Слот обновляет imageWdg и sizeTip после смены файла.
    void updateStats(int id);
    // Слот обновляет imageWdg и sizeTip после смены слоя.
    void updateLayers(int id);
};

#endif // PYRAMID_H
