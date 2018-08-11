#pragma once

#include "effect.h"
#include "list_iterator.h"

enum class looper_state
{
	stopped,
	playing,
	recording,
	overdubbing
};

class effect_tsrapnik_looper: public effect
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
			list_iterator< block* > block_iterator;
			int block_index;

		public:
			track( int block_count );

			bool muted;

			void restart_track();

			void to_next_frame();
			float get_current_frame();
			void record_next_frame( float input_frame, int* length );
			void update_current_frame( float input_frame );
		};

		int length;
		list< track* > tracks;
		list_iterator< track* > active_track_pointer;

	public:
		channel();

		float play();
		void record( float frame );
		void dub( float frame );

		void restart();
		void clear();
		void layer_on_active_track();
		void remove_active_track();
	};

	input* mono_input;
	output* mono_output;

	list< channel* > channels;
	list_iterator< channel* > active_channel_pointer;

	looper_state current_state;

public:
	effect_tsrapnik_looper( rectangle footprint, colour own_colour );
	~effect_tsrapnik_looper();
	virtual void process();
};