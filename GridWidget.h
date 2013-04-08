#ifndef FIELDWIDGET_H
#define FIELDWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include "Grid.h"
#include "StateList.h"

/*!
Klasse zum Anzeigen eines Grids.
*/

class GridWidget : public QWidget
{
	Q_OBJECT

public:
	GridWidget(QWidget *parent);
	~GridWidget();
	void paintEvent(QPaintEvent *);
	void mousePressEvent(QMouseEvent *mouseEvent);
	void mouseMoveEvent(QMouseEvent *mouseEvent);
	void mouseReleaseEvent(QMouseEvent *mouseEvent);
	void setForegroundColor (QColor &aColor);

	void setGridData(Grid *aGrid);
	Grid *getGridData() { return mGridData; }
	void setStateList(StateList *aStateList) { mStates = aStateList; }

private:
   int getNextState(int nx, int ny) const;

   int xCells;
   int yCells;
	bool mousePressed;
	QColor fgColor;
	Grid *mGridData;
	StateList *mStates;
	QPixmap fieldPixmap;
};

#endif
