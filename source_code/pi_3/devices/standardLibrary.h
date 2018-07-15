#pragma once

//This is the layout of the ram memory.
namespace memoryMap
{
	//Mapping of code, which is loaded to memory by the boot procedure.
	void* const codeStart = ( void* const )0x40000000;
	void* const codeEnd = ( void* const )0x4B100000;

	//Mapping of stack (the stack counts down, which is why end and start are switched).
	void* const stackEnd = codeEnd;
	void* const stackStart = ( void* const )0x4ED40000;

	//Mapping of the metadata for the general heap (lists all available memory).
	void* const heapMetaStart = stackStart;
	void* const heapMetaEnd = ( void* const )0x4EF40000;

	//Mapping of the general heap.
	void* const heapStart = heapMetaEnd;
	void* const heapEnd = ( void* const )0x4FF40000;

	//Mapping of the metadata for the looper heap (lists all available memory).
	void* const looperHeapMetaStart = heapEnd;
	void* const looperHeapMetaEnd = ( void* const )0x50000000;

	//Mapping of the looper heap (this memory is only used for the looper effect, as it needs quick memory assignments).
	void* const looperHeapStart = looperHeapMetaEnd;
	void* const looperHeapEnd = ( void* const )0x80000000;
}