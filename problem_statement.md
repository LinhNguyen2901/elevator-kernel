
Skip To Content
Dashboard
Linh Nguyen
Account
Dashboard
Courses
Groups
Calendar
Inbox
History
Help
COP4610-0001.fa26
Pages
Project 2 - modify the Linux kernel t…

2026 Fall - 1
Home
Announcements
Syllabus
Simple Syllabus
Modules
Assignments
Grades11
Quizzes
People
Project 2 - modify the Linux kernel to support a "pet elevator"
Project 2: Elevator Kernel Module
COP4610: Operating Systems
Project 2.pptx

Elevator.pptx

Example Codes (1).zipDownload Example Codes (1).zip

How to use VMs.pptx

Part 2 - Kernel Modules.pptx

Part 3 - Linked_Lists_Threads_locks.pptx

Part 3- Kernel Compilation.pptx

Starter Files.zipDownload Starter Files.zip

Due: Monday, November 2nd, 2026, at 11:59pm US Eastern

Project Objectives
Gain hands-on experience with system calls: Understand the fundamental concepts and implementation details of system calls by working on various aspects of kernel programming.
Explore kernel programming: Develop a deeper understanding of kernel programming by delving into the intricacies of concurrency and synchronization mechanisms within the kernel.
Enhance knowledge of concurrency and synchronization: Apply the acquired knowledge of concurrency and synchronization in the kernel environment to effectively manage and control concurrent access to shared resources.
Explore different techniques and approaches to ensure thread safety and prevent race conditions.
Project Description
This project aims to provide hands-on experience with system calls, kernel programming, concurrency, synchronization, and elevator scheduling algorithms. It consists of parts that are built upon each other to deepen your knowledge and skills in these areas. You can do the work on your own computer (using a hypervisor like VirtualBox) or the CS Proxmox cluster. Using a virtual machine allows you to modify and boot the Linux kernel with the ability to roll back the bootable image should any bugs crash the OS and render it unusable.

In Part 1, by adding system calls to a C program and verifying their correctness using the strace tool, you gain hands-on experience with system call integration and learn about the available system calls for your machine. This part lays the foundation for understanding how system calls interact with the kernel.

Part 2 takes you further into kernel programming. You will develop a kernel module called my_timer that retrieves and stores the current time using the ktime_get_real_ts64() function. This module creates a proc entry and allows you to read the current time and elapsed time since the last call. This part helps you understand how kernel modules work, how to interact with kernel functions, and how to use proc interfaces for communication.

Part 3 focuses on a more complex task: implementing a scheduling algorithm for a pet elevator. You create a kernel module representing the elevator, supporting operations like starting, stopping, and issuing requests. The module also provides a /proc/elevator entry to display important elevator information. This part challenges you to manage concurrency, synchronization, and efficient scheduling within the kernel environment.

Each part of the project builds upon the knowledge and skills gained in the previous parts. Part 1 introduces you to system calls and their integration, which forms the basis for kernel programming in Part 2. Part 2 expands your understanding of kernel modules and communication through proc interfaces, setting the stage for the more advanced concepts explored in Part 3.

By completing this project, you acquire practical experience in system calls, kernel programming, concurrency, synchronization, and scheduling algorithms. These are essential skills for developing efficient and robust software systems, particularly in operating systems and low-level programming domains. Understanding system calls and kernel programming enables you to interact with and extend the functionality of the operating system, while concurrency, synchronization, and scheduling concepts are crucial for efficient resource management and multitasking in complex systems.

Division of Labor
Submit your division of labor document on Canvas by 10/9/26 at 11:59 PM US Eastern.

Part 1: System-Call Tracing
Please follow the instructions below to complete Part 1 of the task:

Create an empty C program named empty.
Make a copy of the empty program and name it part1.
Add exactly five system calls to the part1 program. You can find the available system calls for your machine in /usr/include/unistd.h.
To verify that you have added the correct number of system calls, execute the following commands in the terminal:
$ gcc -o empty empty.c $ strace -o empty.trace ./empty $ gcc -o part1 part1.c $ strace -o part1.trace ./part1
To minimize the length of the output from strace, try to minimize the use of other function calls (e.g., stdlib.h) in your program.

Note: Running strace on an empty C program will generate some system calls. Therefore, when using strace on your Part 1 code, it should produce five more system calls than the empty program.

Please submit the following files:

empty.c
empty.trace
part1.c
part1.trace
Part 2: Timer Kernel Module
In Unix-like operating systems, the time is often represented as the number of seconds since the Unix Epoch (January 1st, 1970). The task requires creating a kernel module named my_timer that utilizes the function ktime_get_real_ts64() to retrieve the time value, which includes seconds and nanoseconds since the Epoch.

Develop a kernel module called my_timer that calls ktime_get_real_ts64() to obtain the current time. This module should store the time value.
When the my_timer module is loaded using insmod, it should create a proc entry named /proc/timer.
When the my_timer module is unloaded using rmmod, the /proc/timer entry should be removed.
On each read operation of /proc/timer, utilize the proc interface to print both the current time and the elapsed time since the last call (if valid).
To insert a kernel module:

$ sudo insmod my_timer.ko
To remove a kernel module:

$ sudo rmmod my_timer.ko
To check for your kernel module:

$ lsmod | grep my_timer
Example Usage:

$ cat /proc/timer current time: 1518647111.760933999 $sleep 1 $ cat /proc/timer current time: 1518647112.768429998 elapsed time: 1.007495999 $sleep 3 $ cat /proc/timer current time: 1518647115.774925999 elapsed time: 3.006496001 $sleep 5 $ cat /proc/timer current time: 1518647120.780421999 elapsed time: 5.005496000
Part 3: Elevator Module
Your task is to implement a scheduling algorithm for a pet elevator. You will create a kernel module elevator to implement this.

The pet elevator can hold a maximum of 5 pets and cannot exceed a maximum weight of 50 lbs.
Each pet is randomly chosen as a Chihuahua, Pug, Pughuahua, or Dachshund, with equal likelihood.
Pets select their starting floor and destination floor.
Pets board the elevator in first-in, first-out (FIFO) order.
Each type of pet has an assigned weight:
Chihuahua: 3 lbs
Pug: 14 lbs
Pughuahua: 10 lbs
Dachshund: 16 lbs
Pets on floors wait indefinitely to be serviced.
Once a pet boards the elevator, they can only disembark.
The elevator must wait for 2.0 seconds when moving between floors and 1.0 seconds when loading/unloading pets.
The building has floor 1 as the minimum (lobby) and floor 5 as the maximum.
Pets can arrive at any time, and each floor must handle an arbitrary number of them.
Step 1: Add System Calls
Download the latest stable linux-kernel-6.16.x and follow the provided slides to compile your kernel. You can download from the following link:

Download KernelLinks to an external site.

You should move the kernel to your /usr/src/ directory and create a soft link to it as so:

$ sudo ln -s /usr/src/[kernel_version] ~/[kernel_version] $ cd ~/[kernel_version]
This will make it easier to modify from elsewhere instead of editing it in a restricted area.

Modify the kernel by adding three system calls to control the elevator and create pets. Assign the following numbers to the system calls:

548 for start_elevator()
549 for issue_request()
550 for stop_elevator()
The respective function prototypes are as follows:

int start_elevator(void)

The start_elevator() system call activates the elevator for service. From this point forward, the elevator exists and will begin to service requests. It returns 1 if the elevator is already active, 0 for a successful start, and -ERRORNUM if initialization fails or -ENOMEM if it could not allocate memory. The elevator is initialized with the following values:

State: IDLE Current floor: 1 Current load: 0 pets
int issue_request(int start_floor, int destination_floor, int type)

The issue_request() system call creates a request for a pet, specifying the start floor, destination floor, and type of pet (0 = Chihuahua, 1 = Pug, 2 = Pughuahua, 3 = Dachshund). It returns 1 if the request is invalid (e.g., out of range or invalid type) and 0 otherwise.

int stop_elevator(void)

The stop_elevator() system call deactivates the elevator. It stops processing new requests (pets waiting on floors), but it must offload all current pets before complete deactivation. Only when the elevator is empty can it be deactivated (state = OFFLINE). The system call returns 1 if the elevator is already in the process of deactivating and 0 otherwise.

You will need to make these files to add the system calls:

[kernel_version]/syscalls/syscalls.c
[kernel_version]/syscalls/Makefile
You will need to modify the following files to add the system calls:

[kernel_version]/arch/x86/syscalls/syscall_64.tbl
[kernel_version]/include/linux/syscalls.h
[kernel_version]/Makefile
Step 2: Compile and Install the Kernel
You will need to disable certain certificates when adding system calls (see slides).

Compile the kernel with the new system calls. The kernel should be compiled with the following options:

$ make menuconfig $ make -j$(nproc) $ sudo make modules_install $ sudo make install $ sudo reboot
Check that you installed your kernel by typing this into the terminal:

$ uname -r
Note: This is a long process! Make sure to do this part early!

Step 3: Test System Calls
You should test if you successfully added the system calls to your installed kernel with the provided tests in your starter file in the directory part3/tests/

Run the following commands:

$ make $ sudo insmod syscalls.ko $ ./test_syscalls
You should get a message that tells you if you have the system calls installed.

Step 4: Implement Elevator
Implement the elevator kernel module. The module should be named elevator and should be loaded using insmod. The module should be unloaded using rmmod.

Recall that these are the details:

Use linked lists to handle the number of pets per floor/elevator.
Use a kthread to control the elevator movement.
Use a mutex to control shared data access between floor and elevators.
Use kmalloc to allocate dynamic memory for pets.
Step 5: Write to the Proc File
The module must provide a proc entry named /proc/elevator. The following information should be printed (labeled appropriately):

The elevator's movement state:
OFFLINE: when the module is installed but the elevator isn't running (initial state)
IDLE: elevator is stopped on a floor because there are no more pets to service
LOADING: elevator is stopped on a floor to load and unload pets
UP: elevator is moving from a lower floor to a higher floor
DOWN: elevator is moving from a higher floor to a lower floor
The current floor the elevator is on
The elevator's current load (weight)
A list of pets in the elevator
The total number of pets waiting
The total number of pets serviced
For each floor of the building, the following should be printed:

An indicator of whether the elevator is on the floor.
The count of waiting pets.
For each waiting pet, 2 characters indicate the pet type and destination floor.
Example Proc File:

Elevator state: LOADING Current floor: 4 Current load: 30 lbs Elevator status: C5 D2 P4 H1 [ ] Floor 5: 1 H3 [ ] Floor 4: 0 [*] Floor 3: 2 P5 C1 [ ] Floor 2: 1 D4 [ ] Floor 1: 0 Number of pets: 4 Number of pets waiting: 4 Number of pets serviced: 27
C is for Chihuahua, P is for Pug, H is for Pughuahua, D is for Dachshund.

Step 6: Test Elevator
Interact with two provided user-space applications that enable communication with the kernel module:

producer.c: creates pets and issues requests to the elevator
$ ./producer [number_of_pets]
consumer.c: calls the start_elevator() or the stop_elevator() system call.
If the flag is --start, the program starts the elevator.
If the flag is --stop, the program stops the elevator.
You can use the following command to see your elevator in action:

$ watch -n [secs] cat [proc_file]
The producer.c and consumer.c programs will be provided to you.

Part 4: Demo
You will be required to give a 15-minute demo of your elevator kernel module to demonstrate your understanding of the project. A week before the project is due, registration for the demonstration will open. The presentations will take place between 11/3/26 and 11/13/26. You will present your project and then demonstrate the functionality of your elevator. Your presentation will involve a brief overview of your elevator, a discussion regarding your implementation, and then a demonstration of your elevator. You will be required to demonstrate:

Project source code
Kernel system calls
Successful compilation of your elevator
Successful installation of your elevator
Successful execution of system calls
Execution of an arbitrary number of test cases on your elevator for 3 minutes
Successful removal of your elevator
Any demonstration failing to compile/install a portion of the project successfully during the demo will receive a 0 for that portion of the project. Be sure that you can install and remove all kernel modules before attempting to demonstrate. The grader may also choose to question you on project components. You should be able to demonstrate an understanding of the project’s implementation. It should be noted that all source files will be pulled from your GitHub repository.

Helpful Resources
Linux
Modifying the kernelLinks to an external site.


Ubuntu downloadLinks to an external site.

Kernel downloadLinks to an external site.

C
Kernel documentationLinks to an external site.

Assumptions
You can assume that the proc file will only be 10,000 characters long.

You can assume that pets will not head to the same floor they entered from.

The demo will run on your chosen machine with your chosen kernel module.

All code run during the demo will be pulled from your GitHub repository.

Restrictions
You are only allowed to use C.

The list of pets waiting at each floor and the list of pets on the elevator must be stored in a linked list, using your own implementation or linux/list.h.

The pets must be allocated dynamically using kmalloc.

The elevator activity must be controlled within a kthread.

The elevator must use locking around shared data, using mutex.

The elevator is not circular. For example, you cannot go from floor 5 to floor 1 or from floor 1 to floor 5 in one move.

Rubric
A program with compilation errors will get 0 points (you must demonstrate your project in front of us).

Please refer to the standard project guidelines: Project Syllabus

Total points: 100

Implementation (70)
Documentation (30)
Implementation
syscalls tracing (5)
timer kernel module (10)
elevator module (45)
compilation/syscalls [5]
use of mutexes [10]
use of kthreads [10]
general functionality [10]
abides by rules [10]
presentation/demo (10)
presentation [5]
demonstration [5]
Documentation
division of labor (before) (5)
readme.md (15)
division of labor (after) [5]
file listing [3]
description of how to compile and run program [2]
development log for each member [3]
description of group meetings [2]
project structure (5)
source files are properly split into modular components according to functionality [2]
files are appropriately placed in the correct directory following guidelines [2]
modules produced in specified directories [1]
format & organization (5)
include all relevant files [1]
doesn't include binaries, executables, or modules [1]
readability (indentation, white space, max column-width < 100 chars) [1]
submission does not contain unnecessary print statements [1]
variable/functions names indicate purpose with appropriate comments [1]
Deliverables
You should have the following files/directories in your Git repository:

part 1/
empty.c
empty.trace
part1.c
part1.trace
Makefile
part 2/
src/
Makefile
part 3/
src/
Makefile
syscalls.c
Makefile
README.md
For this project, you only need to organize your source code and header files into the src/ folder.

Do not include your final executable(s) in your repository; doing so will result in a deduction!

For part 2 and part 3, your makefile should produce a kernel module, my_timer.ko and elevator.ko respectively.

Submissions
Program submissions will be done through GitHub.

