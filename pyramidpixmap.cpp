#include "pyramidpixmap.h"
#include <QtMath>

PyramidPixmap::PyramidPixmap(QString imagePath)
{
    loaded = false;
    path = imagePath;
    if(image.load(imagePath))
    {
        loaded = true;
        imageSize = image.size();
        // Находим диагональ изображения по теореме Пифагора.
        diag = round(sqrt(pow(imageSize.width(), 2) + pow(imageSize.height(), 2)));
    }
}

PyramidPixmap::PyramidPixmap(const PyramidPixmap &copy)
{
    this->path = copy.path;
    this->diag = copy.diag;
    this->image = copy.image;
    this->loaded = copy.loaded;
    this->imageSize = copy.imageSize;
}

bool PyramidPixmap::isLoaded()
{
    return loaded;
}

QSize PyramidPixmap::getImgSize()
{
    return image.size();
}

QString PyramidPixmap::getPath()
{
    return path;
}

double PyramidPixmap::getDiag()
{
    return diag;
}

QPixmap PyramidPixmap::getImage()
{
    return image;
}

QString PyramidPixmap::getImgSizeTip()
{
    return QString::number(imageSize.width()) + "x" + QString::number(imageSize.height());
}
