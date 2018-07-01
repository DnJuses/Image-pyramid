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
        imageLayers.push_back(Layer{image, "Original", image.size()});
        // Находим диагональ изображения по теореме Пифагора.
        diag = round(sqrt(pow(image.width(), 2) + pow(image.height(), 2)));
    }
}

PyramidPixmap::PyramidPixmap(const PyramidPixmap &copy)
{
    this->path = copy.path;
    this->diag = copy.diag;
    this->loaded = copy.loaded;
    this->imageLayers = copy.imageLayers;
}

bool PyramidPixmap::isLoaded()
{
    return loaded;
}

QSize PyramidPixmap::getImgSize()
{
    return imageLayers[0].layerSize;
}

QSize PyramidPixmap::getLayerSize(int i)
{
    return imageLayers[i].layerSize;
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

bool PyramidPixmap::createLayers(int amount, double multiplier)
{
    for(int i = 0; i < amount; i++)
    {
        QPixmap redactImage = imageLayers[imageLayers.size() - 1].layerImage;
        redactImage = redactImage.scaled(QSize(redactImage.width() / multiplier, redactImage.height() / multiplier), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        redactImage = redactImage.scaled(QSize(redactImage.width() * multiplier, redactImage.height() * multiplier), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        imageLayers.push_back(Layer{redactImage, QString::number(imageLayers.size()), redactImage.size()});
    }
    return true;
}

QString PyramidPixmap::getImgSizeTip()
{
    return QString::number(imageLayers[0].layerImage.width()) + "x" + QString::number(imageLayers[0].layerImage.height());
}
