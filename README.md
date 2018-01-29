# stf
A Very Simple Test Framework created with a goal to test your favorite algorithms, prototypes and other experiments without having to write the same old boring boiler plate code and generate makefiles. The framework uses cmake as the build system, so most of the build heavylifting is done for you(hopefully). This helps you focus on what is most important to you.

### Dependencies
* cmake - make sure to install any version greater than 3.0
* g++/clang   - version 4.8 or above(preferably the latest from stable)
* git

### How to use this test framework
The best part is that, the tool is simple to use and should be mostly self explanatory for the most part.

Use --help for usage instructions

```
stf	[--list] - List all available tests
	  [--all] - Run all available tests
	  [--test=group.test_name] - Run a test(test_name) from family(group)
	  [--profile] - Profiles time take for a test in milliseconds resolution,
			 only valid with the --test option
	  [--help] - Print help
```

Choose to run a single test from a group, all tests from a single group, or all tests from all groups. Also get a test report at the end...

```
----------------------
Total tests:  1
Passed:       1
Failed:       0
Skipped:      0
----------------------
```

### How to add new test groups
- steps to follow...
