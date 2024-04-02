#ifndef SURFACE_H
#define SURFACE_H

#include <tile.h>
#include <random>
#include <vector>
#include <algorithm>
#include <QDebug>

///
/// \brief Класс поверхности
///
class Surface
{
private:
    ///
    /// \brief Тайлы поверхности
    ///
    std::vector<std::vector<Tile*>> tiles;

    ///
    /// \brief Тайлы пути капли воды
    ///
    std::vector<Tile*> tilesPath;

    ///
    /// \brief Функция очистки поверхности от воды
    ///
    void clearWater();

    ///
    /// \brief Функция создания поверхности
    ///
    void createSurface();

    ///
    /// \brief Функция установки соседства между тайлами
    ///
    void createNeighbors();

    ///
    /// \brief Получение значений для расчётов из поверхности (максимальная высота и тп)
    ///
    void getValues();

    ///
    /// \brief Функция проверяющая на расположение у края поверхности
    /// \param Тайл
    /// \return Возвращает true - если тайл находится у границы, false - если нет
    ///
    bool atBorder(Tile* tile);

    ///
    /// \brief Функция перемещения блока воды
    /// \param Тайл в котором появляется блок воды
    /// \return Возвращает true - если вода смогла вытечь с поверхности, false - если вода осталась на поверхности
    ///
    bool calculateWater(Tile* currentTile);

    ///
    /// \brief Функция вычисления дождя
    ///
    void calculate();

public:

    ///
    /// \brief Ширина поверхности
    ///
    int width = 1;

    ///
    /// \brief Высота поверхности
    ///
    int height = 1;

    ///
    /// \brief Общий объем воды
    ///
    int totalWater = 0;

    ///
    /// \brief Максимальная высота тайлов
    ///
    int maxHeight = -99;

    ///
    /// \brief Минимальная высота тайлов
    ///
    int minHeight = 99;

    ///
    /// \brief Максимальная разница в высоте
    ///
    int tilesHeight = 0;

    ///
    /// \brief Конструктор класса поверхности
    ///
    Surface();

    ///
    /// \brief Конструктор класса поверхности
    /// \param Ширина поверхности
    /// \param Высота поверхности
    ///
    Surface(int w, int h);

    ///
    /// \brief Функция создания тайла в указанном месте
    /// \param Координата тайла Х
    /// \param Коодрината тайла У
    /// \param Собственная высота тайла
    ///
    void setTile(int x, int y, int height);

    ///
    /// \brief Функция получения поверхности
    /// \param Тип получения данных, если true - после дождя, false - до дождя
    /// \return Возвращает двумерный массив тайлов
    ///
    std::vector<std::vector<Tile*>> getSurface(bool afterRain);

    ///
    /// \brief Функция для установки высоты поверхности
    /// \param Высота поверхности
    ///
    void setRows(int h);

    ///
    /// \brief Функция для установки ширины поверхности
    /// \param Ширина поверхности
    ///
    void setColumns(int w);

    ///
    /// \brief Функция для установки размера поверхности
    /// \param Ширина поверхности
    /// \param Высота поверхности
    ///
    void setSize(int w, int h);

    ///
    /// \brief Задает случайные высоты тайлам поверхности
    ///
    void randSurface();
};



#endif // SURFACE_H
