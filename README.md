# liboca
An open-source library to implement the Open Control Architecture.

You can read about OCA here: http://ocaalliance.com/technology/specifications/

Right now there isn't much to see here, as we're still getting our heads around
OCA, and how best to structure this lib. What we do have is a basic shell project,
with a placeholder example, and a placeholder unit test.

If you know about OCA, are are keen to learn and help out, get in touch.

Watch this space.

## Dependencies
*	python >= 2.7.x
*	boost (headers only)
*	gtest 1.7 (Google Test)

Note that we assume that the dependencies can be found one of two ways:
1.	For those that use `pkg-config`, you must have `pkg-config` installed.
2.	Any other dependencies (eg the two above), must be installed somewhere that
	is in your default lib and include paths. ie we don't want to keep any funky
	`-I` or `-L` lines in our build files.

## Building
	$ git clone https://github.com/guysherman/liboca
	$ cd liboca
	$ ./waf configure
	$ ./waf

## Testing
Assuming you're in the root folder, and you've had a successful build:
	$ build/tests/all


## Coding Standards
This project will adhere to the Ardour coding standards, because I agree whole-heartedly
with them, and probably many of our target developers come from that community. So, to save
myself the effort of writing a coding standard, I'll refer you to the excellent work of
Paul Davis here: http://ardour.org/styleguide.html. While you're there, check out Ardour
and why not donate to it :)
