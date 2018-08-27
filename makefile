#this makefile is intended to act as a generic c++ compiler. it will look for all c++ files in the specified
#source_code subdirectory and all its subdirectories and compile them to a single image file. the makefile
#was built specifically for raspberry pi 3, but can be used for different purposes with only a few minor
#adjustements. adjust the assignments in the ###definitions### part to change compiler, compiler flags and
#names of the folders that are to be used. the compiler only reads source code from the source_directory and
#writes compiled stuff to the automatically generated object_directory. this keeps the source directories
#clean.

###definitions###

#comment next line to show all commands and feedback in the terminal. leave uncommented to get only minor feedback.
hide = @

#compiler definitions and flags.
prefix = aarch64-linux-gnu-

assembler = $(prefix)gcc
compiler = $(prefix)g++
linker = $(prefix)ld
object_dumper = $(prefix)objdump
object_copier = $(prefix)objcopy
cpp_filter = $(prefix)c++filt

architecture = -march=armv8-a -mtune=cortex-a53 -mlittle-endian -mcmodel=small
assembler_flags = $(architecture)
cpp_flags = $(architecture) -Wall -Ofast -fno-exceptions -fno-rtti -std=c++14 -fno-builtin -nostdlib -nostdinc

#definitions of the source code directory and the object directory, where all generated files will be stored.
source_directory = source_code
object_directory = compiled_code

#naming of some specific source and object files.
linker_description = linker_description
image_name = kernel8

###makefile code###

#find all subdirectories in the source_directory and use them to generate the same directory tree for in the
#object_directory and to generate all include directives for compiling.
source_directories = $(shell find $(source_directory) -type d)
object_directories = $(patsubst $(source_directory)%,$(object_directory)%,$(source_directories))
include_directories = $(patsubst %,-I%,$(source_directories))

#find all paths of the source files and use them to generate equivalent paths for the object and dependency files.
sources = $(shell find $(source_directory) -name "*.cpp")
objects = $(patsubst %.cpp,%.o,$(sources))
objects := $(patsubst %.S,%.o,$(objects))
objects := $(patsubst $(source_directory)%,$(object_directory)%,$(objects))
dependencies = $(patsubst %.o,%.d,$(objects))

#build image file and all its dependencies.
all: $(object_directory)/directories_exist $(object_directory)/$(image_name).img

#make all subirectories where the object files will go, otherwise you will get a missing folder complaint when
#compiling. add an empty file in the root of the object_directory, so you have a dependency and have to run this
#rule only once.
$(object_directory)/directories_exist:
	$(info creating directories for object files.)
	$(hide)mkdir -p $(object_directories)
	$(hide)touch $(object_directory)/directories_exist

#link all object and startup files to one elf file and convert it to a binary image file. remove the elf file to avoid
#cluttering the object directory.
$(object_directory)/$(image_name).img: $(source_directory)/$(linker_description).ld $(objects)
	$(info link all objects and generate the image file: $(image_name).img.)
	$(hide)$(linker) -o $(object_directory)/$(image_name).elf -T $(source_directory)/$(linker_description).ld $(objects) $(source_directory)/boot/startup.o $(source_directory)/boot/exceptionstub.o $(source_directory)/boot/delay_loop.o
	$(hide)$(object_copier) $(object_directory)/$(image_name).elf -O binary $(object_directory)/$(image_name).img
	$(hide)rm $(object_directory)/$(image_name).elf

#include all generated dependency files, which include rules for recompiling a source file when one of its included
#header files has changed.
-include $(dependencies)

#compile all S source files in the project.
#the first rule outputs which file is compiled. if the hide option is active this is the only line that will be
#displayed, to avoid an overload of information. the second rule is a generic rule for actually compiling the object
#file from the source file. the third rule generates a dependecy file that is later used for checking changes in
#header files that are included by the source file. the last four rules are to modify the dependency file. the
#target in this file is generated in the form "object.o", which wont work cause we need the full path, so it is
#converted to the form "subdirectory/subdirectory/object.o".
$(object_directory)/%.o: $(source_directory)/%.S
	$(info compile source file: $<.)
	$(hide)$(assembler) $(assembler_flags) $(include_directories) -c -o $@ $<
	$(hide)$(assembler) $(include_directories) -MM $(source_directory)/$*.S > $(object_directory)/$*.d
	$(hide)cp -f $(object_directory)/$*.d $(object_directory)/$*.d.tmp
	$(hide)sed -e 's|.*:|$(object_directory)/$*.o:|' < $(object_directory)/$*.d.tmp > $(object_directory)/$*.d
	$(hide)sed -e 's/.*://' -e 's/\\$$//' < $(object_directory)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(object_directory)/$*.d
	$(hide)rm -f $(object_directory)/$*.d.tmp

#compile all cpp source files in the project the same way as the S files.
$(object_directory)/%.o: $(source_directory)/%.cpp
	$(info compile source file: $<.)
	$(hide)$(compiler) $(cpp_flags) $(include_directories) -c -o $@ $<
	$(hide)$(compiler) $(include_directories) -MM $(source_directory)/$*.cpp > $(object_directory)/$*.d
	$(hide)cp -f $(object_directory)/$*.d $(object_directory)/$*.d.tmp
	$(hide)sed -e 's|.*:|$(object_directory)/$*.o:|' < $(object_directory)/$*.d.tmp > $(object_directory)/$*.d
	$(hide)sed -e 's/.*://' -e 's/\\$$//' < $(object_directory)/$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >> $(object_directory)/$*.d
	$(hide)rm -f $(object_directory)/$*.d.tmp

#remove everything generated by the make command, so next time everything is recompiled.
clean:
	$(info removed entire $(object_directory) folder.)
	$(hide)rm -r $(object_directory)

#define all targets that aren't actually files.
.PHONY: all clean
