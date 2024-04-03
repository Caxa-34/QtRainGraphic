#ifndef SURFACEVIEW_H
#define SURFACEVIEW_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>
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
    /// \brief Массив из тайлов поверхности
    ///
    std::vector<std::vector<Tile*>> tiles;

    ///
    /// \brief Графические элементы соответствующие тайлам
    ///
    std::vector<std::vector<QGraphicsRectItem*>> rectTiles;

public:

    ///
    /// \brief Отображение поверхности
    ///
    void drawSurface(bool afterRain);

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
    SurfaceView(QGraphicsScene *scene, Surface *surface);
};

#endif // SURFACEVIEW_H
