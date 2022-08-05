# Unit Tests

This project includes a unit test suite, built with [catch2](https://github.com/catchorg/Catch2/tree/v2.x)

## Building the tests

from root project folder:

``` $ make test ```

## Running the tests

``` $ ./test/bin/test <test-tag> ```

**```<test-tag>``` is an optional parameter for running a subset of the tests--more on this below

## Writing tests

### Unit tests for firmware

Before writing any tests, it may be important to consider how testable your code is.  External libraries can make it challenging to write good tests because we can't necessarily control their behavior.  On top of this, much of the code in this project is dependent on libraries which are not intended to be compiled and run on your computer!

With that said, there are workarounds for these problems which allow us to test our own code independently of the core systems it depends on.  Just know that if you are writing new tests, you may need to 'trick' the compiler in some way, either by providing your own implementations of external headers/symbols or by using dependency injection.  This project employs both methods.

(see **On Dependencies** below)

### Steps

* create a new test file in the tests folder with ``` #include "catch2.hpp" ``` at the top
  * the tests folder should mirror the structure of src, so if you're writing tests for src/System/MyClass.cpp, the test should be tests/System/MyClassTest.cpp
* create as many test cases as you want using the test macros provided by catch2, for example:
``` c++
TEST_CASE("MyClass constructor test", "[MyClass][ConstructorTests]") {
    MyClass myClass(16);
    REQUIRE( myClass.getNum() == 16 );
}
```
* build and run the new test as described above


### Catch2 Basics

#### Test Cases
In the example above, the TEST_CASE macro is used.  This registers the test under Catch2.

#### Test Descriptions and Tags

The first argument is a description of the test which will be displayed if the test fails.  The second provides a set of tags which you can use to group tests together.  For example, if you have many test cases in different files tagged with [ConstructorTests], you can run them all with:
```
./bin/test [ConstructorTests]
```

#### More on Catch2

Catch2 provides a variety of test functions and assertion macros for writing different kinds of tests.  For more information, please refer to the simple tutorial [here](https://github.com/catchorg/Catch2/blob/v2.x/docs/tutorial.md#top) and the more detailed reference [here](https://github.com/catchorg/Catch2/blob/v2.x/docs/tutorial.md#top)

## On Dependencies

**UnitTestLib**: Particle Device OS is a huge library which is intended to be compiled and run on Particle hardware.  For simplicity's sake, our tests aren't compiled with Device OS source code.  We instead use our own UnitTestLib submodule, which includes implementations of some basic Device OS classes and functions.  Feel free to add anything you need to this repo.

**Dependency injection**: this essentially just refers to wrapping external dependencies in an abstract class (see ```src/System/CanBus``` and its derived classes as an example).  This is great for testing because it allows us to create Mock implementations of these external dependencies which we can explicitly use for testing (see ```test/tests/Mocks```).

**some unit test suites provide APIs for creating mocks without having to hard-code them.  Catch2 does not, so we need to create our own hard-coded mock classes
