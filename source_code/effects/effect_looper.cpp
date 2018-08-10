#include "effectTsrapnikLooper.h"

effectTsrapnikLooper::effectTsrapnikLooper( rectangle footprint, colour ownColour ):
	effect::effect( footprint, ownColour ),
	activeChannelPointer( listIterator<channel*>( &channels ) )
{
	monoInput = new input( rectangle( vector2I( 0, 120 ), vector2I( 30, 40 ) ),
						   colour( 255, 0, 0, 255 ) );
	monoOutput = new output( rectangle( vector2I( 170, 120 ), vector2I( 30, 40 ) ),
							 colour( 0, 255, 0, 255 ) );
	addInput( monoInput );
	addOutput( monoOutput );
}

effectTsrapnikLooper::~effectTsrapnikLooper()
{}

void effectTsrapnikLooper::process()
{
	float frame = 0.0;
	channel* activeChannel = activeChannelPointer.getDataCopy();

	switch( currentState )
	{
		case looperState::stopped:
		{
			break;
		}
		case looperState::recording:
		{
			//Clear all current tracks of the channel, restart channel and add a first track (and point activeTrackPointer to it) before activating this state.
			activeChannel->record( monoInput->getFrame() );
			break;
		}
		case looperState::playing:
		{
			//If there are tracks they should not be empty.
			frame += activeChannel->play();
			break;
		}
		case looperState::overdubbing:
		{
			//If there are tracks they should not be empty.
			frame += activeChannel->play();
			activeChannel->dub( monoInput->getFrame() );
			break;
		}
	}
}

effectTsrapnikLooper::channel::channel():
	activeTrackPointer( listIterator<track*>( &tracks ) )
{}

//Play all unmuted tracks.
float effectTsrapnikLooper::channel::play()
{
	float frame = 0.0f;

	listIterator<track*> currentElement( &tracks );
	for( currentElement.toFirst(); !currentElement.atEnd(); currentElement++ )
	{
		channel::track* currentTrack = currentElement.getDataCopy();
		currentTrack->toNextFrame();
		if( !currentTrack->muted )
		{
			frame += currentTrack->getCurrentFrame();
		}
	}

	return frame;
}

//Record the input.
void effectTsrapnikLooper::channel::record( float frame )
{
	channel::track* activeTrack = activeTrackPointer.getDataCopy();
	activeTrack->recordNextFrame( frame, &length );
}

//Dub active track with the input.
void effectTsrapnikLooper::channel::dub( float frame )
{
	channel::track* activeTrack = activeTrackPointer.getDataCopy();
	activeTrack->updateCurrentFrame( frame );
}

//Sets all tracks to the first frame.
void effectTsrapnikLooper::channel::restart()
{
	listIterator<track*> currentElement( &tracks );
	for( currentElement.toFirst(); !currentElement.atEnd(); currentElement++ )
	{
		track* currentTrack = currentElement.getDataCopy();
		currentTrack->restartTrack();
	}
}

//Delete all tracks.
void effectTsrapnikLooper::channel::clear()
{
	tracks.deleteAll();
	activeTrackPointer.toFirst();
	length = 0;
}

//Add a new track on top of the active track and make this the new active track.
void effectTsrapnikLooper::channel::layerOnActiveTrack()
{
	track* newTrack = new track( length );
	tracks.appendNew( newTrack );
}

//Delete the active track and set the previous track as the active one.
void effectTsrapnikLooper::channel::removeActiveTrack()
{
	activeTrackPointer.deleteCurrent();
}

//Create empty track with "blockCount" blocks.
effectTsrapnikLooper::channel::track::track( int blockCount ):
	blockIterator( &blocks ),
	blockIndex( 0 )
{
	for( int i = 0; i < blockCount; i++ )
	{
		blocks.appendNew();
	}
}

//Sets track to it's first frame.
void effectTsrapnikLooper::channel::track::restartTrack()
{
	blockIterator.toFirst();
	blockIndex = 0;
}

void effectTsrapnikLooper::channel::track::toNextFrame()
{
	blockIndex++;
	if( blockIndex > 255 ) //Load next block when current block is finished.
	{
		blockIndex = 0;
		blockIterator++;
		if( blockIterator.atEnd() ) //Go back to first block when all blocks are traversed.
			blockIterator.toFirst();
	}
}

float effectTsrapnikLooper::channel::track::getCurrentFrame()
{
	track::block* currentBlock = blockIterator.getDataCopy();
	return currentBlock->frames[ blockIndex ];
}

//Fills the next frame with the inputFrame-data, creates a new block if necessary.
void effectTsrapnikLooper::channel::track::recordNextFrame( float inputFrame, int* length )
{
	blockIndex++;
	if( blockIndex > 255 ) //Create new block when current block is finished.
	{
		blockIndex = 0;
		blocks.appendNew();
		blockIterator++;
		length++;
	}
	track::block* currentBlock = blockIterator.getDataCopy(); //Fill the next track.
	currentBlock->frames[ blockIndex ] = inputFrame;
}

void effectTsrapnikLooper::channel::track::updateCurrentFrame( float inputFrame )
{
	track::block* currentBlock = blockIterator.getDataCopy();
	currentBlock->frames[ blockIndex ] = inputFrame;
}
