The RetroCade Synth is made by Gadget Factory (http://www.GadgetFactory.net) and was started on Kickstarter (http://www.kickstarter.com/projects/13588168/retrocade-synth-one-chiptune-board-to-rule-them-al)

Welcome to the R-R-R-R-RetroCade Synth from Gadget Factory.  This exciting new hardware synthesizer is built on Gadget Factory's own Papilio platform, and is designed with both hardware hackers and musicians in mind.  Musicians will love the ease of use and extreme retro glitchiness that the Retrocade has to offer. Hackers will love that it is open-source, and is designed to be HACKED.  Either way, you're in luck.  You will find no better, more flexible synthesizer at this price point anywhere else!

The RetroCade Synth boasts the capability to play the built-in Commodore 64 SID chip, the Yamaha YM-2149 chip, .mod files, and MIDI files - all at the same time! The RetroCade Synth can be played via any external MIDI control interface or via your favorite audio/ sequencing software. We have built a custom VST software dashboard which gives you visual control over all the various parameters the synth has to offer.

Requirements:
  *You must have FlowStone installed to modify the Dashboard. The free version can be downloaded from: http://www.dsprobotics.com/download.php
  
ChangeLog:
9/26/2013       Version 1.1
        -Added SID Analog Filters!!!!!!!!  Thanks to Alvie for writing the VHDL code.
        -Added SidPlayer library to process SID files from smallFS and SD Cards. Thanks to Alvie for porting to the ZPUino.

1/9/2013        Version 1.02
        -NoteOff fix from Lee O'D
		
11/23/2012      Version 1.01
        -RetroCade Sketch
                -YM Player Volume Control

11/23/2012      Version 1.0
        -RetroCade Sketch
                -SID Volume CC added.
                -Crawling Space Invaders added to the Welcome LCD screen.
                -Added ability to browse and play YMD and MOD files from the LCD screen.
                -Ring Mod and Sync CC's added.
                -Added YM2149 Noise Frequency.
                -SID V1 Instrument select from LCD.
                -SID Pitch Control
                -HiHat and Kick split keyboard from mod files on MIDI channel 7

11/1/2012	Version .3
	-FlowStone Dashboard
		-Changed from SynthMaker to FlowStone
		-Added the ability to control the RetroCade without a MIDI output, can control over a Com Port instead.
		-Generated an EXE version of the Dashboard. VST will be generated when FlowStone 3.0 is released in November.
		-Zoom Controls added to Dashboard
			-Pressing Up Arrow toggles full screen.
			-While in full screen pressing Left Arrow fills the screen.
			-Pressing Down Arrow returns to normal size.
	-RetroCade Sketch
		-Control the RetroCade from a COM port or MIDI In
		-Play YMdat and Mod files from SD card or SmallFS filesystem.
			-Place a file name track[1-3].mod or track[1-3].ymd onto the SD card to override the files found in smallFS. Must power cycle the board for the files to be found.
			-YM files must be converted to ymd files using the process and tools detailed here: (http://audio.gadgetfactory.net/index.php?n=Main.YM2149Flash)
			-Mod files play out of RamFS filesystem which is located entirely in the 8MB SDRAM.
	-Merged RetroCade Bit File: Shortcut to load RetroCade-.01d-zpuino-1.0-PapilioPro-S6LX9-RetroCade-1.04.bit with the Papilio Loader
		-Changes from RetroCade Sketch are merged into this bit file.
		-Smallfs version of MOD and YM files are merged into this bit file.
		
10/8/2012	Version .2
	-SynthMaker Dashboard
		-Added Midi In and Midi Out Selectors to the front panel.
		-Added SID Volume
		-Fixed CC#'s for all Presets
		-Renamed Presets from Hoppa to User

9/26/2012	Version .1
	-ZPUino IDE: Shortcut to open RetroCade Sketch with the ZPUino IDE.
	-Merged RetroCade Bit File: Shortcut to load RetroCade-.01b-zpuino-1.0-PapilioPro-S6LX9-RetroCade-1.04.bit with the Papilio Loader
		-MOD files, YM2149, SID chips play on both left and right channel now.
		-Code runs from SDRAM providing 8MB of code space!
		-RetroCade Sketch is merged into this bit file.
		-Smallfs version of MOD and YM files are merged into this bit file.
	-Papilio Loader
	-SynthMaker Dashboard: The dashboard requires that Synthmaker be installed. MIDI selections are not moved to the main page yet.