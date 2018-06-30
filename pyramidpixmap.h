#ifndef PYRAMIDPIXMAP_H
#define PYRAMIDPIXMAP_H
#include <QPixmap>


class PyramidPixmap
{
public:
    PyramidPixmap(QString imagePath);
    PyramidPixmap(const PyramidPixmap &copy);
    bool isLoaded(); // Получение переменной loaded.
    QString getImgSizeTip(); // Получение переменной imageSize, переведенной в QString для более простого использования в QLabel.
    QSize getImgSize(); // Получение переменной imageSize.
    QString getPath(); // Получение переменной path.
    double getDiag(); // Получение переменной diag.
    QPixmap getImage(); // Получение переменной image.
private:
    QPixmap image; // Хранит само изображение.
    QSize imageSize; // Хранит размер изображения.
    QString path; // Хранит путь к изображению.
    double diag; // Хранит диагональ изображения.
    bool loaded; // True - изображение успешно загружено и готово к использованию. Иначе false.
};

#endif // PYRAMIDPIXMAP_H
