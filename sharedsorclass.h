#ifndef SORCLASS_H
#define SORCLASS_H
/*
 *  sharedsorclass.h
 *  Dotur
 *
 *  Created by Sarah Westcott on 1/8/09.
 *  Copyright 2009 Schloss Lab UMASS Amherst. All rights reserved.
 *
 */

/* This class implements the SharedSorClass estimator on two groups. 
It is a child of the calculator class. */


#include "calculator.h"

/***********************************************************************/

class SorClass : public Calculator  {
	
public:
	SorClass() :  Calculator("SorClass", 3) {};
	EstOutput getValues(SAbundVector*) {return data;};
	EstOutput getValues(SharedRAbundVector*, SharedRAbundVector*);
private:
	
};
/***********************************************************************/

#endif
