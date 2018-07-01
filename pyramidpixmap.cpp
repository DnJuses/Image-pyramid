#include "pyramidpixmap.h"
#include <QtMath>

PyramidPixmap::PyramidPixmap(QString imagePath)
{
    QPixmap image;
    loaded = false;
    path = imagePath;
    if(image.load(imagePath))
    {
        loaded = true;
        imageLayers.push_back(Layer{image, "Original"});
        imageSize = image.size();
        // Находим диагональ изображения по теореме Пифагора.
        diag = round(sqrt(pow(imageSize.width(), 2) + pow(imageSize.height(), 2)));
    }
}

PyramidPixmap::PyramidPixmap(const PyramidPixmap &copy)
{
    this->path = copy.path;
    this->diag = copy.diag;
    this->loaded = copy.loaded;
    this->imageSize = copy.imageSize;
    this->imageLayers = copy.imageLayers;
}

bool PyramidPixmap::isLoaded()
{
    return loaded;
}

QSize PyramidPixmap::getImgSize()
{
    return imageLayers[0].layerImage.size();
}

QString PyramidPixmap::getPath()
{
    return path;
}

double PyramidPixmap::getDiag()
{
    return diag;
}

QPixmap PyramidPixmap::getImage(int i)
{
    return imageLayers[i].layerImage;
}

QString PyramidPixmap::getLayerName(int i)
{
    return imageLayers[i].layerName;
}

int PyramidPixmap::getVectorSize()
{
    return imageLayers.size();
}

QString PyramidPixmap::getImgSizeTip()
{
    return QString::number(imageSize.width()) + "x" + QString::number(imageSize.height());
}
