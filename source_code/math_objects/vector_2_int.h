#pragma once

class vector_2_int
{
  public:
	int x, y;
	vector_2_int();
	vector_2_int(int x, int y);
	~vector_2_int();
	static vector_2_int add(const vector_2_int &first, const vector_2_int &second);
	static vector_2_int subtract(const vector_2_int &first, const vector_2_int &second);
};