
# PlanAhead Launch Script for Pre-Synthesis Floorplanning, created by Project Navigator

create_project -name gatemanpoly -dir "C:/dbdev/My Dropbox/GadgetFactory/Staging_Area/trunk/gatemanpoly/planAhead_run_1" -part xc3s250evq100-4
set_param project.pinAheadLayout yes
set srcset [get_property srcset [current_run -impl]]
set_property top GateManPoly $srcset
set_param project.paUcfFile  "GateManPoly.ucf"
set hdlfile [add_files [list {sat.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {DeltaSigma.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {tuning_ROM.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {TTYuartrx.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {SVF_multi_1.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {spi_dac_out.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {sine_tab.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {noise_iir.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {MIDIuartrx.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
add_files [list {MIDICTRL.V}]
set hdlfile [add_files [list {kcpsm3.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {DCM32to50.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {ADSR.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
set hdlfile [add_files [list {GateManPoly.v}]]
set_property file_type Verilog $hdlfile
set_property library work $hdlfile
add_files "GateManPoly.ucf" -fileset [get_property constrset [current_run]]
open_rtl_design -part xc3s250evq100-4
