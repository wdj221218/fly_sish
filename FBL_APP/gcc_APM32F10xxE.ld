/*!
 * @file       gcc_APM32F10xxE.ld
 *
 * @brief      Linker script for APM32F10xxE Device with
 *             512KByte FLASH, 128KByte RAM
 *
 * @version    V1.0.0
 *
 * @date       2022-12-01
 *
 * @attention
 *
 *  Copyright (C) 2022 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Entry Point */
ENTRY(Reset_Handler)

_fbl_rom_base =  0x08000000;
_fbl_rom_size =  0x00002000;
_sbl_rom_base =  0x08002000;
_sbl_rom_size =  0x00004FF0;
_sbl_mark_base = 0x08006FFC;
_sbl_mark_size = 0x00000004;
_app_rom_base =  0x08007000;
_app_rom_size =  0x00031FF0;
_app_mark_base = 0x08038FFC;
_app_mark_size = 0x00000004;
_ram_base      = 0x20000000;
_ram_size      = 0x00010000;
_end_stack     = 0x2000fff0;
_heap_size     = 0x200;
_stack_size    = 0x400;

MEMORY
{
USED_FLASH (rx)      : ORIGIN = _fbl_rom_base,  LENGTH = _fbl_rom_size
SBL_FLASH (rx)      : ORIGIN = _sbl_rom_base,  LENGTH = _sbl_rom_size
SBL_MARK  (rx)      : ORIGIN = _sbl_mark_base, LENGTH = _sbl_mark_size
APP_FLASH (rx)      : ORIGIN = _app_rom_base,  LENGTH = _app_rom_size
APP_MARK  (rx)      : ORIGIN = _app_mark_base, LENGTH = _app_mark_size
RAM (xrw)           : ORIGIN = _ram_base,      LENGTH = _ram_size
}

SECTIONS
{
  .apm32_isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.apm32_isr_vector))
    KEEP(*(.target_info))
    . = ALIGN(4);
  } >USED_FLASH
  
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    *(.glue_7)
    *(.glue_7t)
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;
  } >USED_FLASH

  
  .mark_data :
  {
    KEEP(*(.mark_data))
  } >APP_MARK
  
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >USED_FLASH

  .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) } >USED_FLASH
  .ARM : {
    __exidx_start = .;
    *(.ARM.exidx*)
    __exidx_end = .;
  } >USED_FLASH

  .preinit_array     :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array*))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  } >USED_FLASH
  .init_array :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array*))
    PROVIDE_HIDDEN (__init_array_end = .);
  } >USED_FLASH
  .fini_array :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT(.fini_array.*)))
    KEEP (*(.fini_array*))
    
    PROVIDE_HIDDEN (__fini_array_end = .);
  } >USED_FLASH

  _start_address_init_data = LOADADDR(.data);

  .data : 
  {
    . = ALIGN(4);
    _start_address_data = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _end_address_data = .;
  } >RAM AT> USED_FLASH

  
  . = ALIGN(4);
  .bss :
  {

    _start_address_bss = .;
    __bss_start__ = _start_address_bss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _end_address_bss = .;
    __bss_end__ = _end_address_bss;
  } >RAM

  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _heap_size;
    . = . + _stack_size;
    . = ALIGN(8);
  } >RAM

  /DISCARD/ :
  {
    libc.a ( * )
    libm.a ( * )
    libgcc.a ( * )
  }

  .ARM.attributes 0 : { *(.ARM.attributes) }
}

