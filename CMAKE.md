# CLI basics for pico

## Configure

Configure the build process. Do compilation inside a directory `build` using source code from the current directory `.`.

```cmake -B build -S .```
If you make changes to the CMakeLists, you will need to reconfigure.

## Build
Run the build process. Use the build setup inside the directory `build`

```cmake --build build```

This builds all binaries, including test binaries.

## Flash
Flash the compiled ELF binary to the microcontroller.
```picotool load -f build/src/myproject.elf```

If you specify the `-f` flag, it should switch the device back into BOOTSELECT mode without needing to be unplugged and plugged back in. This won't work if you currently have a serial connection attached.

You can also use the custom `flash` or `flash_test` target defined in the CMakeLists.txt from the class template.
```cmake --build build --target flash```

## Clean
You can remove build artifacts by running the `clean` target.
```cmake --build build --target clean```
If reconfiguring doesn't seem to make the correct changes in the build process, you can always delete the directory and start over with a fresh configuration.

# VSCode integration
The Raspberry Pi SDK plugin wraps the CMake plugin functionality. Because we are using a modified CMake file from the default, you should not use the "Import Project" button in the Pico extension tab.

Some detailed documentation:

https://code.visualstudio.com/docs/cpp/cmake-linux#_configure-hello-world

https://github.com/microsoft/vscode-cmake-tools/blob/main/docs/how-to.md

## Running tests.
Several tasks are defined in the .vscode configuration in the class template. The launch.json and task.json files have been modified to use the currently selected CMake target binary rather than the single target allowed by the Pico extension. You can switch the current launch/build target from the Command Palette or the CMake extension side panel with "Set Launch/Debug Target".
