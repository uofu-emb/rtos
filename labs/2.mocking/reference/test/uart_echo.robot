*** Settings ***
Test Timeout            20s

*** Variables ***
${UART}                       sysbus.usart1

*** Keywords ***
Create Machine
    [Arguments]  ${elf}

    Execute Command          mach create
    Execute Command          machine LoadPlatformDescription @platforms/boards/stm32f072b_discovery.repl
    Execute Command          sysbus LoadELF ${elf}

*** Test Cases ***

Should Run Zephyr Hello Sample
    Create Machine           @https://dl.antmicro.com/projects/renode/stm32f072b_disco--zephyr-hello_world.elf-s_451704-a4d8c888e36c324dcc1dfde33bac33fc6ed3ed1b
    Execute Command          showAnalyzer ${UART}
    Create Terminal Tester   ${UART}
    Start Emulation

    Wait For Line On Uart    Hello World! stm32f072b_disco

Should Run Zephyr Console Echo Sample
    Create Machine           '${CURDIR}/../.pio/build/disco_f072rb/firmware.elf'
    Execute Command          showAnalyzer ${UART}
    Create Terminal Tester   ${UART}

    Start Emulation
    Write Line To Uart     hello world!
    Wait For Line On Uart  HELLO WORLD!
