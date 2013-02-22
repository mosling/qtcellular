#ifndef STOPWATCH_H
#define STOPWATCH_H

#include <math.h>
#include <windows.h>
#include <time.h>
#include <QString>

class StopWatch
{
public:
	StopWatch(void);
	~StopWatch(void);
	void start();
	void stop();
	double getTime();
	QString getTimeString();
	double getFrequency();

private:
	bool hardClock;
	time_t t_begin, t_end;
	LARGE_INTEGER begin,end,frq;

};

#endif
