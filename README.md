# Blinky Project for Mbed LPC1768

This project can be used to verify the basic tool setup and hardware connection to the
[Mbed LPC1768](https://os.mbed.com/platforms/mbed-LPC1768/) development board.

The application blinks all LEDs one after another in a 1 second interval.

Refer to [Project Configuration](#project-configuration) for board specific settings.

![Mbed LPC1768 Pinout](./images/lpc1768_pinout.png)

## Prerequisites

The following tools need to be installed on your machine:

- [CMSIS-Toolbox v2.12.0](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/releases) or newer
- [Microsoft Visual Studio Code](https://code.visualstudio.com/download) with
  [Keil Studio Pack](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) extension (optional,
  alternatively [CLI](#using-command-line-interface-cli) can be used)
- [Arm Compiler 6](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded) (automatically
  installed when using Visual Studio Code with vcpkg)

## Project Configuration

### Keil RTX5 real-time operating system

The real-time operating system [Keil RTX5](https://arm-software.github.io/CMSIS-RTX/latest/index.html) implements
the resource management.

It is configured with the following settings:

- [Global Dynamic Memory size](https://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html#systemConfig_glob_mem):
  1024 bytes
- [Default Thread Stack size](https://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html#threadConfig): 256 bytes
- [Idle Thread Stack size](https://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html#threadConfig): 128 bytes
- [Timer Thread Stack size](hhttps://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html#timerConfig): 192 bytes
- [Stack Overflow Checking](https://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html#threadConfig_ovfcheck) and
  [Stack Usage Watermark](https://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html#threadConfig_watermark)
  enabled

Refer to [Configure RTX v5](https://arm-software.github.io/CMSIS-RTX/latest/config_rtx5.html) for a detailed
description of all configuration options.

#### GPIO Configuration and usage

| Pin   | Peripheral | Signal    | Identifier | Pin Settings
|:------|:-----------|:----------|:-----------|:------------
| P1.18 | GPIO1      | 18        | LED1       | default
| P1.20 | GPIO1      | 20        | LED2       | default
| P1.21 | GPIO1      | 21        | LED3       | default
| P1.23 | GPIO1      | 23        | LED4       | default

#### CMSIS-Driver mapping

| CMSIS-Driver VIO  | Physical board hardware
|:------------------|:-----------------------
| vioLED0           | LED1
| vioLED1           | LED2
| vioLED2           | LED3
| vioLED3           | LED4

## Build solution

### Using Keil Studio

The following is written for [Keil Studio](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack), a
set of VS Code extensions.

Required tools described in file `vcpkg-configuration.json` should be automatically installed by vcpkg. You can see the
status of vcpkg in the status bar.

Required CMSIS packs need to be also installed. In case a required pack is missing, a notification window will pop-up
to install the missing pack.

Open the **CMSIS view** from the side bar and press the **Build** button.

### Using command line interface (CLI)

Download required packs (not required if the packs are already available) by executing the following commands:

```sh
csolution list packs -s LPC1768.csolution.yml -m > packs.txt
cpackget update-index
cpackget add -f packs.txt
```

Build the project by executing the following command:

```sh
cbuild LPC1768.csolution.yml
```

## Run and debug in Keil Studio

### Run

- Connect the board's Mini-USB to the PC (provides also power).
- Open the 'CMSIS' view from the side bar and press the **Run** button and wait until the image is programmed and
  starts running.

### Debug

Open the **CMSIS** view from the side bar and press the **Debug** button. A connection is established via the on-board
CMSIS-DAP debug adapter via [pyOCD](https://mdk-packs.github.io/vscode-cmsis-solution-docs/configuration.html#pyocd).

RTOS awareness is available through the **XRTOS** view in the bottom panel.

> [!NOTE]
> We recommend using the [CMSIS-DAP beta firmware](https://forums.mbed.com/t/new-beta-firmware-for-lpc1768/18917) for
> connecting to the development board.
