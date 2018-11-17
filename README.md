## flashing firmware onto the board

Generally, using the CLI compiler is easier along with the cloud compile feature. Assuming you have the Particle CLI installed, run the following command in the root directory:

`particle compile electron src --saveTo firmware.bin` 

This will build a `firmware.bin` file to your root directory, which you can then flash onto the Particle board. 

Once you have the .bin file, put the Particle board in DFU mode:

1. Hold down both buttons
2. Release only the `RESET` button while holding down the `SETUP` button
3. Wait for the LED to start flashing yellow 
4. Release the `SETUP` button 

Then connect the board to your PC, and run:

`particle flash --usb firmware.bin` 

This will flash the firmware onto the board. 

## Notes about CAN 
CAN requires us to acknowledge all messages or the node that's sending will retransmit unacknowledged frames over and over. So in order to account for buffer overflow, we should take steps to acknowledge all the telemetry data. 