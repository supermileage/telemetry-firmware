## Cloning

Since we're using submodules for the can-telemetry library (not the others, because they contain changes specific to this project), we must also clone them when originally cloning this module. 

```
git clone --recurse-submodules https://github.com/supermileage/particle-io-canbus.git
```

Or, if it's already cloned:
```
git submodule init
git submodule update
```

## flashing firmware onto the board

Generally, using the CLI compiler is easier along with the cloud compile feature. Assuming you have the Particle CLI installed, run the following command in the root directory:

`particle compile electron . --saveTo firmware.bin` 

This will build a `firmware.bin` file to your root directory, which you can then flash onto the Particle board. Note that you need the current directory because it will recursively search for code to compile.  

Once you have the .bin file, put the Particle board in DFU mode:

1. Hold down both buttons
2. Release only the `RESET` button while holding down the `SETUP` button
3. Wait for the LED to start flashing yellow 
4. Release the `SETUP` button 

Then connect the board to your PC, and run:

`particle flash --usb firmware.bin` 

This will flash the firmware onto the board. 

## Using Workbench

Particle Workbench has all this functionality built-in, so it abstracts away a lot of the hassle of learning the CLI. Use that if you're unsure. 
