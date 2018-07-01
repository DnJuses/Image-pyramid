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

private:
    QVector<PyramidPixmap*> openedImages;
    QWidget *centralWidget;                 QWidget* createCentral();
    QVBoxLayout *centralLayout;             QVBoxLayout* createCLayout();
    QWidget *imageWdg; QPalette *img;       QWidget* createImage();
    QScrollArea *imageScroll;               QScrollArea* createScroll();
    QComboBox *filesBox;                    QComboBox* createFilesBox();
    QComboBox *layersBox;                   QComboBox* createLayersBox();
    QHBoxLayout *boxLayout;                 QHBoxLayout* createBoxLay();
    QLabel *fileTip, *layerTip, *sizeTip; // Создаются в createBoxLay;
    QPushButton *spawnLayers;               QPushButton* createSpawnerButton();
    QDoubleSpinBox *multiplier;             QDoubleSpinBox* createMultiplier();
    QSpinBox *layersAmount;                 QSpinBox* createLayersAmount();
    QHBoxLayout *lowerEnd;                  QHBoxLayout* createLowerEnd();
    QLabel *multiplierTip, *amountTip; // Создаются в createLowerEnd;
                                            void createMenu();
                                            void createAll();


    void setSizeTip(QString imageSize);
    bool isDuplicate(QString checkPath);
    void sortAndRefill();
private slots:
    bool openImage();
    void updateStats(int id);
};

#endif // PYRAMID_H
