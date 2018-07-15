#pragma once

template <typename type>
struct node
{
	type data;
	node* previous;
	node* next;

	template <typename... arguments>
	node( node* previous, node* next, arguments... dataInitializer ):
		data( dataInitializer... ),
		previous( previous ),
		next( next )
	{}

	template <typename... arguments>
	node( node* previous, node* next, type newData ):
		data( newData ),
		previous( previous ),
		next( next )
	{}
};