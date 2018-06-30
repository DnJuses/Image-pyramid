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
    QComboBox *files;                       QComboBox* createFilesBox();
    QLabel *fileTip, *layerTip, *sizeTip; // Создаются в createFilesBox;
    QComboBox *layers;                      QComboBox* createLayersBox();
    QHBoxLayout *boxLayout;                 QHBoxLayout* createBoxLay();
    QPushButton *spawnLayers;               QHBoxLayout* createSpawner();
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
