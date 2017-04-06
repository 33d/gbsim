cmake_minimum_required (VERSION 2.8.4)
project (gbsim)

# Don't include simavr - the sim_core_decl.h in there was auto-generated,
# so use the manually created one in include instead
include_directories(
	simavr/cores
	simavr/sim
	include
)

# We should benefit from SSE
if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
	set(CMAKE_C_FLAGS "-O3 -march=pentium4")
endif()

set(SOURCES
	# Add only the cores we'll actually use (and see include/sim_core_config.h)
	simavr/cores/sim_mega328.c
	simavr/cores/sim_megax8.c
	
	simavr/sim/avr_uart.c
	simavr/sim/avr_flash.c
	simavr/sim/avr_bitbang.c
	simavr/sim/avr_twi.c
	simavr/sim/avr_watchdog.c
	simavr/sim/sim_core.c
	simavr/sim/avr_timer.c
	simavr/sim/avr_extint.c
	simavr/sim/sim_irq.c
	simavr/sim/avr_spi.c
	simavr/sim/avr_eeprom.c
	simavr/sim/avr_ioport.c
	simavr/sim/sim_gdb.c
	simavr/sim/avr_lin.c
	simavr/sim/sim_avr.c
	simavr/sim/avr_usb.c
	simavr/sim/sim_elf.c
	simavr/sim/sim_vcd_file.c
	simavr/sim/sim_hex.c
	simavr/sim/sim_cycle_timers.c
	simavr/sim/avr_adc.c
	simavr/sim/sim_io.c
	simavr/sim/sim_interrupts.c
	
	gbsim/sdl_display.c
	gbsim/gb_keypad.c
	gbsim/main.c
	gbsim/pcd8544.c
)

file(MAKE_DIRECTORY build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY build)

add_executable(gbsim ${SOURCES})
if(WIN32)
  target_link_libraries(gbsim wsock32 ws2_32)
endif()

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(LIBELF REQUIRED libelf)
include_directories(
	${SDL2_INCLUDE_DIRS}
	${LIBELF_INCLUDE_DIRS}
)
target_link_libraries(gbsim
	${SDL2_LIBRARIES} 
	${LIBELF_LIBRARIES}
)
