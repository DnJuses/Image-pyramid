#include "pyramidpixmap.h"
#include <QtMath>

PyramidPixmap::PyramidPixmap(const QString &imagePath)
{
    QPixmap *image = new QPixmap;
    loaded = false;
    path = imagePath;
    if(image->load(imagePath))
    {
        loaded = true;
        /* Инициализация структуры Layer:
         * 1. Указатель на QPixmap - изображение слоя (Первый слой всегда оригинальное изображение).
         * 2. QString с названием слоя (Первый слой всегда Original).
         * 3. QSizeF - размер слоя в пикселах (1 - Ширина, 2 - Длина).
         * 4. Коэффициент изменения изображения (Первый слой всегда имеет коэф. = 1)
         */
        originalPyramidLayers.push_back(Layer{image,
                                              "Original",
                                              image->size(),
                                              1});
        imageLayers.push_back(Layer{image,
                                    "Original",
                                    image->size(),
                                    1});
        this->setOpVector(0);
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
bool PyramidPixmap::isLoaded() const
{
    return loaded;
}

// Получение переменной imageSize, переведенной в QString для более простого использования в QLabel.
QString PyramidPixmap::getImgSizeTip(size_t i) const
{
    return QString::number((*manipulatingVector)[i].layerSize.toSize().width()) + "x" + QString::number((*manipulatingVector)[i].layerSize.toSize().height());
}

// Получение переменной imageSize.
QSize PyramidPixmap::getImgSize() const
{
    return (*manipulatingVector)[0].layerSize.toSize();
}

// Получение переменной layerSize из структуры Layer.
QSizeF PyramidPixmap::getLayerSize(size_t i) const
{
    return (*manipulatingVector)[i].layerSize;
}

// Получение переменной path.
QString PyramidPixmap::getPath() const
{
    return path;
}

// Получение переменной diag.
double PyramidPixmap::getDiag() const
{
    return diag;
}

// Получение переменной layerImage из структуры Layer.
QPixmap* PyramidPixmap::getImage(size_t i) const
{
    return (*manipulatingVector)[i].layerImage;
}

// Получение переменной multiplication из структуры Layer.
long double PyramidPixmap::getMult(size_t i) const
{
    return (*manipulatingVector)[i].multiplication;
}

// Получение переменной layerName из структуры Layer.
QString PyramidPixmap::getLayerName(size_t i) const
{
    return (*manipulatingVector)[i].layerName;
}

// Получение размера вектора imageLayers.
int PyramidPixmap::getVectorSize() const
{
    return manipulatingVector->size();
}

// Дает указателю на векторы новый вектор.
void PyramidPixmap::setOpVector(bool mode)
{
    if (mode == 0)
    {
        manipulatingVector = &imageLayers;
    }
    else if(mode == 1)
    {
        manipulatingVector = &originalPyramidLayers;
    }
}

// Функция создает указатели на изображения и данные о множителе, размере и имени
// Позже указатели будут в слоте updateLayers из класса pyramid преобразовываться в обычные изображения.
// В результате такого подхода существенно экономится память, а также увеличивается скорость генерации слоев.
// Из минусов можно выделить только более долгое время получения доступа к слою (мгновенно->0.5 сек).
void PyramidPixmap::createLayers(int amount, double multiplier)
{
    for(size_t i = 0; i < amount; i++)
    {
        QPixmap *redactImage = (*manipulatingVector)[0].layerImage;
        int manipulatingSize = (*manipulatingVector).size() - 1;
        (*manipulatingVector).push_back(Layer{redactImage,
                                              QString::number(manipulatingSize + 1),
                                              (*manipulatingVector)[manipulatingSize].layerSize / multiplier,
                                              (*manipulatingVector)[manipulatingSize].multiplication * multiplier});
    }
}
