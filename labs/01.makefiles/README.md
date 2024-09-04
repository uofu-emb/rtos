# Lab 1: make and compilation toolchains
In this lab, we will be examining C compilation for embedded systems. We will examine the function of the Unix make tool, and its place in the C ecosystem, and its general utility in the Unix environment. We will also learn the role of various tools in the embedded toolchain.

## Learning objectives
- Identify the major steps of the C toolchain and explain their roles.
- Write Makefile recipe to build a target.
- Use substitution patterns to make flexible targets
- Explain difference between phony and regular targets
- Express target dependency relationships in recipes.
- Analyze the advantages and disadvantages of the make tool.
- Practice command line usage.

# Prelab
We will specifically be using the GNU implementation of make and the GNU compiler collection.
The manual is a lot like a datasheet, you don't have to read the whole thing.
Looking at my browser history, I have looked at 38 out 178 sections, so even for complicated tasks you just need to know the relevant information.

## Reading
### GNU make manual
Read the following sections of the manual (https://www.gnu.org/software/make/manual/html_node/index.html)
- The introduction to chapter 2, and sections 2.1, 2.2, 2.3, 2.4, and 2.7 (skip sections 2.5 and 2.6)
- Section 3.1
- Section 4.1, 4.2, 4.7
- Section 10.5.1, 10.5.2, 10.5.3

### Compilation toolchains
Read this overview of tool names.
https://labs.dese.iisc.ac.in/embeddedlab/hardware-and-software-setup/

Read chapter 4 from Programming Embedded Systems.
https://learning.oreilly.com/library/view/programming-embedded-systems/0596009836/ch04.html

You will need to log in through the Marriot Library.
https://utah-primoprod.hosted.exlibrisgroup.com/primo-explore/fulldisplay?docid=UUU_ALMA51594934220002001&context=L&vid=UTAH&lang=en_US&search_scope=EVERYTHING&adaptor=Local%20Search%20Engine&tab=everything&query=any,contains,Programming%20Embedded%20Systems&offset=0

### Other resources

Detailed slides that may be of use.
https://www.slideshare.net/slideshow/cu-cmp445-lec13introductiontotoolchain1/48396432#1

# Lab
<a title="ShiinaKaze, CC BY-SA 4.0 &lt;https://creativecommons.org/licenses/by-sa/4.0&gt;, via Wikimedia Commons" href="https://commons.wikimedia.org/wiki/File:GCC_Translation_Diagram.jpg"><img width="512" alt="GCC Translation Diagram" src="https://upload.wikimedia.org/wikipedia/commons/thumb/b/b6/GCC_Translation_Diagram.jpg/512px-GCC_Translation_Diagram.jpg?20240331041930"></a>https://opensource.com/article/22/5/gnu-c-compiler

## Project setup
### Tasks
- In a new VSCode window, select "Open Folder", and create a new folder. **Do not create a Pico project**, we need a blank folder.
- Initialize a git repository with `git init`.
- Create a new file called `Makefile`.
- Create a new file called `main.c`
- Copy the following into `main.c`
```
int main(void) {
    while(1);
    return 0;
}
```
- Commit the files.
- Create a new repository on Github.
- Add the Github repository as a remote `git remote add github git@github.com:uofu-embed/my-lab2`
- Push to the remote.

## Basic Makefile rules

Open the terminal, and run `make` (with no arguments).
```
% make
make: *** No targets.  Stop.
```

Next try to run `make hello.txt`

```
% make hello.txt
make: *** No rule to make target `hello.txt'.  Stop.
```

1. What do these error messages mean? Briefly discuss with your partner.

Add the following to `Makefile`.
```
hello.txt:
    echo "hello world!" > hello.txt
```

1. Which part is the *rule*, the *target*, and the *recipe*?

Run make again. A file should be created.
```
% make hello.txt
echo "hello world!" > hello.txt
% ls
Makefile        hello.txt       main.c
```

1. Predict what will happen if you run the same command again.

```
% make hello.txt
make: `hello.txt' is up to date.
```

Make checks to see if any of the *dependencies* of the rule have changed, and only runs the recipe if there are changes.

Before wrapping up this section, we need to do some house keeping.

Run `git status`. You'll notice that hello.txt is shown as a new file.
This file is a *build artifact*, something that is generated from the source code.
You should not commit build artifacts.
If you add this file to source control, the next time you build, it will change, forcing you to add it again.
This will cause horrendous problems in your history, as the artifacts are constantly changing.

You may think that this is a benefit - maybe someone wants a prebuilt version of the code!
But what if I forget to commit the build artifacts  everytime, or commit an artifact that includes changes that aren't committed?
This is why we use CI/CD! They can get the artifact from the build server, and are guaranteed to have an up-to-date copy from a fresh repository.

Create a new file called `.gitignore`, and add a line with `hello.txt`
Now run `git status`. The build artifact is no longer listed as a new file. You will see the ignore file.

Commit everything, including the `.gitignore` file, before moving to the next section.

## Dependencies
Our basic rule is pretty trivial. Let's introduce the first member of our toolchain.

```
PICO_TOOLCHAIN_PATH?=~/.pico-sdk/toolchain/13_2_Rel1
CPP=$(PICO_TOOLCHAIN_PATH)/bin/arm-none-eabi-cpp

main.i: main.c
    $(CPP) main.c > main.i
```
The C pre-processor (cpp) performs macro and pre-processor directive expansion. Note that we put the path to the tool binary into a variable to make it easier to specify the location if working in a different environment. Many of these tools have standard variables which you can set in your shell environment.

The `PICO_TOOLCHAIN_PATH` environment variable is normally set by the VSCode plugin. You can set this variable to your `~/.profile`, `~/.bashrc`, `~/.zshrc` if you want to have access to the path elsewhere in the system. You can also specify the variable as part of invoking make, e.g. `FOO=bar make`.

Try to run `make main.i`. What output do you predict if you run it a second time?

Modify the `main.c` file, adding a comment or changing the return value. What do you predict will happen if you run make again?

The rule for `main.i` *depends* on the file `make.c` Make will track the modified status of files, and will only run a rule 1) if the target does not exist or 2) if any of the dependencies have been modified. Rules may have multiple dependencies, and a change in any dependency forces the rule being run.

Commit everything before moving to the next section.

## Cleaning up
A common task is to remove all the build artifacts to start with a clean build. Let's add a rule to remove the two artifacts we have.

```
clean:
    rm -f main.i hello.txt
```
Run `make clean`. What do you predict will happen if you run it again?

The `clean` rule does not generate the target file.
But this breaks our concept of a rule, which specifies a recipe to generate a target!
However, this is what we want - everytime you invoke make with the `clean` target, it should run the recipe and remove all the files.
We need to specify that the clean rule does not generate a target file.
Add the following.

```
.PHONY: clean
```

`.PHONY` is a special target - its dependencies will not generate a target file. We will use phony targets for helper rules and aliases.


Commit everything before moving to the next section.

## Implicit rules.

Let's continue with the compiler and assembler.

```
CC=$(PICO_TOOLCHAIN_PATH)/bin/arm-none-eabi-gcc
AS=$(PICO_TOOLCHAIN_PATH)/bin/arm-none-eabi-as

main.s: main.i
    $(CC) -S main.i

main.o: main.s
    $(AS) main.s -o main.o
```

`cc` is the historic name of the the C compiler (the gcc name is for the GNU version). `as` is the name for the assembler.

The C compiler works on individual .c files. There is one .o file for every .c file.
This saves us time waiting for compilation to finish, only recompiling files that have changed.
The .s file contains the assembly language - textual and generally human readable.
The .o file contains the instructions and data in binary as the processor will read them.

Let's add a second .c file.
Create another file with an empty function in it.
We just need something that will compile.
What is your strategy for compiling the second file?

Our projects will consist of many files, and adding rules for every file is impractical.
What if we had a way to define a rule in so that any .c file can be compiled?

```
main.o: main.s
    $(AS) main.s -o main.o

second.o: second.s
    $(AS) second.s -o second.o
```

We can see that the difference between the rules is the dependency and target filenames.
We also have a convention where the input file and the output file share a name with different extensions.
We can create an *implicit rule* using a *pattern*.
Modify your rule to look like the following.
```
%.o: %.s
    $(AS) second.s -o second.o
```
The % symbol is our pattern.
Any file which ends in .s will match the pattern and the rule will be applied.
This solves the problem of matching target name to dependency.
We still have to deal with the recipe.
The recipe uses a different syntax than the target, and the pattern is not available.
Modify your rule.

```
%.o: %.s
    $(AS) $< -o $@
```

`$@` and `$<` are special *automatic variables*.
They are the target and the first dependency respectively, generated when make matches the rule.
There are [many variables available to you](https://www.gnu.org/software/make/manual/html_node/Automatic-Variables.html).

Update your rules to use patterns and automatic variables.

Commit everything before moving to the next section.

### Historical notes
Why do all of these commands have different ways of specifying the input file and the output file?
These tools trace their roots back to the earliest days of Unix in the 1970s.
We've been stuck with backwards compatibility with the strange quirks ever since.
The C compilation environment is very primitive compared to modern tools like gradle or cargo.
It does not handle any package management, and requires significant manual setup.
You won't often generate Makefiles directly, rather you will use a tool like CMake (which is what we will be using with the pico).
These type of tools are an improvement, but don't fully solve the problem in the way more sophisticated tools do.
Why do we keep doing this?
Most of this is just [Stockholm syndrome](https://en.wikipedia.org/wiki/Stockholm_syndrome) - it's the way we've always done it, and it works good enough.
You'll get used to it, and then you'll pass the trauma on to the next generation.
Luckily, when you start a job, the build system will largely be set up for you.

## Linking it all together
We now have a Makefile that can compile our C files to binary objects.
We need to combine them together into a single binary that can be executed.
We will use a tool called a *linker*.
When you reference a variable or function that exists outside your C file, the object files contain a placeholder location.
The linker will take all object files, allocate memory to the objects, and replace placeholders with allocated memory locations.
The linker will also bring in any library object files.

```
LD=$(PICO_TOOLCHAIN_PATH)/bin/arm-none-eabi-ld
SRC=main.c second.c
OBJS=$(patsubst %.c,%.o,$(SRC))

firmware.elf: $(OBJS)
    $(LD) -o $@ $^
```

Rather than list out each .o file, we can use variables to list the files and use patterns to substitute the file names.
`$^` is the automatic variable that contains all dependencies.

We engineers are lazy, so let's add a shortcut to run everything.
* Define a new rule named `all` that depends on `firmware.elf`. Place it as the **first recipe**.
* Leave the recipe empty.
* Remember to add `all` to the `.PHONY` list.

`all` is a special target by convention - when you invoke make with no targets, it will build the `all` target.

Commit everything before moving to the next section.

## CI/CD
Create a file `.github/workflows/main.yml`. Add the following configuration.

```
name: CMake
on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - name: Clean workspace
        run: |
          echo "Cleaning up previous run"
          rm -rf "${{ github.workspace }}"
          mkdir -p "${{ github.workspace }}"

      - name: Install GCC
        id: arm-none-eabi-gcc-action
        uses: carlosperate/arm-none-eabi-gcc-action@v1
        with:
          release: '13.2.Rel1'

      - uses: actions/checkout@v3
        with:
          path: ${{github.workspace}}/source

      - name: Say hello
        shell: bash
        working-directory: ${{github.workspace}}/source
        run: |
            ls
            test ! -f hello.txt
            test ! -f main.o
            test ! -f main.i
            test ! -f main.s
            make hello.txt
            test -f hello.txt

      - name: Test Compile
        shell: bash
        working-directory: ${{github.workspace}}/source
        run: |
            test ! -f firmware.elf
            PICO_TOOLCHAIN_PATH=${{ steps.arm-none-eabi-gcc-action.outputs.path }}/.. make
            test -f firmware.elf

      - name: Test Clean
        shell: bash
        working-directory: ${{github.workspace}}/source
        run: |
            make clean
            test ! -f hello.txt
            test ! -f main.o
            test ! -f main.i
            test ! -f main.s
            test ! -f firmware.elf

```

# Next steps
We've briefly covered a simple Makefile.
We have not covered the more complex aspects of C compilation such as headers and libraries, optimization, or compiler options.
We probably would want targets for loading the firmware onto the microcontroller.
If we were to continue expanding this Makefile, we would need to start introducing these additional aspects.
Luckily we don't have to set up the compilation process for the pico directly.
The toolchain uses a tool called CMake, which is a higher level abstraction that generates the Makefile for you.
You can find the generated makefiles in the build directory of a project.
VSCode works directly with the CMake and Makefiles when you use the build button.

Make is a useful tool for other tasks besides compilation.
Any shell commands can be used as a recipe, so you can write makefiles to run reports, or compile your scientific data, or any task you need to automate.
Make is capable of running multiple targets in parallel with the `-j` flag.
This is really useful if you want to parallelize a bunch of tasks.
If a task fails, fix your Makefile and run it again.

In this lab you've been reminded in each section to commit regularly to source control.
Future labs remove the training wheels, it's up to you to remember good source control habits.
