// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------
// THIS FILE MUST CONFORM TO ANSI-C TO BE COMPATIBLE WITH SWIFT
// -----------------------------------------------------------------------------

/* This file defines all constants and data types that are exposed to the GUI.
 * All definitions comply to standard ANSI-C to make the file accessible by
 * Swift. Note that the Swift GUI does not interact directly with any of the
 * public API methods of the emulator. Since Swift cannot deal with C++ code
 * directly yet, all API accesses are routed through the proxy layer written in
 * Objective-C.
 */

#pragma once

#include "Aliases.h"

#include "AgnusPublicTypes.h"
#include "CPUPublicTypes.h"
#include "CIAPublicTypes.h"
#include "DenisePublicTypes.h"
#include "DiskPublicTypes.h"
#include "DmaDebuggerPublicTypes.h"
#include "DrivePublicTypes.h"
#include "EventHandlerPublicTypes.h"
#include "FilePublicTypes.h"
#include "FSPublicTypes.h"
#include "KeyboardPublicTypes.h"
#include "MemoryPublicTypes.h"
#include "MsgQueuePublicTypes.h"
#include "PaulaPublicTypes.h"
#include "PortPublicTypes.h"
#include "RTCPublicTypes.h"

//
// Enumerations
//

enum_long(OPT)
{
    // Agnus
    OPT_AGNUS_REVISION,
    OPT_SLOW_RAM_MIRROR,
    
    // Denise
    OPT_DENISE_REVISION,
    OPT_BRDRBLNK,
    
    // Real-time clock
    OPT_RTC_MODEL,

    // Memory
    OPT_CHIP_RAM,
    OPT_SLOW_RAM,
    OPT_FAST_RAM,
    OPT_EXT_START,
    OPT_SLOW_RAM_DELAY,
    OPT_BANKMAP,
    OPT_UNMAPPING_TYPE,
    OPT_RAM_INIT_PATTERN,
    
    // Disk controller
    OPT_DRIVE_CONNECT,
    OPT_DRIVE_SPEED,
    OPT_LOCK_DSKSYNC,
    OPT_AUTO_DSKSYNC,

    // Drives
    OPT_DRIVE_TYPE,
    OPT_EMULATE_MECHANICS,
    
    // Ports
    OPT_SERIAL_DEVICE,

    // Compatibility
    OPT_HIDDEN_SPRITES,
    OPT_HIDDEN_LAYERS,
    OPT_HIDDEN_LAYER_ALPHA,
    OPT_CLX_SPR_SPR,
    OPT_CLX_SPR_PLF,
    OPT_CLX_PLF_PLF,
        
    // Blitter
    OPT_BLITTER_ACCURACY,
    
    // CIAs
    OPT_CIA_REVISION,
    OPT_TODBUG,
    OPT_ECLOCK_SYNCING,
    OPT_ACCURATE_KEYBOARD,
    
    // Paula audio
    OPT_SAMPLING_METHOD,
    OPT_FILTER_TYPE,
    OPT_FILTER_ALWAYS_ON,
    OPT_AUDPAN,
    OPT_AUDVOL,
    OPT_AUDVOLL,
    OPT_AUDVOLR,
    
    OPT_COUNT
};
typedef OPT Option;

enum_long(EMULATOR_STATE)
{
    EMULATOR_STATE_OFF,
    EMULATOR_STATE_PAUSED,
    EMULATOR_STATE_RUNNING,

    EMULATOR_STATE_COUNT
};
typedef EMULATOR_STATE EmulatorState;

enum_long(ErrorCode)
{
    ERR_OK,
    ERR_ROM_MISSING,
    ERR_AROS_NO_EXTROM,
    ERR_AROS_RAM_LIMIT,
    ERR_CHIP_RAM_LIMIT
};

//
// Structures
//

typedef struct
{
    Cycle cpuClock;
    Cycle dmaClock;
    Cycle ciaAClock;
    Cycle ciaBClock;
    long frame;
    long vpos;
    long hpos;
}
AmigaInfo;
