#ifndef CA_GRID_H
#define CA_GRID_H

#include <QPoint>
#include <QMap>
#include "Field.h"

//!
class Grid : public Field
{
    Q_OBJECT

public:
    Grid();
    virtual ~Grid();
    // allgemeine Funktionen
    virtual qint32 FieldType () const { return 1;}
    virtual QString GetFieldInfo () const { return "GridField";}

    // Iterator-Funktionen
    virtual void    SetFirstCell (iterationType aType, qint32);
    virtual void    NextCell ();
    virtual bool    LastCell () const;
    virtual qint32  GetCellState () const;
    virtual void    SetCellState (qint32 st);
    virtual void    SetNewState (qint32 st);
    virtual void    MarkCellMask (qint32);
    virtual qint32  GetCellMask () const;
    virtual QVector<qint32>& getNeighborSum(bool useLast) const;
    virtual qint32  getNeighborState(qint32 aIndex);

    // Funktionen mit der aktiven Zelle
    // \todo no grid methods
    virtual void     SetActiveCell (qint32 x, qint32 y);
    virtual void     GetActiveCell(qint32 &x, qint32 &y);
    virtual void     MoveActiveCell(qint32 xd, qint32 yd);
    virtual void     MoveActiveCell(qint32 nIdx);

    // Funktionen f"ur das gesamte Feld
    virtual void InitField (qint32 x, qint32 y);
    virtual qint32  InitField (QString aFileName);
    virtual void loadField(QString aFileName);
    virtual void loadAsciiField(QString aFileName);
    virtual void saveField (QString aFileNname);

    virtual void clearField ();
    virtual void updateField ();
    virtual void ClearMask ();
    virtual void CheckObservers () const;
    virtual void finishInit();

    virtual qint32  getNeighborSize() const;
    virtual void addNeighbor (const qint32 a, const qint32 b, qint32 n);
    virtual void setAddingCell(bool aWithCell);

    // direkte Kommunikation mit einer Zelle
    virtual qint32  GetCellState (qint32 x, qint32 y) const;
    virtual void    SetCellState (qint32 x, qint32 y, qint32 st);
    virtual void    SetNewState (qint32 x, qint32 y, qint32 st);
    virtual void    MarkCellMask (qint32 x, qint32 y, qint32 mt);
    virtual qint32  GetCellMask (qint32 x, qint32 y) const;

    // Grid-spezifische Funktionen
    virtual void setBorder(qint32 bf, qint32 bs);
    virtual qint32 getXCells() const { return max_xcellspl1; }
    virtual qint32 getYCells() const { return max_ycellspl1; }
    virtual qint32 getXIndex() const { return currentCell.x; }
    virtual qint32 getYIndex() const { return currentCell.y; }

    qint32 getNeighborhoodIndex() const { return nhIndex; }

private:
    qint32 max_xcells;
    qint32 max_ycells;
    qint32 max_xcellspl1;
    qint32 max_ycellspl1;
    qint32 stepx;
    qint32 stepy;
    qint32 border_state;
    qint32 *aktfield;
    qint32 *newfield;
    qint32 *help_field_poqint32er;
    qint32 *maskfield;
    qint32 Field_maxy;
    qint32 Field_size;
    qint32 grid;
    qint32 borderform;
    bool fieldlc;
    QMap< qint32, QVector<QPoint> > neighborPoints;
    QMap< qint32, QVector<QPoint> > mirrorNeighborPoints;
    bool fieldValid;
    bool addCell;
    qint32 currentIndex;
    struct { qint32 x,y,lx,ly,o; } currentCell;
    iterationType whichCells;
    qint32 nhIndex;

    void clear_mask ();

    qint32 getFieldType (qint32 x, qint32 y) const;
    qint32 getFieldIndex (qint32 x, qint32 y, const QPoint &p, qint32 *xn, qint32 *yn);
    void indexToCurrentCell();
    void put_field (qint32 x, qint32 y, qint32 st);
    void put_newfield (qint32 x, qint32 y, qint32 st);
    void mark_mask (qint32,qint32,qint32);
    qint32 get_mask (qint32 x, qint32 y) const;
    void fill_field (qint32 st);
    void simple_show_field ();
    void check_observers () const;
    void get_old_field ();
    void update_field ();
    void update_undo_field ();
    void init_field (qint32 x, qint32 y);
    void re_init_field (qint32 x, qint32 y);
    void close_field ();
    //void save_tex_file ();

};

#endif
