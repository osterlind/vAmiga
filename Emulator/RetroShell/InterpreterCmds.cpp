// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Interpreter.h"
#include "RetroShell.h"
#include "Amiga.h"

namespace vamiga {

void
Interpreter::initCommons(Command &root)
{
    //
    // Common commands
    //

    root.newGroup("Controlling the shell");

    root.add({"."},
             "Enters or exists the debugger",
             &RetroShell::exec <Token::debug>);

    root.add({"clear"},
             "Clears the console window",
             &RetroShell::exec <Token::clear>);

    root.add({"close"},
             "Hides the console window",
             &RetroShell::exec <Token::close>);

    root.add({"help"}, { }, {Arg::command},
             "Prints usage information",
             &RetroShell::exec <Token::help>);

    root.add({"joshua"},
             "",
             &RetroShell::exec <Token::easteregg>);

    root.add({"source"}, {Arg::path},
             "Processes a command script",
             &RetroShell::exec <Token::source>);

    root.add({"wait"}, {Arg::value, Arg::unit},
             "Pauses the execution of a command script",
             &RetroShell::exec <Token::wait>);

    root.hide({"joshua"});
    root.hide({"wait"});
}

void
Interpreter::initCommandShell(Command &root)
{
    initCommons(root);

    //
    // Top-level commands
    //

    root.newGroup("Regression testing");

    root.add({"regression"},
             "Runs the regression tester");

    root.newGroup("Managing components");

    root.add({"amiga"},
             "The virtual Amiga");

    root.add({"memory"},
             "Ram and Rom");

    root.add({"cpu"},
             "Motorola 68k CPU");

    root.add({"ciaa"},
             "Complex Interface Adapter A");

    root.add({"ciab"},
             "Complex Interface Adapter B");

    root.add({"agnus"},
             "Custom chip");

    root.add({"blitter"},
             "Coprocessor");

    /*
     root.add({"copper"},
     "Coprocessor");
     */
    /*
     root.add({"paula"},
     "Custom chip");
     */

    root.add({"denise"},
             "Custom chip");

    root.add({"diskcontroller"},
             "Disk Controller");

    root.add({"rtc"},
             "Real-time clock");

    root.add({"controlport1"},
             "Control port 1");

    root.add({"controlport2"},
             "Control port 2");

    root.add({"serial"},
             "Serial port");

    root.add({"zorro"},
             "Expansion boards");

    root.add({"dmadebugger"},
             "DMA Debugger");


    root.newGroup("Managing peripherals");

    root.add({"keyboard"},
             "Keyboard");

    root.add({"mouse1"},
             "Port 1 mouse");

    root.add({"mouse2"},
             "Port 2 mouse");

    root.add({"joystick1"},
             "Port 1 joystick");

    root.add({"joystick2"},
             "Port 2 joystick");

    root.add({"df0"},
             "Floppy drive 0");

    root.add({"df1"},
             "Floppy drive 1");

    root.add({"df2"},
             "Floppy drive 2");

    root.add({"df3"},
             "Floppy drive 3");

    root.add({"dfn"},
             "Floppy drive (df0, df1, df2, df3, dfn)");

    root.add({"hd0"},
             "Hard drive 0");

    root.add({"hd1"},
             "Hard drive 1");

    root.add({"hd2"},
             "Hard drive 2");

    root.add({"hd3"},
             "Hard drive 3");

    root.add({"hdn"},
             "Hard drive (hd0, hd1, hd2, hd3, hdn)");


    root.newGroup("Configuring audio and video");

    root.add({"monitor"},
             "Amiga monitor");

    root.add({"audio"},
             "Audio Unit (Paula)");


    root.newGroup("Miscellaneous");

    root.add({"server"},
             "Remote connections");


    //
    // Regression testing
    //

    root.newGroup("");

    root.add({"regression", "setup"}, { Arg::argument, Arg::argument }, { Arg::argument },
             "Initializes the test environment",
             &RetroShell::exec <Token::regression, Token::setup>);

    root.add({"regression", "run"}, { Arg::argument },
             "Launches a regression test",
             &RetroShell::exec <Token::regression, Token::run>);
    
    root.add({"screenshot"},
             "Manages regression tests");

    root.add({"screenshot", "set"},
             "Configures the regression test");

    root.add({"screenshot", "set", "filename"}, { Arg::path },
             "Assigns the screen shot filename",
             &RetroShell::exec <Token::screenshot, Token::set, Token::filename>);

    root.add({"screenshot", "set", "cutout"}, { Arg::value, Arg::value, Arg::value, Arg::value },
             "Adjusts the texture cutout",
             &RetroShell::exec <Token::screenshot, Token::set, Token::cutout>);

    root.add({"screenshot", "save"}, { Arg::path },
             "Saves a screenshot and exits the emulator",
             &RetroShell::exec <Token::screenshot, Token::save>);

    
    //
    // Amiga
    //

    root.newGroup("");

    root.add({"amiga", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::amiga, Token::config>);

    root.add({"amiga", "set"},
             "Configures the component");

    root.add({"amiga", "set", "pal"},
             "Emulates a PAL machine",
             &RetroShell::exec <Token::amiga, Token::set, Token::pal>);

    root.add({"amiga", "set", "ntsc"},
             "Emulates a NTSC machine",
             &RetroShell::exec <Token::amiga, Token::set, Token::ntsc>);

    root.add({"amiga", "vsync"},
             "Turns VSYNC on or off");

    root.add({"amiga", "vsync", "on"},
             "Turns VSYNC on",
             &RetroShell::exec <Token::amiga, Token::vsync, Token::on>);

    root.add({"amiga", "vsync", "off"},
             "Turns VSYNC off",
             &RetroShell::exec <Token::amiga, Token::vsync, Token::off>);

    root.add({"amiga", "init"}, { Arg::argument },
             "Initializes the Amiga with a predefined scheme",
             &RetroShell::exec <Token::amiga, Token::init>);

    root.add({"amiga", "power"}, { Arg::onoff },
             "Switches the Amiga on or off",
             &RetroShell::exec <Token::amiga, Token::power>);

    root.add({"amiga", "reset"},
             "Performs a hard reset",
             &RetroShell::exec <Token::amiga, Token::reset>);

    root.add({"amiga", "defaults"},
             "Displays the user defaults storage",
             &RetroShell::exec <Token::amiga, Token::defaults>);


    //
    // Memory
    //

    root.newGroup("");

    root.add({"memory", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::memory, Token::config>);

    root.add({"memory", "set"},
             "Configures the component");

    root.add({"memory", "set", "chip"}, { Arg::value },
             "Configures the amouts of chip memory",
             &RetroShell::exec <Token::memory, Token::set, Token::chip>);

    root.add({"memory", "set", "slow"},  { Arg::value },
             "Configures the amouts of slow memory",
             &RetroShell::exec <Token::memory, Token::set, Token::slow>);

    root.add({"memory", "set", "fast"}, { Arg::value },
             "Configures the amouts of flow memory",
             &RetroShell::exec <Token::memory, Token::set, Token::fast>);

    root.add({"memory", "set", "extstart"}, { Arg::value },
             "Sets the start address for Rom extensions",
             &RetroShell::exec <Token::memory, Token::set, Token::extstart>);

    root.add({"memory", "set", "saveroms"}, { Arg::boolean },
             "Determines whether Roms should be stored in snapshots",
             &RetroShell::exec <Token::memory, Token::set, Token::saveroms>);

    root.add({"memory", "set", "slowramdelay"}, { Arg::boolean },
             "Enables or disables slow Ram bus delays",
             &RetroShell::exec <Token::memory, Token::set, Token::slowramdelay>);

    root.add({"memory", "set", "bankmap"}, { Arg::argument },
             "Selects the bank mapping scheme",
             &RetroShell::exec <Token::memory, Token::set, Token::bankmap>);

    root.add({"memory", "set", "unmapped"}, { Arg::argument },
             "Determines the behaviour of unmapped memory",
             &RetroShell::exec <Token::memory, Token::set, Token::unmappingtype>);

    root.add({"memory", "set", "raminit"}, { Arg::argument },
             "Determines how Ram is initialized on startup",
             &RetroShell::exec <Token::memory, Token::set, Token::raminitpattern>);
    
    root.add({"memory", "load"},
             "Installs a Rom image");

    root.add({"memory", "load", "rom"}, { Arg::path },
             "Installs a Kickstart Rom",
             &RetroShell::exec <Token::memory, Token::load, Token::rom>);

    root.add({"memory", "load", "extrom"},
             "Installs a Rom extension",
             &RetroShell::exec <Token::memory, Token::load, Token::extrom>);

    
    //
    // CPU
    //

    root.newGroup("");

    root.add({"cpu", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::cpu, Token::config>);

    root.add({"cpu", "set"},
             "Configures the component");

    root.add({"cpu", "set", "revision"}, { CPURevisionEnum::argList() },
             "Selects the emulated chip model",
             &RetroShell::exec <Token::cpu, Token::set, Token::revision>);

    root.add({"cpu", "set", "overclocking"}, { Arg::value },
             "Overclocks the CPU by the specified factor",
             &RetroShell::exec <Token::cpu, Token::set, Token::overclocking>);

    root.add({"cpu", "set", "regreset"}, { Arg::argument },
             "Selects the reset value of data and address registers",
             &RetroShell::exec <Token::cpu, Token::set, Token::regreset>);

    root.add({"cpu", "callstack" },
             "Prints recorded subroutine calls (DEPRECATED)",
             &RetroShell::exec <Token::cpu, Token::callstack>);


    //
    // CIA
    //

    root.newGroup("");

    for (isize i = 0; i < 2; i++) {
        
        string cia = (i == 0) ? "ciaa" : "ciab";
        
        root.add({cia, ""},
                 "Displays the current configuration",
                 &RetroShell::exec <Token::cia, Token::config>, i);
        
        root.add({cia, "set"},
                 "Configures the component");
        
        root.add({cia, "set", "revision"}, { Arg::revision },
                 "Selects the emulated chip model",
                 &RetroShell::exec <Token::cia, Token::set, Token::revision>, i);
        
        root.add({cia, "set", "todbug"}, { Arg::boolean },
                 "Enables or disables the TOD hardware bug",
                 &RetroShell::exec <Token::cia, Token::set, Token::todbug>, i);
        
        root.add({cia, "set", "esync"}, { Arg::boolean },
                 "Turns E-clock syncing on or off",
                 &RetroShell::exec <Token::cia, Token::set, Token::esync>, i);
    }
    
    
    //
    // Agnus
    //

    root.newGroup("");

    root.add({"agnus", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::agnus, Token::config>);
    
    root.add({"agnus", "set"},
             "Configures the component");

    root.add({"agnus", "set", "revision"}, { Arg::revision },
             "Selects the emulated chip model",
             &RetroShell::exec <Token::agnus, Token::set, Token::revision>);

    root.add({"agnus", "set", "slowrammirror"}, { Arg::boolean },
             "Enables or disables ECS Slow Ram mirroring",
             &RetroShell::exec <Token::agnus, Token::set, Token::slowrammirror>);

    root.add({"agnus", "set", "ptrdrops"}, { Arg::boolean },
             "Emulate dropped register writes",
             &RetroShell::exec <Token::agnus, Token::set, Token::ptrdrops>);

    
    //
    // Blitter
    //

    root.newGroup("");

    root.add({"blitter", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::blitter, Token::config>);
    
    root.add({"blitter", "set"},
             "Configures the component");

    root.add({"blitter", "set", "accuracy"}, { Arg::value },
             "Selects the emulation accuracy level",
             &RetroShell::exec <Token::blitter, Token::set, Token::accuracy>);


    //
    // Copper
    //
    
    
    //
    // Denise
    //

    root.newGroup("");

    root.add({"denise", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::denise, Token::config>);

    root.add({"denise", "set"},
             "Configures the component");

    root.add({"denise", "set", "revision"}, { Arg::revision },
             "Selects the emulated chip model",
             &RetroShell::exec <Token::denise, Token::set, Token::revision>);

    root.add({"denise", "set", "tracking"}, { Arg::boolean },
             "Enables or disables viewport tracking",
             &RetroShell::exec <Token::denise, Token::set, Token::tracking>);

    root.add({"denise", "set", "clxsprspr"}, { Arg::boolean },
             "Enables or disables sprite-sprite collision detection",
             &RetroShell::exec <Token::denise, Token::set, Token::clxsprspr>);

    root.add({"denise", "set", "clxsprplf"}, { Arg::boolean },
             "Enables or disables sprite-playfield collision detection",
             &RetroShell::exec <Token::denise, Token::set, Token::clxsprplf>);

    root.add({"denise", "set", "clxplfplf"}, { Arg::boolean },
             "Enables or disables playfield-playfield collision detection",
             &RetroShell::exec <Token::denise, Token::set, Token::clxplfplf>);
    
    root.add({"denise", "hide"},
             "Hides bitplanes, sprites, or layers");

    root.add({"denise", "hide", "bitplanes"}, { Arg::value },
             "Wipes out certain bitplane data",
             &RetroShell::exec <Token::denise, Token::hide, Token::bitplanes>);

    root.add({"denise", "hide", "sprites"}, { Arg::value },
             "Wipes out certain sprite data",
             &RetroShell::exec <Token::denise, Token::hide, Token::sprites>);

    root.add({"denise", "hide", "sprites"}, { Arg::value },
             "Makes certain drawing layers transparent",
             &RetroShell::exec <Token::denise, Token::hide, Token::layers>);

    root.add({"denise", "inspect"},
             "Displays the internal state");

    root.add({"denise", "inspect", "state"},
             "Displays the current state",
             &RetroShell::exec <Token::denise, Token::inspect, Token::state>);

    root.add({"denise", "inspect", "registers"},
             "Displays the current register value",
             &RetroShell::exec <Token::denise, Token::inspect, Token::registers>);

    
    //
    // DMA Debugger
    //

    root.add({"dmadebugger", "open"},
             "Opens the DMA debugger",
             &RetroShell::exec <Token::dmadebugger, Token::open>);

    root.add({"dmadebugger", "close"},
             "Closes the DMA debugger",
             &RetroShell::exec <Token::dmadebugger, Token::close>);

    root.add({"dmadebugger", "show"},
             "Enables the debugger for a certain DMA channel");

    root.add({"dmadebugger", "show", "copper"},
             "Visualizes Copper DMA",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::copper>);

    root.add({"dmadebugger", "show", "blitter"},
             "Visualizes Blitter DMA",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::blitter>);

    root.add({"dmadebugger", "show", "disk"},
             "Visualizes Disk DMA",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::disk>);

    root.add({"dmadebugger", "show", "audio"},
             "Visualizes Audio DMA",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::audio>);

    root.add({"dmadebugger", "show", "sprites"},
             "Visualizes Sprite DMA",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::sprites>);

    root.add({"dmadebugger", "show", "bitplanes"},
             "Visualizes Bitplane DMA",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::bitplanes>);

    root.add({"dmadebugger", "show", "cpu"},
             "Visualizes CPU accesses",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::cpu>);

    root.add({"dmadebugger", "show", "refresh"},
             "Visualizes memory refresh cycles",
             &RetroShell::exec <Token::dmadebugger, Token::show, Token::refresh>);

    root.add({"dmadebugger", "hide"},
             "Disables the debugger for a certain DMA channel");

    root.add({"dmadebugger", "hide", "copper"},
             "Hides Copper DMA",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::copper>);

    root.add({"dmadebugger", "hide", "blitter"},
             "Hides Blitter DMA",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::blitter>);

    root.add({"dmadebugger", "hide", "disk"},
             "Hides Disk DMA",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::disk>);

    root.add({"dmadebugger", "hide", "audio"},
             "Hides Audio DMA",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::audio>);

    root.add({"dmadebugger", "hide", "sprites"},
             "Hides Sprite DMA",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::sprites>);

    root.add({"dmadebugger", "hide", "bitplanes"},
             "Hides Bitplane DMA",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::bitplanes>);

    root.add({"dmadebugger", "hide", "cpu"},
             "Hides CPU accesses",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::cpu>);

    root.add({"dmadebugger", "hide", "refresh"},
             "Hides memory refresh cycles",
             &RetroShell::exec <Token::dmadebugger, Token::hide, Token::refresh>);

    
    //
    // Monitor
    //

    root.add({"monitor", "set"},
             "Configures the component");

    root.add({"monitor", "set", "palette"}, { Arg::argument },
             "Selects the color palette",
             &RetroShell::exec <Token::monitor, Token::set, Token::palette>);

    root.add({"monitor", "set", "brightness"}, { Arg::value },
             "Adjusts the brightness of the Amiga texture",
             &RetroShell::exec <Token::monitor, Token::set, Token::brightness>);

    root.add({"monitor", "set", "contrast"}, { Arg::value },
             "Adjusts the contrast of the Amiga texture",
             &RetroShell::exec <Token::monitor, Token::set, Token::contrast>);

    root.add({"monitor", "set", "saturation"}, { Arg::value },
             "Adjusts the saturation of the Amiga texture",
             &RetroShell::exec <Token::monitor, Token::set, Token::saturation>);

    
    //
    // Audio
    //

    root.newGroup("");

    root.add({"audio", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::audio, Token::config>);

    root.add({"audio", "set"},
             "Configures the component");

    root.add({"audio", "set", "sampling"}, { Arg::argument },
             "Selects the sampling method",
             &RetroShell::exec <Token::audio, Token::set, Token::sampling>);

    root.add({"audio", "set", "filter"}, { Arg::argument },
             "Configures the audio filter",
             &RetroShell::exec <Token::audio, Token::set, Token::filter>);
    
    root.add({"audio", "set", "volume"},
             "Sets the volume");

    root.add({"audio", "set", "volume", "channel0"}, { Arg::volume },
             "Sets the volume for audio channel 0",
             &RetroShell::exec <Token::audio, Token::set, Token::volume>, 0);
    
    root.add({"audio", "set", "volume", "channel1"}, { Arg::volume },
             "Sets the volume for audio channel 1",
             &RetroShell::exec <Token::audio, Token::set, Token::volume>, 1);
    
    root.add({"audio", "set", "volume", "channel2"}, { Arg::volume },
             "Sets the volume for audio channel 2",
             &RetroShell::exec <Token::audio, Token::set, Token::volume>, 2);
    
    root.add({"audio", "set", "volume", "channel3"}, { Arg::volume },
             "Sets the volume for audio channel 3",
             &RetroShell::exec <Token::audio, Token::set, Token::volume>, 3);
    
    root.add({"audio", "set", "volume", "left"}, { Arg::volume },
             "Sets the master volume for the left speaker",
             &RetroShell::exec <Token::audio, Token::set, Token::volume>, 4);
    
    root.add({"audio", "set", "volume", "right"}, { Arg::volume },
             "Sets the master volume for the right speaker",
             &RetroShell::exec <Token::audio, Token::set, Token::volume>, 5);

    root.add({"audio", "set", "pan"},
             "Sets the pan for one of the four audio channels");
    
    root.add({"audio", "set", "pan", "channel0"}, { Arg::value },
             "Sets the pan for audio channel 0",
             &RetroShell::exec <Token::audio, Token::set, Token::pan>, 0);
    
    root.add({"audio", "set", "pan", "channel1"}, { Arg::value },
             "Sets the pan for audio channel 1",
             &RetroShell::exec <Token::audio, Token::set, Token::pan>, 1);
    
    root.add({"audio", "set", "pan", "channel2"}, { Arg::value },
             "Sets the pan for audio channel 2",
             &RetroShell::exec <Token::audio, Token::set, Token::pan>, 2);
    
    root.add({"audio", "set", "pan", "channel3"}, { Arg::value },
             "Sets the pan for audio channel 3",
             &RetroShell::exec <Token::audio, Token::set, Token::pan>, 3);

    root.add({"audio", "inspect"},
             "Displays the internal state");

    root.add({"audio", "inspect", "state"},
             "Displays the current state",
             &RetroShell::exec <Token::audio, Token::inspect, Token::state>);

    root.add({"audio", "inspect", "registers"},
             "Displays the current register value",
             &RetroShell::exec <Token::audio, Token::inspect, Token::registers>);
    
    
    //
    // Paula
    //


    //
    // RTC
    //

    root.newGroup("");

    root.add({"rtc", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::rtc, Token::config>);

    root.add({"rtc", "set"},
             "Configures the component");

    root.add({"rtc", "set", "revision"}, { Arg::revision },
             "Selects the emulated chip model",
             &RetroShell::exec <Token::rtc, Token::set, Token::revision>);

    root.add({"rtc", "inspect"},
             "Displays the internal state");

    root.add({"rtc", "inspect", "registers"},
             "Displays the current register value",
             &RetroShell::exec <Token::rtc, Token::inspect, Token::registers>);

    
    //
    // Control port
    //

    root.newGroup("");

    for (isize i = 0; i < 2; i++) {

        string port = (i == 0) ? "controlport1" : "controlport2";
        
        root.add({port, ""},
                 "Displays the current configuration",
                 &RetroShell::exec <Token::controlport, Token::config>, i);
        
        root.add({port, "inspect"},
                 "Displays the internal state",
                 &RetroShell::exec <Token::controlport, Token::inspect>, i);
    }


    //
    // Keyboard
    //

    root.newGroup("");

    root.add({"keyboard", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::keyboard, Token::config>);
    
    root.add({"keyboard", "set"},
             "Configures the component");

    root.add({"keyboard", "set", "accuracy"}, { Arg::value },
             "Determines the emulation accuracy level",
             &RetroShell::exec <Token::keyboard, Token::set, Token::accuracy>);

    root.add({"keyboard", "inspect"},
             "Displays the internal state",
             &RetroShell::exec <Token::keyboard, Token::inspect>);

    root.add({"keyboard", "press"}, { Arg::value },
             "Sends a keycode to the keyboard",
             &RetroShell::exec <Token::keyboard, Token::press>);

    
    //
    // Mouse
    //

    root.newGroup("");

    for (isize i = 0; i < 2; i++) {

        string mouse = (i == 0) ? "mouse1" : "mouse2";
        
        root.add({mouse, ""},
                 "Displays the current configuration",
                 &RetroShell::exec <Token::mouse, Token::config>, i);
        
        root.add({mouse, "set"},
                 "Configures the component");
        
        root.add({mouse, "set", "pullup"}, { Arg::boolean },
                 "Enables or disables the emulation of pull-up resistors",
                 &RetroShell::exec <Token::mouse, Token::set, Token::pullup>, i);
        
        root.add({mouse, "set", "shakedetector"}, { Arg::boolean },
                 "Enables or disables the shake detector",
                 &RetroShell::exec <Token::mouse, Token::set, Token::shakedetector>, i);
        
        root.add({mouse, "set", "velocity"}, { Arg::value },
                 "Sets the horizontal and vertical mouse velocity",
                 &RetroShell::exec <Token::mouse, Token::set, Token::velocity>, i);
        
        root.add({mouse, "inspect"},
                 "Displays the internal state",
                 &RetroShell::exec <Token::mouse, Token::inspect>, i);

        root.add({mouse, "press"},
                 "Presses a mouse button");

        root.add({mouse, "press", "left"},
                 "Presses the left mouse button",
                 &RetroShell::exec <Token::mouse, Token::press, Token::left>, i);
        
        root.add({mouse, "press", "right"},
                 "Presses the right mouse button",
                 &RetroShell::exec <Token::mouse, Token::press, Token::right>, i);
    }

    
    //
    // Joystick
    //

    root.newGroup("");

    for (isize i = 0; i < 2; i++) {

        string joystick = (i == 0) ? "joystick1" : "joystick2";
        
        root.add({joystick, ""},
                 "Displays the current configuration",
                 &RetroShell::exec <Token::joystick, Token::config>, i);
        
        root.add({joystick, "set"},
                 "Configures the component");
        
        root.add({joystick, "set", "autofire"}, { Arg::boolean },
                 "Enables or disables auto-fire mode",
                 &RetroShell::exec <Token::joystick, Token::set, Token::autofire>, i);
        
        root.add({joystick, "set", "bullets"},  { Arg::value },
                 "Sets the number of bullets per auto-fire shot",
                 &RetroShell::exec <Token::joystick, Token::set, Token::bullets>, i);
        
        root.add({joystick, "set", "velocity"}, { Arg::value },
                 "Configures the auto-fire delay",
                 &RetroShell::exec <Token::joystick, Token::set, Token::delay>, i);
        
        root.add({joystick, "inspect"},
                 "Displays the internal state",
                 &RetroShell::exec <Token::joystick, Token::inspect>, i);

        root.add({joystick, "press"}, { Arg::value },
                 "Presses a joystick button",
                 &RetroShell::exec <Token::joystick, Token::press>, i);

        root.add({joystick, "unpress"}, { Arg::value },
                 "Releases a joystick button",
                 &RetroShell::exec <Token::joystick, Token::unpress>, i);

        root.add({joystick, "pull"},
                 "Pulls the joystick");

        root.add({joystick, "pull", "left"},
                 "Pulls the joystick left",
                 &RetroShell::exec <Token::joystick, Token::pull, Token::left>, i);
        
        root.add({joystick, "pull", "right"},
                 "Pulls the joystick right",
                 &RetroShell::exec <Token::joystick, Token::pull, Token::right>, i);

        root.add({joystick, "pull", "up"},
                 "Pulls the joystick up",
                 &RetroShell::exec <Token::joystick, Token::pull, Token::up>, i);

        root.add({joystick, "pull", "down"},
                 "Pulls the joystick down",
                 &RetroShell::exec <Token::joystick, Token::pull, Token::down>, i);

        root.add({joystick, "release"},
                 "Release a joystick axis");

        root.add({joystick, "release", "x"},
                 "Releases the x-axis",
                 &RetroShell::exec <Token::joystick, Token::release, Token::xaxis>, i);

        root.add({joystick, "release", "y"},
                 "Releases the y-axis",
                 &RetroShell::exec <Token::joystick, Token::release, Token::yaxis>, i);
    }
    
    
    //
    // Serial port
    //

    root.newGroup("");

    root.add({"serial", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::serial, Token::config>);

    root.add({"serial", "set"},
             "Configures the component");

    root.add({"serial", "set", "device"}, { Arg::argument },
             "Connects a device",
             &RetroShell::exec <Token::serial, Token::set, Token::device>);

    root.add({"serial", "inspect"},
             "Displays the internal state",
             &RetroShell::exec <Token::serial, Token::inspect>);

    
    //
    // Disk controller
    //

    root.newGroup("");

    root.add({"diskcontroller", ""},
             "Displays the current configuration",
             &RetroShell::exec <Token::dc, Token::config>);

    root.add({"diskcontroller", "set"},
             "Configures the component");

    root.add({"diskcontroller", "set", "speed"}, { Arg::argument },
             "Configures the drive speed",
             &RetroShell::exec <Token::dc, Token::speed>);

    root.add({"diskcontroller", "dsksync"},
             "Secures the DSKSYNC register");

    root.add({"diskcontroller", "dsksync", "auto"}, { Arg::boolean },
             "Always receive a SYNC signal",
             &RetroShell::exec <Token::dc, Token::dsksync, Token::autosync>);

    root.add({"diskcontroller", "dsksync", "lock"}, { Arg::boolean },
             "Prevents writes to DSKSYNC",
             &RetroShell::exec <Token::dc, Token::dsksync, Token::lock>);

    root.add({"diskcontroller", "inspect"},
             "Displays the internal state",
             &RetroShell::exec <Token::dc, Token::inspect>);


    //
    // Df0, Df1, Df2, Df3
    //

    root.newGroup("");

    for (isize i = 0; i < 4; i++) {

        string df = "df" + std::to_string(i);

        root.add({df, ""},
                 "Displays the current configuration",
                 &RetroShell::exec <Token::dfn, Token::config>, i);
        
        root.add({df, "connect"},
                 "Connects the drive",
                 &RetroShell::exec <Token::dfn, Token::connect>, i);
        root.seek("df0")->remove("connect");
        
        root.add({df, "disconnect"},
                 "Disconnects the drive",
                 &RetroShell::exec <Token::dfn, Token::disconnect>, i);
        root.seek("df0")->remove("disconnect");
        
        root.add({df, "eject"},
                 "Ejects a floppy disk",
                 &RetroShell::exec <Token::dfn, Token::eject>, i);
        
        root.add({df, "insert"}, { Arg::path },
                 "Inserts a floppy disk",
                 &RetroShell::exec <Token::dfn, Token::insert>, i);
    }
    
    for (isize i = 0; i < 5; i++) {

        string df = i < 4 ? "df" + std::to_string(i) : "dfn";

        root.add({df, "set"},
                 "Configures the component");
        
        root.add({df, "set", "model"}, { Arg::model },
                 "Selects the drive model",
                 &RetroShell::exec <Token::dfn, Token::set, Token::model>, i);
        
        root.add({df, "set", "mechanics"}, { Arg::boolean },
                 "Enables or disables the emulation of mechanical delays",
                 &RetroShell::exec <Token::dfn, Token::set, Token::mechanics>, i);
        
        root.add({df, "set", "searchpath"}, { Arg::path },
                 "Sets the search path for media files",
                 &RetroShell::exec <Token::dfn, Token::set, Token::searchpath>, i);

        root.add({df, "set", "swapdelay"}, { Arg::value },
                 "Sets the disk change delay",
                 &RetroShell::exec <Token::dfn, Token::set, Token::swapdelay>, i);

        root.add({df, "set", "pan"}, { Arg::value },
                 "Sets the pan for drive sounds",
                 &RetroShell::exec <Token::dfn, Token::set, Token::pan>, i);
        
        root.add({df, "audiate"},
                 "Sets the volume of drive sounds");
        
        root.add({df, "audiate", "insert"}, { Arg::volume },
                 "Makes disk insertions audible",
                 &RetroShell::exec <Token::dfn, Token::audiate, Token::insert>, i);
        
        root.add({df, "audiate", "eject"}, { Arg::volume },
                 "Makes disk ejections audible",
                 &RetroShell::exec <Token::dfn, Token::audiate, Token::eject>, i);
        
        root.add({df, "audiate", "step"},  { Arg::volume },
                 "Makes head steps audible",
                 &RetroShell::exec <Token::dfn, Token::audiate, Token::step>, i);
        
        root.add({df, "audiate", "poll"},  { Arg::volume },
                 "Makes polling clicks audible",
                 &RetroShell::exec <Token::dfn, Token::audiate, Token::poll>, i);
    }
    
    for (isize i = 0; i < 4; i++) {

        string df = "df" + std::to_string(i);
        
        root.add({df, "inspect"},
                 "Displays the component state",
                 &RetroShell::exec <Token::dfn, Token::inspect>);

        root.add({df, "catch"}, { Arg::argument },
                 "Creates a catchpoint for the specfied file",
                 &RetroShell::exec <Token::dfn, Token::cp>);
    }

    //
    // Hd0, Hd1, Hd2, Hd3
    //

    root.newGroup("");

    for (isize i = 0; i < 4; i++) {
        
        string hd = "hd" + std::to_string(i);

        root.add({hd, ""},
                 "Displays the current configuration",
                 &RetroShell::exec <Token::hdn, Token::config>, i);
    }
    
    for (isize i = 0; i < 5; i++) {

        string hd = i < 4 ? "hd" + std::to_string(i) : "hdn";

        root.add({hd, "set"},
                 "Configures the component");
        
        root.add({hd, "set", "pan"}, { Arg::value },
                 "Sets the pan for drive sounds",
                 &RetroShell::exec <Token::hdn, Token::set, Token::pan>, i);
        
        root.add({hd, "audiate"},
                 "Sets the volume of drive sounds");

        root.add({hd, "audiate", "step"}, { Arg::volume },
                 "Makes head steps audible",
                 &RetroShell::exec <Token::hdn, Token::audiate, Token::step>, i);
    }
    
    for (isize i = 0; i < 4; i++) {

        string hd = "hd" + std::to_string(i);
        
        root.add({hd, "inspect"},
                 "Displays the component state");

        root.add({hd, "inspect", "drive"},
                 "Displays hard drive parameters",
                 &RetroShell::exec <Token::hdn, Token::inspect, Token::drive>, i);

        root.add({hd, "inspect", "volumes"},
                 "Displays summarized volume information",
                 &RetroShell::exec <Token::hdn, Token::inspect, Token::volumes>, i);

        root.add({hd, "inspect", "partitions"},
                 "Displays information about all partitions",
                 &RetroShell::exec <Token::hdn, Token::inspect, Token::partition>, i);

        root.add({hd, "inspect", "state"},
                 "Displays the internal state",
                 &RetroShell::exec <Token::hdn, Token::inspect, Token::state>, i);

        root.add({hd, "geometry"},  { Arg::value, Arg::value, Arg::value },
                 "Changes the disk geometry",
                 &RetroShell::exec <Token::hdn, Token::geometry>, i);
    }
    
    //
    // Zorro boards
    //

    root.add({"zorro", "list"},
             "Lists all connected boards",
             &RetroShell::exec <Token::zorro, Token::list>);

    root.add({"zorro", "inspect"}, { Arg::value },
             "Inspects a specific Zorro board",
             &RetroShell::exec <Token::zorro, Token::inspect>);

    
    //
    // Remote server
    //

    root.newGroup("");

    root.add({"server", ""},
             "Displays a server status summary",
             &RetroShell::exec <Token::server>);

    root.add({"server", "serial"},
             "Serial port server");

    root.add({"server", "serial", "config"},
             "Displays the current configuration",
             &RetroShell::exec <Token::server, Token::serial, Token::config>);

    root.add({"server", "serial", "set"},
             "Configures the component");

    root.add({"server", "serial", "set", "port"}, { Arg::value },
             "Assigns the port number",
             &RetroShell::exec <Token::server, Token::serial, Token::set, Token::port>);

    root.add({"server", "serial", "set", "verbose"}, { Arg::boolean },
             "Switches verbose mode on or off",
             &RetroShell::exec <Token::server, Token::serial, Token::set, Token::verbose>);

    root.add({"server", "serial", "inspect"},
             "Displays the internal state",
             &RetroShell::exec <Token::server, Token::serial, Token::inspect>);

    root.add({"server", "rshell"},
             "Retro shell server");

    root.add({"server", "rshell", "start"},
             "Starts the retro shell server",
             &RetroShell::exec <Token::server, Token::rshell, Token::start>);

    root.add({"server", "rshell", "stop"},
             "Stops the retro shell server",
             &RetroShell::exec <Token::server, Token::rshell, Token::stop>);

    root.add({"server", "rshell", "disconnect"},
             "Disconnects a client",
             &RetroShell::exec <Token::server, Token::rshell, Token::disconnect>);

    root.add({"server", "rshell", "config"},
             "Displays the current configuration",
             &RetroShell::exec <Token::server, Token::rshell, Token::config>);

    root.add({"server", "rshell", "set"},
             "Configures the component");

    root.add({"server", "rshell", "set", "port"}, { Arg::value },
             "Assigns the port number",
             &RetroShell::exec <Token::server, Token::rshell, Token::set, Token::port>);

    root.add({"server", "serial", "set", "verbose"}, { Arg::boolean },
             "Switches verbose mode on or off",
             &RetroShell::exec <Token::server, Token::rshell, Token::set, Token::verbose>);

    root.add({"server", "rshell", "inspect"},
             "Displays the internal state",
             &RetroShell::exec <Token::server, Token::rshell, Token::inspect>);

    root.add({"server", "gdb"},
             "GDB server");

    root.add({"server", "gdb", "attach"}, { Arg::argument },
             "Attaches the GDB server to a process",
             &RetroShell::exec <Token::server, Token::gdb, Token::attach>);

    root.add({"server", "gdb", "detach"},
             "Detaches the GDB server from a process",
             &RetroShell::exec <Token::server, Token::gdb, Token::detach>);

    root.add({"server", "gdb", "config"},
             "Displays the current configuration",
             &RetroShell::exec <Token::server, Token::gdb, Token::config>);

    root.add({"server", "gdb", "set"},
             "Configures the component");

    root.add({"server", "gdb", "set", "port"}, { Arg::value },
             "Assigns the port number",
             &RetroShell::exec <Token::server, Token::gdb, Token::set, Token::port>);

    root.add({"server", "gdb", "set", "verbose"}, { Arg::boolean },
             "Switches verbose mode on or off",
             &RetroShell::exec <Token::server, Token::gdb, Token::set, Token::verbose>);

    root.add({"server", "gdb", "inspect"},
             "Displays the internal state",
             &RetroShell::exec <Token::server, Token::gdb, Token::inspect>);

    // Hide some commands
    root.hide({"regression"});
    root.hide({"screenshot"});
    root.hide({"df0"});
    root.hide({"df1"});
    root.hide({"df2"});
    root.hide({"df3"});
    root.hide({"hd0"});
    root.hide({"hd1"});
    root.hide({"hd2"});
    root.hide({"hd3"});
}

}
