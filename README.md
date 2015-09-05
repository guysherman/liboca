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
*	libboost-system1.55.0, libboost-system1.55.0-dev
*	gtest 1.7 (Google Test)

## Required tools
In order to verify the quality of this code, we use the following tools, so if
you want to contribute, it is worth having them, because PRs that don't pass these
checks will be rejected without any further examination.
*	cppcheck (the build runs this each time you compile)
*	valgrind

Note that we assume that the dependencies can be found one of two ways:
1.	For those that use `pkg-config`, you must have `pkg-config` installed.
2.	Any other dependencies (eg the two above), must be installed somewhere that
	is in your lib and include paths when you run waf. ie we don't want to keep any funky
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

## Branching
All development work will be integrated to the `develop` branch, with features on `feature_<name>` branches and bug fixes on `issue_<id>` branches. So, if you are working
in a fork of this repository and want to contribute, it would be super helpful if you could do two things:
1.	Don't commit anything to the `develop` branch in your fork, keep that as a way to bring changes in from upstream.
2.	When it comes time to send changes back to us, pull the latest `develop` from us, into `develop` in your fork, and then rebase your changes onto that. Then send us a PR.

This is how the git commands would look:
	$ git checkout develop
	$ git pull upstream develop		# assuming you've added us as a remote called upstream
	$ git checkout feature_mine
	$ git rebase develop
	$ git push origin feature_mine

What you end up with is your feature branch, fast-forwardable over our develop.

For good info on rebasing see here: https://www.atlassian.com/git/tutorials/merging-vs-rebasing/conceptual-overview
