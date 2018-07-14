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
    PyramidPixmap(const QString &imagePath);
    PyramidPixmap(const PyramidPixmap &copy);
    ~PyramidPixmap();
    bool isLoaded() const; // Получение переменной loaded.
    QString getImgSizeTip(size_t i) const; // Получение переменной imageSize, переведенной в QString для более простого использования в QLabel.
    QSize getImgSize() const; // Получение переменной imageSize.
    QSizeF getLayerSize(size_t i) const; // Получение переменной layerSize из структуры Layer.
    QString getPath() const; // Получение переменной path.
    double getDiag() const; // Получение переменной diag.
    QPixmap *getImage(size_t i) const; // Получение переменной layerImage из структуры Layer.
    long double getMult(size_t i) const; // Получение переменной multiplication из структуры Layer.
    QString getLayerName(size_t i) const; // Получение переменной layerName из структуры Layer.
    int getVectorSize() const; // Получение размера вектора imageLayers.
    void setOpVector(bool mode); // Дает указателю на векторы новый вектор.
    // Функция создает указатели на изображения и данные о множителе, размере и имени
    // Позже указатели будут в слоте updateLayers из класса pyramid преобразовываться в обычные изображения.
    // В результате такого подхода существенно экономится память, а также увеличивается скорость генерации слоев.
    // Из минусов можно выделить только более долгое время получения доступа к слою (мгновенно->0.5 сек).
    void createLayers(int amount, double multiplier);
private:
    QPixmap *originalImage;
    QVector<Layer> *manipulatingVector; // Указатель на векторы. С его помощью идет манипулирование двумя другими векторами.
    QVector<Layer> imageLayers; // Хранит слои с изображением и его название с размером 1-ого режима отображения.
    QVector<Layer> originalPyramidLayers; // Хранит слои с изображением и его название с размером 2-ого режима отображения.
    QString path; // Хранит полный путь к изображению.
    double diag; // Хранит диагональ изображения.
    bool loaded; // True - изображение успешно загружено и готово к использованию. Иначе false.
};
#endif // PYRAMIDPIXMAP_H
