#include <stdio.h>
#include <math.h>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QStringList>

#include "CellularException.h"
#include "Grid.h"

//! Diese Klasse speichert ein Gitter mit den zugehörigen Field-Funktionen.
//!
//! Die Speicherung erfolgt in einem Feld der Größe dimx*dimy. Die Speicherung
//! erfolgt Spaltenweise. Der Index einer Position ist also idx(x,y)=x*dimy+y.
Grid::Grid(): Field(),
    max_xcells(0),
    max_ycells(0),
    max_xcellspl1(0),
    max_ycellspl1(0),
    stepx(0),
    stepy(0),
    border_state(0),
    aktfield(NULL),
    newfield(NULL),
    maskfield(NULL),
    Field_maxy(0),
    Field_size(0),
    grid(0),
    borderform(0),
    fieldlc(false),
    fieldValid(true),
    addCell(false),
    currentIndex(0),
    whichCells(Field::ALL),
    nhIndex(0)
{
}

/*!
Erzeugten Felder wieder freigeben.
*/
Grid::~Grid()
{
    delete [] aktfield; aktfield = NULL;
    delete [] newfield; newfield = NULL;
    delete [] maskfield; maskfield = NULL;
}

/*!
Allokieren der notwendigen Speicher.
*/
void Grid::InitField (int x,int y)
{
    fieldValid = false;
    if (NULL != aktfield)
    {
        if ( x != max_xcellspl1 || y != max_ycellspl1)
        {
            close_field();
        }
        else
        {
            return;
        }

    }

    max_xcells = x - 1;
    max_ycells = y - 1;
    max_xcellspl1 = x;
    max_ycellspl1 = y;
    Field_maxy = y;
    Field_size = x*y;

    aktfield = new qint32[Field_size];
    newfield = new qint32[Field_size];
    maskfield = new qint32[Field_size];

    qDebug() << QObject::tr("Adresse aktfield ") << aktfield;
    qDebug() << QObject::tr("Adresse newfield ") << newfield;
    qDebug() << QObject::tr("Adresse maskfiedl ") << maskfield;
    clearField();
    fieldValid = true;
}

void Grid::finishInit()
{
    QMutableMapIterator<int, QVector<QPoint> > it(neighborPoints);
    while (it.hasNext())
    {
        it.next();
        it.value().squeeze();
    }

    QMutableMapIterator<int, QVector<QPoint> > it2(mirrorNeighborPoints);
    while (it2.hasNext())
    {
        it2.next();
        it2.value().squeeze();
    }
}

int Grid::InitField (QString fname)
{
    loadField (fname);
    return 1;
}

int Grid::getNeighborSize() const
{
    return neighborPoints[nhIndex].size();
}

//! Hinzufügen eines Nachbarschaftsindex. Diese sind relativ zur
//! späteren Zelle und sind in der Reihe ihres Hinzufügens nummeriert
//! von 1 bis n. Mit Index 0 wird die aktuelle Zelle adressiert.
//! Mit dem gespiegelten Punkt kann die zugehörige Zelle ermittelt werden,
//! die diesen Puntk als Nachbarn hat.
void Grid::addNeighbor (int a, int b, int n)
{
    QPoint p, mp;
    p.setX(a);
    p.setY(b);
    mp.setX(-a);
    mp.setY(-b);

    qDebug() << "add neighbor point[" << n << "] (" << a << "," << b << ")";
    neighborPoints[n].append(p);
    mirrorNeighborPoints[n].append(mp);
}

void Grid::setAddingCell(bool aWithCell)
{
    addCell = aWithCell;
}

//! Umwandeln des Indexwertes in die x- und y-Werte der currentCell-Variablen.
void Grid::indexToCurrentCell()
{
    currentCell.x = currentIndex / max_ycellspl1;
    currentCell.y = currentIndex - (max_ycellspl1 * currentCell.x);
}

//! Wenn im Viewer eine Zelle angeklickt wurde, dann wird
//! diese Funktion aufgerufen.
void Grid::SetActiveCell (int x, int y)
{
    currentCell.x = x;
    currentCell.y = y;
}

void Grid::GetActiveCell(int &x, int &y)
{
    x = currentCell.x;
    y = currentCell.y;
}

//! Turingmaschinen bewegen einfach nur die aktive Zelle.
void Grid::MoveActiveCell(int xd, int yd)
{
    QPoint pn(xd,yd);
    int xn, yn;

    getFieldIndex(currentCell.x, currentCell.y, pn, &xn, &yn);
    currentCell.x = xn;
    currentCell.y = yn;
}

void Grid::MoveActiveCell(int nIdx)
{
    MoveActiveCell(neighborPoints[nhIndex][nIdx].x(),
                   neighborPoints[nhIndex][nIdx].y());
}

void Grid::SetFirstCell (iterationType aType, qint32 n)
{
    fieldlc = false;
    currentIndex=0;
    currentCell.x = 0;
    currentCell.y = 0;

    whichCells = aType;
    if (whichCells == Field::PARTIAL)
    {
        while ( currentIndex < Field_size
                && maskfield[currentIndex] != CELL_CHANGED ) ++currentIndex;
        if (currentIndex < Field_size) indexToCurrentCell();
    }
    else if (whichCells == Field::MARGOLUS)
    {
        currentCell.x = n;
        currentCell.y = n;
        currentCell.lx = n;
        currentCell.ly = n;
        currentCell.o = n; // merken für die nächste Zeile
        nhIndex = 0;
    }

}

//! Nummerierung nhIndex bei Margolus innerhalb eines Blockes
//! +---+---+
//! | 0 | 1 |
//! +---+---+
//! | 3 | 2 |
//! +---+---+
void Grid::NextCell ()
{

    if (whichCells == Field::PARTIAL)
    {
        currentIndex++;
        while ( currentIndex < Field_size
                && maskfield[currentIndex] != CELL_CHANGED ) ++currentIndex;
        if (currentIndex < Field_size) indexToCurrentCell();
    }
    else if (whichCells == Field::MARGOLUS)
    {
        if (3 == nhIndex)
        {
            // Block weiterrücken, l?-Werte verwenden
            nhIndex = 0;

            if (currentCell.lx + 2 <= max_xcells)
            {
                currentCell.lx += 2;
            }
            else if (currentCell.ly + 2 <= max_ycells)
            {
                currentCell.lx = currentCell.o;
                currentCell.ly += 2;
            }
            else
            {
                fieldlc = true;
            }
            currentCell.x = currentCell.lx;
            currentCell.y = currentCell.ly;
        }
        else
        {
            // innerhalb der Margolus-Umgebung
            nhIndex++;
            switch (nhIndex)
            {
            case 1:
                currentCell.x = (currentCell.lx + 1) % max_xcellspl1;
                break;
            case 2:
                currentCell.x = (currentCell.lx + 1) % max_xcellspl1;
                currentCell.y = (currentCell.ly + 1) % max_ycellspl1;
                break;
            case 3:
                currentCell.x = currentCell.lx;
                currentCell.y = (currentCell.ly + 1) % max_ycellspl1;
                break;
            }
        }
    }
    else
    {
        if (currentCell.x < max_xcells)
        {
            currentCell.x++;
        }
        else if (currentCell.y < max_ycells)
        {
            currentCell.x = 0;
            currentCell.y++;
        }
        else
        {
            fieldlc = true;
        }
    }
}

//! Funktion gibt true zurück, wenn die letzte Zelle erreicht ist.
bool Grid::LastCell () const
{
    return (fieldlc || (currentIndex == Field_size));
}

qint32 Grid::GetCellState () const
{
    return *(aktfield+(currentCell.x*Field_maxy+currentCell.y));
}

void Grid::SetCellState (qint32 st)
{
    put_field (currentCell.x, currentCell.y, st);
}

void Grid::SetNewState(qint32 st)
{
    put_newfield (currentCell.x, currentCell.y, st);
}

void Grid::MarkCellMask (qint32 mt)
{
    mark_mask (currentCell.x, currentCell.y, mt);
}

qint32 Grid::GetCellMask () const
{
    return *(maskfield+(currentCell.x*Field_maxy+currentCell.y));
}

void Grid::ClearMask ()
{
    memset (maskfield,0,Field_size*sizeof(qint32));
}

void Grid::CheckObservers () const
{
    check_observers ();
}

void Grid::setBorder(int bf, int bs)
{
    borderform = bf;
    border_state = bs;
}

void Grid::clearField ()
{
    qDebug() << "init grid fiels with empty cells";

    memset (aktfield,0,Field_size*sizeof(qint32));
    memset (newfield,0,Field_size*sizeof(qint32));
    memset (maskfield,2,Field_size*sizeof(qint32));
}

void Grid::updateField ()
{
    qint32 *help_field_pointer = aktfield;
    aktfield = newfield;
    newfield = help_field_pointer;
    memset (newfield,0,Field_size*sizeof(qint32));
}

//! Diese Methode bestimmt den Wert einer Zelle mit dem Index (x,y).
//! Dabei wird insbesondere die eingestellte Randbehandlung beachtet.
qint32 Grid::getFieldType (int x,int y) const
{
    signed int hx,hy;

    hx = x;
    hy = y;
    if (borderform == 0)
    {
        /* zyklische randverkn"upfung */
        if (hx > max_xcells) hx = hx - max_xcellspl1;
        else if (hx < 0) hx = max_xcellspl1 + hx;
        if (hy > max_ycells) hy = hy - max_ycellspl1;
        else if (hy < 0) hy = max_ycellspl1 + hy;
    }
    else if (borderform == 1)
    {
        /* Nur wenn plane als borderform */
        if ((hx > max_xcells) || (hx < 0) ||
                (hy > max_ycells) || (hy < 0))
            return (border_state);
    }
    else
    {
        /* reflektierender rand */
        if (hx > max_xcells)
            hx = (max_xcells<<1) - hx; /* 2*max_xcells - hx */
        else if (hx < 0)
            hx = hx<0?hx*-1:hx;
        if (hy > max_ycells)
            hy = (max_ycells<<1) - hy; /* 2*max_ycells - hy */
        else if (hy < 0)
            hy = hy<0?hy*-1:hy;
    }

    return *(aktfield+(hx*Field_maxy+hy));
}

//! Bestimmen einer neuen Feldposition. Dabei wird von Position (x,y)
//! ausgegangen und die Differenz aus (p.x,p.y) verwendet um den neuen
//! Punkt (xn,yn) zu bestimmen.
qint32 Grid::getFieldIndex (int x, int y,
                              const QPoint &p,
                              int *xn, int *yn)
{
    signed int hx,hy;

    hx = x + p.x();
    hy = y + p.y();
    if (borderform == 0)
    {
        /* cyclic coupled */
        if (hx > max_xcells) hx = hx - max_xcellspl1;
        else if (hx < 0) hx = max_xcellspl1 + hx;
        if (hy > max_ycells) hy = hy - max_ycellspl1;
        else if (hy < 0) hy = max_ycellspl1 + hy;
        *xn = hx; *yn = hy;
        return 1;
    }
    else
    {
        if ((hx > max_xcells) || (hx < 0) ||
                (hy > max_ycells) || (hy < 0))
            return 0;
        else
        {
            *xn = hx; *yn = hy;
            return 1;
        }
    }
}

void Grid::put_field (int x,int y,qint32 st)
{
    *(aktfield+(x*Field_maxy+y)) = st;
}

void Grid::put_newfield (int x,int y,qint32 st)
{
    *(newfield+(x*Field_maxy+y)) = st;
}

void Grid::mark_mask (int x,int y,qint32 mt)
{
    *(maskfield+(x*Field_maxy+y)) = mt;
}

qint32 Grid::get_mask (int x,int y) const
{
    return *(maskfield+(x*Field_maxy+y));
}


// !!! abwarten, ob die Funktion noch ben"otigt wird
void Grid::fill_field(qint32 st)
{
    int i;

    for (i=0;i<Field_size;i++)
    {
        *(aktfield+i) = st;
        *(newfield+i) = st;
    }
}


void Grid::check_observers () const
{
    /*
 int i;

    if (observe_states) {
        for (i=0;i<=max_states;i++)
            fprintf (statistic_file,"%ld  ",observer_states[i]);
        fprintf (statistic_file,"\n");
    }
    if (observe_fields) {
        for (i=0;i<Field_size;i++)
            fprintf (fields_file,"%c",*(aktfield+i));
    }
    if (observe_changes)
        fprintf (changes_file, "%d\n", number_of_changes);
*/
}


void Grid::get_old_field ()
{
    memcpy (newfield,aktfield,Field_size);
}

void Grid::close_field ()
{
    qDebug() << "delete aktfield " << aktfield;
    delete [] aktfield;
    qDebug() << "delete newfield " << newfield;
    delete [] newfield;
    qDebug() << "delete maskfiedl " << maskfield;
    delete [] maskfield;
    aktfield = NULL;
    newfield = NULL;
    maskfield = NULL;
}

/*!
Laden einer Daten-Datei in das Feld.
*/
void Grid::loadField (QString aFileName)
{
    QFile data(aFileName);
    qint32 b,x,y;

    if (data.open(QFile::ReadOnly) )
    {
        QDataStream in(&data);
        in >> x >> y;
        InitField(x,y);
        memset (maskfield,CELL_CHANGED,Field_size*sizeof(qint32));
        for (int i=0; i < Field_size; ++i)
        {
            in >> b;
            aktfield[i] = b;
            maskfield[i] = CELL_CHANGED;
        }
        data.close();
    }
}

void Grid::loadAsciiField(QString aFileName)
{
    QFile data(aFileName);
    QString line;
    QStringList l;
    int x,y,c;
    bool b1,b2,b3;

    if (data.open(QFile::ReadOnly) )
    {
        QTextStream in(&data);
        line = in.readLine();

        memset (maskfield,CELL_CHANGED,Field_size*sizeof(qint32));
        while (!line.isNull())
        {
            l = line.split(" ", QString::SkipEmptyParts);
            if (l.size() == 3)
            {
                x = l.at(0).toInt(&b1);
                y = l.at(1).toInt(&b2);
                c = l.at(2).toInt(&b3);
                if (b1 && b2 && b3)
                {
                    if (y < max_ycellspl1 && x < max_xcellspl1)
                    {
                        put_field(x,y,c);
                        mark_mask(x,y,CELL_CHANGED);
                    }
                    else
                    {
                        qDebug() << "Punkt liegt außerhalb des Feldes";
                    }
                }
            }
            else
            {
                qDebug() << "line " << line << " has more than 3 numbers";
            }
            line = in.readLine();
        }
        data.close();
    }
    else
    {
        qDebug() << "Can't open field " << aFileName;
    }
}

/*!
Speichern des aktuellen Feldes.
*/
void Grid::saveField (QString aFileName)
{
    QFile data(aFileName);

    if (data.open(QFile::WriteOnly))
    {
        QDataStream out(&data);
        out << static_cast<qint32>(max_xcellspl1);
        out << static_cast<qint32>(max_ycellspl1);

        for (int i=0; i<Field_size; ++i)
        {
            out << static_cast<qint32>(aktfield[i]);
        }
        data.close();
    }
}

/*
void Grid::save_tex_file ()
{
  // at this time max 10 different markers
  char *markers[] = {
    "{$\\bullet$}", "{$\\circ$}", "{$\\star$}", "{$\\ast$}",
    "{$\\ominus$}", "{$\\otimes$}", "{$\\oslash$}","{$\\oplus$}",
    "{$\\odot$}","{$\\diamond$}"};
  int i,j,k,max_marker=10;
  qint32 st;
  FILE *fp;
  char name[200];

  sprintf (name,"%s_%d.tex",automata_name,value("TIME"));
  fp = fopen (name,"w");
  if (fp==NULL) {
    printf (CAFI_002,name);
    return;
  }
  printf (CAFI_003, name );
  for (i=1;i<=max_states;i++) {
    if (i>max_marker)
    fprintf (fp,"\\def\\marker%c%c{{$\\diamond$}}\n",
       65+(i/20),65+(i%20));
    else
    fprintf (fp,"\\def\\marker%c%c{%s}\n",
       65+(i/20),65+(i%20),markers[i-1]);
  }
  fprintf (fp,"\\beginpicture\n");
  fprintf (fp,"\\cellarray %d,%d,{%s at Timestep %d}\n",
     max_xcellspl1,max_ycellspl1,automata_name,value("TIME"));

  fprintf (fp,"\\put {$\\Box$~~~%s} [l] at -0.25 -1.6\n",state_array[0].name);
  for (k=1;k<=max_states;k++) {
    fprintf (fp,"\\put {\\marker%c%c~~~%s} [l] at 0 %d.6\n",
       65+(k/20),65+(k%20),state_array[k].name,-(k+1));
    for (i=0;i<=max_xcells;i++)
    for (j=0;j<=max_ycells;j++) {
      st=*(aktfield+(i*Field_maxy+j));
      if (st==k)
        fprintf (fp,"\\at %d,%d,\\marker%c%c \n",
           i,max_ycells-j,65+(k/20),65+(k%20));
    }
  }

  fprintf (fp,"\\endpicture\n");

  fclose (fp);
}
*/

//! Die Nachbarzellen sind von 0 bis n nummeriert. Mit dieser
//! Methode bekommt man den Zustand des Nachbarn mit dem
//! Index aIndex der aktuellen Zelle.
//!
//! Unabhängig von der tatsächlichen Nachbarschaft ist aIndex=0
//! immer die aktuelle Zelle.
qint32 Grid::getNeighborState(int aIndex)
{
    if (aIndex == 0)
    {
        return *(aktfield+(currentCell.x*Field_maxy+currentCell.y));
    }
    else if (aIndex > 0 && aIndex <= neighborPoints[nhIndex].size())
    {
        aIndex--;
        return getFieldType(currentCell.x+neighborPoints[nhIndex][aIndex].x(),
                            currentCell.y+neighborPoints[nhIndex][aIndex].y());
    }

    throw CellularException(__FILE__,__LINE__,
                            QString("Neighbour index %1 out of bounds 0...%2")
                            .arg(aIndex).arg(neighborPoints[nhIndex].size()) );

    return 0;
}

//! Berechnen der Nachbarschaftsumme. Die Liste enthält für jeden
//! Zustandswert die Anzahl dieser Zustände in der Nachbarschaft.
//! Ist der Parameter current true, wird eine vorhandene Liste wieder
//! verwendet.
//! \todo Anzahl der möglichen Zustände darf nicht konstant sein.
QVector<int>& Grid::getNeighborSum (bool useLast) const
{
    static QVector<int> nsum(256);
    static bool recomputeSum = true;
    qint32 st;

    if (useLast)
    {
        if (recomputeSum)
        {
            nsum.fill(0);
            if (addCell)
            {
                st = getFieldType(currentCell.x, currentCell.y);
                nsum[st]++;
            }

            foreach (QPoint p, neighborPoints[nhIndex])
            {
                st = getFieldType(currentCell.x+p.x(), currentCell.y+p.y());
                nsum[st]++;
            }

            recomputeSum = false;
        }
    }
    else
    {
        recomputeSum = true;
        nsum.fill(0);
    }

    return nsum;
}

qint32 Grid::GetCellState (int x, int y) const
{
    return getFieldType (x,y);
}

void Grid::SetCellState (int x, int y, qint32 st)
{
    put_field (x,y,st);
    currentCell.x = x; currentCell.y = y;
}

void Grid::SetNewState (int x, int y, qint32 st)
{
    put_newfield (x,y,st);
}

void Grid::MarkCellMask (int x, int y, qint32 mt)
{
    mark_mask (x,y,mt);
}

qint32 Grid::GetCellMask (int x, int y) const
{
    return get_mask (x,y);
}

