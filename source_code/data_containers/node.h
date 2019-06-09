#pragma once

#include "uart.h"
extern uart* a_uart;

template <typename type>
struct node
{
	type data;
	node *previous;
	node *next;

	template <typename... arguments>
	node(node *previous, node *next, arguments... data_initializer) : data(data_initializer...),
																	  previous(previous),
																	  next(next)
	{
	}

	template <typename... arguments>
	node(node *previous, node *next, type new_data) : data(new_data),
													  previous(previous),
													  next(next)
	{
		a_uart->write("in new node.\r\n"); //todo: remove.
	}
};