#ifndef SURFACEVIEW_H
#define SURFACEVIEW_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QLabel>
#include <surface.h>
#include <vector>

///
/// \brief Класс для обработки сцены с отображением поверхности
///
class SurfaceView : public QObject
{
    Q_OBJECT

private:
    ///
    /// \brief Сцена для отображения поверхности
    ///
    QGraphicsScene *scene;

    ///
    /// \brief Ручка для отображения границ тайлов
    ///
    QPen pen;

    ///
    /// \brief Поверхность с тайлами
    ///
    Surface *surface;

    ///
    /// \brief Надпись для вывода общего объёма воды
    ///
    QLabel *txtTotal;

    ///
    /// \brief Массив из тайлов поверхности
    ///
    std::vector<std::vector<Tile*>> tiles;

    ///
    /// \brief Графические элементы соответствующие тайлам
    ///
    std::vector<std::vector<QGraphicsRectItem*>> rectTiles;

    ///
    /// \brief Если сейчас происходит вычисление - true, инчае false
    ///
    bool isCulculate = false;

public slots:

    ///
    /// \brief Отображение поверхности и разблокировка работы с тайлами после расчётов
    ///
    void startDraw();

    ///
    /// \brief Функция блокировки работы с тайлами при расчётах
    ///
    void startCulc();


public:

    ///
    /// \brief Отображение поверхности
    ///
    void drawSurface();

    ///
    /// \brief Событие нажатия на поверхность
    /// \return Возвращает тайл на который было нажатие или null если нажатие не попало на тайл
    ///
    Tile* getTile(QPointF point);

    ///
    /// \brief Функция заполнения поверхности
    ///
    void inputSurface();

    ///
    /// \brief Размер тайла
    ///
    int size = 40;

    ///
    /// \brief Конструктор класса отображателя поверхности на сцене
    /// \param Сцена для рисования
    /// \param Поверхность для отображения
    ///
    SurfaceView(QGraphicsScene *scene, Surface *surface, QLabel *txtTotal);
};

#endif // SURFACEVIEW_H
