# Copyright 2009-2012 Jack Gassett
# Creative Commons Attribution license
# Made for the Papilio FPGA boards

COM_PORT=COM12
	

		echo "Programming Arcade MegaWing Verification to Papilio"
		# Find device id and choose appropriate bscan bit file
	
		device_id=`./papilio-prog.exe -j | ./gawk '{print $9}'`
		return_value=$?
		
		case $device_id in
			XC3S250E)
				echo "This test plan is not supported on the P1 250K board"
				;;	
			XC3S500E)
				echo "Programming a Papilio One 500K"
				bitfile=zpuino-1.0-papilioone-500-apollo.bit
				bscan_bitfile=bscan_spi_xc3s500e.bit
				;;
			XC6SLX9)
				echo "Papilio Plus LX9 not supported yet"
				exit
				;;				
			*)
				echo "Unknown Papilio Board"
				;;
		esac

		./papilio-prog.exe -v -f $bitfile -b $bscan_bitfile -sa -r
		#Cause the Papilio to restart
		./papilio-prog.exe -c
		./zpuinoprogrammer.exe -s 1000000 -d\\.\\$COM_PORT -bRetroCade_MegaWing_TestPlan.bin -R -esmallfs.dat
		return_value=$?

		
if [ $return_value == 1 ] #If programming failed then show error.
then
	./dialog --timeout 5 --msgbox "The bit file failed to program to the Papilio, please check that the Papilio is plugged into a USB port." 15 55
	read -n1 -r -p "Press any key to continue..." key
fi
