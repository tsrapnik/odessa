#pragma once

//this is the layout of the ram memory.
namespace memory_map
{
	//mapping of code, which is loaded to memory by the boot procedure.
	void* const code_start = ( void* const )0x40000000;
	void* const code_end = ( void* const )0x4b100000;

	//mapping of stack (the stack counts down, which is why end and start are switched).
	void* const stack_end = code_end;
	void* const stack_start = ( void* const )0x4ed40000;

	//mapping of the metadata for the general heap (lists all available memory).
	void* const heap_meta_start = stack_start;
	void* const heap_meta_end = ( void* const )0x4ef40000;

	//mapping of the general heap.
	void* const heap_start = heap_meta_end;
	void* const heap_end = ( void* const )0x4ff40000;

	//mapping of the metadata for the looper heap (lists all available memory).
	void* const looper_heap_meta_start = heap_end;
	void* const looper_heap_meta_end = ( void* const )0x50000000;

	//mapping of the looper heap (this memory is only used for the looper effect, as it needs quick memory assignments).
	void* const looper_heap_start = looper_heap_meta_end;
	void* const looper_heap_end = ( void* const )0x80000000;
}