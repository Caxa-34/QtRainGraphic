#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextCodec>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QColor>
#include <QtConcurrent>
#include <random>
#include <vector>

#include <tile.h>
#include <surfaceview.h>
#include <tile.h>
#include <sqlite.h>
#include <surface.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnApplyTile_clicked();
    void on_listSave_itemDoubleClicked(QListWidgetItem *item);
    void on_spbWidth_valueChanged(int arg1);
    void on_spbHeight_valueChanged(int arg1);
    void on_btnSaveIntoDB_clicked();
    void on_btnStart_clicked();
    void on_btnRandom_clicked();

private:
    Ui::MainWindow *ui;

    ///
    /// \brief Сцена для рисования поверхности
    ///
    QGraphicsScene *scene;

    ///
    /// \brief Тайлы поверхности
    ///
    std::vector<std::vector<Tile*>> tiles;

    ///
    /// \brief Поверхность с тайлами
    ///
    Surface *surface;
    ///
    ///
    /// \brief Отображатель поверхности на сцене
    ///
    SurfaceView *surfaceView;

    ///
    /// \brief Поизиция курсора при нажатии клавиши (press)
    ///
    QPoint currentPos;

    ///
    /// \brief Предыдущая позиция при движении курсора (move)
    ///
    QPoint lastPos;

    ///
    /// \brief Функция отображения информации о выбранном тайле
    /// \param Тайл на который произошло нажатие
    ///
    void setDataTile(Tile *tile);

    ///
    /// \brief Минимальный масштаб
    ///
    const qreal minScale = 0.2;

    ///
    /// \brief Максимальный масштаб
    ///
    const qreal maxScale = 10.0;

    ///
    /// \brief Коэффициент маштабирования
    ///
    qreal scaleFactor = 1.1;

    ///
    /// \brief Функция изменения масштаба сцены
    /// \param Число обозначающее изменение, отрицательное - отдаление, положительное - приближение
    ///
    void scale(int delta);

    ///
    /// \brief Функция поиска событий (применяется для работы с QGraphicView)
    /// \param obj
    /// \param ev
    /// \return
    ///
    bool eventFilter(QObject * obj, QEvent * ev);

    ///
    /// \brief Переменная для блокировки при движениии, true - клавиша мыши зажата и курсор отслеживается, false - клавиша мыши отпущена
    ///
    bool wasMousePress = false;

    ///
    /// \brief Функция отображения объектов для работы с выбранным тайлом
    /// \param False - спрятать объекты, true - отобразить объекты
    ///
    void setVisibleTileData(bool visible);

    ///
    /// \brief Класс для работы с локальной базой данных
    ///
    Sqlite *sqlite;

    ///
    /// \brief Список в который выводятся сохраненные в бд поверхности
    ///
    QListWidget *listSave;

    ///
    /// \brief Отображение поверхности
    ///
    void showSurface(bool afterRain);

    ///
    /// \brief Заадёт элементам интерфейса активность
    /// \param Значение активности
    ///
    void setEnabled(bool enabled);
};
#endif // MAINWINDOW_H
