#this makefile is intended to act as a generic c++ compiler. it will look for all c++ files in the specified
#source_z subdirectory and all its subdirectories and compile them to a single image file. the makefile
#was built specifically for raspberry pi 3, but can be used for different purposes with only a few minor
#adjustements. adjust the assignments in the ###definitions### part to change compiler, compiler flags and
#names of the folders that are to be used. the compiler only reads source code from the source_directory and
#writes compiled stuff to the automatically generated object_directory. this keeps the source directories
#clean.

###definitions###

#comment next line to show all commands and feedback in the terminal. leave uncommented to get only minor feedback.
hide = @

#compiler definitions and flags.
prefix = aarch64-elf-

assembler = $(prefix)gcc
compiler = $(prefix)g++
linker = $(prefix)gcc
object_copier = $(prefix)objcopy
object_dumper = $(prefix)objdump
object_mapper = $(prefix)nm

compiler_flags = -Wall -O3 -mcpu=cortex-a53+fp+simd -ffreestanding -nostartfiles -std=c++17 -mstrict-align -fno-tree-loop-vectorize -fno-tree-slp-vectorize -Wno-nonnull-compare -fno-exceptions -fno-rtti -fno-builtin -nostdlib -nostdinc

#definitions of the source code directory and the object directory, where all generated files will be stored.
source_directory = source_code
object_directory = compiled_code
boot_source_directory = $(source_directory)/boot
boot_object_directory = $(object_directory)/boot

#naming of some specific source and object files.
boot_source_name = startup
linker_description_name = linker_description
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
objects := $(patsubst $(source_directory)%,$(object_directory)%,$(objects))
dependencies = $(patsubst %.o,%.d,$(objects))
boot_source = $(boot_source_directory)/$(boot_source_name).S
boot_object = $(boot_object_directory)/$(boot_source_name).o
linker_description = $(boot_source_directory)/$(linker_description_name).ld
image = $(object_directory)/$(image_name).img
image_elf = $(object_directory)/$(image_name).elf
image_dump = $(object_directory)/$(image_name).txt
image_map = $(object_directory)/$(image_name).map
object_directory_tree = $(object_directory)/directories_exist

#build image file and all its dependencies.
all: $(object_directory_tree) $(image)

#make all subirectories where the object files will go, otherwise you will get a missing folder complaint when
#compiling. add an empty file in the root of the object_directory, so you have a dependency and have to run this
#rule only once.
$(object_directory_tree):
	$(info creating directories for object files.)
	$(hide)mkdir -p $(object_directories)
	$(hide)touch $(object_directory_tree)

#link all object and startup files to one elf file and convert it to a binary image file. remove the elf file to avoid
#cluttering the object directory.
$(image): $(linker_description) $(boot_object) $(objects)
	$(info link all objects and generate the image file: $(image).)
	$(hide)$(linker) $(compiler_flags) -o $(image_elf) -T $(linker_description) $(boot_object) $(objects) -Wl,--build-id=none
	$(hide)$(object_copier) $(image_elf) -O binary $(image)
	$(hide)$(object_dumper) -d $(image_elf) > $(image_dump)
	$(hide)$(object_mapper) -n $(image_elf) > $(image_map)
	$(hide)rm $(image_elf)

#include all generated dependency files, which include rules for recompiling a source file when one of its included
#header files has changed.
-include $(dependencies)

#compile the boot file.
$(boot_object): $(boot_source)
	$(info compile boot source file: $<.)
	$(hide)$(assembler) $(compiler_flags) $(include_directories) -c -o $@ $<

#compile all source files in the project.
#the first rule outputs which file is compiled. if the hide option is active this is the only line that will be
#displayed, to avoid an overload of information. the second rule is a generic rule for actually compiling the object
#file from the source file. the third rule generates a dependecy file that is later used for checking changes in
#header files that are included by the source file. the last rule is to modify the dependency file. the
#target in this file is generated in the form "object.o", which wont work cause we need the full path, so it is
#converted to the form "subdirectory/subdirectory/object.o".
$(object_directory)/%.o: $(source_directory)/%.cpp
	$(info compile source file: $<.)
	$(hide)$(compiler) $(compiler_flags) $(include_directories) -c -o $@ $<
	$(hide)$(compiler) $(include_directories) -MM $(source_directory)/$*.cpp > $(object_directory)/$*.d
	$(hide)sed -i 's|.*:|$(object_directory)/$*.o:|' $(object_directory)/$*.d

#remove everything generated by the make command, so next time everything is recompiled.
clean:
	$(info removed entire $(object_directory) folder.)
	$(hide)rm -r $(object_directory)

#define all targets that aren't actually files.
.PHONY: all clean
