#ifndef TILE_H
#define TILE_H
#include <vector>

///
/// \brief Класс тайла конструкции
///
class Tile
{
public:
    ///
    /// \brief Собственная высота
    ///
    int height = 0;

    ///
    /// \brief Высота воды
    ///
    int heightWater = 0;

    ///
    /// \brief Общая высота (собственная + вода)
    ///
    int heightWithWater = 0;

    ///
    /// \brief Соседние тайлы
    ///
    std::vector<Tile*> neighbors;

    ///
    /// \brief Координата тайла Х
    ///
    int x = 0;

    ///
    /// \brief Координата тайла У
    ///
    int y = 0;

    ///
    /// \brief Конструктор тайла
    /// \param Координата Х
    /// \param Координата У
    /// \param Собственная высота
    ///
    Tile(int x, int y, int h);

    ~Tile()
    {
        // Освобождаем память для всех соседних тайлов
        for (auto& neighbor : neighbors)
        {
            delete neighbor;
        }
    }
};

#endif // TILE_H
