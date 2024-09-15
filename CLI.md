# Introduction to UNIX and the CLI
This is a quick tips and tricks guide to using the command line.

# Basic terminology.
The filesystem has a number of conventions for file names.

* Paths are separated by forward slash "/".
* "." means the current directory.
* ".." means the directory above the current one.
* "~" is your home directory.
* File names that begin with a period "." are hidden by default.

The "shell" is the program you interact with, where you type commands. The "terminal emulator" is the window that you launch in your window manager. "Terminal" is used generally to refer to whole thing.

# "Meta" key.
[Historical terminology](https://en.wikipedia.org/wiki/Space-cadet_keyboard) is to call the "Option" key on a standard keyboard "Meta", and the "Windows" or "Command" key "Super". "Control" is still "Control".

You will sometimes see a control sequence written as "^s", which means hold control and type the s key. "^S" often means hold shift as well (i.e. type a capital S).

Other source will write "C-s" and "C-S" for command and "M-s" for meta prefix.

## OSX Terminal setup
Follow the instructions to set "Option as Meta key".
https://support.apple.com/guide/terminal/change-profiles-keyboard-settings-trmlkbrd/mac

# Prompt setup
Any good Unix hacker has a prompt setup to maximize their productivity.

You can set up a decent one if you use Bash by putting the following in your `~/.bashrc` file.
`PROMPT_COMMAND='PS1_CMD1=$(__git_ps1 " (%s)")'; PS1='\t \u@\h [\w]${PS1_CMD1} \$'`

You can go nuts with [generating a custom prompt](https://bash-prompt-generator.org/).

# PATH and finding executables.
If you get the following error, you will need to setup the PATH environment variable.
```
% foo
bash: foo: command not found
```

PATH is a colon (:) separated list of locations the shell will search when you provide a command. It might look something like this.

`PATH=/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin`

You can list currently set environment variables with the `env` command.

If a program is installed in a location not in your PATH, you can add the location in your `~/.bashrc` or `~/.profile` configuration.

`export PATH=$PATH:/home/snelgrov/.pico-sdk/toolchain/13_2_Rel1/bin`

# Using the command line like a pro
The command line can seem slow. Typing out everything *is* slow. The folks who designed the tools made them to be as efficient as possible. **The tab key is your best friend.** Everytime you hit tab, the shell will attempt to autocomplete. If there are multiple possible completions, it will complete as much as it can and show you all the options. Type a few more characters and the tab again. When you are changing directories, you do not have to change one directory, list the contents of the directory, then change directory again. Just add a slash, and then keep using autocomplete..

Previous commands you've executed are still available. If you need to repeat a command or fix a mistake, you can use "C-p" or the up arrow to go back through history. If you need to rerun a previous command further back in history, use "C-r" to do a reverse search back through history.

## Efficient navigation
The key to efficiency is to minimize movement. Every time you take your hands off the keyboard, you slow yourself down. If you reach over to the mouse, the arrow keys, or the navigation keys, you are wasting time. Standard command line tools support the following key shortcuts (among many others). These are also available in the editor Emacs, which along with vim are the gold standard for text editing in Unix environments. The same principle applies in other programs you use, especially your IDE. Learn the keyboard shortcuts!

Using the command line will be slow at first. You will be tempted to reach for the arrow keys or the mouse. Deliberate practice is required, so take the time to learn the habits you need.

### History
- C-n: go to the **n**ext command in history
- C-p: go to **p**revious command in history
- C-r: **r**everse search back through history
- M-.: cycle through previous arguments (I got nothing for a mnemonic)
### Text navigation

- C-f: move cursor **f**orward one character
- C-b: move cursor **b**ack one character
- M-f: **f**orward one word
- M-b: move **back** one word
- C-a: move to beginning of line (**a** is on the far the left side of the keyboard, just like the beginning of the line.)
- C-e: move to **e**nd of line

### Editing

- C-d: **d**elete char forward
- M-d: **d**elete word forward
- M-del: delete word backwards
- C-k: delete (**k**ill) from cursor to end of line
- M-u: make **u**ppercase
- M-c: make first letter **c**apital
- M-1: make **l**owercase

### Process management

- C-c **c**ancels the current process
