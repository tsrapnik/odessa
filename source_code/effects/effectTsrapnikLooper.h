#pragma once

#include "effect.h"
#include "listIterator.h"

enum class looperState
{
	stopped,
	playing,
	recording,
	overdubbing
};

class effectTsrapnikLooper: public effect
{
private:
	class channel
	{
	private:
		class track
		{
		private:
			struct block
			{
				float frames[ 256 ];
			};

			list< block* > blocks;
			listIterator< block* > blockIterator;
			int blockIndex;

		public:
			track( int blockCount );

			bool muted;

			void restartTrack();

			void toNextFrame();
			float getCurrentFrame();
			void recordNextFrame( float inputFrame, int* length );
			void updateCurrentFrame( float inputFrame );
		};

		int length;
		list< track* > tracks;
		listIterator< track* > activeTrackPointer;

	public:
		channel();

		float play();
		void record( float frame );
		void dub( float frame );

		void restart();
		void clear();
		void layerOnActiveTrack();
		void removeActiveTrack();
	};

	input* monoInput;
	output* monoOutput;

	list< channel* > channels;
	listIterator< channel* > activeChannelPointer;

	looperState currentState;

public:
	effectTsrapnikLooper( rectangle footprint, colour ownColour );
	~effectTsrapnikLooper();
	virtual void process();
};