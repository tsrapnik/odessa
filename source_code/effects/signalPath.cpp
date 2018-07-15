#include "signalPath.h"

signalPath::signalPath( screen* ownScreen )
{
	this->ownScreen = ownScreen;
}

signalPath::~signalPath()
{}

void signalPath::update()
{
	listIterator< effect* > currentEffect( &effects );
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		currentEffect.getDataCopy()->update();
	}
}

void signalPath::draw()
{
	ownScreen->clear( colour( 10, 20, 125, 255 ) );

	listIterator< effect* > currentEffect( &effects );
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		currentEffect.getDataCopy()->draw();
	}
	
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		currentEffect.getDataCopy()->drawConnections();
	}
}

void signalPath::addEffect( effect* newEffect )
{
	newEffect->setBoundaries( ownScreen->getFootprint() );
	newEffect->setOwnScreen( ownScreen );
	effects.appendCopy( newEffect );
}

effect* signalPath::selectEffect( vector2I mousePointer )
{
	listIterator< effect* > currentEffect( &effects );
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		effect* selection = ( effect* )currentEffect.getDataCopy()->isSelected( mousePointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}

graphicObject * signalPath::selectButton( vector2I mousePointer )
{
	listIterator< effect* > currentEffect( &effects );
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		graphicObject* selection = currentEffect.getDataCopy()->selectButton( mousePointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}

output * signalPath::selectOutput( vector2I mousePointer )
{
	listIterator< effect* > currentEffect( &effects );
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		output* selection = nullptr;
		selection = currentEffect.getDataCopy()->selectOutput( mousePointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}

input* signalPath::selectInput( vector2I mousePointer )
{
	listIterator< effect* > currentEffect( &effects );
	for( currentEffect.toFirst(); !currentEffect.atEnd(); currentEffect++ )
	{
		input* selection = nullptr;
		selection = currentEffect.getDataCopy()->selectInput( mousePointer );
		if( selection != nullptr )
		{
			return selection;
		}
	}

	return nullptr;
}