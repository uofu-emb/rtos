# Lab 0: Build ALL THE THINGS!
In this lab, we'll be setting up the development environment. This environment will allow you to build, test, deploy, and manage your code.
## Learning objective:
- Navigate documentation
- Setup toolchain
- Create a project.
- Commit to source control
- Build project
- Run tests
- Flash a program
- Use the debugger
- Setup CICD
# Prelab
Complete the following tasks before lab. At the end of this section, you should have the tools installed and a working project skeleton.

## Install Visual Studio Code
### Overview
We will use the Visual Studio Code integrated development environment (IDE). Modern software development leverages many tools - the IDE integrates them into a single environment. You'll use the IDE to write, manage, build, deploy, and debug your code.

We will also be heavily using the command line. The IDE automates and provides GUI shortcuts, but these are just wrappers over the underlying commands available.
### Tasks
- Download and install VSCode, available at the following link. https://code.visualstudio.com/
## Install PlatformIO
### Overview
PlatformIO is a tool to simplify and unify the development process for embedded systems. It allows easy bootstrapping of projects, and a consistent interface between different build tools, frameworks, libraries, and hardware.

We'll be using the VSCode extension integration to setup and use PlatformIO.
### Tasks
- Install the PlatformIO extension in VSCode. (We will go through the quickstart in the next task in the lab)
https://docs.platformio.org/en/latest/integration/ide/vscode.html#installation
## Create a new project
### Overview
PlatformIO and other build tools use a set of conventions to setup a project. This usually defines a consistent organization for source code, header, and test code. The configuration of the project, library dependencies, along with any custom build tasks, will be located in the build file. For PlatformIO the build file `platform.ini` is located at the root of the project. The build file will be committed to your git repository with the rest of your source code. Anyone else working on the project will be able to use the same project configuration by simply checking out the code.

PlatformIO provides a unified command, `pio`. This command has many subcommands for various tasks. We'll use this command to set up our project.

### Tasks
These instructions are tailored to our particular board and framework. A more detailed tutorial using a different board can be found here https://docs.platformio.org/en/latest/core/quickstart.html

1. Open the "PlatformIO Core CLI".
    1. Click on the PlatformIO extension in the left side bar of VSCode.
    1. In the expand "Miscellaneous" and click on "PlatformIO Core CLI". A terminal window should open.
1. Optional: Add the PlatformIO commands to your system PATH variable so you can use them from the system terminal.
https://docs.platformio.org/en/latest/core/installation/shell-commands.html#piocore-install-shell-commands
1.  Search for the board id for the discovery board..
    1. In the terminal, run `pio boards disco`
    1. Find the id for the STM32 Discovery 072. We will pass this as an argument to the project init task
1. Create a directory for your project and open the new directory. `mkdir my_project; cd my_project`
1.  Once inside that directory, run `pio project init --board disco_f072rb --project-option "framework=zephyr" --project-option "test_framework=unity"`
    1. This sets up the project with the correct board, the STM32 platform, and the Zephyr RTOS framework.
https://docs.platformio.org/en/latest/core/userguide/project/cmd_init.html#cmd-project-init
1. Read the output from the init command and identify the function of the different directories and files.
1. Look at the contents of the platformio.ini
https://docs.platformio.org/en/latest/projectconf/index.html#projectconf

## Reading
https://docs.github.com/en/pull-requests/collaborating-with-pull-requests/proposing-changes-to-your-work-with-pull-requests/about-pull-requests


# Lab
# Commit project
## Overview
Now that we have our project, let's get it into source control.
## Tasks
1. Create repository on github
    1. Do not initialize the repository with a README, gitignore, or license.
1. Initialize the local repository.
    1. From your project directory, run `git init`. This sets up the repository metadata.
    1. Stage all of the files in the current working directory `git add .gitignore src include lib test platformio.ini`
    1. Commit the staged files with a descriptive commit message `git commit -m "Initialize PlatformIO project"`
    1. Create a main branch `git branch -M main`
    1. Add your github repository as a remote. `git remote add origin git@github.com:uofu-embed/embedded_demo.git`
    1. Push the main branch and the commit to the remote. `git push -u origin main`

# Building a project

## Tasks
1. Create a new git branch. You can call it whatever you like, but it is useful to use a descriptive name for what is being developed in the branch.`git checkout -b compilation-demo`
1. Copy in the example blink code to your `main.c` file. We will explore this code in the next lab. For now, we just need something to compile.
https://github.com/platformio/platform-ststm32/blob/master/examples/zephyr-blink/src/main.c
1. Build project with `pio run`. The run subcommand is used to execute different __targets__. The default target compiles your project.
https://docs.platformio.org/en/latest/core/userguide/cmd_run.html#cmd-run
1. Commit the new main file.
# Run tests
## Overview
Now that the code is compiling, it needs to be tested. Manually testing the system by running it and observing the behavior is often easy, but does not scale once a project grows in size and complexity. Automated testing demonstrates the behavior of your system and codifies that behavior. Any change to the system should maintain the previous behavior of the system outside the change.

Remember, "if you can't measure it, you can't change it".
## Tasks
1. Copy in the example tests to your project `tests` directory.
1. Add the following configuration to your platformio.ini file. This will add an environment to run our unit tests locally on your computer, including the library dependency on the Unity test framework.
```[env:unit-test]
build_type = test
platform = native
lib_deps =
    throwtheswitch/Unity@^2.5.2
```
1. Run the tests with `pio test --environment native`
https://docs.platformio.org/en/latest/core/userguide/cmd_test.html
1. Verify that all tests pass.
1. Commit the new test file and the changes you made to platformio.ini.
# Setup Continuous Integration
## Overview
Continuous Integration (CI), often paired with Continuous Delivery (CICD), is a development pattern to rapidly deliver consistent working software. The basic principle is to always keep your project in a working state, with small incremental changes. The project should be monitored with automated tests and performance instrumentation. When code is pushed to the central repository, a build system will run the automated tests. If the tests pass, the code is ready to be reviewed and then deployed.

Github provides an automated build system called Github Actions. We will set up your project to build and run tests on push. We will then show the passing or failing status of your tests on your repository README.
## Tasks
1. Create the metadata directories. In the root of your repository, create a directory `.github` (note the leading dot). Inside that directory, create another directory `workflows`.
1. Create a workflow.
    1. Worflows are defined in yaml configuration files, which define a series of steps that will be executed in response to an event, such as a push.
    1. Create a file `.github/workflows/main.yml`. Add the following configuration.
```
name: PlatformIO CI

on: [push]

jobs:
  build:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v3
      - uses: actions/cache@v3
        with:
          path: |
            ~/.cache/pip
            ~/.platformio/.cache
          key: ${{ runner.os }}-pio
      - uses: actions/setup-python@v4
        with:
          python-version: '3.9'
      - name: Install PlatformIO Core
        run: pip install --upgrade platformio

      - name: Build PlatformIO Project
        run: pio run
     - name: Run Tests
        run: pio test --environment unit-test
```

https://docs.github.com/en/actions/learn-github-actions
    1. Commit the file and push.
1. add test status badge to your repo README.
    1. Create a file named `README.md` in the root of your project.
    1. Add a brief description of you project.
    1. Add a badge image showing the current status of the workflow
```
![example workflow](https://github.com/<OWNER>/<REPOSITORY>/actions/workflows/main.yml/badge.svg)
```
https://docs.github.com/en/actions/monitoring-and-troubleshooting-workflows/adding-a-workflow-status-badge
    1. Commit your README and push.
    1. Open a pull request to merge your branch into main branch.
