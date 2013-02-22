#include "StopWatch.h"

StopWatch::StopWatch(void)
{
	hardClock = true;

	if (QueryPerformanceFrequency(&frq)==0)
	{
		hardClock=false;
		// keine Frequenz zu ermitteln, setzen 1 damit es 
		// keine Division durch 0 Fehler gibt.
		frq.QuadPart = 1;
	}
}

StopWatch::~StopWatch(void)
{
}

double StopWatch::getFrequency()
{
	return (double)frq.QuadPart;
}

/*!
Starten der Uhr.
*/
void StopWatch::start()
{
	if (hardClock)
	{
		QueryPerformanceCounter(&begin);
	}
	else
	{
		time(&t_begin);
	}
}

/*!
Anhalten der Uhr. Die Variable end/finish  wird mit dem Wert gefüllt.
Mit mehrmaligen Aufruf dieser Methode können Zwischenzeiten genommen
werden.
*/
void StopWatch::stop()
{	
	if (hardClock)
	{
		QueryPerformanceCounter(&end);
	}
	else
	{
		time(&t_end);
	}
}

/*!
Es wird die gemessene Zeit in Sekunden geliefert. 
*/
double StopWatch::getTime()
{
	double elapsed_time;
	
	if (hardClock)
	{
		elapsed_time=(double)(end.QuadPart - begin.QuadPart)/(double)frq.QuadPart;
	}
	else
	{
		elapsed_time = difftime( t_end, t_begin );
	}

	return elapsed_time;
}

/*!
Rückgabe der bisher vergangenen Zeit als Zeichenkette.
*/
QString StopWatch::getTimeString()
{
	QString vResult("");
	QChar c('0');
	double t = getTime();
	int d;
	double sp;
	bool b=false;

	sp = t-floor(t);
	t = floor(t);
	if (t > 3600)
	{
		d = t/3600;
		t = t-(d*3600);
		vResult=QString("%1h ").arg(d);
		b=true;
	}
	if (t>60)
	{
		d = t/60;
		t = t-(d*60);
		vResult+=QString("%1m ").arg(d,2,10,c);
		b=true;
	}
	else if (b)
	{
		vResult+="00m ";
	}
	if (t>0)
	{
		d = t;
		vResult+=QString("%1s").arg(d,2,10,c);
		b = true;
	}
	else if (b)
	{
		vResult+="00s";
	}
	else
	{
		vResult+="0";
	}
	d = sp*1000;
	vResult+=QString(".%1").arg(d,3,10,c);
	if (!b)
	{
		vResult += "s";
	}

	return vResult;
}
