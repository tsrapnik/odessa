#pragma once

class buffer
{
protected:
	int maxLengthMask;
	double* data;
public:
	buffer( int maxLength );
	virtual ~buffer() = 0;
};