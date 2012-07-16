# POSIX Lib #

POSIX Lib is a set of C++ binding for the POSIX API.

# DISCLAIMER #

This library is not finished and at any point there may be non-working
portions. Consult the bug list at the bottom of the page.


<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## What's Implemented ##

* Child processes through the fork system call
* File descriptor handles
* File descriptor streams
* Pipes
* Shared Memory


<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## Child Processes ##

The child process interface provides a concise and simple way of
forking and performing some functionality.

### Creation ###

To create a child process, simple call the `make_child_process` helper
function, passing in a function or anything that behaves as such.

	auto child_1 = mrr::posix::make_child_process(
	  [] (int n)
	  {
		std::cout << n << std::endl;
	  }
	);


### Forking ###

Simply call the fork member function passing in any paramaters you
wish.

	child_1.fork(5);


### Waiting ###

You have the option of waiting for the child process after you fork
it. By default, it will wait until the child is done or you can pass
in any of the valid options. In the example we will continue without
waiting for the child to finish.

	child_1.wait(WNOHANG);


### Information About the Child ###

Provided are function `pid()` and `status()` which of course return
the pid of the child and the return status.

### Don't Worry About Waiting ###

If you don't wait for the child explicitly, the destructor will do it
for you.



<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## File Descriptor Handle ##

This portion of the library provides a handle for a file
descriptor. On destruction it handles closing the file descriptor and
takes care of errno since close is re-entrant.

## Use With Other Functions ##

The **cast operator** is overloaded for `int` so you can use this
anywhere you would normally use an `int` to represent the file
descriptor.


### Copying and Moving ###

Due to the fact that on destruction the file descriptor will be
closed, making a temporary of a file descriptor may yield unwanted
results such as closing the file descriptor before you really want it
closed.

Currently I have not decided on the best way to remedy this. The
options that I am thinking of are:

1. Provide a move constructor so if moving them into a container, the
   file descriptor copied from will no longer close the file handle, it
   will be closed when the container is destroyed. (Curretly what I am using)

2. By default, a newly created file descriptor will not automatically
   close it's handle, only if an `auto_close` variable is set


<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## File Descriptor Streams ##

Also provided with the library are stream classes for file
descriptors. These class templates will work on any character type but
convenient type aliases `fd_istream` and `fd_ostream` are provided.

### Usage ###

Usage is exactly what you would expect from a stream object.



<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## Pipes ##

A simple interface for creating pipes is provided.  Upon constructing
a `mrr::posix::pipe` object, a pipe is created which you can then
access though the member functions.

### Read and Write End of the Pipe ###

The member functions `read_end()` and `write_end()` will return a
`fd_type` which you can use.


### Closing the Pipe ###

You can close both ends of the pipe at any time with the `close()`
member function. You can also close either one of the ends of the pipe
by calling `close_read_end()` or `close_write_end()` as is common
practice when processes are communicating.


### Creating a New Pipe ###

At any point, you can create a new pipe by calling the `create()`
member function. This closes the read and write end of the pipe
curretly owned by the object and opens a new one.



<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## Shared Memory ##

A simple yet powerful interface is provided for sharing memory between
processes for any non-dynamic data type.


### Single Types ###

If you share memory for a single type (ie. non-string and non-array),
you can use it just like you would a variable of that type.

The following example creates a shared integer that you can attach to
in other processes. 

	mrr::posix::shared_memory<int> number(S_IRUSR | S_IWUSR);
	std::cout << "Segment id = " << number.segment_id() << std::endl;

As you may have guessed the `segment_id()` member function returns the
memory segment to which other processes can attatch.


### Arrays ###

If you make an array, the `begin()`, `end()` and `operator []` are
provided so you can treat it just like you would a normal array.

	mrr::posix::shared_memory<int,10> array(S_IRUSR | S_IWUSR);
	for(int& x : array)
		x = 5;
	array[0] = 0;
	

### Strings ###

Since you can't have dynamic shared memory, string functionality is
provided if you make an array of characters.  In addition to the
operations provided by shared memory arrays, you can use a segment of
string shared memory as you would `std::string`


	mrr::posix::shared_memory<char,1000> string(S_IRUSR | S_IWUSR);
	string = "Hello there!! :)";
	std::cout << "String is \"" << string << "\"\n";


### Attaching Shared Memory in Another Process ##

The default constructor for `mrr::posix::shared_memory` does not
allocate any memory, it simply sets up the data structure to either
create memory later or to attach to another segment of shared memory.

Attaching to another segment is easy as pie! Look how we would attach
to the shared memory we created in the **Single Type** section in
another process that was passed the segment id as `argv[1]`:

*Note: to_type<T>() is a function template that takes a string and
 retuns it as type T*

	int segment_identifier = to_type<int>(argv[1]);
	mrr::posix::shared_memory<int> num;
	num.attach(segment_identifier);
	std::cout << "Num = " << num << std::endl;



<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
# BUGS #

* Automatically closing file descriptors causes unwanted behaviour in
  many cases
* File descriptor streams not fully working, not giving what was
  written to them
