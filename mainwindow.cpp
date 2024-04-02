#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("CP1251"));

    surface = new Surface;
    scene = new QGraphicsScene;
    listSave = ui->listSave;
    sqlite = new Sqlite(surface, listSave);
    surfaceView = new SurfaceView(scene, surface);

    ui->grSurface->setScene(scene);
    ui->grSurface->setTransformationAnchor(QGraphicsView::NoAnchor);
    ui->grSurface->setRenderHint(QPainter::Antialiasing);

    // подключение обработчиков событий мыши
    ui->grSurface->viewport()->installEventFilter(this);
    surfaceView->drawSurface(false);
    setVisibleTileData(false);

    // нужно что бы нормально отрабатывал скейл, без появления прокрутки
    QRectF sceneRect = ui->grSurface->sceneRect();
    ui->grSurface->setSceneRect(sceneRect);
}

MainWindow::~MainWindow()
{
    delete ui;
}


bool MainWindow::eventFilter(QObject * obj, QEvent * ev) {
    if (ui->grSurface->viewport()) {
        if (ev->type() == QEvent::MouseMove) //движение
        {
            if (!wasMousePress) return false;
            QMouseEvent *event = (QMouseEvent*)ev;
            if (event->pos() == currentPos) return false;

            QPointF oldPoint = ui->grSurface->mapToScene(lastPos);
            QPointF newPoint = ui->grSurface->mapToScene(event->pos());
            QPointF translation = newPoint - oldPoint;

            QRectF sceneRect = ui->grSurface->sceneRect();
            sceneRect.translate(-translation);
            ui->grSurface->setSceneRect(sceneRect);

            lastPos = event->pos();
        }
        if (ev->type() == QEvent::MouseButtonPress) //нажатие
        {
            wasMousePress = true;
            QMouseEvent *event = (QMouseEvent*)ev;
            currentPos = event->pos();
            lastPos = event->pos();
        }
        if (ev->type() == QEvent::MouseButtonRelease) //отпускание после нажатия
        {
            wasMousePress = false;
            QMouseEvent *event = (QMouseEvent*)ev;
            if (event->pos() != currentPos) return false;

            //поиск тайла по координате нажатия
            QPointF currentPoint = ui->grSurface->mapToScene(event->pos());
            Tile *currentTile = surfaceView->getTile(currentPoint);
            if (currentTile == nullptr) {
                setVisibleTileData(false);
                return false;
            }
            setDataTile(currentTile);
        }
        if (ev->type() == QEvent::Wheel) //прокрутка колесика
        {
            QWheelEvent *event = (QWheelEvent*)ev;
            scale(event->delta());
        }
    }
    return false;
}

void MainWindow::setDataTile(Tile *currentTile) {
    setVisibleTileData(true);
    ui->spbTileX->setValue(currentTile->x+1);
    ui->spbTileY->setValue(currentTile->y+1);
    ui->spbTileHeight->setValue(currentTile->height);
    ui->spbTileWater->setValue(currentTile->heightWater);
}

void MainWindow::setVisibleTileData(bool visible) {
    ui->spbTileX->setVisible(visible);
    ui->spbTileY->setVisible(visible);
    ui->spbTileHeight->setVisible(visible);
    ui->spbTileWater->setVisible(visible);
    ui->btnApplyTile->setVisible(visible);
    ui->labelXTile->setVisible(visible);
    ui->labelYTile->setVisible(visible);
    ui->labelHeightTile->setVisible(visible);
    ui->labelWaterTile->setVisible(visible);
}

void MainWindow::scale(int delta) {
    // ограничения масштабирования
    if (ui->grSurface->transform().m11() < minScale) {
        ui->grSurface->scale(scaleFactor, scaleFactor);
    }
    if (ui->grSurface->transform().m11() > maxScale) {
        ui->grSurface->scale(1.0/scaleFactor, 1.0/scaleFactor);
    }

    // если delta положительный, увеличиваем масштаб
    if (delta > 0) {
        ui->grSurface->scale(scaleFactor, scaleFactor);
    }
    // если delta отрицательный, уменьшаем масштаб
    if (delta < 0) {
        ui->grSurface->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
}

void MainWindow::on_spbWidth_valueChanged(int arg1) {
    surface->setColumns(arg1);
    showSurface(false);
}

void MainWindow::on_spbHeight_valueChanged(int arg1) {
    surface->setRows(arg1);
    showSurface(false);
}

void MainWindow::showSurface(bool afterRain) {
    surfaceView->drawSurface(afterRain);
    if (afterRain) ui->txtTotal->setText( trUtf8("Итого воды: ") + QString::number(surface->totalWater));
}

void MainWindow::on_btnRandom_clicked() {
    surface->randSurface();
    showSurface(true);
}

void MainWindow::on_btnStart_clicked() {
    showSurface(true);
}

void MainWindow::on_btnApplyTile_clicked() {
    int x = ui->spbTileX->value() - 1,
    y = ui->spbTileY->value() - 1,
    heightTile = ui->spbTileHeight->value();
    surface->setTile(x, y, heightTile);
    setVisibleTileData(false);
    showSurface(true);
}

void MainWindow::on_btnSaveIntoDB_clicked()
{
    QString name = ui->leNameSaveIntoDB->text();
    if (name == "") name = trUtf8("Без имени");
    int result = sqlite->saveIntoDB(name);
    if (result == 0) {
        QMessageBox::warning(this, trUtf8("Ошибка"), trUtf8("Ошибка записи в базу данных!"));
    }
    else {
        QMessageBox::information(this, trUtf8("Успех"), trUtf8("Поверхность была записана!"));
        showSurface(true);
    }
}

void MainWindow::on_listSave_itemDoubleClicked(QListWidgetItem *item) {
    int num = item->data(Qt::UserRole).toInt();
    if (!sqlite->getFromDB(num)) {
        QMessageBox::warning(this, trUtf8("Ошибка"), trUtf8("Ошибка получения поверхности!"));
    }
    else {
        showSurface(true);
        ui->spbWidth->setValue(surface->width);
        ui->spbHeight->setValue(surface->height);
    }
}
