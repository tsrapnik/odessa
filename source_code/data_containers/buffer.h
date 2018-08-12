#pragma once

class buffer
{
  protected:
	int max_length_mask;
	double *data;

  public:
	buffer(int max_length);
	virtual ~buffer() = 0;
};