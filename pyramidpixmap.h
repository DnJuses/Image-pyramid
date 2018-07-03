#ifndef PYRAMIDPIXMAP_H
#define PYRAMIDPIXMAP_H
#include <QPixmap>
#include <QVector>
#include <QString>

struct Layer
{
    QPixmap *layerImage; // Изображение слоя.
    QString layerName; // Название слоя.
    QSizeF layerSize; // Размер слоя.
    long double multiplication; // Множитель, полученный путем умножения предыдущих множителей.
};

class PyramidPixmap
{
public:
    PyramidPixmap(QString imagePath);
    PyramidPixmap(const PyramidPixmap &copy);
    bool isLoaded(); // Получение переменной loaded.
    QString getImgSizeTip(int i); // Получение переменной imageSize, переведенной в QString для более простого использования в QLabel.
    QSize getImgSize(); // Получение переменной imageSize.
    QSizeF getLayerSize(int i); // Получение переменной layerSize из структуры Layer.
    QString getPath(); // Получение переменной path.
    double getDiag(); // Получение переменной diag.
    void setOpVector(int num); // Дает указателю на векторы новый вектор.
    QPixmap *getImage(int i); // Получение переменной layerImage из структуры Layer.
    long double getMult(int i); // Получение переменной multiplication из структуры Layer.
    QString getLayerName(int i); // Получение переменной layerName из структуры Layer.
    int getVectorSize(); // Получение размера вектора imageLayers.
    // Функция создает указатели на изображения и данные о множителе, размере и имени
    // Позже указатели будут в слоте из класса pyramid updateLayers преобразовываться в обычные изображения.
    // В результате такого подхода существенно экономится память, а также увеличивается скорость генерации слоев.
    // Из минусов можно выделить только более долгое время получения доступа к слою (мгновенно->0.5 сек).
    bool createLayers(int amount, double multiplier);
private:
    QVector<Layer> *manipulatingVector; // Указатель на векторы. С его помощью идет манипулирование двумя другими векторами.
    QVector<Layer> imageLayers; // Хранит слой с изображением и его название с размером.
    QVector<Layer> originalPyramidLayers; // Хранит оригинальные, уменьшаемые слои пирамиды.
    QString path; // Хранит путь к изображению.
    double diag; // Хранит диагональ изображения.
    bool loaded; // True - изображение успешно загружено и готово к использованию. Иначе false.
};
#endif // PYRAMIDPIXMAP_H
