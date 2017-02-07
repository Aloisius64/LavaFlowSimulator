#pragma once
#include "dataset.h"
class DataSet2 : public DataSet
{
public:
	DataSet2(void);
	~DataSet2(void);
	DataSet2(string fileName);

	void readData();
};

