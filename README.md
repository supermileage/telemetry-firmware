# Telemetry Firmware

![Telemetry 2.0 System](https://github.com/supermileage/telemetry-firmware/blob/update-docs/media/telemetry.jpg)

Firmware for the Supermileage Telemetry module. The Telemetry system is an LTE-connected data-collection module that records data from multiple sensors on each of the three Supermileage vehicles, logs the data, and uploads it to the cloud. The data gets processed and stored on [Google Cloud](https://github.com/supermileage/telemetry-gcp-deployment) and can be accessed using [Grafana](https://github.com/supermileage/telemetry-grafana). There is also a [custom PCB design](https://github.com/supermileage/telemetry-pcb) available. 

The Telemetry system can log data from the following sensors:
- Temperature (on Telemetry)
- Accelerometer & Gyroscope (on Telemetry)
- 2x Thermocouples
- GPS (via i2c)
- Engine ECU (via UART)
- Fuel-Cell Controller (via optical UART)
- Brushless Motor Controller (via CAN)
- Lithium-Ion BMS (via CAN)
- Vehicle Accessories (via CAN)
- Steering Wheel (via CAN)

## Cloning

Some of our libraries are submodules, so we have to clone with those in mind. If starting anew, use the following command:

```sh
git clone --recurse-submodules https://github.com/supermileage/telemetry-firmware.git
```

Or, if it's already cloned:

```sh
git submodule init
git submodule update
```

### Note about symlinks

Common libraries are carried over and compiled using a symlink. For POSIX OSes, this wouldn't be a problem when cloning. However, if developing on Windows, make sure you install Git with symbolic link support (as illustrated [here](https://stackoverflow.com/a/42137273)). Then, clone with:

```sh
git clone --recurse-submodules -c core.symlinks=true https://github.com/supermileage/telemetry-firmware.git
```

**IMPORTANT**: you need administrator permissions for this, so make sure you clone it in a command prompt ran as admin.

## Building

All options should generate a `.bin` file, which will be your application firmware payload for flashing.

### With Particle CLI

The CLI supports cloud compilation, which requires you to sign into the Particle Cloud. If this is the most accessible option, you can invoke it with the following (assuming you are in the root directory):

```sh
particle compile electron <directory> --saveTo firmware.bin --followSymlinks
```

Where `<directory>` is the directory with the `project.properties` file. For example, [urban](urban/).

### With Particle Workbench

The Particle workbench also supports compilation (locally, too). Once installed, invoke it in the command bar by searching for `Particle: Compile application (local)`. **Note that this requires you to be in the nested vehicle directory in your VSCode workspace.** This means you should open the instance of VSCode in the nested directory and work with that; any changes to the libraries will be reflected across all vehicles. **Make sure you're targetting 1.5.2**.

### With Docker

A pre-built image is available that includes the dependencies to compile. The command is wrapped up with a Makefile in the root directory, meaning you can simply run `make proto` or `make urban` to build the firmware for the respective vehicles.

## Flashing

## flashing firmware onto the board

Once you have the `.bin` file, put the Particle board in DFU mode:

1. Hold down both buttons
2. Release only the `RESET` button while holding down the `SETUP` button
3. Wait for the LED to start flashing yellow
4. Release the `SETUP` button

Then connect the board to your PC, and run:

```sh
particle flash --usb firmware.bin
```

This will flash the firmware onto the board.

## Using Workbench

Particle Workbench has all this functionality built-in, so it abstracts away a lot of the hassle of learning the CLI. Use that if you're unsure.

## Tests

This project contains a set of unit tests.  For information on building and writing tests, see the README.md in the test folder.
