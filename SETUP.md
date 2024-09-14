# Overview
We will use the Visual Studio Code integrated development environment (IDE). Modern software development leverages many tools - the IDE integrates them into a single environment. You'll use the IDE to write, manage, build, deploy, and debug your code.

We will also be heavily using the command line in a Unix environment. The IDE automates and provides GUI shortcuts, but these are just wrappers over the underlying commands available. **Don't use the GUI when a command line or keyboard shortcut is available**.

# Get a Unix
Windows users can use the (Windows Subsystem for Linux)[https://learn.microsoft.com/en-us/windows/wsl/install]. Linux is the most popular open-source version. Apple's Mac OS is a descendant of the Berkeley System Distribution (BSD); other major BSD descendants are FreeBSD and OpenBSD.

Linux is packaged in a distribution. (Popular distributions)[https://distrowatch.com/] include (Debian)[https://www.debian.org/], (Mint)[https://linuxmint.com/], (Ubuntu)[https://www.ubuntu.com], (Arch)[http://www.archlinux.org/], and (Fedora)[https://fedoraproject.org/]. If you aren't sure which to install, Mint is a fine choice.

## Installation methods
### Virtual machine
Tools such as (Virtualbox)[https://www.virtualbox.org/] allow you to create a virtual machine, where you can install Linux.

### Dual boot Linux
You can create a separate partition on your computer to switch between booting your regular operating system and Linux.

https://opensource.com/article/18/5/dual-boot-linux

### Windows Subsystem for Linux (WSL)
WSL is a Microsoft tool that embeds a Linux environment into Windows. It's an crime against nature, but it works fine.

https://learn.microsoft.com/en-us/windows/wsl/install

### Mac OS
Apple's Mac OS from OSX forward uses the Darwin BSD kernel. You'll want to install (Homebrew)[https://brew.sh/] for package management.

# Install VSCode
We'll be using the VSCode extension integration to setup the SDK and the build toolchain.

Detailed installation instructions are available in the chapters 2 and 3 in the getting started datasheet https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf

1. Download and install VSCode, available at the following link. https://code.visualstudio.com/
1. Install these plugins
    - https://marketplace.visualstudio.com/items?itemName=raspberry-pi.raspberry-pi-pico
    - https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools
    - https://marketplace.visualstudio.com/items?itemName=twxs.cmake
    - https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools
    - https://marketplace.visualstudio.com/items?itemName=d-biehl.robotcode

If you want to install the SDK manually, there are instructions on https://github.com/raspberrypi/pico-sdk

# Windows specific issues
NOTE: If you are using a windows device, install this: https://zadig.akeo.ie/
Use this to install a driver for the RP2 USB-Device, after you have plugged in your RP2040 from the stockroom to your computer.

# Linux specific issues
You may need to install this udev rule to allow non-root access over USB.

https://github.com/raspberrypi/picotool/blob/master/udev/99-picotool.rules

You may need to add your user to the dialout group to access serial devices.

`sudo usermod -a -G dialout $USER`

# Install Robot Framework
We'll use the Robot Framework for running our tests.

https://docs.robotframework.org/docs/getting_started/testing#install-robot-framework

Note you will want to install with `pip3 install --user robotframework` to add a user-wide installation.

# Install Renode
**Skip this step, Renode is currently unable to run Pico code running FreeRTOS**
1. Download and install Renode, available at the following link https://renode.io/#downloads

## Additional renode setup
The Raspberry Pico needs configuration files for Renode to work properly.

* On MacOS, the installation location is `/Applications/Renode.app/Contents/MacOs`
* On Linux, the location for Debian, Fedora, and Arch is `/opt/renode`
* On Windows, the location is `C://Program Files/Renode`

To add the Pico configuration files from the `renode` directory in this repository:
1. Copy `rp2040_spinlock.py` and `rp2040_divider.py` to the `scripts/pydev` directory of your Renode installation.
1. Copy `rpi_pico_rp2040_w.repl` to the `platforms/cpus` directory.

# Setup paths
VSCode will setup environment variables in terminals spawned from inside the IDE. If you want access in your general terminal, add the following to `~/.profile` (the file is hidden with the period prefix in your home directory). This isn't required, but is convenient.

```
export PICO_SDK_PATH=${HOME}/.pico-sdk/sdk/2.0.0
export PICO_TOOLCHAIN_PATH=${HOME}/.pico-sdk/toolchain/13_2_Rel1
export PATH=$PATH::${PICO_TOOLCHAIN_PATH}/bin
export PATH=$PATH:${HOME}/.pico-sdk/cmake/v3.28.6/bin
export PATH=$PATH:${HOME}/.pico-sdk/picotool/2.0.0/picotool
export PATH=$PATH:${HOME}/.pico-sdk/ninja/v1.12.1
```

# SSH keys for GitHub authentication.
If you don't have a Github account, go create one now. Github requires SSH authentication to push to a repository.

1. Create an ssh key for authentication with Github
    1. Generate: https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent
    1. Add to your account: https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account

When you clone a repository, you need to use the URL of the form `git@github.com:org/repo`, *not* the https version.
