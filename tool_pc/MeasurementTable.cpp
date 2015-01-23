#include <string>
#include "MeasurementTable.h"

#define ENTRY(ID, var, rightshift, resolution, unit, name, minvalue, maxvalue) \
{(ID), (rightshift), (resolution), (unit), (name), (minvalue), (maxvalue)},

measurement_t measurementTable[] =
{
    #include "../mtable.inc"
};

measurement_t parameterTable[] =
{
	#include "../ptable.inc"
};
