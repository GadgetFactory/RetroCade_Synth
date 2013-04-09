
© Copyright 2010-2012, Xilinx, Inc. All rights reserved.
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
KCPSM6 : PicoBlaze for Spartan-6, Virtex-6 and 7-Series Devices 
-------------------------------------------------------------------------------------------------

Release 5.

Ken Chapman - Xilinx Ltd - 30th September 2012  

Welcome to the fifth release of KCPSM6, the PicoBlaze optimised for Spartan-6, Virtex-6 and 
7-Series devices. Whether this is your first experience of PicoBlaze or you have experience of 
the previous versions we hope you will find KCPSM6 useful, and most of all, fun! This package 
also contains optimised UART macros (and PicoTerm) which are ideal for use with PicoBlaze 
especially as so many boards provide a USB/UART interface.  

Although KCPSM6 is not difficult to use, it is highly recommended that you take some time to 
look at the documentation. In particular, pages 5 to 30 of 'KCPSM6_User_Guide_30Sept12.pdf' 
introduce you to KCPSM6 and take you step by step through the complete design flow and should 
result in your first working design. Everything else is reference so please don't feel that you 
need to read everything before you start (that wouldn't be fun!). The UART macros are described 
in 'UART6_User_Guide.pdf'and are accompanied by couple of simple but complete PicoBlaze reference 
designs to get you started. The designs are presented on the ML605 Virtex-6 Evaluation Kit and 
the ATLYS Spartan-6 Design Platform but both could be easily ported to alternative hardware. 

Likewise, this may be called the 'READ_ME_FIRST' file but it does contain quite a lot of 
additional information! Whilst you will not need to read every word meticulously before you 
start using KCPSM6, it is highly recommended that you take a quick look at the 'Requirements',
'Known Limitations' and 'Known Issues' sections to obtain a general appreciation of what they 
do contain. In that way, you should immediately avoid any pitfalls (e.g. if using ISE v12.x)
and know that there is more advice to read should you stumble across one of the more unusual 
situations. Certainly you should remember to come back and look in the 'Known Issues' section
if you do encounter any issues using KCPSM6 in your designs in the future. 

It should be noted that the default files provided in this package assume that you will be using 
ISE v13.x or later as this is the version of the tools required to implement 7-Series designs.
However, if you are currently happy using ISE v12.x to implement a Spartan-6 or Virtex-6 design 
and do not wish to update at this time then just take a look at 'Potential issues when using ISE 
12.x' in the 'Known Issues' section towards the end of this file. This describes which file you 
should use to ensure everything works smoothly for you first time.

For general discussion and to share ideas and experiences please make use of the PicoBlaze forum.
It is likely that commonly asked questions will be discussed here and provide a resource for 
everyone to be able to read.  

http://forums.xilinx.com/t5/PicoBlaze/bd-p/PicoBlaze

Xilinx Technical support is also available to answer your questions. However it is recommended
that you take the time to consider exactly what your issue is before asking any questions. Just 
because your design contains a PicoBlaze processor doesn't mean you actually have a problem 
with PicoBlaze! If you are a Xilinx novice and encounter difficulties then make sure you can 
get a simple HDL design through ISE before including PicoBlaze.   

http://www.xilinx.com/support/clearexpress/websupport.htm

Although this is the fifth public release of KCPSM6, further feedback would still be appreciated 
to help improve the complete solution for you and other users in the future. Please send your 
comments via email to 'chapman@xilinx.com'. 


Contents of this file
---------------------

  - Principle Features of KCPSM6
  - Package Contents
  - Requirements
  - Changes and Additions in each release.
  - Other Useful Stuff?
  - Known Limitations




-------------------------------------------------------------------------------------------------
Principle Features of KCPSM6
-------------------------------------------------------------------------------------------------

  - Only 26 Slices plus program memory (BRAM).
  - 100% embedded solution.
  - 8-bit data.
  - Performance 52 MIPS to 120 MIPs depending on device family and clock rate. 
  - Supports programs up to 4K instructions.
  - 32 General Purpose Registers arranged in 2 banks.
  - 256 General Purpose Input Ports.
  - 256 General Purpose Output Ports.
  - 16 Constant-Optimised Output Ports.
  - 64-bytes of scratch pad memory expandable to 128 and 256-bytes (additional 2 and 6 Slices).
  - Fully automatic CALL/RETURN stack supporting nested subroutines to 30 levels. 
  - Interrupt with user definable interrupt vector and maximum response time of 4 clock cycles.  
  - Power saving features including 'sleep' mode.
  - Superset of KCPSM3 with high degree of code compatibility.


-------------------------------------------------------------------------------------------------
Package Contents
-------------------------------------------------------------------------------------------------


              READ_ME_FIRST.txt - This file!

 KCPSM6_User_Guide_30Sept12.pdf - The main KCPSM6 User Guide document.

   Reference_Design_License.pdf - Copy of the Reference Design License Agreement under 
                                  which KCPSM6 and the UART macros are released.

    kcpsm6_assembler_readme.txt - Supplementary information related specifically to the KCPSM6 
                                  assembler. Describes in detail the options and features when 
                                  invoking the assembler from batch files or using the 'drag 
                                  and drop' technique.   

          all_kcpsm6_syntax.psm - Examples and detailed descriptions of all PSM syntax supported 
                                  by the KCPSM6 assembler. 

                     kcpsm6.vhd - The KCPSM6 Processor v1.1.
 
                     kcpsm6.exe - The KCPSM6 Assembler for Windows v2.31.

                   ROM_form.vhd - Default program memory template for use during development.
 
     kcpsm6_design_template.vhd - Collection of VHDL reference code for KCPSM6 designs.



                    JTAG_Loader - A sub-directory containing JTAG Loader executables. Select 
                                  the executable file corresponding with your operating system. 
                                  Please note that simplicity the documentation (i.e. pages 25-29 of
                                  'KCPSM6_User_Guide_30Sept12.pdf') assumes that the selected 
                                  executable has been renamed 'jtagloader' but you can retain the 
                                  original name if you prefer.  

                       JTAG_Loader_WinXP_32.exe - Windows-XP 32-Bit.
                       JTAG_Loader_WinXP_64.exe - Windows-XP 64-Bit.
                        JTAG_Loader_Win7_32.exe - Windows-7 32-Bit.
                        JTAG_Loader_Win7_64.exe - Windows-7 64-Bit.
                              JTAG_Loader_RH_32 - Linux 32-Bit.
                              JTAG_Loader_RH_64 - Linux 64-Bit.
                                   msvcr100.dll - This DLL is required by JTAG Loader when using Windows-7
                                                  (see 'Requirements' section below).


 
             ROM_form_templates - A sub-directory containing a copy of the default templates and 
                                  ten optional 'production' templates. Their use is described on
                                  page 47 of the user guide and in 'kcpsm6_assembler_readme.txt'.
                                  Each file includes a comprehensive description. 
     
                       ROM_form_JTAGLoader_16Aug11.vhd - Copy of the default development template 
                                                         use with ISE 13.x and later
                       ROM_form_JTAGLoader_3Mar11.vhd - Development template for ISE 12.x users

                       ROM_form_S6_1K_5Aug11.vhd - Spartan-6 1K (1 BRAM)
                       ROM_form_S6_2K_5Aug11.vhd - Spartan-6 2K (2 BRAM)
                       ROM_form_V6_1K_5Aug11.vhd - Virtex-6 1K (0.5 BRAM)
                       ROM_form_V6_2K_5Aug11.vhd - Virtex-6 2K (1 BRAM)
                       ROM_form_V6_4K_5Aug11.vhd - Virtex-6 4K (2 BRAM)
                       ROM_form_7S_1K_5Aug11.vhd - 7-Series 1K (0.5 BRAM)
                       ROM_form_7S_2K_5Aug11.vhd - 7-Series 2K (1 BRAM)
                       ROM_form_7S_4K_5Aug11.vhd - 7-Series 4K (2 BRAM)

                       ROM_form_7S_2K_with_error_detection_9Aug12.vhd - 7-Series 2K (1 BRAM) with 
                                                  error detection circuit (see User Guide page 117) 

                       ROM_form_256_5Aug11.vhd - Spartan-6, Virtex-6 and 7-Series 
                                                   0.25K (18 Slices)




                        verilog - A sub-directory containing the Verilog equivalent of all the 
                                  VHDL files to be used in exactly the same way in a design.

                                    kcpsm6.v
                                    ROM_form.v
                                    kcpsm6_design_template.v
                                    ROM_form_JTAGLoader_16Aug11.v
                                    ROM_form_JTAGLoader_3Aug11.v
                                    ROM_form_S6_1K_5Aug11.v
                                    ROM_form_S6_2K_5Aug11.v
                                    ROM_form_V6_1K_5Aug11.v
                                    ROM_form_V6_2K_5Aug11.v
                                    ROM_form_V6_4K_5Aug11.v
                                    ROM_form_7S_1K_5Aug11.v
                                    ROM_form_7S_2K_5Aug11.v
                                    ROM_form_7S_4K_5Aug11.v
                                    ROM_form_256_5Aug11.v

                        
                           UART - A sub-directory containing the Ultra-Compact UART macros
                                  together with documentation (readme and PDF), reference designs
                                  and a terminal application (PicoTerm) that is ideally suited 
                                  for use with PicoBlaze based designs.

                                    UART6_README.txt
                                    UART6_User_Guide_30Sept.pdf

                                    uart_rx6.vhd
                                    uart_tx6.vhd
                                    uart_rx6.v
                                    uart_tx6.v

                                    PicoTerm_README.txt
                                    PicoTerm.exe
 
                                    ML605_design - Reference design presented on ML605 board.

                                                     uart6_ml605.vhd
                                                     uart6_ml605.v
                                                     uart_control.psm
                                                     uart_interface_routines.psm
                                                     uart6_ml605.ucf

                                    ATLYS_design - Reference design presented on ATLYS board.

                                                     uart6_atlys.vhd
                                                     uart6_atlys.v
                                                     atlys_real_time_clock.psm
                                                     PicoTerm_routines.psm
                                                     soft_delays_100mhz.psm
                                                     uart6_atlys.ucf        


                  Miscellaneous - A sub-directory for miscellaneous files 
                                  described in 'Known Issues' where appropriate.



-------------------------------------------------------------------------------------------------
Requirements
-------------------------------------------------------------------------------------------------

ISE v13.x or later (Please see 'Known Issues' below if using ISE 12.x). KCPSM6 has also been used
successfully with Vivado 2012.x.

Windows operating system for KCPSM6 Assembler and JTAG Loader utility.
   Users have reported that the KCPSM6 Assembler has worked well with both 32-Bit and 64-bit 
   versions of both Windows-XP and Win-7 environments. It has also been successfully used with 
   Wine within a 64-bit Linux environment.   

Platform Cable USB or Digilent equivalent JTAG programming solution for JTAG Loader. Note that
these circuits are often included on development boards and evaluation kits rather than as a 
separate 'pod'. 

To state the obvious, KCPSM6 is optimised for Spartan-6, Virtex-6 and 7-Series devices. It will 
NOT map to any previous generations of device including Spartan-3 Generation and Virtex-5.  

For JTAG Loader to work correctly it must know the location of your Xilinx ISE software. This 
requires both your 'PATH' and 'XILINX' environment variable to be set appropriately. Below are 
typical examples for an ISE v13.2 installation. It is possible that they have already been set 
but check that they accurately reflect your installation. 

PATH = C:\Xilinx\13.2\ISE_DS\common\lib\nt;C:\Xilinx\13.2\ISE_DS\ISE\lib\nt;
XILINX = C:\Xilinx\13.2\ISE_DS\ISE

As shown on page 25 of 'KCPSM6_User_Guide_30Sept12.pdf' the 'ISE Design Suite Command Prompt'
can be used, but for maximum flexibility when using JTAG_Loader (e.g. the ability to invoke 
JTAG Loader from a batch file located anywhere on your PC) it is highly recommended that PATH
and XILINX be set as part of your 'System Properties'.... 
  Right click on 'My Computer' and select 'Properties'. 
  Go to the 'Advanced' tab and choose 'Environment Variables'. 
  Use 'New' or 'Edit' as necessary.   

JTAG Loader must also access some system level DLL files. In the case of a Windows-XP environment 
then it is normal for the PATH to contain 'C:\WINDOWS\system32;' or similar. So if you receive 
a system error indicating that 'PCMSVCR100.dll' is missing or could not be found then add the 
appropriate definition to your PATH. When using a Windows-7 environment then it is more likely
that 'MSVVCR100.dll' will become the subject of a system error message and in this case your 
PC may not have a copy of this file at all as it is not part of a default Windows-7 installation.
So the quickest solution is to place a copy of 'msvcr100.dll' provided in this zip file in to  
the same directory as the JTAG Loader executable.



-------------------------------------------------------------------------------------------------
Changes and Additions in each release
-------------------------------------------------------------------------------------------------


Release 1 (30 September 2010)
-----------------------------

Initial public release.


Release 2 (31 March 2011)
-------------------------

kcpsm6.vhd
  Correction to logic used to calculate parity (CARRY flag) during a TEST instruction.
  The specific conditions under which v1.0 would generate an incorrect result were:-
    TEST instruction (not TESTCY).
    The least significant 2 bits of the logical AND register had to be "01".
    The carry flag had to be set before the TEST instruction was executed. 
  The most likely coding style in which the defect in v1.0 would be observed would be...
       TEST s4, 01
       JUMP C, bit0_was_set 
  Even the above code works in v1.0 if the CARRY flag was clear before the TEST instruction.
  Fortunately the most common coding style uses the ZERO flag which works correctly so 
  the alternative code shown below would work perfectly in v1.0 as well...
       TEST s4, 01
       JUMP NZ, bit0_was_set 
   
Additions to ROM_form templates.
  Mainly additions including support of 7-Series devices but some minor corrections too.

Assembler
  Please see 'kcpsm6_assembler_readme.txt' for additions.  

User Guide
  General additions and corrections.  

Verilog equivalent of all VHDL files is now provided.

64-bit version of JTAG_Loader provided in addition to standard version. Physical size of 
executable files now much smaller (19KB compared with 468KB).


Release 3 (30 September 2011)
-----------------------------

Adjustments to ROM_form templates to be compatible with with ISE 13.x. 
ROM_form templates to support 7-Series devices.

Assembler
  Please see 'kcpsm6_assembler_readme.txt' for additions.   

User Guide
  General additions and corrections.  

Optimised UART6 macros provided with reference design and documentation.


Release 4 (30 April 2012)
-------------------------

JTAG Loader
  Support for Linux operating system.
  Support for Digilent JTAG Programming solution.
  Support for spaces in the names of directories in PATH specifications.

Assembler
  Addition of an 'INCLUDE' directive.
  Support for spaces in the names of directories in PATH specifications.
  Enhancements to LOG file contents.
  Generation of a 'session log' file. 
  Please see user guide, 'kcpsm6_assembler_readme.txt' and 'all_kcpsm6_syntax.psm' for details.

UART
  Verilog version of the reference design to complement the existing VHDL version.
  PSM reference code provides an example of the new INCLUDE directive and is more portable .
  PicoTerm v1.03 is provided as a simple terminal ideal for use with PicoBlaze designs.

General additions, enhancements and corrections to all documentation.


New in this release (Release 5 - 30 September 2012)
---------------------------------------------------

Assembler
  Enhancements to LOG file contents including instruction and memory usage statistics.
  Ability to specify an environment variable in a CONSTANT, STRING or TABLE directive
    which in turn provides the value, string or data list. Please see description in 
    'all_kcpsm6_syntax.psm' for details.
  Support for 'ROM_form_7S_2K_with_error_detection_9Aug12.vhd' (see 'Documentation' below).
  Examples of 'HWBUILD' instruction added to 'all_kcpsm6_syntax.psm'.

UART
  An additional reference design presented on the ATLYS Spartan-6 Design Platform
    and exploiting new special features provided in PicoTerm (Hint: Reusable code).   
  PicoTerm v1.30 includes a Virtual 7-Segment Display, Virtual LEDs and other features 
    as well as provided the simple terminal ideal for use with PicoBlaze designs.

JTAG Loader
  New command line option (-i) can be used to modify the BSCAN 'USER' number.
    Please see 'JTAG Loader and BSCAN Users' in the 'Known Issues' section below.

Documentation 
  A detailed study and discussion concerning KCPSM6 reliability (see pages 106-118 of 
    'KCPSM6_User_Guide_30Sept12.pdf'). In addition to the documentation an error detection
    scheme has been provided for a 2K program memory in 7-Series devices for those seeking 
    the very highest levels of design reliability.
  General additions, enhancements and corrections to all documentation.




-------------------------------------------------------------------------------------------------
Other Useful Stuff?
-------------------------------------------------------------------------------------------------

The following may be of interest....


openPICIDE
----------

http://www.openpicide.org

openPICIDE is an integrated assembler development environment for the PicoBlaze processor family
and has been developed by Christoph Fauck and works with Windows, Linux and Mac.




-------------------------------------------------------------------------------------------------
Known Limitations
-------------------------------------------------------------------------------------------------


iSim Display of Strings
-----------------------

The way in which iSim displays text strings is not ideal for the observation of 'kcpsm6_opcode'
and 'kcpsm6_status' during simulation. Hopefully this will be resolved in a future release of
ISE.


DATA2MEM
--------

There are some issues associated with the use of DATA2MEM contained in ISE 12.x that prevent its 
use in modifying a KCPSM6 program contained in block memory within Spartan-6 and Virtex-6 
devices. It may be possible to use DATA2MEM in ISE 13.x but at this time it has not been 
verified and the method is not described. Please use JTAG Loader which has the advantage of being
faster anyway. If you have a  requirement to modify more than one KCPSM6 program in the same 
design then please contact us to discuss how this may be achieved using JTAG Loader.


4K Program for Spartan-6 Designs
--------------------------------

There is currently no ROM_form templates supporting 4K in a Spartan-6 device. Please contact 
the author (chapman@xilinx.com) to discuss your requirements.  



-------------------------------------------------------------------------------------------------
Known Issues
-------------------------------------------------------------------------------------------------


Potential issues when using ISE 12.x 
------------------------------------

Using ISE v12.x may result in XST generating errors similar to the following...

    HDLCompiler:1314 - "<program_ROM_filename>.vhd" Line 390: 
      Formal port/generic <rdaddr_collision_hwconfig> is not declared in <ramb18e1>

The logic primitives including BRAM are defined in libraries that XST reads as it elaborates
your design. Not surprisingly these libraries change with each release of the ISE tools. In 
particular the ISE v13.x tools introduced support for the 7-Series devices a obviously this 
required quite significant additions and alterations to the underlying libraries. 

The ROM_form templates have been prepared to match with the libraries provided with ISE v13.x
and therefore the potential exists for a something to be absent from an older library 
resulting in a error similar to that shown above. 

It is recommended that you use ISE v13.2 or later with KCPSM6 but if this is not convenient 
then simply replace the default 'ROM_form.vhd' or 'ROM_form.v' template with a copy of an 
older template that was supplied with 'Release 2' of KCPSM6 and included in this package 
for your convenience.

  Provided for use with ISE v13.x or later

               ROM_form_JTAGLoader_16Aug11.vhd    
               ROM_form_JTAGLoader_16Aug11.v  

  Provided for use with ISE v12.x only

               ROM_form_JTAGLoader_3Mar11.vhd    
               ROM_form_JTAGLoader_3Mar11.v  




WARNING:Xst:647 - Input <instruction<0:11>> is never used
---------------------------------------------------------

XST in ISE v12.x and ISE v13.x incorrectly reports the following warning message....

   WARNING:Xst:647 - Input <instruction<0:11>> is never used. 

This warning can safely be ignored. However, it should be recognised that if a similar warning 
message reports that all 18-bits of instruction (instruction<0:17>) are never used then it 
probably means that you really didn't connect the program memory to KCPSM6 correctly. 

This issue was fixed in ISE version 14.1. 



JTAG Loader not working
-----------------------

If you experience any issues related to not being able to find a DLL then please check the 
'Requirements' section above to ensure that your environment variables are set appropriately. 

JTAG Loader may not work correctly if you have more that Platform Cable USB connected to your 
PC at the same time so the obvious workaround is only to connect the cable associated with 
the chain in which your target device is located. Please note that many development boards and 
evaluation kits such as ATLYS or ML605 boards have Platform Cable USB circuit or Digilent 
equivalent included on them so the most common reason for appearing to have multiple cables 
connected is when one or more of these boards are connected (or one of these boards and a real
Platform Cable USB).



JTAG Loader may take ~25 seconds to load a new program when using an ATLYS board
--------------------------------------------------------------------------------

Loading a KCPSM6 program normally takes about 5 seconds so this issue is under investigation.
However, it is only a case of being slower than expected; operation is correct and reliable.



Designs containing multiple KCPSM6 processors
---------------------------------------------

It is common practice for designs to contain multiple instances of PicoBlaze with each typically 
acting as an independent 'state machine'. There are also some designs in which hundreds, or even 
thousands, are used to implement amazing structures and algorithms. Regardless of how many 
KCPSM6 processors are included, the use model and design method is really the same for each 
instance so there is really very little to consider just because there is more than one. That
said, the following points may be helpful in making your multi-KCPSM6 enjoyable.

When using the default 'ROM_form' template your assembled program file provides you with the 
option to enable the JTAG Loader circuit. However, you do need to remember that only one program 
memory can have this feature enabled at any time. Hence, only one instance can have 
'C_JTAG_LOADER_ENABLE' assigned the value '1' and all other instances must be assigned '0'.

Although compliance with the fundamental limitation described above should result in a design 
that will successfully pass through the ISE tools you will find that WARNING messages are 
generated for each instance or program memory assembled using the default 'ROM_form' template.
This is because the default 'ROM_form' template includes the definition of the JTAG Loader 
circuit and this means that synthesis observes a repeated definition of the JTAG Loader circuit
(and a function) in each instance irrespective of the loader being enabled or not. These warnings
can be safely ignored but if you are looking for more elegance (and why shouldn’t you?), then 
here are two techniques for you to consider.

a) Replace the default 'ROM_form' template with the appropriate 'Production Memory' template. 
   These are described on page 47 of the KCPSM6 user guide and in 'kcpsm6_assembler_readme.txt'.
   Once a program is assembled using a production template then the memory definition file 
   only contains the specific BRAM(s) necessary for your application. JTAG Loader is no longer 
   included and hence replicated definition is avoided. Use of 'Production Memory' does require
   small changes to your design (i.e. the instantiation no longer includes generics or the 'rdl'
   port) but this is a recommended step before release of a product anyway and suitable once 
   any program has become stable.

b) If you still want to maintain the ability to enable the JTAG Loader on a program instance 
   in the design (obviously you can only enable one at a time) then you have to keep the 
   JTAG Loader option available within each instance. To avoid those warning messages you 
   need to ensure that the JTAG Loader is only defined once in overall design. This ultimately 
   means separating the definition of JTAG Loader from the definition of the program memory.

   Start by making a copy of the default template ('ROM_form_JTAGLoader_16Aug11.vhd') and remove 
   all items defining JTAG Loader. Locate and delete the code near the top that defines a function
   called 'addr_width_calc' and all the code towards the end following the 'JTAG Loader' comment
   banner that defines the actual JTAG Loader circuit. In the 'Miscellaneous' directory you can 
   find 'ROM_form_for_multiple_instances.vhd' which has already had these items removed from the 
   default template and ready for you to use.

   You must (only) assemble one program using the default template which will include the definition 
   of JTAG Loader. All other programs must be assembled using the modified template which only 
   define the program memory (hint - assemble programs in different directories containing the 
   'ROM_form' template required).



'global_opt' may result in incorrect implementation
---------------------------------------------------

Setting 'global_opt' to anything other than 'off' (the default) in MAP when using ISE v13.1 or
ISE 13.2 may result in incorrect implementation of the KCPSM6 logic and therefore a failure to 
execute code in the way expected (e.g. shift and rotate operations may not work properly). The 
'area' setting may even prevent your design from passing through the MAP at all. This issue had
not been observed when using ISE v12.4 and there are no issues as long as 'global_opt' is set 
to 'off'. Note that when using ISE v13.2 to target a 7-Series device the 'global_opt' option 
is not available and therefore this issue can only occur when targeting Spartan-6 or Virtex-6.
 
The cause of this issue was located and then fixed in ISE v13.4 so you should use ISE v13.4 
when 'global_opt' needs to be set to anything other than 'off' in order to process other parts
of your design.



'Pack:2811' errors in MAP when using ChipScope
----------------------------------------------

Connection of ChipScope can generate 'Pack:2811' errors in MAP. This mainly appears to happen 
when connecting 'out_port' or 'port_id' directly to ChipScope. It has also been known to 
happen when connecting ChipScope directly to the 'address' or 'instruction' ports.
  
There are four potential workarounds for this issue.
  a) To insert a pipeline register between KCPSM6 and ChipScope.
  b) Set the 'Keep Hierarchy' option in XST to 'Yes' (default is 'No').
     However this may not work if there are more than one KCPSM6 (see below).  
  c) Set the following system environment variable: XIL_MAP_OLD_SAVE=1.
     Close ISE.
     Right click on 'My Computer' and select 'Properties'.
     Go to the 'Advanced' tab and choose 'Environment Variables'. 
     Use 'New' or 'Edit' as necessary.
     Open and run ISE again.  
  d) Remove or comment out all the Slice packing directives (HBLKNM attributes) in the KCPSM6 
     source file. The 'kcpsm6_without_slice_packing_attributes.vhd' located on the 'Miscellaneous' 
     directory already has these attributes commented out. Using this workaround will result in 
     KCPSM6 occupying more Slices and having a lower peak performance and therefore it is 
     better to only resort to using it if the other methods cannot be used or are unsuccessful. 


'Pack:2811' errors in MAP when using 'Keep Hierarchy' and design contains multiple KCPSM6.
------------------------------------------------------------------------------------------

This error has been observed when a design contains multiple instances of KCPSM6 and 
the 'Keep Hierarchy' option in XST has been set to 'Yes'. Therefore the obvious solution is
to revert to the default setting of 'No'. Alternatively the 'Allow Logic Optimization Across
Hierarchy' option in MAP can be enabled (tick box in Project navigator or apply the 
-ignore_keep_hierarchy switch on the command line).

If it is undesirable to adjust your implementation settings then please see 'c' and 'd' 
workarounds in the issue above. 


'Pack:2811' errors in MAP when using a low 'Max Fanout' value in XST.
---------------------------------------------------------------------

The 'Max Fanout' parameter is a 'Xilinx Specific Option' for XST and has the default value 
of 100000 for the devices used with KCPSM6. It has been known for very low values (e.g. <20)
to result in a subsequent error in MAP. Should this occur, please increase the value. If 
you have a particular reason to use such a low value then synthesize KCPSM6 separately and 
include it in your design as a 'black box'.


'PhysDesignRules:1422' errors reported by BITGEN
------------------------------------------------

Should this error report occur then it will probably look something like this....

ERROR:PhysDesignRules:1385 - Issue with pin connections and/or configuration on
block:<instance_name>/stack_ram_high_RAMD_D1>:<LUT_OR_MEM6>. For RAMMODE programming
set with DPRAM32 or SPRAM32 or SRL16 the DI2 input pin must be connected.

This error has only occurred in designs where the user has not connected all 12-bits of the 
address bus to a program memory. Hence the simple and obvious solution is to ensure that 
all address bits are connected to something.

Regardless of the memory size, all the supplied 'ROM_form' templates connect all address bits
to something so that these signals and associated logic are preserved. This makes it easier to
increase or decrease the memory size and avoids warning messages (as well as this error). As 
such, if you have encountered this error you are probably using your own 'ROM_form' template 
in which one or more of the (most significant) address bits are not required and have not been
connected to something in order to preserve them.

Whilst it is generally a good idea for unused logic to be trimmed, KCPSM6 is so optimised 
to the architecture and so tightly packed into the logic Slices that any logic trimming is
insignificant. Furthermore, any trimming only leads to the formation of unusable 'holes' in 
otherwise used Slices so there is nothing to be gained. This is particularly true of what 
happens to the memory used to implement the program counter stack when any of the address 
bits are unused and leads to the error being generated. Obviously it would be better if the 
tool chain could handle this better but it just happens to be one of those cases that is 
more challenging than it first appears to be!  



JTAG Loader and BSCAN Users 
---------------------------

The JTAG Loader utility employs a BSCAN primitive within the device to form a bridge to the 
KCPSM6 program memory. Other applications may also exploit a BSCAN primitive such as ChipScope
which implements a bridge between ChipScope Analyser and an associated ICON core. The good news
is that there are four BSCAN primitives in each device so it is unlikely that you will not have
enough of them. Clearly, if you do exceed the number available then your only recourse is to  
reduce the number required; possibly disabling JTAG Loader so that the rest of your design can 
fit will be the easiest solution.

However, since there are normally enough BSCAN primitives available, the more common issue 
relates to the allocation of the 'USER' address to each BSCAN primitive. As provided, the JTAG 
Loader is assigned to 'USER2'. When generating an ICON core for ChipScope there is an option to 
set the 'boundary scan' value to USER1, USER2, USER3 or USER4 but this is normally set to 'USER1' 
so in most cases ChipScope and JTAG Loader happily co-exist.

If you find it necessary to assign JTAG Loader to a different 'USER' then you will need to 
make a small adjustment to hardware and use JTAG Loader with the '-i' option as shown below.

To modify the hardare, open the default 'ROM_form' template and locate the line shown below and 
adjust the number '2' to '1', '3' or '4' as desired...

    'ROM_form.vhd' (approximately line 256 and part of 'component jtag_loader_6') 
  
             C_JTAG_CHAIN : integer := 2;
      
    'ROM_form.v' (approximately line 295 and part of 'module jtag_loader_6')
 
             parameter integer C_JTAG_CHAIN = 2;

You will then need to assemble your PSM file such that the new assignment is transferred into
your actual design file. Obviously, since this is a change to the hardware definition you must 
also process the whole design, generate a BIT file and configure the device with it too.

When you run the JTAG Loader utility it assumes the default USER number is '2' so you will now 
need to use the '-i' option to specify the same USER number that you defined in your template.
For example, if the line in the VHDL template was changed to 'C_JTAG_CHAIN : integer := 4;' 
then to update the KCPSM6 program using JTAG Loader the command will be...

     jtagloader -i4 -l your_program.hex

   (where 'jtagloader' is the required executable for your operating system). 

 


Program memory corrupted when device configured with ChipScope Analyser.
------------------------------------------------------------------------

If your PicoBlaze design has JTAG Loader enabled and the device is configured with the BIT 
file using ChipScope Analyser (rather than iMPACT) then this can result in corruption to one 
location of the program memory. The corruption will almost certainly be to the 4th instruction
in the memory (address 003) and result in that location being cleared to 00000 Hex. This value
is equivalent to a 'LOAD s0, s0' instruction which will do nothing but obviously that still 
means that your intended instruction is missing and this will still effect the execution of 
your program.

Note that there does not need to be a ChipScope Core in the design, it is purely the act of 
using ChipScope Analyser to configure the device that has this effect. The issue is related 
to the way ChipScope Analyser searches for a ChipScope core in your design and that process 
interfering with JTAG Loader which makes use of a BSCAN primitive in a very similar way to 
that of a ChipScope core. 

If you suspect that this is happening then JTAG Loader can be used to confirm it using its 
read back facility. First read back the contents of the memory into a temporary hex file...

   jtagloader -r temp.hex

Then compare the contents of this hex file with the hex file generated by the KCPSM6 assembler
for your program. It should be easy to see the '00000' value near the top of the file if 
corruption has taken place.

Fortunately there are several workarounds:-

  a) Use iMPACT to configure the device with your BIT file.
  b) Disable JTAG Loader if you don't need to use it.
  c) Following configuration by ChipScope Analyser, use JTAG Loader to reload the program 
     memory with a valid image (you will do this naturally if using JTAG Loader during 
     program development).
  d) Start your PSM program with directive 'ADDRESS 004' such that your code begins at 
     address 004. In this way the first four locations of memory will default to 00000 hex
     and the clearing effect of the corruption will become irrelevant.
  e) Modify the ChipScope Analyser project file '.cpj' as described below.
     
     Note that this method requires ISE v13.3 or later to work correctly. 

       i) Insert the line 'avoidUserRegDevice0=2' in your '.cpj' file.
          This tells ChipScope Analyser to avoid 'USER2' which is assigned to JTAG Loader
          from being scanned in the first device (device '0') in the JTAG chain. Adjust 
          as appropriate for the device position in your JTAG chain.
      ii) Start ChipScope Anaylser and open the project (.cpj file). You must do this first.
     iii) Then you can 'Open Cable/Search JTAG Chain' and you should see a messages similar
          to "INFO: Skipping xsdb core scan on device 0, user register 2" displayed in 
          the console confirming that 'USER2' has been avoided.
      iv) Configure your device (probably worth using 'jtagloader -r' to confirm that 
          everything worked correctly the first time you try it but should be Ok after that).

     Answer Record 19337 (http://www.xilinx.com/support/answers/19337.htm) may also be 
     useful reference when using ChipScope Analyser.


-------------------------------------------------------------------------------------------------
End of file 'READ_ME_FIRST.txt'
-------------------------------------------------------------------------------------------------
