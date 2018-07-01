#ifndef PYRAMIDPIXMAP_H
#define PYRAMIDPIXMAP_H
#include <QPixmap>
#include <QVector>
#include <QString>

struct Layer
{
    QPixmap layerImage;
    QString layerName;
    QSize layerSize;
};

class PyramidPixmap
{
public:
    PyramidPixmap(QString imagePath);
    PyramidPixmap(const PyramidPixmap &copy);
    bool isLoaded(); // Получение переменной loaded.
    QString getImgSizeTip(); // Получение переменной imageSize, переведенной в QString для более простого использования в QLabel.
    QSize getImgSize(); // Получение переменной imageSize.
    QSize getLayerSize(int i); // Получение переменной layerSize из структуры Layer.
    QString getPath(); // Получение переменной path.
    double getDiag(); // Получение переменной diag.
    QPixmap getImage(int i); // Получение переменной layerImage из структуры Layer.
    QString getLayerName(int i); // Получение переменной layerName из структуры Layer.
    int getVectorSize(); // Получение размера вектора imageLayers.
    bool createLayers(int amount, double multiplier);
private:
    QVector<Layer> imageLayers; // Хранит слой с изображением и его название с размером.
    QString path; // Хранит путь к изображению.
    double diag; // Хранит диагональ изображения.
    bool loaded; // True - изображение успешно загружено и готово к использованию. Иначе false.
};
#endif // PYRAMIDPIXMAP_H
