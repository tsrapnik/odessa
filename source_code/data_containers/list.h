#pragma once

#include "node.h"

//Double linked list containing elemennts of type. The list has ownership over all its elements and will
//delete them when the list is deleted.
template <typename type>
class list
{
	template <typename otherType> friend class listIterator;
private:
	node<type>* first;
	node<type>* last;

public:
	list();
	~list();

	bool isEmpty();

	template <typename... arguments>
	void prependNew( arguments... initArguments );
	void prependCopy( type newElement );
	template <typename... arguments>
	void appendNew( arguments... initArguments );
	void appendCopy( type newElement );

	void deleteFirst();
	void deleteLast();
	void deleteAll();
};

//Create new list without any elements. At least one node should be added before using any other members, to avoid nullpointer references.
template<typename type>
list<type>::list():
	first( nullptr ),
	last( nullptr )
{}

//Delete the list including all elements.
template<typename type>
list<type>::~list()
{
	deleteAll();
}

//Returns true when the list is empty.
template<typename type>
bool list<type>::isEmpty()
{
	return first == nullptr; //When first and last are both nullpointers, the list is surely empty.
}

//Create element at the beginning of the list, initialized with passed initArguments.
template<typename type>
template <typename... arguments>
void list<type>::prependNew( arguments... initArguments )
{
	if( first == nullptr ) //Check if the list is empty.
	{
		node<type>* newNode = new node<type>( nullptr, nullptr, initArguments... ); //Create the first element.

		first = newNode; //This element is the first and the last.
		last = newNode;
	}
	else
	{
		node<type>* newNode = new node<type>( nullptr, first, initArguments... ); //Create new element and put it before the first element.
		first->previous = newNode;

		first = newNode; //Set new element as first.
	}
}

//Create copy of the passed element at the beginning of the list.
template<typename type>
void list<type>::prependCopy( type newElement )
{
	if( first == nullptr ) //Check if the list is empty.
	{
		node<type>* newNode = new node<type>( nullptr, nullptr, newElement ); //Create the first element.

		first = newNode; //This element is the first and the last.
		last = newNode;
	}
	else
	{
		node<type>* newNode = new node<type>( nullptr, first, newElement ); //Create new element and put it before the first element.
		first->previous = newNode;

		first = newNode; //Set new element as first.
	}
}

//Create element at the end of the list, initialized with passed initArguments.
template<typename type>
template <typename... arguments>
void list<type>::appendNew( arguments... initArguments )
{
	if( last == nullptr ) //Check if the list is empty.
	{
		node<type>* newNode = new node<type>( nullptr, nullptr, initArguments... ); //Create the first element.

		first = newNode; //This element is the first and the last.
		last = newNode;
	}
	else
	{
		node<type>* newNode = new node<type>( last, nullptr, initArguments... ); //Create new element and put it after the last element.
		last->next = newNode;

		last = newNode; //Set new element as last.
	}
}

//Create copy of the passed element at the end of the list.
template<typename type>
void list<type>::appendCopy( type newElement )
{
	if( last == nullptr ) //Check if the list is empty.
	{
		node<type>* newNode = new node<type>( nullptr, nullptr, newElement ); //Create the first element.

		first = newNode; //This element is the first and the last.
		last = newNode;
	}
	else
	{
		node<type>* newNode = new node<type>( last, nullptr, newElement ); //Create new element and put it after the last element.
		last->next = newNode;

		last = newNode; //Set new element as last.
	}
}

//Delete the first element of the list.
template<typename type>
void list<type>::deleteFirst()
{
	node<type>* second = first->next; //Find the second element.

	if( second != nullptr ) //Check if the first isn't the only element in the list.
	{
		second->previous = nullptr; //Make sure the second element points to no previous element.
		node<type>* condemned = first; //Remember the element that should be deleted.
		first = second; //Set second element as new first.
		delete condemned;
	}
	else //If it is the only element delete it and make first and last element nullpointers.
	{
		delete first;
		first = last = nullptr;
	}
	
}

//Delete the last element of the list.
template<typename type>
void list<type>::deleteLast()
{
	node<type>* secondLast = last->previous;

	if( secondLast != nullptr ) //Check if the last element isn't the only element in the list.
	{
		last->previous->next = nullptr; //Make sure the second last element points to no next element.
		node<type>* condemned = first; //Remember the element that should be deleted.
		last = last->previous; //Set second last element as new last.
		delete condemned;
	}
	else //If it is the only element delete it and make first and last element nullpointers.
	{
		delete last;
		first = last = nullptr;
	}
}

//Delete all elements in the list.
template<typename type>
void list<type>::deleteAll()
{
	while( first != nullptr ) //Delete last element until there are no more elements.
	{
		deleteLast();
	}
}