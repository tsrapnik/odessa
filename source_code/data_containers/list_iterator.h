#pragma once

#include "list.h"

//creates an iterator which can iterate through a specified list and access its elements.
template <typename type>
class list_iterator
{
private:
	list<type>* parent;
	node<type>* current;

public:
	list_iterator( list<type>* parent );
	~list_iterator();

	void to_first();
	void to_last();
	bool at_end();
	
	list_iterator<type> operator++( int );
	list_iterator<type> operator--( int );
	type get_data_copy();
	type* get_data_pointer();
	void delete_current();
	template <typename... arguments>
	void insert_new_before( arguments... init_arguments );
	void insert_copy_before( type new_element );
	template <typename... arguments>
	void insert_new_after( arguments... init_arguments );
	void insert_copy_after( type new_element );
};

//create new iterator which points to the first element of the given parent list.
template<typename type>
list_iterator<type>::list_iterator( list<type>* parent ):
	parent( parent ),
	current( parent->first )
{
}

template<typename type>
list_iterator<type>::~list_iterator()
{
}

//point iterator to the first element in the list.
template<typename type>
void list_iterator<type>::to_first()
{
	current = parent->first;
}

//point iterator to the last element in the list.
template<typename type>
void list_iterator<type>::to_last()
{
	current = parent->last;
}

//check if the iterator is past the first or last element in the list (the iterator then points to a nullpointer).
template<typename type>
bool list_iterator<type>::at_end()
{
	return current == nullptr;
}

//point iterator to the next element in the list.
template<typename type>
list_iterator<type> list_iterator<type>::operator++( int )
{
	current = current->next;
	return *this;
}

//point iterator to the previous element in the list.
template<typename type>
list_iterator<type> list_iterator<type>::operator--( int )
{
	current = current->previous;
	return *this;
}

//get a copy of the data of the element the iterator points to.
template<typename type>
type list_iterator<type>::get_data_copy()
{
	return current->data;
}

//get a pointer to the data of the element the iterator points to.
template<typename type>
type* list_iterator<type>::get_data_pointer()
{
	return &( current->data );
}

//delete the element the iterator points to and points the iterator to the previous element.
template<typename type>
void list_iterator<type>::delete_current()
{
	if( current == parent->first ) //if current is the first in the list, use the appropriate parent function.
		parent->delete_first();
	else if( current == parent->last ) //if current is the last in the list, use the appropriate parent function.
		parent->delete_last();
	else
	{
		current->previous->next = current->next; //point previous and next node to each other.
		current->next->previous = current->previous;

		node<type>* condemned = current; //remember the node that should be deleted.

		current = current->previous; //set prevoius node as the new current one.

		delete condemned;
	}
}

//create element before the element the iterator points to, initialised with passed init_arguments.
//after calling this member, the iterator keeps pointing to the original element.
template<typename type>
template <typename... arguments>
void list_iterator<type>::insert_new_before( arguments... init_arguments )
{
	if( current == parent->first ) //if current is the first in the list, use the appropriate parent function.
		parent->prepend( init_arguments... );
	else
	{
		node<type>* new_node = new node<type>( current->previous, current, init_arguments... ); //create new node and put it before the current node.
		current->previous->next = new_node;
		current->previous = new_node;
	}
}

//create copy of the passed new_element before the element the iterator points to.
//after calling this member, the iterator keeps pointing to the original element.
template<typename type>
void list_iterator<type>::insert_copy_before( type new_element )
{
	if( current == parent->first ) //if current is the first in the list, use the appropriate parent function.
		parent->prepend( new_element );
	else
	{
		node<type>* new_node = new node<type>( current->previous, current, new_element ); //create new node and put it before the current node.
		current->previous->next = new_node;
		current->previous = new_node;
	}
}

//create element after the element the iterator points to, initialised with passed init_arguments.
//after calling this member, the iterator keeps pointing to the original element.
template<typename type>
template <typename... arguments>
void list_iterator<type>::insert_new_after( arguments... init_arguments )
{
	if( current == parent->last ) //if current is the last in the list, use the appropriate parent function.
		parent->append( init_arguments... );
	else
	{
		node<type>* new_node = new node<type>( current, current->next, init_arguments... ); //create new node and put it after the current node.
		current->next->previous = new_node;
		current->next = new_node;
	}
}

//create copy of the passed new_element after the element the iterator points to.
//after calling this member, the iterator keeps pointing to the original element.
template<typename type>
void list_iterator<type>::insert_copy_after( type new_element )
{
	if( current == parent->last ) //if current is the last in the list, use the appropriate parent function.
		parent->append( new_element );
	else
	{
		node<type>* new_node = new node<type>( current, current->next, new_element ); //create new node and put it after the current node.
		current->next->previous = new_node;
		current->next = new_node;
	}
}