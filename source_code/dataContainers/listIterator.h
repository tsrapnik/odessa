#pragma once

#include "list.h"

//Creates an iterator which can iterate through a specified list and access its elements.
template <typename type>
class listIterator
{
private:
	list<type>* parent;
	node<type>* current;

public:
	listIterator( list<type>* parent );
	~listIterator();

	void toFirst();
	void toLast();
	bool atEnd();
	
	listIterator<type> operator++( int );
	listIterator<type> operator--( int );
	type getDataCopy();
	type* getDataPointer();
	void deleteCurrent();
	template <typename... arguments>
	void insertNewBefore( arguments... initArguments );
	void insertCopyBefore( type newElement );
	template <typename... arguments>
	void insertNewAfter( arguments... initArguments );
	void insertCopyAfter( type newElement );
};

//Create new iterator which points to the first element of the given parent list.
template<typename type>
listIterator<type>::listIterator( list<type>* parent ):
	parent( parent ),
	current( parent->first )
{
}

template<typename type>
listIterator<type>::~listIterator()
{
}

//Point iterator to the first element in the list.
template<typename type>
void listIterator<type>::toFirst()
{
	current = parent->first;
}

//Point iterator to the last element in the list.
template<typename type>
void listIterator<type>::toLast()
{
	current = parent->last;
}

//Check if the iterator is past the first or last element in the list (the iterator then points to a nullpointer).
template<typename type>
bool listIterator<type>::atEnd()
{
	return current == nullptr;
}

//Point iterator to the next element in the list.
template<typename type>
listIterator<type> listIterator<type>::operator++( int )
{
	current = current->next;
	return *this;
}

//Point iterator to the previous element in the list.
template<typename type>
listIterator<type> listIterator<type>::operator--( int )
{
	current = current->previous;
	return *this;
}

//Get a copy of the data of the element the iterator points to.
template<typename type>
type listIterator<type>::getDataCopy()
{
	return current->data;
}

//Get a pointer to the data of the element the iterator points to.
template<typename type>
type* listIterator<type>::getDataPointer()
{
	return &( current->data );
}

//Delete the element the iterator points to and points the iterator to the previous element.
template<typename type>
void listIterator<type>::deleteCurrent()
{
	if( current == parent->first ) //If current is the first in the list, use the appropriate parent function.
		parent->deleteFirst();
	else if( current == parent->last ) //If current is the last in the list, use the appropriate parent function.
		parent->deleteLast();
	else
	{
		current->previous->next = current->next; //Point previous and next node to each other.
		current->next->previous = current->previous;

		node<type>* condemned = current; //Remember the node that should be deleted.

		current = current->previous; //Set prevoius node as the new current one.

		delete condemned;
	}
}

//Create element before the element the iterator points to, initialised with passed initArguments.
//After calling this member, the iterator keeps pointing to the original element.
template<typename type>
template <typename... arguments>
void listIterator<type>::insertNewBefore( arguments... initArguments )
{
	if( current == parent->first ) //If current is the first in the list, use the appropriate parent function.
		parent->prepend( initArguments... );
	else
	{
		node<type>* newNode = new node<type>( current->previous, current, initArguments... ); //Create new node and put it before the current node.
		current->previous->next = newNode;
		current->previous = newNode;
	}
}

//Create copy of the passed newElement before the element the iterator points to.
//After calling this member, the iterator keeps pointing to the original element.
template<typename type>
void listIterator<type>::insertCopyBefore( type newElement )
{
	if( current == parent->first ) //If current is the first in the list, use the appropriate parent function.
		parent->prepend( newElement );
	else
	{
		node<type>* newNode = new node<type>( current->previous, current, newElement ); //Create new node and put it before the current node.
		current->previous->next = newNode;
		current->previous = newNode;
	}
}

//Create element after the element the iterator points to, initialised with passed initArguments.
//After calling this member, the iterator keeps pointing to the original element.
template<typename type>
template <typename... arguments>
void listIterator<type>::insertNewAfter( arguments... initArguments )
{
	if( current == parent->last ) //If current is the last in the list, use the appropriate parent function.
		parent->append( initArguments... );
	else
	{
		node<type>* newNode = new node<type>( current, current->next, initArguments... ); //Create new node and put it after the current node.
		current->next->previous = newNode;
		current->next = newNode;
	}
}

//Create copy of the passed newElement after the element the iterator points to.
//After calling this member, the iterator keeps pointing to the original element.
template<typename type>
void listIterator<type>::insertCopyAfter( type newElement )
{
	if( current == parent->last ) //If current is the last in the list, use the appropriate parent function.
		parent->append( newElement );
	else
	{
		node<type>* newNode = new node<type>( current, current->next, newElement ); //Create new node and put it after the current node.
		current->next->previous = newNode;
		current->next = newNode;
	}
}