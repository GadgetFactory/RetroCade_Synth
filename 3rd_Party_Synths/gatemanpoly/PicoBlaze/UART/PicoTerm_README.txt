
© Copyright 2012, Xilinx, Inc. All rights reserved.
This file contains confidential and proprietary information of Xilinx, Inc. and is
protected under U.S. and international copyright and other intellectual property laws.

Disclaimer:
  This disclaimer is not a license and does not grant any rights to the materials
  distributed herewith. Except as otherwise provided in a valid license issued to you
  by Xilinx, and to the maximum extent permitted by applicable law: (1) THESE MATERIALS
  ARE MADE AVAILABLE "AS IS" AND WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL
  WARRANTIES AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING BUT NOT LIMITED
  TO WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR
  PURPOSE; and (2) Xilinx shall not be liable (whether in contract or tort, including
  negligence, or under any other theory of liability) for any loss or damage of any
  kind or nature related to, arising under or in connection with these materials,
  including for any direct, or any indirect, special, incidental, or consequential
  loss or damage (including loss of data, profits, goodwill, or any type of loss or
  damage suffered as a result of any action brought by a third party) even if such
  damage or loss was reasonably foreseeable or Xilinx had been advised of the
  possibility of the same.

CRITICAL APPLICATIONS
  Xilinx products are not designed or intended to be fail-safe, or for use in any
  application requiring fail-safe performance, such as life-support or safety devices
  or systems, Class III medical devices, nuclear facilities, applications related to
  the deployment of airbags, or any other applications that could lead to death,
  personal injury, or severe property or environmental damage (individually and
  collectively, "Critical Applications"). Customer assumes the sole risk and
  liability of any use of Xilinx products in Critical Applications, subject only to
  applicable laws and regulations governing limitations on product liability.

THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS PART OF THIS FILE AT ALL TIMES. 



-------------------------------------------------------------------------------------------------
PicoTerm v1.30 
-------------------------------------------------------------------------------------------------


           ____  _         _____
          |  _ \(_) ___ __|_   _|__ _ __ _ __ ____
          | |_) | |/ __/ _ \| |/ _ \ '__| '_ ` _  \ 
          |  __/| | (_| (_) | |  __/ |  | | | | | | 
          |_|   |_|\___\___/|_|\___|_|  |_| |_| |_| 




6th September 2012

Ken Chapman - Xilinx Ltd - email:chapman@xilinx.com


PicoTerm is a very simple PC based terminal ideally suited for communication with PicoBlaze 
based designs that utilise the UART macros connected to a USB/UART port on a development 
board or evaluation kit. Given that PicoTerm can be used to communicate with any 'COM' port 
(real or virtual) and it is just a simple terminal it could be used with virtually any 
peripheral. 

The primary motivation for the development of PicoTerm was to provide a quick and reliable way 
to establish a working connection with a PicoBlaze based UART design. Whilst this should be 
easy to achieve with any terminal application, correctly setting all the communication and 
ASCII options often makes this a challenge. PicoTerm has been pre-configured to match the 
parameters required for a PicoBlaze/UART design and has a default BAUD rate of 115200. This 
means that in most cases only the COM port needs to be specified and it even helps to make 
that easier to do.

PicoTerm also has some features that you would not expect to find with a normal terminal. These 
special features are described later in this document and will hopefully appeal to users of 
PicoBlaze for fun, education or serious applications. The 'PicoTerm_routines.psm' file 
provided with PicoTerm contains a set of KCPSM6 routines with descriptions ready to be used with
PicoTerm. What will you use the virtual 7-Segment Display for? 


-------------------------------------------------------------------------------------------------
Requirements 
-------------------------------------------------------------------------------------------------

Windows-XP or Windows-7 Operating System.

A 'COM' port to communicate with - This appears to be obvious but do remember that when using a 
                                   USB/UART connection a driver may need to be installed to 
                                   provide you with a 'virtual COM port'. You also need to have 
                                   the hardware connected to your PC and power turned on.

You need to know the number of the COM port and the required BAUD rate (see 'Usage' section).



-------------------------------------------------------------------------------------------------
Usage
-------------------------------------------------------------------------------------------------


Quick Start Method
------------------

PicoTerm has the communication fixed to 8-bit, 1 Stop Bit, No Parity and No Handshake which is 
immediately compatible with the UART macros provided with PicoBlaze. This means that the only 
two variables are the number of the COM port and the BAUD rate. However, even the BAUD rate
defaults to 115200 so if you use this in your design (e.g. as used in 'uart6_ml605' reference 
design provided with the PicoBlaze UART macros) then you don't have to worry about that either.

So if the required BAUD rate is indeed 115200 then simply execute PicoTerm and it will prompt
you to enter a COM Port number. All you need to do is enter the right number. Unfortunately 
this vital piece of information may not be so obvious especially when using a USB/UART where 
the virtual COM port number has been automatically allocated by the driver. But don't worry, 
we can look that information up and PicoTerm is a quick and easy way to make connection attempts.

To find the possible COM port number on your PC then make sure your hardware is connected and 
has the power turned on....

  Right click on 'My Computer' and select 'System Properties'
    Select the 'Hardware' tab.
      Click on 'Device Manager'
        Scan down the list to find 'Ports (COM & LPT)
          Click on '+' to open this section and review COM port numbers.

   For example, a Xilinx Evaluation Kit such as the VC707 board will show something like...
    
      Silicon Labs (CP210x USB to UART Bridge (COM13)   

   Hint - Temporarily disconnecting the USB cable connected to a USB/UART port will typically
          cause the COM port list in the 'Device Manager' to update so you can see which COM 
          port disappears and reappears as you unplug and reconnect it.    


Having entered a COM port number into PicoTerm, it will attempt to open that port. If it is 
unable to open that port then it will tell you. It may be that you specified an invalid port 
number but you should also remember to check that you have no other applications open that are 
already accessing the same port before trying again. 

When a COM port is opened successfully then a message similar to the following will be displayed.

   COM13 is open for communication at 115200 baud.

Then as soon as anything is received from that port or any key is pressed on the keyboard the
screen will automatically clear and start displaying any receive characters. So don't expect 
to see the message above if your hardware is transmitting information as you connect (it will
be obvious that it is working anyway!).


To close PicoTerm press the 'Esc' key or close the window. Although no issues have been 
encountered when simply closing the PicoTerm window, using the 'Esc' key is preferred as it 
does result in a definitive closing of the COM port at the end of the session.



Opening PicoTerm with pre-defined COM port number and/or BAUD rate
------------------------------------------------------------------

If you need to specify a  BAUD rate different to 115200 and/or you already know the COM port
number to specify in advance (i.e. you always want to open the same port) then PicoTerm has two
command line options that you can use.

          PicoTerm -c<port> and -b<baud>

  Examples

     PicoTerm -c 13                   Open COM13 with default baud rate of 115200  

     PicoTerm -c13 -b9600             Open COM13 with baud rate of 9600  

     PicoTerm -b9600                  Set baud rate to 9600 but still prompt for port number.   

   
Supported baud rates are 1200, 2400, 4800, 9600, 19200, 38400, 57600 and 115200.




You can invoke PicoTerm from a DOS window command line or a Batch file. However, it is probably 
easier to create a PicoTerm shortcut and edit its properties. 

How to create a short cut and edit its properties...
   Locate 'PicoTerm.exe' in Explorer.
     Right click on the 'PicoTerm.exe' and select 'Create Shortcut' 
       This should make file called 'Shortcut to PicoTerm.exe' in the same directory
       (note that the icon has a small arrow in a white box on it)
          If you wish, modify the name of the short cut (e.g. 'PicoTerm for COM13').
            Right click on the shortcut and select 'Properties'.
               Append the required options to the 'Target'.
               e.g.   Target:    C:\utilities\PicoTerm\PicoTerm.exe -c13 -b9600  
                  If you want the shortcut on your desktop then simply drag and drop it.




-------------------------------------------------------------------------------------------------
PicoTerm Features
-------------------------------------------------------------------------------------------------

PicoTerm is a deliberately simple terminal but it does incorporate some features that make it 
more compatible with typical PicoBlaze applications as well as for general use.

Wide display with 1.6:1 aspect ratio - Fits well on most landscape monitors and supports 47 lines 
of 144 characters. As with most applications, the physical window size can be adjusted by 
dragging the borders with your mouse but the active terminal size remains 144 x 47 characters.
No characters are displayed once the end of a line has been reached (i.e. line wrapping has been
deliberately prevented from occurring) but will automatically scroll. 


Return (0D hex = 13)    - Carriage return with automatic Line Feed with. Note that this avoids
                          the requirement for Line Feed characters (0A hex = 10) to be 
                          transmitted (except for special circumstances) which helps to keep
                          PicoBlaze programs smaller and easier to develop. Whilst other 
                          terminals can support this automatic line feed functionality it can 
                          often be difficult to find an ASCII setup option to enable it and your
                          display can be a real mess until you do!

Line Feed (0A hex = 10) - Feeds a new line but the cursor remains at the same position
                          along the new line as it was on the previous line.

'VT' (0B hex = 10)      - Moves cursor up one line. Note that the cursor cannot move up if
                          it is already located on the top line of the visible screen. 
                          (i.e. the cursor has to have space to move up into, it will not 
                          cause the display to scroll downwards within the visible screen).

Back Space (08 hex = 8) - Moves the cursor one position to the left and deletes any character 
 or DEL (7F hex = 127)    previously in that position. If the cursor is already at the start 
                          of a line then any character at the start of the line will be 
                          deleted and the cursor will not move (i.e. start of the current line).
                          
TAB (09 hex = 9)        - Advances the cursor to the start of the next column automatically 
                          clearing any previously displayed characters between the current 
                          cursor position and its new position. Each column is 8 characters 
                          wide so the display width of 144 characters is exactly 18 columns.

BEL (07 hex = 7)        - Will make a short sound (providing your speaker is turned on!).



Escape Sequences
----------------

PicoTerm supports the following 'Escape Sequences'....

  Move cursor to HOME position (upper-left of screen) and set text colour to black.

    'ESC' (1B hex = 27)
    '['   (5B hex = 91)
    'H'   (48 hex = 72)

  Clear screen, move cursor to HOME position and set text colour to black.

    'ESC' (1B hex = 27)
    '['   (5B hex = 91)
    '2'   (32 hex = 50)
    'J'   (4A hex = 74)
 
  
  Set the colour for the display of characters that follow.
 
    'ESC' (1B hex = 27)
    '['   (5B hex = 91)
    'n'   (Where 'n' defines the colour)
          ( 1E hex = 30 for Black      )
          ( 1F hex = 31 for Red        )
          ( 20 hex = 32 for Green      )
          ( 21 hex = 33 for Yellow     )
          ( 22 hex = 34 for Blue       )
          ( 23 hex = 35 for Magenta    )
          ( 24 hex = 36 for Cyan       )
          ( 25 hex = 37 for White/Grey )



Device Control Strings
----------------------

PicoTerm also implements some 'Device Control Strings' (DCS) that can be useful in PicoBlaze
applications. When PicoTerm receives one of the DCS sequences then it will perform a special 
operation. Some DCS commands will result in PicoTerm responding with another Device Control 
String containing appropriate information such as the time on the PC whilst others are used 
to open and control separate windows representing virtual LED's and 7-Segment digits.

When a DCS is used to facilitate the transfer of information between PicoBlaze (or similar) 
and the PC (e.g. a request for time) then a 'PicoTerm DCS Transaction' window will automatically
open and display a message confirming the request and information exchanged. This ensures 
that all communications with PicoTerm results in something visible on the PC screen which is 
often reassuring as well as useful during PicoBlaze code development. 

The following characters that begin and end all 'Device Control Strings' have codes that 
are beyond the normal 7-bit ASCII range.
  'DCS' = 'Device Control String' character (90 hex = 144).
   'ST' = 'String Terminator' character (9C hex = 156).
Likewise, the contents of a Device Control String can contain bytes of any value (i.e. data).

Hint - When PicoTerm responds with a Device Control String it always starts with the same 
       character that was used to make the request. Although PicoBlaze would have made the 
       request and therefore should know what response to expect it is often convenient to 
       implement a DCS handing routine that can operate fairly independently. Therefore, having
       the first character of the response string to identify the meaning of the information 
       can be very useful for such a handling routine. Note that the 'ping' sequence is a 
       special case (see description below). 

Hint - Even if PicoTerm makes a DCS request for some information, the PicoTerm DCS response  
       may not be the very next thing waiting to be read from the UART receiver. Other keyboard
       characters may still be waiting in the receiver FIFO buffer and need to be processed 
       first. 

Note that PicoTerm will always transmit a complete DCS response. Keyboard entries can be made 
during the time that a DCS request and response is taking place but keyboard characters will 
always be transmitted either before or after the DCS response (i.e. will never become part 
of the response string).


  'Ping' Sequence
  ---------------

    The 'Ping' sequence provides a simple way for PicoBlaze (or similar) to determine if it 
    is communicating with PicoTerm rather than a different terminal. Whilst a DCS sequence 
    such as requesting the time string (described below) could also be used to achieve the
    same thing the 'Ping' sequence is deliberately simple and the response is easy to handle.

    Request to PicoTerm

    'DCS'      
    'p'     
    'ST'        

    When PicoTerm receives this sequence it will display 'Ping!' in the 'PicoTerm DCS 
    Transaction' window and respond with the following sequence.

 
    Response from PicoTerm

    'DCS'      
    'P'     
    'ST'        
 
    Note that the initial request should contain a lower case 'p' (70 hex = 112) but the 
    response is an upper case 'P' (50 Hex = 80). This is the only time that the DCS response 
    from PicoTerm begins with a different (albeit similar) character. This is to ensure that 
    that a simple echo or loop-back connection cannot be confused with a connection to 
    PicoTerm.



  Time String Sequence
  --------------------

    Request to PicoTerm

    'DCS'      
    'T'     
    'ST'        

    PicoTerm response is a string of 8 ASCII characters describing the current time
    on the PC. The time is 24-hour with an hour value range of '00' to '23'.
    For example...   14:27:58 

    'DCS'  
    'T'  
    '1'
    '4'
    ':'
    '2'
    '7'
    ':'
    '5'
    '8'
    'ST' 


  Time Value Sequence
  -------------------

    Request to PicoTerm

    'DCS'      
    't'     
    'ST'        

    PicoTerm response is a series of 3 byte values representing the current time on the 
    PC in hours, minutes and seconds. 

    'DCS'  
    't'  
    hours     (byte value 00 to 17 hex = 0 to 23)
    minutes   (byte value 00 to 3B hex = 0 to 59)
    seconds   (byte value 00 to 3B hex = 0 to 59)
    'ST' 


  Date String Sequence
  --------------------

    Request to PicoTerm

    'DCS'      
    'D'    
    'ST'       

    PicoTerm response is a string of 11 ASCII characters describing the current date 
    on the PC. The day is always represented by 2 characters, the month by 3 characters
    and the year by 4 characters. For example...   02 May 2012

    'DCS'    
    'D'
    '0'
    '2'
    ' '
    'M'
    'a'
    'y'
    ' '
    '2'
    '0'
    '1'
    '2'
    'ST' 


  Date Value Sequence
  -------------------

    Request to PicoTerm

    'DCS'      
    'd'     
    'ST'        

    PicoTerm response is a series of 3 byte values representing the current date on the 
    PC as year, month and day.  

    'DCS'  
    'd'  
    year      (byte value 00 to 63 hex = 0 to 99)   e.g. '12' for the year 2012.
    month     (byte value 01 to 0C hex = 1 to 12)
    day       (byte value 01 to 1F hex = 1 to 31)
    'ST' 



Virtual 7-Segment Display
-------------------------

Although the main purpose of PicoTerm is to display characters on the screen and allow keyboard 
entries to be made it also has the ability to open a pop-up window containing a virtual 4-digit,
7-segment display. The digits and their segments are identified below.


              Digit 3             Digit 2             Digit 1             Digit 0

                 a                   a                   a                   a
                ___                 ___                 ___                 ___
              |     |
            f |     | b         f |     | b         f |     | b         f |     | b 
              |  g  |             |  g  |             |  g  |             |  g  |
                ___                 ___                 ___                 ___
    
              |     |             |     |             |     |             |     |
            e |     | c         e |     | c         e |     | c         e |     | c
              |  d  |             |  d  |             |  d  |             |  d  |
                ___   p            ___    p             ___   p             ___   p


The virtual display is opened and updated using a 'Device Control String' (DCS) sequence. When 
PicoTerm receives the first virtual display DCS sequence it will open the window and display 
the digits with the specified segments 'turned on'. Subsequent virtual display DCS sequences 
will modify the display to reflect the new control values provided. Note that PicoTerm does 
not transmit a DCS sequence back to the COM port.

The DCS sequence for the virtual 7-Segment display is as follows (please read the 'Device Control 
Strings' section above if you are unfamiliar with this concept)...

    'DCS'      
    '7'
    digit0_segment_control_byte    
    digit1_segment_control_byte    
    digit2_segment_control_byte    
    digit3_segment_control_byte    
    'ST'       

The segments of each digit are controlled by the bits contained in the control bytes. Each 
digit has 7 segments and a decimal point and a segment will be 'turned on' when the corresponding
bit of the control byte is High (1).

           Segment  Bit
        
              a      0
              b      1                  
              c      2
              d      3
              e      4
              f      5
              g      6
              p      7   decimal point


Hint - See the 'nibble_to_7seg' routine provided in the 'PicoTerm_routines.psm' file.



Virtual LED Display
-------------------

Although the main purpose of PicoTerm is to display characters on the screen and allow keyboard 
entries to be made it also has the ability to open a pop-up window containing 24 virtual LEDs.
There are 8 red, 8 yellow and 8 green LEDs arranged in 3 rows as shown below.

         
                      7   6   5   4   3   2   1   0
         
             Red      O   O   O   O   O   O   O   O
             Amber    O   O   O   O   O   O   O   O
             Green    O   O   O   O   O   O   O   O
         

The virtual display is opened and updated using a 'Device Control String' (DCS) sequence. When 
PicoTerm receives the first virtual LED display DCS sequence it will open the virtual LED window
with the specified LEDs 'turned on'. Subsequent virtual LED display DCS sequences will modify 
the LEDs to reflect the new control values provided. Note that PicoTerm does not transmit 
a DCS sequence back to the COM port.

The DCS sequence for the virtual LED display is as follows (please read the 'Device Control 
Strings' section above if you are unfamiliar with this concept)...

    'DCS'      
    'L'
    RED_control_byte    
    YELLOW_control_byte    
    GREEN_control_byte    
    'ST'       

The virtual LEDs of each colour are controlled by the corresponding bits contained in each of 
control bytes. For example the least significant bit of 'GREEN_control_byte' will control the 
virtual LED in the lower right hand corner of the display.



Invalid Characters and Control Sequences
----------------------------------------

In an ideal world your application will only transmit valid characters and valid escape and DCS
sequences to PicoTerm and everything will work precisely as intended. However, mistakes do 
happen especially during code development so it is useful to know how PicoTerm has been designed
to react when it receives unexpected characters and sequences.

Any ASCII codes for characters with a value greater than 127 (7F hex) will be displayed as a '*'
in the main terminal window. Typical causes of this are when an application mistakenly transmits 
a byte value as a raw 8-bit value rather than converting it to representative ASCII characters 
or when there has been an incorrect DCS definition and byte values associated with the attempt 
are then interpreted as ASCII characters (see below).

When an escape sequence does not match with any of those supported then PicoTerm will abandon
the processing of the sequence at the earliest opportunity with any subsequent characters being 
displayed in the main terminal window. For example, if a mistake is made when attempting to 
issue a clear screen sequence such that 'ESC', '[', '3', 'J' is transmitted to PicoTerm rather 
that 'ESC', '[', '2', 'J' then PicoTerm will abandon the sequence as soon as '3' is received 
and both '3' and 'J' will be displayed in the main terminal window. 

In a similar way, when a DCS sequence does not match with any of those supported then PicoTerm
will abandon the processing of the sequence at the earliest opportunity. 'Invalid string! will 
be displayed in the 'PicoTerm DCS Transactions' window and any subsequent characters will be 
displayed in the main terminal window. Since some DCS sequences are associated with byte data 
the subsequent characters displayed could be anything and quite often '*' would be observed 
because byte values can easily be in the range 128 to 255 (80 to FF hex). 



-------------------------------------------------------------------------------------------------
End of 'PicoTerm_README.txt'
-------------------------------------------------------------------------------------------------

