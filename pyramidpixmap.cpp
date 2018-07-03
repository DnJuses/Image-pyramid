#include "pyramidpixmap.h"
#include <QtMath>

PyramidPixmap::PyramidPixmap(QString imagePath)
{
    QPixmap *image = new QPixmap;
    loaded = false;
    path = imagePath;
    if(image->load(imagePath))
    {
        loaded = true;
        imageLayers.push_back(Layer{image, "Original", image->size(), 1});
        // Находим диагональ изображения по теореме Пифагора.
        diag = round(sqrt(pow(image->width(), 2) + pow(image->height(), 2)));
    }
}

PyramidPixmap::PyramidPixmap(const PyramidPixmap &copy)
{
    this->path = copy.path;
    this->diag = copy.diag;
    this->loaded = copy.loaded;
    this->imageLayers = copy.imageLayers;
}

// Получение переменной loaded.
bool PyramidPixmap::isLoaded()
{
    return loaded;
}

// Получение переменной imageSize.
QSize PyramidPixmap::getImgSize()
{
    return imageLayers[0].layerSize.toSize();
}

// Получение переменной layerSize из структуры Layer.
QSizeF PyramidPixmap::getLayerSize(int i)
{
    return imageLayers[i].layerSize;
}

// Получение переменной path.
QString PyramidPixmap::getPath()
{
    return path;
}

// Получение переменной diag.
double PyramidPixmap::getDiag()
{
    return diag;
}

// Получение переменной layerImage из структуры Layer.
QPixmap* PyramidPixmap::getImage(int i)
{
    return imageLayers[i].layerImage;
}

// Получение переменной multiplication из структуры Layer.
long double PyramidPixmap::getMult(int i)
{
    return imageLayers[i].multiplication;
}

// Получение переменной layerName из структуры Layer.
QString PyramidPixmap::getLayerName(int i)
{
    return imageLayers[i].layerName;
}

// Получение размера вектора imageLayers.
int PyramidPixmap::getVectorSize()
{
    return imageLayers.size();
}

// Функция создает указатели на изображения и данные о множителе, размере и имени
// Позже указатели будут в слоте из класса pyramid updateLayers преобразовываться в обычные изображения.
// В результате такого подхода существенно экономится память, а также увеличивается скорость генерации слоев.
// Из минусов можно выделить только более долгое время получения доступа к слою (мгновенно->0.5 сек).
bool PyramidPixmap::createLayers(int amount, double multiplier)
{
    for(int i = 0; i < amount; i++)
    {
        QPixmap *redactImage = imageLayers[0].layerImage;
        imageLayers.push_back(Layer{redactImage, QString::number(imageLayers.size()), imageLayers[imageLayers.size() - 1].layerSize / multiplier, imageLayers[imageLayers.size() - 1].multiplication * multiplier});
    }
    return true;
}

// Получение переменной imageSize, переведенной в QString для более простого использования в QLabel.
QString PyramidPixmap::getImgSizeTip(int i)
{
    return QString::number(imageLayers[i].layerSize.toSize().width()) + "x" + QString::number(imageLayers[i].layerSize.toSize().height());
}
