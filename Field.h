#ifndef FIELD_H
#define FIELD_H

#include <QObject>
#include <QVector>
#include <QString>
//#include "global.h"

//! This class is a interface class for the field access.
class Field : public QObject
{

public:

	enum iterationType {ALL = 0, PARTIAL, MARGOLUS};
    enum cellModification {NOTHING = 0, CELL_CHANGED};

	// virtueller Destruktor
	virtual ~Field() {}

	// allgemeine Funktionen
	virtual int FieldType () const = 0;
	virtual QString GetFieldInfo() const  =0;

	// Iterator-Funktionen
	virtual void SetFirstCell (iterationType aType, qint32 n) =0;
	virtual void NextCell () =0;
	virtual bool LastCell () const =0;

	// Funktionen die das gesamte Feld betreffen, Initiierung
	virtual void InitField (int,int) =0;
	virtual void finishInit () =0;
	virtual int  InitField (QString aFileName) =0;
	virtual void loadField(QString aFileName) =0;
	virtual void loadAsciiField(QString aFileName) =0;
	virtual void saveField (QString aFileNname) =0;
	virtual void clearField () =0;
	virtual void updateField () =0;
	virtual void ClearMask () =0;
	virtual void CheckObservers () const =0;
    virtual void addNeighbor (const int a, const int b, int n) =0;
	virtual void setAddingCell(bool aWithCell) =0;

	// direkte Kommunikation mit einer Zelle
    virtual qint32 GetCellState (int, int) const =0;
    virtual void SetCellState (int, int, qint32) =0;
    virtual void SetNewState (int, int, qint32) =0;
    virtual void MarkCellMask (int, int, qint32) =0;
    virtual qint32 GetCellMask (int, int) const =0;

	// Funktionen die mit einer aktiven Zelle arbeiten
	virtual void SetActiveCell (int,int) =0;
	virtual void GetActiveCell (int&,int&) =0;
	virtual void MoveActiveCell(int xd, int yd) =0;
	virtual void MoveActiveCell(int nIdx) =0;
    virtual qint32 GetCellState () const =0;
    virtual void SetCellState (qint32) =0;
    virtual void SetNewState (qint32) =0;
    virtual void MarkCellMask (qint32) =0;
    virtual qint32 GetCellMask () const =0;

	// Nachbarschaftsinformationen für die aktuelle Zelle
	virtual int getNeighborSize() const=0;
	virtual int getNeighborhoodIndex() const=0;
	virtual QVector<int>& getNeighborSum(bool useLast) const =0;
    virtual qint32 getNeighborState(int aIndex) =0;
};

#endif
