#ifndef SQLITE_H
#define SQLITE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QtDebug>
#include <QTextCodec>
#include <QSqlQuery>
#include <QDateTime>
#include <QListWidget>
#include <QFileInfo>
#include <QDir>

#include "surface.h"

///
/// \brief Класс для работы с локальной бд
///
class Sqlite
{
private:
    ///
    /// \brief Функция создания базы данных
    /// \return Возвращает true - если бд не было и она создана, false - если бд существует или при создании возникла ошибка
    ///
    bool createDatabase();

    ///
    /// \brief База данных
    ///
    QSqlDatabase db;

    ///
    /// \brief Поверхность с которой будет работать бд
    ///
    Surface *surface;

    ///
    /// \brief Массив из тайлов поверхности
    ///
    std::vector<std::vector<Tile*>> tiles;

    ///
    /// \brief Список в который выводятся сохраненные в бд поверхности
    ///
    QListWidget *listSave;
public:
    ///
    /// \brief Конструктор класса для работы с бд
    /// \param Поверхность с которой будет работать бд
    ///
    Sqlite(Surface *surface, QListWidget *listSave);

    ///
    /// \brief Функция сохранения поверхности в базу данных
    /// \return Возвращает id поверхности или 0 если запись не произошла
    ///
    int saveIntoDB(QString name);

    ///
    /// \brief Функция отображения поверхности из базы данных
    /// \param id нужной записи
    ///
    bool getFromDB(int num);

    ///
    /// \brief Функция отображения сохранений из бд
    ///
    void updateListSave();
};

#endif // SQLITE_H
