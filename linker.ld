ENTRY(_start)

SECTIONS
{
    . = 1M;
    
    /* Multiboot header first at 1MB */
    .multiboot :
    {
        *(.multiboot)
    }
    
    /* Then text section with _start */
    .text :
    {
        *(.text)
    }
    
    .rodata :
    {
        *(.rodata)
    }
    
    .data :
    {
        *(.data)
    }
    
    .bss :
    {
        *(.bss)
    }
}