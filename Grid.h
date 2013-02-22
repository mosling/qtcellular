#ifndef CA_GRID_H
#define CA_GRID_H

#include <QPoint>
#include <QMap>
#include "Field.h"

/*!
Klasse für die Speicherung eines zweidimensionalen Feldes.
*/
class Grid : public Field
{
public:
	Grid();
	virtual ~Grid();
	// allgemeine Funktionen
	virtual int FieldType () const { return 1;}
	virtual QString GetFieldInfo () const { return "GridField";}

	// Iterator-Funktionen
	virtual void SetFirstCell (iterationType aType, qint32);
	virtual void NextCell ();
	virtual bool LastCell () const;

	// Funktionen mit der aktiven Zelle
	virtual void     SetActiveCell (int x, int y);
	virtual void     GetActiveCell(int &x, int &y);
	virtual void     MoveActiveCell(int xd, int yd);
	virtual void     MoveActiveCell(int nIdx);
	virtual CellType GetCellState () const;
	virtual void     SetCellState (CellType st);
	virtual void     SetNewState (CellType st);
	virtual void     MarkCellMask (MaskType);
	virtual MaskType GetCellMask () const;
	virtual QVector<int>& getNeighborSum(bool useLast) const;
	virtual CellType getNeighborState(int aIndex);

	// Funktionen f"ur das gesamte Feld
	virtual void InitField (int x, int y);
	virtual int  InitField (QString aFileName);
	virtual void loadField(QString aFileName);
	virtual void loadAsciiField(QString aFileName);
	virtual void saveField (QString aFileNname);

	virtual void clearField ();
	virtual void updateField ();
	virtual void ClearMask ();
	virtual void CheckObservers () const;
	virtual void finishInit();

	virtual int  getNeighborSize() const;
	virtual void addNeighbor (int a, int b, int n);
	virtual void setAddingCell(bool aWithCell);

	// direkte Kommunikation mit einer Zelle
	virtual CellType GetCellState (int x, int y) const;
	virtual void SetCellState (int x, int y, CellType st);
	virtual void SetNewState (int x, int y, CellType st);
	virtual void MarkCellMask (int x, int y, MaskType mt);
	virtual MaskType GetCellMask (int x, int y) const;

	// Grid-spezifische Funktionen
	virtual void setBorder(int bf, int bs);
	virtual int getXCells() const { return max_xcellspl1; }
	virtual int getYCells() const { return max_ycellspl1; }
	virtual int getXIndex() const { return currentCell.x; }
	virtual int getYIndex() const { return currentCell.y; }

	int getNeighborhoodIndex() const { return nhIndex; }

private:
	int max_xcells;
	int max_ycells;
	int max_xcellspl1;
	int max_ycellspl1;
	int stepx;
	int stepy;
	int border_state;
	CellType *aktfield;
	CellType *newfield;
	CellType *help_field_pointer;
	MaskType *maskfield;
	int Field_maxy;
	int Field_size;
	int grid;
	int borderform;
	bool fieldlc;
	QMap< int, QVector<PointType> > neighborPoints;
	QMap< int, QVector<PointType> > mirrorNeighborPoints;
	bool fieldValid;
	bool addCell;
	int currentIndex;
        struct { int x,y,lx,ly,o; } currentCell;
	iterationType whichCells;
	int nhIndex;

	void clear_mask ();

	CellType getFieldType (int x, int y) const;
	CellType getFieldIndex (int x, int y, const PointType &p, int *xn, int *yn);
	void indexToCurrentCell();
	void put_field (int x, int y, CellType st);
	void put_newfield (int x, int y, CellType st);
	void mark_mask (int,int,MaskType);
	MaskType get_mask (int x, int y) const;
	void fill_field (CellType st);
	void simple_show_field ();
	void check_observers () const;
	void get_old_field ();
	void update_field ();
	void update_undo_field ();
	void init_field (int x, int y);
	void re_init_field (int x, int y);
	void close_field ();
	//void save_tex_file ();

};

#endif
