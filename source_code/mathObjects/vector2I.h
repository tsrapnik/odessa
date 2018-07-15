#pragma once

class vector2I
{
public:
	int x, y;
	vector2I();
	vector2I( int x, int y );
	~vector2I();
	static vector2I add( const vector2I& first, const vector2I& second );
	static vector2I subtract( const vector2I& first, const vector2I& second );
};