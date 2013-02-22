#ifndef FIELDALGORITHMS_H
#define FIELDALGORITHMS_H

#include "Field.h"
#include "StateList.h"

class FieldAlgorithms
{

public:
	 static void Randomize(Field *field, StateList *sl);

private:
    FieldAlgorithms();


};

#endif // FIELDALGORITHMS_H
