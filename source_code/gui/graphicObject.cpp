#include "graphicObject.h"

graphicObject::graphicObject( rectangle footprint, colour ownColour )
{
	this->footprint = footprint;
	this->boundaries = nullptr;
	this->ownScreen = nullptr;
	this->ownColour = ownColour;
}

void graphicObject::setBoundaries( rectangle* boundaries )
{
	if( footprint.origin.x < 0 )
		footprint.origin.x = 0;
	if( footprint.origin.x + footprint.size.x > boundaries->size.x )
		footprint.size.x = boundaries->size.x - footprint.origin.x;
	if( footprint.origin.y < 0 )
		footprint.origin.y = 0;
	if( footprint.origin.y + footprint.size.y > boundaries->size.y )
		footprint.size.y = boundaries->size.y - footprint.origin.y;

	footprint.origin = vector2I::add( footprint.origin, boundaries->origin );
	this->boundaries = boundaries;
}

void graphicObject::setOwnScreen( screen* ownScreen )
{
	this->ownScreen = ownScreen;
}

rectangle graphicObject::getFootprint()
{
	return footprint;
}

void graphicObject::setFootprint( rectangle newFootprint )
{
	footprint = newFootprint;
}

colour graphicObject::getColour()
{
	return ownColour;
}

graphicObject::~graphicObject()
{}

void graphicObject::draw()
{
	for( int x = footprint.origin.x; x < footprint.origin.x + footprint.size.x; x++ )
		for( int y = footprint.origin.y; y < footprint.origin.y + footprint.size.y; y++ )
			ownScreen->putTransparentPixel( vector2I( x, y ), ownColour );
}

vector2I graphicObject::move( vector2I displacement )
{
	if( footprint.origin.x + displacement.x < boundaries->origin.x )
		displacement.x = boundaries->origin.x - footprint.origin.x;
	if( footprint.origin.x + footprint.size.x + displacement.x > boundaries->origin.x + boundaries->size.x )
		displacement.x = boundaries->origin.x + boundaries->size.x - footprint.origin.x - footprint.size.x;
	if( footprint.origin.y + displacement.y < boundaries->origin.y )
		displacement.y = boundaries->origin.y - footprint.origin.y;
	if( footprint.origin.y + footprint.size.y + displacement.y > boundaries->origin.y + boundaries->size.y )
		displacement.y = boundaries->origin.y + boundaries->size.y - footprint.origin.y - footprint.size.y;

	footprint.origin = vector2I::add( footprint.origin, displacement );

	return displacement;
}

graphicObject * graphicObject::isSelected( vector2I mousePointer )
{
	if( mousePointer.x >= footprint.origin.x &&
		mousePointer.x < footprint.origin.x + footprint.size.x &&
		mousePointer.y >= footprint.origin.y &&
		mousePointer.y < footprint.origin.y + footprint.size.y )
		return this;
	return nullptr;
}
