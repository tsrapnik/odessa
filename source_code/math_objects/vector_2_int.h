#pragma once

class vector2_i
{
public:
	int x, y;
	vector2_i();
	vector2_i( int x, int y );
	~vector2_i();
	static vector2_i add( const vector2_i& first, const vector2_i& second );
	static vector2_i subtract( const vector2_i& first, const vector2_i& second );
};