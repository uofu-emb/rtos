# Lab 1: make and compilation toolchains
In this lab, we will be examining C compilation for embedded systems. We will examine the function of the Unix make tool, and its place in the C ecosystem, and its general utility in the Unix environment. We will also learn the role of various tools in the embedded toolchain.

## Learning objectives
Identify the major steps of the C toolchain and explain their roles.
Write Makefile recipe to build a target.
Use substitution patterns to make flexible targets
Explain difference between phony and regular targets
Express target dependency relationships in recipes.
Analyze the advantages and disadvantages of the make tool.
Practice command line usage.

# Prelab
We will specifically be using the GNU implementation of make and the GNU compiler collection.
The manual is a lot like a datasheet, you don't have to read the whole thing.
Looking at my browser history visited links, I have looked at 38 out 178 sections, so you don't even need a significant percentage.

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
    return 0
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
This is why we use CI/CD! They can get the artifact from the build server, and are guaranteedwill have an up to date copy from a fresh repository.

Create a new file called `.gitignore`, and add a line with `hello.txt`
Now run `git status`. The build artifact is no longer listed as a new file. You will see the ignore file.

Commit everything, including the `.gitignore` file, before moving to the next section.
