// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Interpreter.h"
#include "Amiga.h"

namespace vamiga {

void
Interpreter::initDebugShell(Command &root)
{
    initCommons(root);

    //
    // Monitor
    //

    root.setGroup("Monitor commands");

    root.add({"goto"}, { }, { Arg::value },
             "Goto address",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        if (argv.empty()) {
            amiga.run();
        } else {
            debugger.jump(u32(parseNum(argv)));
        }
    });

    root.add({"step"},
             "Step into the next instruction",
             [this](Arguments& argv, long value) {

        debugger.stepInto();
    });

    root.add({"next"},
             "Step over the next instruction",
             [this](Arguments& argv, long value) {

        debugger.stepOver();
    });

    //
    // Inspect
    //
    root.add({"inspect"},
             "Inspect component");

    root.add({"inspect", "amiga"},         "Main computer");
    root.add({"inspect", "memory"},        "RAM and ROM");
    root.add({"inspect", "cpu"},           "Motorola 68k CPU");
    root.add({"inspect", "ciaa"},          "Complex Interface Adapter A");
    root.add({"inspect", "ciab"},          "Complex Interface Adapter B");
    root.add({"inspect", "agnus"},         "Custom Chipset");
    root.add({"inspect", "blitter"},       "Coprocessor");
    root.add({"inspect", "copper"},        "Coprocessor");
    root.add({"inspect", "paula"},         "Custom Chipset");
    root.add({"inspect", "denise"},        "Custom Chipset");
    root.add({"inspect", "rtc"},           "Real-time clock");
    root.add({"inspect", "zorro"},         "Expansion boards");
    root.add({"inspect", "controlport"},   "Control ports");
    root.add({"inspect", "serial"},        "Serial port");

    //
    //
    //

    root.add({"disassemble"}, { }, { Arg::address },
             "Disassemble instructions",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        auto addr = argv.empty() ? cpu.getPC0() : u32(parseNum(argv));
        cpu.disassembleRange(ss, addr, 16);

        retroShell << '\n' << ss << '\n';
    });

    root.add({"dump"}, { }, { Arg::address },
             "Dump memory",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        argv.empty() ?
        debugger.memDump<ACCESSOR_CPU>(ss, 16, 1) :
        debugger.memDump<ACCESSOR_CPU>(ss, u32(parseNum(argv)), 16, 1);

        retroShell << '\n' << ss << '\n';
    });

    root.add({"dump.b"}, { }, { Arg::address }, "",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        argv.empty() ?
        debugger.memDump<ACCESSOR_CPU>(ss, 16, 1) :
        debugger.memDump<ACCESSOR_CPU>(ss, u32(parseNum(argv)), 16, 1);

        retroShell << '\n' << ss << '\n';
    });

    root.add({"dump.w"}, { }, { Arg::address }, "",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        argv.empty() ?
        debugger.memDump<ACCESSOR_CPU>(ss, 16, 2) :
        debugger.memDump<ACCESSOR_CPU>(ss, u32(parseNum(argv)), 16, 2);

        retroShell << '\n' << ss << '\n';
    });

    root.add({"dump.l"}, { }, { Arg::address }, "",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        argv.empty() ?
        debugger.memDump<ACCESSOR_CPU>(ss, 16, 4) :
        debugger.memDump<ACCESSOR_CPU>(ss, u32(parseNum(argv)), 16, 4);

        retroShell << '\n' << ss << '\n';
    });

    root.add({"register"}, { ChipsetRegEnum::argList() }, { Arg::value },
             "Reads or modifies a custom chipset register",
             [this](Arguments& argv, long value) {

        auto reg = parseEnum<ChipsetRegEnum>(argv);

        if (argv.size() == 1) {

            std::stringstream ss;
            ss << ChipsetRegEnum::key(reg) << " = " << util::hex(debugger.readCs(reg));
            retroShell << ss;

        } else {

            debugger.writeCs(reg, u16(parseNum(argv, 1)));
        }
    });

    root.setGroup("Miscellaneous");

    root.add({"?"}, { Arg::value },
             "Convert a value into different formats",
             [this](Arguments& argv, long value) {

        std::stringstream ss;

        if (isNum(argv)) {
            debugger.convertNumeric(ss, parseNum(argv));
        } else {
            debugger.convertNumeric(ss, argv.front());
        }

        retroShell << '\n' << ss << '\n';
    });





    //
    // Top-level commands
    //



    root.setGroup("Guarding the program execution");

    root.add({"break"},     "Manages CPU breakpoints");
    root.add({"watch"},     "Manages CPU watchpoints");
    root.add({"catch"},     "Manages CPU catchpoints");
    root.add({"cbreak"},    "Manages Copper breakpoints");
    root.add({"cwatch"},    "Manages Copper watchpoints");

    root.setGroup("Debugging components");

    root.add({"amiga"},         "Main computer");
    root.add({"memory"},        "RAM and ROM");
    root.add({"cpu"},           "Motorola 68k CPU");
    root.add({"ciaa"},          "Complex Interface Adapter A");
    root.add({"ciab"},          "Complex Interface Adapter B");
    root.add({"agnus"},         "Custom Chipset");
    root.add({"blitter"},       "Coprocessor");
    root.add({"copper"},        "Coprocessor");
    root.add({"paula"},         "Custom Chipset");
    root.add({"denise"},        "Custom Chipset");
    root.add({"rtc"},           "Real-time clock");
    root.add({"zorro"},         "Expansion boards");
    root.add({"controlport"},   "Control ports");
    root.add({"serial"},        "Serial port");

    root.setGroup("Debugging peripherals");

    root.add({"keyboard"},      "Keyboard");
    root.add({"mouse"},         "Mouse");
    root.add({"joystick"},      "Joystick");
    root.add({"df0"},           "Floppy drive 0");
    root.add({"df1"},           "Floppy drive 1");
    root.add({"df2"},           "Floppy drive 2");
    root.add({"df3"},           "Floppy drive 3");
    root.add({"hd0"},           "Hard drive 0");
    root.add({"hd1"},           "Hard drive 1");
    root.add({"hd2"},           "Hard drive 2");
    root.add({"hd3"},           "Hard drive 3");

    root.setGroup("Miscellaneous");

    root.add({"host"},          "Host computer");
    root.add({"os"},            "AmigaOS debugger");
    root.add({"server"},        "Remote connections");

    
    //
    // Breakpoints
    //

    root.setGroup("");

    root.add({"break", ""},
             "Lists all breakpoints",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.cpu, Category::Breakpoints);
    });

    root.add({"break", "at"}, { Arg::address },
             "Sets a breakpoint at the specified address",
             [this](Arguments& argv, long value) {

        cpu.setBreakpoint(u32(parseNum(argv)));
    });

    root.add({"break", "delete"}, { Arg::address },
             "Deletes a breakpoint",
             [this](Arguments& argv, long value) {

        cpu.deleteBreakpoint(parseNum(argv));
    });

    root.add({"break", "enable"}, { Arg::address },
             "Enables a breakpoint",
             [this](Arguments& argv, long value) {

        cpu.enableBreakpoint(parseNum(argv));
    });

    root.add({"break", "disable"}, { Arg::address },
             "Disables a breakpoint",
             [this](Arguments& argv, long value) {

        cpu.disableBreakpoint(parseNum(argv));
    });

    root.add({"break", "ignore"}, { Arg::address, Arg::value },
             "Ignores a breakpoint a certain number of times",
             [this](Arguments& argv, long value) {

        cpu.ignoreBreakpoint(util::parseNum(argv[0]), util::parseNum(argv[1]));
    });

    
    //
    // Watchpoints
    //

    root.setGroup("");

    root.add({"watch", ""},
             "Lists all watchpoints",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.cpu, Category::Watchpoints);
    });

    root.add({"watch", "at"}, { Arg::address },
             "Sets a watchpoint at the specified address",
             [this](Arguments& argv, long value) {

        cpu.setWatchpoint(u32(parseNum(argv)));
    });

    root.add({"watch", "delete"}, { Arg::address },
             "Deletes a watchpoint",
             [this](Arguments& argv, long value) {

        cpu.deleteWatchpoint(parseNum(argv));
    });

    root.add({"watch", "enable"}, { Arg::address },
             "Enables a watchpoint",
             [this](Arguments& argv, long value) {

        cpu.enableWatchpoint(parseNum(argv));
    });

    root.add({"watch", "disable"}, { Arg::address },
             "Disables a watchpoint",
             [this](Arguments& argv, long value) {

        cpu.disableWatchpoint(parseNum(argv));
    });

    root.add({"watch", "ignore"}, { Arg::address, Arg::value },
             "Ignores a watchpoint a certain number of times",
             [this](Arguments& argv, long value) {

        cpu.ignoreWatchpoint(util::parseNum(argv[0]), util::parseNum(argv[1]));
    });


    //
    // Catchpoints
    //

    root.setGroup("");

    root.add({"catch", ""},
             "Lists all catchpoints",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.cpu, Category::Catchpoints);
    });

    root.add({"catch", "vector"}, { Arg::value },
             "Catches an exception vector",
             [this](Arguments& argv, long value) {

        auto nr = parseNum(argv);
        if (nr < 0 || nr > 255) throw VAError(ERROR_OPT_INVARG, "0...255");
        cpu.setCatchpoint(u8(nr));
    });

    root.add({"catch", "interrupt"}, { Arg::value },
             "Catches an interrupt",
             [this](Arguments& argv, long value) {

        auto nr = parseNum(argv);
        if (nr < 1 || nr > 7) throw VAError(ERROR_OPT_INVARG, "1...7");
        cpu.setCatchpoint(u8(nr + 24));
    });

    root.add({"catch", "trap"}, { Arg::value },
             "Catches a trap instruction",
             [this](Arguments& argv, long value) {

        auto nr = parseNum(argv);
        if (nr < 0 || nr > 15) throw VAError(ERROR_OPT_INVARG, "0...15");
        cpu.setCatchpoint(u8(nr + 32));
    });

    root.add({"catch", "delete"}, { Arg::value },
             "Deletes a catchpoint",
             [this](Arguments& argv, long value) {

        cpu.deleteCatchpoint(parseNum(argv));
    });

    root.add({"catch", "enable"}, { Arg::value },
             "Enables a catchpoint",
             [this](Arguments& argv, long value) {

        cpu.enableCatchpoint(parseNum(argv));
    });

    root.add({"catch", "disable"}, { Arg::value },
             "Disables a catchpoint",
             [this](Arguments& argv, long value) {

        cpu.disableCatchpoint(parseNum(argv));
    });

    root.add({"catch", "ignore"}, { Arg::value, Arg::value },
             "Ignores a catchpoint a certain number of times",
             [this](Arguments& argv, long value) {

        cpu.ignoreCatchpoint(util::parseNum(argv[0]), util::parseNum(argv[1]));
    });


    //
    // Copper breakpoints
    //

    root.add({"cbreak", ""},
             "Lists all breakpoints",
             [this](Arguments& argv, long value) {

        retroShell.dump(copper.debugger, Category::Breakpoints);
    });

    root.add({"cbreak", "at"}, { Arg::value },
             "Sets a breakpoint at the specified address",
             [this](Arguments& argv, long value) {

        copper.debugger.setBreakpoint(u32(parseNum(argv)));
    });

    root.add({"cbreak", "delete"}, { Arg::value },
             "Deletes a breakpoint",
             [this](Arguments& argv, long value) {

        copper.debugger.deleteBreakpoint(parseNum(argv));
    });

    root.add({"cbreak", "enable"}, { Arg::value },
             "Enables a breakpoint",
             [this](Arguments& argv, long value) {

        copper.debugger.enableBreakpoint(parseNum(argv));
    });

    root.add({"cbreak", "disable"}, { Arg::value },
             "Disables a breakpoint",
             [this](Arguments& argv, long value) {

        copper.debugger.disableBreakpoint(parseNum(argv));
    });

    root.add({"cbreak", "ignore"}, { Arg::value, Arg::value },
             "Ignores a breakpoint a certain number of times",
             [this](Arguments& argv, long value) {

        copper.debugger.ignoreBreakpoint(util::parseNum(argv[0]), util::parseNum(argv[1]));
    });


    //
    // Copper watchpoints
    //

    root.add({"cwatch", ""},
             "Lists all watchpoints",
             [this](Arguments& argv, long value) {

        retroShell.dump(copper.debugger, Category::Watchpoints);
    });

    root.add({"cwatch", "at"}, { Arg::value },
             "Sets a watchpoint at the specified address",
             [this](Arguments& argv, long value) {

        copper.debugger.setWatchpoint(u32(parseNum(argv)));
    });

    root.add({"cwatch", "delete"}, { Arg::value },
             "Deletes a watchpoint",
             [this](Arguments& argv, long value) {

        copper.debugger.deleteWatchpoint(parseNum(argv));
    });

    root.add({"cwatch", "enable"}, { Arg::value },
             "Enables a watchpoint",
             [this](Arguments& argv, long value) {

        copper.debugger.enableWatchpoint(parseNum(argv));
    });

    root.add({"cwatch", "disable"}, { Arg::value },
             "Disables a watchpoint",
             [this](Arguments& argv, long value) {

        copper.debugger.disableWatchpoint(parseNum(argv));
    });

    root.add({"cwatch", "ignore"}, { Arg::value, Arg::value },
             "Ignores a watchpoint a certain number of times",
             [this](Arguments& argv, long value) {

        copper.debugger.ignoreWatchpoint(util::parseNum(argv[0]), util::parseNum(argv[1]));
    });


    //
    // Amiga
    //

    root.setGroup("");

    root.add({"amiga", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga, { Category::Config, Category::State } );
    });


    //
    // Memory
    //

    root.add({"memory", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(mem, { Category::Config, Category::State } );
    });

    root.add({"memory", "dump"}, { Arg::address },
             "Generates a memory hexdump",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        debugger.memDump<ACCESSOR_CPU>(ss, u32(parseNum(argv)), 16, 2);
        retroShell << '\n' << ss << '\n';
    });

    root.add({"memory", "bankmap"},
             "Dumps the memory bank map",
             [this](Arguments& argv, long value) {

        retroShell.dump(mem, Category::BankMap);
    });

    root.add({"memory", "write"}, { Arg::value, Arg::value },
             "Writes a word into memory",
             [this](Arguments& argv, long value) {

        auto addr = (u32)util::parseNum(argv[0]);
        auto val = (u16)util::parseNum(argv[1]);
        mem.patch(addr, val);
    });


    //
    // CPU
    //

    root.add({"cpu", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(cpu, { Category::Config, Category::State, Category::Registers } );
    });

    root.add({"cpu", "vectors"},
             "Dumps the vector table",
             [this](Arguments& argv, long value) {

        retroShell.dump(cpu, Category::Vectors);
    });


    //
    // CIA
    //

    for (isize i = 0; i < 2; i++) {

        string cia = (i == 0) ? "ciaa" : "ciab";

        root.add({cia, ""},
                 "Inspects the internal state",
                 [this](Arguments& argv, long value) {

            if (value == 0) {
                retroShell.dump(ciaa, { Category::Config, Category::State, Category::Registers } );
            } else {
                retroShell.dump(ciab, { Category::Config, Category::State, Category::Registers } );
            }
        }, i);

        root.add({cia, "tod"},
                 "Displays the state of the 24-bit counter",
                 [this](Arguments& argv, long value) {

            if (value == 0) {
                retroShell.dump(ciaa.tod, Category::State );
            } else {
                retroShell.dump(ciab.tod, Category::State );
            }
        }, i);
    }


    //
    // Agnus
    //

    root.add({"agnus", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(agnus, { Category::Config, Category::State, Category::Registers } );
    });

    root.add({"agnus", "beam"},
             "Displays the current beam position",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.agnus, Category::Beam);
    });

    root.add({"agnus", "dma"},
             "Prints all scheduled DMA events",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.agnus, Category::Dma);
    });

    root.add({"agnus", "sequencer"},
             "Inspects the sequencer logic",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.agnus.sequencer, { Category::State, Category::Registers, Category::Signals } );
    });

    root.add({"agnus", "events"},
             "Inspects the event scheduler",
             [this](Arguments& argv, long value) {

        retroShell.dump(amiga.agnus, Category::Events);
    });


    //
    // Blitter
    //

    root.add({"blitter", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(blitter, { Category::Config, Category::State, Category::Registers } );
    });


    //
    // Copper
    //

    root.add({"copper", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(copper, { Category::Config, Category::State, Category::Registers } );
    });

    root.add({"copper", "list"}, { Arg::value },
             "Prints the Copper list",
             [this](Arguments& argv, long value) {

        auto nr = parseNum(argv);

        switch (nr) {

            case 1: retroShell.dump(amiga.agnus.copper, Category::List1); break;
            case 2: retroShell.dump(amiga.agnus.copper, Category::List2); break;

            default:
                throw VAError(ERROR_OPT_INVARG, "1 or 2");
        }
    });


    //
    // Paula
    //

    root.add({"paula", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(paula, Category::Registers);
    });

    root.add({"paula", "audio"},
             "Audio unit");

    root.add({"paula", "dc"},
             "Disk controller");

    root.add({"paula", "uart"},
             "Universal Asynchronous Receiver Transmitter");

    root.add({"paula", "audio", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(paula, { Category::Config, Category::State, Category::Registers } );
    });

    root.add({"paula", "audio", "filter"},
             "Inspects the internal filter state",
             [this](Arguments& argv, long value) {

        retroShell.dump(paula.muxer.filter, { Category::Config, Category::State } );
    });

    root.add({"paula", "dc", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(diskController, { Category::Config, Category::State } );
    });

    root.add({"paula", "uart", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(uart, Category::State);
    });


    //
    // Denise
    //

    root.add({"denise", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(denise, { Category::Config, Category::State, Category::Registers } );
    });


    //
    // RTC
    //

    root.add({"rtc", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(rtc, { Category::Config, Category::State, Category::Registers } );
    });


    //
    // Zorro boards
    //

    root.add({"zorro", ""},
             "Lists all connected boards",
             [this](Arguments& argv, long value) {

        retroShell.dump(zorro, Category::Slots);
    });

    root.add({"zorro", "inspect"}, { Arg::value },
             "Inspects a specific Zorro board",
             [this](Arguments& argv, long value) {

        auto nr = parseNum(argv);

        if (auto board = zorro.getBoard(nr); board != nullptr) {

            retroShell.dump(*board, { Category::Properties, Category::State, Category::Stats } );
        }
    });


    //
    // Control ports
    //

    for (isize i = 1; i <= 2; i++) {

        string nr = (i == 1) ? "1" : "2";

        root.add({"controlport", nr},
                 "Control port " + nr);

        root.add({"controlport", nr, ""},
                 "Inspects the internal state",
                 [this](Arguments& argv, long value) {

            if (value == 1) retroShell.dump(controlPort1, Category::State);
            if (value == 2) retroShell.dump(controlPort2, Category::State);

        }, i);
    }


    //
    // Serial port
    //

    root.add({"serial", ""},
             "Displays the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(serialPort, { Category::Config, Category::State } );
    });


    //
    // Keyboard, Mice, Joystick
    //

    root.add({"keyboard", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(keyboard, { Category::Config, Category::State } );
    });

    for (isize i = 1; i <= 2; i++) {

        string nr = (i == 1) ? "1" : "2";

        root.add({"mouse", nr},
                 "Mouse in port " + nr);

        root.add({"mouse", nr, ""},
                 "Inspects the internal state",
                 [this](Arguments& argv, long value) {

            if (value == 1) retroShell.dump(controlPort1.mouse, { Category::Config, Category::State } );
            if (value == 2) retroShell.dump(controlPort2.mouse, { Category::Config, Category::State } );

        }, i);

        root.add({"joystick", nr},
                 "Joystick in port " + nr);

        root.add({"joystick", nr, ""},
                 "Inspects the internal state",
                 [this](Arguments& argv, long value) {

            if (value == 1) retroShell.dump(controlPort1.joystick, Category::State);
            if (value == 2) retroShell.dump(controlPort2.joystick, Category::State);

        }, i);
    }

    //
    // Df0, Df1, Df2, Df3
    //

    for (isize i = 0; i < 4; i++) {

        string df = "df" + std::to_string(i);

        root.add({df, ""},
                 "Inspects the internal state",
                 [this](Arguments& argv, long value) {

            retroShell.dump(*amiga.df[value], { Category::Config, Category::State } );

        }, i);

        root.add({df, "disk"},
                 "Inspects the inserted disk",
                 [this](Arguments& argv, long value) {

            retroShell.dump(*amiga.df[value], Category::Disk);

        }, i);
    }

    //
    // Hd0, Hd1, Hd2, Hd3
    //

    for (isize i = 0; i < 4; i++) {

        string hd = "hd" + std::to_string(i);

        root.add({hd, ""},
                 "Inspects the internal state",
                 [this](Arguments& argv, long value) {

            retroShell.dump(*amiga.hd[value], { Category::Config, Category::State } );

        }, i);

        root.add({hd, "drive"},
                 "Displays hard drive parameters",
                 [this](Arguments& argv, long value) {

            retroShell.dump(*amiga.df[value], Category::Drive);

        }, i);

        root.add({hd, "volumes"},
                 "Displays summarized volume information",
                 [this](Arguments& argv, long value) {

            retroShell.dump(*amiga.df[value], Category::Volumes);

        }, i);

        root.add({hd, "partitions"},
                 "Displays information about all partitions",
                 [this](Arguments& argv, long value) {

            retroShell.dump(*amiga.hd[value], Category::Partitions);

        }, i);
    }


    //
    // Debug variables
    //

    root.add({"set"}, { "<variable>", Arg::value },
             "Sets an internal debug variable",
             [this](Arguments& argv, long value) {

        Amiga::setDebugVariable(argv[0], int(parseNum(argv, 1)));
    });


    //
    // Host computer
    //

    root.add({"host", ""},
             "Displays information about the host machine",
             [this](Arguments& argv, long value) {

        retroShell.dump(host, Category::State);
    });


    //
    // OSDebugger
    //

    root.add({"os", "info"},
             "Displays basic system information",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        osDebugger.dumpInfo(ss);
        retroShell << ss;
    });

    root.add({"os", "execbase"},
             "Displays information about the ExecBase struct",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        osDebugger.dumpExecBase(ss);
        retroShell << ss;
    });

    root.add({"os", "interrupts"},
             "Lists all interrupt handlers",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        osDebugger.dumpIntVectors(ss);
        retroShell << ss;
    });

    root.add({"os", "libraries"}, { }, {"<library>"},
             "Lists all libraries",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        isize num;

        if (argv.empty()) {
            osDebugger.dumpLibraries(ss);
        } else if (util::parseHex(argv.front(), &num)) {
            osDebugger.dumpLibrary(ss, (u32)num);
        } else {
            osDebugger.dumpLibrary(ss, argv.front());
        }

        retroShell << ss;
    });

    root.add({"os", "devices"}, { }, {"<device>"},
             "Lists all devices",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        isize num;

        if (argv.empty()) {
            osDebugger.dumpDevices(ss);
        } else if (util::parseHex(argv.front(), &num)) {
            osDebugger.dumpDevice(ss, (u32)num);
        } else {
            osDebugger.dumpDevice(ss, argv.front());
        }

        retroShell << ss;
    });

    root.add({"os", "resources"}, { }, {"<resource>"},
             "Lists all resources",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        isize num;

        if (argv.empty()) {
            osDebugger.dumpResources(ss);
        } else if (util::parseHex(argv.front(), &num)) {
            osDebugger.dumpResource(ss, (u32)num);
        } else {
            osDebugger.dumpResource(ss, argv.front());
        }

        retroShell << ss;
    });

    root.add({"os", "tasks"}, { }, {"<task>"},
             "Lists all tasks",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        isize num;

        if (argv.empty()) {
            osDebugger.dumpTasks(ss);
        } else if (util::parseHex(argv.front(), &num)) {
            osDebugger.dumpTask(ss, (u32)num);
        } else {
            osDebugger.dumpTask(ss, argv.front());
        }

        retroShell << ss;
    });

    root.add({"os", "processes"}, { }, {"<process>"},
             "Lists all processes",
             [this](Arguments& argv, long value) {

        std::stringstream ss;
        isize num;

        if (argv.empty()) {
            osDebugger.dumpProcesses(ss);
        } else if (util::parseHex(argv.front(), &num)) {
            osDebugger.dumpProcess(ss, (u32)num);
        } else {
            osDebugger.dumpProcess(ss, argv.front());
        }

        retroShell << ss;
    });

    root.add({"os", "catch"}, {"<task>"},
             "Pauses emulation on task launch",
             [this](Arguments& argv, long value) {

        diagBoard.catchTask(argv.back());
        retroShell << "Waiting for task '" << argv.back() << "' to start...\n";
    });

    root.add({"os", "set"},
             "Configures the component");

    root.add({"os", "set", "diagboard" }, { Arg::boolean },
             "Attaches or detaches the debug expansion board",
             [this](Arguments& argv, long value) {

        diagBoard.setConfigItem(OPT_DIAG_BOARD, parseBool(argv));
    });


    //
    // Remote server
    //

    root.setGroup("");

    root.add({"server", ""},
             "Displays a server status summary",
             [this](Arguments& argv, long value) {

        retroShell.dump(remoteManager, Category::Status);
    });

    root.add({"server", "serial"},
             "Serial port server");

    root.add({"server", "serial", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(remoteManager.serServer, { Category::Config, Category::State } );
    });

    root.add({"server", "rshell"},
             "Retro shell server");

    root.add({"server", "rshell", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(remoteManager.rshServer, { Category::Config, Category::State } );
    });

    root.add({"server", "gdb"},
             "GDB server");

    root.add({"server", "gdb", ""},
             "Inspects the internal state",
             [this](Arguments& argv, long value) {

        retroShell.dump(remoteManager.gdbServer, { Category::Config, Category::State } );
    });
}

}
