/* Linker script for kernel integration with static library
   This script ensures no conflicts when linking with kernel code
*/

ENTRY(_start)

SECTIONS
{
    /* Keep all input sections for the library
       Prefix with .lib_ to avoid conflicts with kernel sections
    */
    .lib_text :
    {
        KEEP(*(.text))
        KEEP(*(.text.*))
    } :text

    .lib_rodata :
    {
        KEEP(*(.rodata))
        KEEP(*(.rodata.*))
        KEEP(*(.constdata))
        KEEP(*(.constdata.*))
    } :rodata

    .lib_data :
    {
        KEEP(*(.data))
        KEEP(*(.data.*))
    } :data

    .lib_bss (NOLOAD) :
    {
        KEEP(*(.bss))
        KEEP(*(.bss.*))
        KEEP(*(COMMON))
    } :bss

    /* Discard unnecessary sections */
    /DISCARD/ :
    {
        *(.note)
        *(.note.*)
        *(.comment)
        *(.gnu.version*)
    }
}

/* Memory layout hints (customize based on your kernel layout)
MEMORY
{
    CODE (rx)  : ORIGIN = 0x40000000, LENGTH = 64M
    DATA (rw)  : ORIGIN = 0x44000000, LENGTH = 32M
}
*/
