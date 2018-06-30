#ifndef PYRAMID_H
#define PYRAMID_H

#include <QMainWindow>
#include <QObject>
#include <QPixmap>
#include <QLabel>
#include <QVBoxLayout>
#include <QVector>

class pyramid : public QMainWindow
{
    Q_OBJECT
public:
    explicit pyramid(QWidget *parent = nullptr);

private:
    QVector<QPixmap*> openedImages;
    QLabel *imageLab;                       void *createImage();
                                            void createMenu();
                                            void createAll();
private slots:
    bool openImage();
};

#endif // PYRAMID_H
