SECTIONS
{
    /*is start address, where the arm processor will start executing code.*/
	. = 0x80000;

    /*contains all code.*/
	.text :
	{
		. = ALIGN(4);
		__text_start = .;
		*(.text.start)
		*(.text)
		 __text_end = .;
	}

    /*contains all constant values not included in the text section, for example string literals.*/
	.rodata :
	{
       	. = ALIGN(4);
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
   	}
    
    /*contains pointers to functions that initialize the data in the data section.*/
	.init_array : {
       	. = ALIGN(4);
        __init_start = .;
		*(.init_array*)
	    __init_end = .;
	}

    /*contains static data that is initialized to a specific value.*/
	.data :
	{
		. = ALIGN(4);
		 __data_start = .;
		*(.data)
		 __data_end = .;
	}

    /*contains static data that is initialized to zero.*/
	.bss :
	{
		. = ALIGN(4);
		 __bss_start = .;
		*(.bss)
		 __bss_end = .;
   	}

    /*stacks for all cores in all exception levels.*/
	.stack_core_0 :
	{
        . = ALIGN(16);
        __stack_core_0_start = .;
        . = . + 512;
        __stack_core_0_el0 = .;
        . = . + 16384;
        __stack_core_0_el1 = .;
        . = . + 512;
        __stack_core_0_el2 = .;
        __stack_core_0_end = .;    
    }

	.stack_core_1 :
	{
        . = ALIGN(16);
        __stack_core_1_start = .;
        . = . + 512;
        __stack_core_1_el0 = .;
        . = . + 16384;
        __stack_core_1_el1 = .;
        . = . + 512;
        __stack_core_1_el2 = .;
        __stack_core_1_end = .;    
    }

	.stack_core_2 :
	{
        . = ALIGN(16);
        __stack_core_2_start = .;
        . = . + 512;
        __stack_core_2_el0 = .;
        . = . + 16384;
        __stack_core_2_el1 = .;
        . = . + 512;
        __stack_core_2_el2 = .;
        __stack_core_2_end = .;    
    }

	.stack_core_3 :
	{
        . = ALIGN(16);
        __stack_core_3_start = .;
        . = . + 512;
        __stack_core_3_el0 = .;
        . = . + 16384;
        __stack_core_3_el1 = .;
        . = . + 512;
        __stack_core_3_el2 = .;
        __stack_core_3_end = .;    
    }

    /*contains space for all runtime allocatable data.*/
	.heap :
	{
     	. = ALIGN(4);
     	__heap_start = .;
	}
}
