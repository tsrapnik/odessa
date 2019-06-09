#pragma once

#include "type_definitions.h"
#include "node.h"

//linked list containing elements of given type. the list has ownership over all its elements and will
//delete them when the list is deleted.
template <typename type>
class list
{
    template <typename other_type>
    friend class list_iterator;

    private:
    node<type>* first;
    node<type>* last;
    u32 size;

    public:
    list();
    ~list();

    bool is_empty() const;
    u32 get_size() const;

    template <typename... arguments>
    void prepend_new(arguments... init_arguments);
    void prepend_copy(type new_element);
    template <typename... arguments>
    void append_new(arguments... init_arguments);
    void append_copy(type new_element);

    void delete_first();
    void delete_last();
    void delete_all();

    type get_copy_first();
    type& get_reference_first();
    type get_copy_last();
    type& get_reference_last();
};

//todo: check if comment below is still valid and if so fix it.
//create new list without any elements. at least one node should be added before using any other members, to avoid nullpointer references.
template <typename type>
list<type>::list() :
    first(nullptr),
    last(nullptr),
    size(0u)
{
}

//delete the list including all elements.
template <typename type>
list<type>::~list()
{
    delete_all();
}

//returns true when the list is empty.
template <typename type>
bool list<type>::is_empty() const
{
    return first == nullptr; //when first and last are both nullpointers, the list is surely empty.
}

//returns how many elements are in the list.
template <typename type>
u32 list<type>::get_size() const
{
    return size;
}

//create element at the beginning of the list, initialized with passed init_arguments.
template <typename type>
template <typename... arguments>
void list<type>::prepend_new(arguments... init_arguments)
{
    if(first == nullptr) //check if the list is empty.
    {
        node<type>* new_node = new node<type>(nullptr, nullptr, init_arguments...); //create the first element.

        first = new_node; //this element is the first and the last.
        last = new_node;
    }
    else
    {
        node<type>* new_node = new node<type>(nullptr, first, init_arguments...); //create new element and put it before the first element.
        first->previous = new_node;

        first = new_node; //set new element as first.
    }
    size++;
}

//create copy of the passed element at the beginning of the list.
template <typename type>
void list<type>::prepend_copy(type new_element)
{
    if(first == nullptr) //check if the list is empty.
    {
        node<type>* new_node = new node<type>(nullptr, nullptr, new_element); //create the first element.

        first = new_node; //this element is the first and the last.
        last = new_node;
    }
    else
    {
        node<type>* new_node = new node<type>(nullptr, first, new_element); //create new element and put it before the first element.
        first->previous = new_node;

        first = new_node; //set new element as first.
    }
    size++;
}

//create element at the end of the list, initialized with passed init_arguments.
template <typename type>
template <typename... arguments>
void list<type>::append_new(arguments... init_arguments)
{
    if(last == nullptr) //check if the list is empty.
    {
        node<type>* new_node = new node<type>(nullptr, nullptr, init_arguments...); //create the first element.

        first = new_node; //this element is the first and the last.
        last = new_node;
    }
    else
    {
        node<type>* new_node = new node<type>(last, nullptr, init_arguments...); //create new element and put it after the last element.
        last->next = new_node;

        last = new_node; //set new element as last.
    }
    size++;
}

//create copy of the passed element at the end of the list.
template <typename type>
void list<type>::append_copy(type new_element)
{
    if(last == nullptr) //check if the list is empty.
    {
        node<type>* new_node = new node<type>(nullptr, nullptr, new_element); //create the first element.

        first = new_node; //this element is the first and the last.
        last = new_node;
    }
    else
    {
        node<type>* new_node = new node<type>(last, nullptr, new_element); //create new element and put it after the last element.
        last->next = new_node;

        last = new_node; //set new element as last.
    }
    size++;
}

//delete the first element of the list.
template <typename type>
void list<type>::delete_first()
{
    node<type>* second = first->next; //find the second element.

    if(second != nullptr) //check if the first isn't the only element in the list.
    {
        second->previous = nullptr; //make sure the second element points to no previous element.
        node<type>* condemned = first; //remember the element that should be deleted.
        first = second; //set second element as new first.
        delete condemned;
    }
    else //if it is the only element delete it and make first and last element nullpointers.
    {
        delete first;
        first = last = nullptr;
    }
    size--;
}

//delete the last element of the list.
template <typename type>
void list<type>::delete_last()
{
    node<type>* second_last = last->previous;

    if(second_last != nullptr) //check if the last element isn't the only element in the list.
    {
        last->previous->next = nullptr; //make sure the second last element points to no next element.
        node<type>* condemned = first; //remember the element that should be deleted.
        last = last->previous; //set second last element as new last.
        delete condemned;
    }
    else //if it is the only element delete it and make first and last element nullpointers.
    {
        delete last;
        first = last = nullptr;
    }
    size--;
}

//delete all elements in the list.
template <typename type>
void list<type>::delete_all()
{
    while(first != nullptr) //delete last element until there are no more elements.
    {
        delete_last();
    }
}

//get a copy of the first element.
template <typename type>
type list<type>::get_copy_first()
{
    return first->data;
}

//get a reference of the first element.
template <typename type>
type& list<type>::get_reference_first()
{
    return first->data;
}

//get a copy of the last element.
template <typename type>
type list<type>::get_copy_last()
{
    return last->data;
}

//get a reference of the last element.
template <typename type>
type& list<type>::get_reference_last()
{
    return last->data;
}