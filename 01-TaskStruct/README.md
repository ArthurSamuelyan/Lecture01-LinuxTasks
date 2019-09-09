## How to build and run

# Compile and load the kernel module

Of course, everything here is done as root.

1. `# cd module`

2. Update the `MODULE_DIR` variable in Makefile. 
It shall be a correct absolute path to `module` directory.

3. `# make`

4. `# ./task-struct-cdev.init start` - automatically loads 
the module. After this there is to appear a new char device
`task-struct-cdev`.

4. `cd ..`

# Compile and run test program

1. `$ make`

2. `$ ./01-task-struct` - press any key until it stops.

3. `$ dmesg | tail -n40` - see the output from kernel module.

# Unload the module

Of course as root.

1. `# cd module`

2. `# ./task-struct-cdev.init stop`
