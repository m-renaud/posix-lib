# POSIX Lib #

POSIX Lib is a set of C++ binding for the POSIX API.

# DISCLAIMER #

This library is not finished and at any point there may be non-working
portions. Consult the bug list at the bottom of the page.


<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## What's Implemented ##

* Child processes through the fork system call
* File descriptor handles
* Pipes
* File descriptor streams
* Select on file descriptors
* Shared memory


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
descriptor.  It is mostly used interally for the other portions of the
library but you are free to use it if necessary. With the design of
the library you will very infrequently find yourself needing to
manipulate file descriptors explicitly.

## Use With Other Functions ##

The **cast operator** is overloaded for `int` so you can use this
anywhere you would normally use an `int` to represent the file
descriptor.



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
## File Descriptor Streams ##

Also provided with the library are stream classes for file
descriptors. The base class is `basic_fd_ostream<CharT,CharTraits>`
but type aliases `fd_istream` and `fd_ostream` are provided which
default to `char` as the underlying type, similar to how the standard
provides `std::istream` and `std::ostream`.

### Usage ###

Usage is exactly what you would expect from a stream object.

### Nuances ###

When using one of these stream objects with a pipe, care must be taken
to write some type of *end of record* character. This is due to the
fact that when reading out of the pipe with an `fd_istream` object,
the associated `operator >>` overload for a type needs to be sure that
there is no more data coming.

As a simple example, consider writing a number down a pipe as follows:

	mrr::posix::pipe mypipe;
	mrr::posix::fd_ostream write_to_pipe(mypipe.write_end());
	mrr::posix::fd_istream read_from_pipe(mypipe.write_end());
	int number;

	write_to_pipe << 12345;
	read_from_pipe >> number;

What you will notice is that the `read_from_pipe >> number` will
hang. This is because there is nothing to indicate that there is no
more information coming down the pipe.  Both operations are in the
same process so obviously we know that all the information has been
written, but if the two ends of the pipe are in separate processes, we
have no such guarantee.

Suppose that "123" had been written to the pipe, if the
`read_from_pipe` operation simply returned after extracting the number
`123` I think you would be a little upset. You can easily remedy this
by adding some type of *end of record* as mentioned previously. For
example, if the last thing you are writing is a number, provide some
non-numeric character such as a space or newline and it will behave
exactly how you expect. There is probably a more elegant solution, I
just have not it yet. If anyone reading this has any suggestions feel
free to contact me.


<!-- =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- -->
## Select on File Descriptors ##

This hangle provides an easy way to handle multiple file descriptors
and perform actions based on action from one of them.

### Watching A File Descriptor ###

You can instruct the select object to watch multiple file descriptors
by calling the `watch` member function. There are two versions of this
funcion:

	void watch(int fd);

This version is to be used if you want to manually handle the file
descriptors that become active. It is to be used with the `listen`
member function.

	void watch(int fd, std::function<void(int)> f);

This version is to be used when you want to carry out an action based
on what file descriptor becomes active. This is the one that you will
probably use most often and is to be used with the
`listen_with_action` member function.

### Listening for Action ###

After you instruct the select handler which file handlers you wish for
it to handle, you simply tell it to wait for one of those file
descriptors to become active. As with the `watch` member functions,
there are two versions:

	int listen();



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
