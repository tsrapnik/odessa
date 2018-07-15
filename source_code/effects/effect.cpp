#include "effect.h"

#include "analogButton.h"

effect::effect( rectangle footprint, colour ownColour ) :
	graphicObject( footprint, ownColour )
{}

effect::~effect()
{}

void effect::setOwnScreen( screen* ownScreen )
{
	graphicObject::setOwnScreen( ownScreen );

	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
		inputsIterator.getDataCopy()->setOwnScreen( ownScreen );

	listIterator< output* > outputsIterator( &outputs );
	for( outputsIterator.toFirst(); !outputsIterator.atEnd(); outputsIterator++ )
		outputsIterator.getDataCopy()->setOwnScreen( ownScreen );

	listIterator< button* > buttonsIterator( &buttons );
	for( buttonsIterator.toFirst(); !buttonsIterator.atEnd(); buttonsIterator++ )
		buttonsIterator.getDataCopy()->setOwnScreen( ownScreen );
}

void effect::addButton( button* newButton )
{
	newButton->setBoundaries( &this->footprint );
	buttons.appendCopy( newButton );
}

void effect::addInput( input* newInput )
{
	newInput->setBoundaries( &this->footprint );
	inputs.appendCopy( newInput );
}

void effect::addOutput( output* newOutput )
{
	newOutput->setBoundaries( &this->footprint );
	outputs.appendCopy( newOutput );
}

//Calculates all outputs and marks them as ready. Returns false when the inputs weren't ready yet.
bool effect::update()
{
	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
	{
		if( !inputsIterator.getDataCopy()->frameReady() )
			return false; //Return false when one of the inputs is not ready, so no processing is done.
	}

	process();

	return true;
}

void effect::draw()
{
	graphicObject::draw();

	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
		inputsIterator.getDataCopy()->draw();

	listIterator< output* > outputsIterator( &outputs );
	for( outputsIterator.toFirst(); !outputsIterator.atEnd(); outputsIterator++ )
		outputsIterator.getDataCopy()->draw();

	listIterator< button* > buttonsIterator( &buttons );
	for( buttonsIterator.toFirst(); !buttonsIterator.atEnd(); buttonsIterator++ )
		buttonsIterator.getDataCopy()->draw();
}

void effect::drawConnections()
{
	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
		inputsIterator.getDataCopy()->drawConnection();
}

vector2I effect::move( vector2I displacement )
{
	displacement = graphicObject::move( displacement );

	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
		inputsIterator.getDataCopy()->move( displacement );

	listIterator< output* > outputsIterator( &outputs );
	for( outputsIterator.toFirst(); !outputsIterator.atEnd(); outputsIterator++ )
		outputsIterator.getDataCopy()->move( displacement );

	listIterator< button* > buttonsIterator( &buttons );
	for( buttonsIterator.toFirst(); !buttonsIterator.atEnd(); buttonsIterator++ )
		buttonsIterator.getDataCopy()->move( displacement );

	return displacement;
}

graphicObject * effect::selectButton( vector2I mousePointer )
{
	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
	{
		graphicObject* selection = inputsIterator.getDataCopy()->isSelected( mousePointer );
		if( selection != nullptr )
			return selection;
	}

	listIterator< output* > outputsIterator( &outputs );
	for( outputsIterator.toFirst(); !outputsIterator.atEnd(); outputsIterator++ )
	{
		graphicObject* selection = outputsIterator.getDataCopy()->isSelected( mousePointer );
		if( selection != nullptr )
			return selection;
	}

	listIterator< button* > buttonsIterator( &buttons );
	for( buttonsIterator.toFirst(); !buttonsIterator.atEnd(); buttonsIterator++ )
	{
		graphicObject* selection = buttonsIterator.getDataCopy()->isSelected( mousePointer );
		if( selection != nullptr )
			return selection;
	}

	return nullptr;
}

output* effect::selectOutput( vector2I mousePointer )
{
	listIterator< output* > outputsIterator( &outputs );
	for( outputsIterator.toFirst(); !outputsIterator.atEnd(); outputsIterator++ )
	{
		output* selection = ( output* )outputsIterator.getDataCopy()->isSelected( mousePointer );
		if( selection != nullptr )
			return selection;
	}

	return nullptr;
}

input* effect::selectInput( vector2I mousePointer )
{
	listIterator< input* > inputsIterator( &inputs );
	for( inputsIterator.toFirst(); !inputsIterator.atEnd(); inputsIterator++ )
	{
		input* selection = ( input* )inputsIterator.getDataCopy()->isSelected( mousePointer );
		if( selection != nullptr )
			return selection;
	}

	return nullptr;
}
