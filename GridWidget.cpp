#include <QtDebug>
#include "GridWidget.h"

GridWidget::GridWidget(QWidget *parent)
    : QWidget(parent), mousePressed(false)
{
    xCells=10;
    yCells=10;
    this->setMouseTracking(true);
}

GridWidget::~GridWidget()
{
}

void GridWidget::paintEvent(QPaintEvent *)
{
    bool allNew = false;
    StateListNode *sn;
    QPainter gwPainter(this);

    if (fieldPixmap.size() != size())
    {
        allNew = true;
        fieldPixmap = QPixmap(size());
    }

    QPainter painter(&fieldPixmap);

    // Jetzt die Felder zeichnen
    xCells = mGridData->getXCells();
    yCells = mGridData->getYCells();
    bool drawGrid = this->width() > (8*xCells);

    painter.setWindow(0,0,xCells,yCells);
    if (allNew)
        mGridData->SetFirstCell(Field::ALL, 0);
    else
        mGridData->SetFirstCell(Field::PARTIAL, 0);
    while (!mGridData->LastCell ())
    {
        sn = mStates->getState(mGridData->GetCellState());
        int x = mGridData->getXIndex();
        int y = mGridData->getYIndex();

        if (sn != NULL)
        {
            switch (sn->how)
            {
            case 1: /* kleines Bildchen */
                break;
            case 2: /* als Zeichen anzeigen */
                break;
            case 3: /* die Zustandsnr. anzeigen */
                break;
            default: /* farbiges Rechteck */
                painter.fillRect(x,y,1,1,sn->color);
                break;
            }
        }
        if (drawGrid)
        {
            painter.setPen(fgColor);
            painter.setPen(Qt::SolidLine);
            painter.drawRect(x,y,1,1);
        }
        mGridData->NextCell();
    }

    if (allNew)
    {
        // draw the frame
        painter.setPen(fgColor);
        painter.setPen(Qt::SolidLine);
        painter.setWindow(0,0,width(),height());
        painter.drawRect(0,0, width()-1, height()-1);
        painter.setWindow(0,0,xCells,yCells);
    }

    // zum Schluss das Werk anzeigen
    gwPainter.drawPixmap(0,0,fieldPixmap);
}

void GridWidget::mousePressEvent(QMouseEvent *mouseEvent)
{
    int nx = mouseEvent->x()*xCells/this->width();
    int ny = mouseEvent->y()*yCells/this->height();

    // qDebug() << "press mouse " << nx << ", " << ny;
    mGridData->ClearMask();
    mGridData->SetCellState(nx,ny,getNextState(nx,ny));
    mGridData->MarkCellMask(nx, ny, Field::CELL_CHANGED);
    update();
    mousePressed = true;
}

void GridWidget::mouseMoveEvent(QMouseEvent *mouseEvent)
{
    int nx = mouseEvent->x()*xCells/this->width();
    int ny = mouseEvent->y()*yCells/this->height();
    // qDebug() << "move mouse " << nx << ", " << ny;
    if (mousePressed && Field::CELL_CHANGED != mGridData->GetCellMask(nx,ny))
    {
        mGridData->SetCellState(nx,ny,getNextState(nx,ny));
        mGridData->MarkCellMask(nx, ny, Field::CELL_CHANGED);
        update();
    }
}

void GridWidget::mouseReleaseEvent(QMouseEvent *mouseEvent)
{
    //int nx = mouseEvent->x()*xCells/this->width();
    //int ny = mouseEvent->y()*yCells/this->height();
    // qDebug() << "release mouse " << nx << ", " << ny;
    mGridData->ClearMask();
    mousePressed = false;
}

void GridWidget::setForegroundColor (QColor &aColor)
{
    this->fgColor = aColor;
}

void GridWidget::setGridData(Grid *aGrid)
{
    mGridData = aGrid;
}

int GridWidget::getNextState(int nx, int ny) const
{
    int nc = mStates->getMouseState();
    if ( nc < 0)
    {
        nc = mGridData->GetCellState(nx, ny);
        nc = (nc+1)%mStates->size();
    }
    return nc;
}

