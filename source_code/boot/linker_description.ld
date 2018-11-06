SECTIONS
{
	. = 0x80000;

	.text :
	{
		. = ALIGN(4);
		 __text_start = .;
		*(.text)
		 __text_end = .;
	}

	.rodata :
	{
       	. = ALIGN(4);
        __rodata_start = .;
        *(.rodata)
        __rodata_end = .;
   	}

	.data :
	{
		. = ALIGN(4);
		 __data_start = .;
		*(.data)
		 __data_end = .;
	}

	.bss :
	{
		. = ALIGN(4);
		 __bss_start = .;
		*(.bss)
		 __bss_end = .;
   	}

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

	.heap :
	{
     	. = ALIGN(4);
     	__heap_start = .;
	}
}
