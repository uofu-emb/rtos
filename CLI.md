# Introduction to UNIX and the CLI
This is a quick tips and tricks guide to using the command line.

# Basic terminology.
The filesystem has a number of conventions for file names.

* Paths are separated by forward slash `/`.
* `.` means the current directory.
* `..` means the directory above the current one.
* `~` is your home directory.
* File names that begin with a period `.` are hidden by default.
* `*` is a glob, which you can use as a wildcard in paths. `**` is a recursive glob (descends multiple levels).

The "shell" is the program you interact with, where you type commands. The "terminal emulator" is the window that you launch in your window manager. "Terminal" is used generally to refer to whole thing. Most Linux distributions use `bash` as the default login shell. OSX is starting to use `zsh`. This guide references bash config `.bashrc`, but the same applies to the config file for zsh confige `.zshrc`.

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


# Efficient navigation
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

# Aliases
## Shell aliases
You can create aliases to common commands to simplify your workflow. In any shell session, you can create an alias for any command.
```
alias piconfig='cmake -B build -S .'
alias pibuild='cmake --build build
alias piflash='picotool -f build/src/something.elf'
alias g=git
alias py=python3
```

If you want to use an alias across sessions, you can add them to your `~/.profile` or `~/.bashrc` file.

## Git aliases
You can create aliases for git as well.

In `~/.gitconfig` you can put aliases (among other useful config). In my configuration I have the following aliases.
```
[pretty]
    treeline= %C(bold blue)%h%C(reset) %C(yellow)%ai%C(reset) %C(white)%s%C(reset) %C(dim white)- %an%C(reset)%C(bold yellow)%d%C(reset)

[alias]
    co = checkout
    br = branch
    ci = commit
    st = status
    mt = mergetool
    dt = difftool
    rb = rebase
    mg = merge
    sm = submodule
    praise = blame
    tree = log --graph --all --abbrev-commit --decorate --pretty=treeline
    treehere = log --graph --abbrev-commit --decorate --pretty=treeline
    root = rev-parse --show-toplevel
```

On my terminal, with shell and git aliases I run `git status` with `g st`.

# Put your config in source control!
Everything goes in source control, even your config. Create a folder (I use `.dotfiles`) to store all your configuration (the "dot files"). From your home directory, create symlinks to the config (e.g. `ln -s .dotfiles/.bashrc`). Whenever you setup on a new computer, you just have to clone your config and link. Config changes are easy to move between machines.
Just don't forget to commit and push when you make changes.

# Scripting
The real power of the shell is that you work in a full programming environment! Common constructs like loops, variables, conditionals, and more are at your fingertips. You can put a set of commands into a script file for repeated execution. The syntax may be a bit different from what you find in C, but isn't too hard to pick up. You don't need to learn it all at once - as you need to use a feature, look up the syntax.

[Bash beginners guide](https://tldp.org/LDP/Bash-Beginners-Guide/html/index.html)

[The bash manual](https://www.gnu.org/software/bash/manual/bash.html)
