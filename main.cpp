#include "pyramid.h"
#include <iostream>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QProcess>
#include <QStringList>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    pyramid w;
    w.setWindowTitle("Image pyramid");
    w.show();
    QCommandLineParser parser;
    parser.setApplicationDescription("");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", QCoreApplication::translate("main", "(String) Path to the image file."));
    parser.addPositionalArgument("amount", QCoreApplication::translate("main", "(Int) Amount of layers to create."));
    parser.addPositionalArgument("multiplier", QCoreApplication::translate("main", "(Float) Layers multiplier."));

    // Опция, с помощью которой передается путь к изображению.
    QCommandLineOption passFileOption(QStringList() << "f" << "file",
    QCoreApplication::translate("main", "Passes an image file <file> into program."),
    QCoreApplication::translate("main", "file"));
    parser.addOption(passFileOption);
    // Опция, с помощью которой передается количество создаваемых слоев.
    QCommandLineOption setAmountOption(QStringList() << "a" << "amount",
    QCoreApplication::translate("main", "Sets amount <amount> of layers needed to create."),
    QCoreApplication::translate("main", "amount"));
    parser.addOption(setAmountOption);
    // Опция, с помощью которой передается множитель уменьшения изображений.
    QCommandLineOption setMultiplierOption(QStringList() << "m" << "multiplier",
    QCoreApplication::translate("main", "Sets multiplier <multiplier> for layers."),
    QCoreApplication::translate("main", "multiplier"));
    parser.addOption(setMultiplierOption);


    parser.process(a);
    QStringList passedFiles = parser.values(passFileOption);
    QStringList passedAmounts = parser.values(setAmountOption);
    QStringList passedMultipliers = parser.values(setMultiplierOption);
    if(passedFiles.length() != 0)
    {
        // Проверка аргументов на валидность.
        {
           int id = 0; int invalidElems = 0;
           // Проверка аргументов <amount> на валидность.
           foreach(QString num, passedAmounts)
           {
               if(num.toInt() == 0)
               {
                   passedAmounts.erase(passedAmounts.begin() + id);
                   invalidElems++;
                   id--;
               }
               id++;
           }
           if(invalidElems > 0)
           {
               QMessageBox elemsDeleted;
               elemsDeleted.setWindowTitle("Argument passing");
               elemsDeleted.setText(QString::number(invalidElems) + " invalid <amount> arguments deleted.");
               elemsDeleted.setIcon(QMessageBox::Warning);
               elemsDeleted.exec();
           }
           id = 0;
           invalidElems = 0;
           // Проверка аргументов <multiplier> на валидность.
           foreach(QString num, passedMultipliers)
           {
               if(num.toDouble() == 0)
               {
                   passedMultipliers.erase(passedMultipliers.begin() + id);
                   id--;
                   invalidElems++;
               }
               id++;
           }
           if(invalidElems > 0)
           {
               QMessageBox elemsDeleted;
               elemsDeleted.setWindowTitle("Argument passing");
               elemsDeleted.setText(QString::number(invalidElems) + " invalid <multiplier> arguments deleted.");
               elemsDeleted.setIcon(QMessageBox::Warning);
               elemsDeleted.exec();
           }
        }
        // Проверка на количество переданных "количественных" аргументов
        if(passedAmounts.length() != passedFiles.length())
        {
            int aLen = passedAmounts.length(),
            fLen = passedFiles.length();
            // Аргументов меньше чем файлов - добавляем fLen - aLen стандартных аргументов
            // Стандартный аргумент - 3
            if(aLen < fLen)
            {
                QMessageBox NE;
                NE.setWindowTitle("Argument passing");
                NE.setText("Not enough <amount> arguments passed. Added " + QString::number(fLen - aLen) + " standart arguments");
                NE.setIcon(QMessageBox::Warning);
                NE.exec();
                for(int i = aLen; i < fLen; i++)
                {
                   passedAmounts.push_back("3");
                }
            }
            // Аргументов больше чем файлов - удаляем aLen - fLen последних аргументов
            else
            {
                QMessageBox tooMany;
                tooMany.setWindowTitle("Argument passing");
                tooMany.setText("Too many <amount> arguments passed. Removed " + QString::number(aLen - fLen) + " last arguments");
                tooMany.setIcon(QMessageBox::Warning);
                tooMany.exec();
                for(int i = 0; i < aLen - fLen; i++)
                {
                    passedAmounts.erase(passedAmounts.end() - 1);
                }
            }
        }
        // Проверка на количеcтво переданных "множительных" аргументов
        if(passedMultipliers.length() != passedFiles.length())
        {
            int mLen = passedMultipliers.length(),
                fLen = passedFiles.length();
            // Аргументов меньше чем файлов - добавляем fLen - mLen стандартных аргументов
            // Стандартный аргумент - 2
            if(mLen < fLen)
            {
                QMessageBox NE;
                NE.setWindowTitle("Argument passing");
                NE.setText("Not enough <multiplier> arguments passed. Added " + QString::number(fLen - mLen) + " standart arguments");
                NE.setIcon(QMessageBox::Warning);
                NE.exec();
                for(int i = mLen; i < fLen; i++)
                {
                    passedMultipliers.push_back("2");
                }
            }
            // Аргументов больше чем файлов - удаляем mLen - fLen последних аргументов
            else
            {
                QMessageBox tooMany;
                tooMany.setWindowTitle("Argument passing");
                tooMany.setText("Too many <multiplier> arguments passed. Removed " + QString::number(mLen - fLen) + " last arguments");
                tooMany.setIcon(QMessageBox::Warning);
                tooMany.exec();
                for(int i = 0; i < mLen - fLen; i++)
                {
                    passedMultipliers.erase(passedMultipliers.end() - 1);
                }
            }
        }
        QMessageBox checksSuccess;
        checksSuccess.setWindowTitle("Argument passing");
        checksSuccess.setText("All checks passed!");
        checksSuccess.exec();
        w.openImagesFromList(passedFiles, passedAmounts, passedMultipliers);
    }
    return a.exec();
}
