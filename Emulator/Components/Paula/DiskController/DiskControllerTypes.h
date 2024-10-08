// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Types.h"
#include "Reflection.h"

//
// Enumerations
//

enum_long(DRIVE_DMA_STATE)
{
    DRIVE_DMA_OFF,     // Drive is idle

    DRIVE_DMA_WAIT,    // Drive is waiting for the sync word
    DRIVE_DMA_READ,    // Drive is reading

    DRIVE_DMA_WRITE,   // Drive is writing
    DRIVE_DMA_FLUSH,   // Drive is finishing up the write process
};
typedef DRIVE_DMA_STATE DriveState;

#ifdef __cplusplus
struct DriveStateEnum : vamiga::util::Reflection<DriveStateEnum, DriveState>
{
    static constexpr long minVal = 0;
    static constexpr long maxVal = DRIVE_DMA_FLUSH;
    
    static const char *prefix()
    {
        return "DRIVE_DMA";
    }
    
    static const char *_key(long value)
    {
        switch (value) {
                
            case DRIVE_DMA_OFF:    return "OFF";

            case DRIVE_DMA_WAIT:   return "WAIT";
            case DRIVE_DMA_READ:   return "READ";

            case DRIVE_DMA_WRITE:  return "WRITE";
            case DRIVE_DMA_FLUSH:  return "FLUSH";
        }
        return "???";
    }
};
#endif

//
// Structures
//

typedef struct
{
    /* Acceleration factor. This value equals the number of words that get
     * transfered into memory during a single disk DMA cycle. This value must
     * be 1 to emulate a real Amiga. If it set to, e.g., 2, the drive loads
     * twice as fast. A value of -1 indicates a turbo drive. In this case,
     * the exact value of the acceleration factor has no meaning.
     */
    i32 speed;

    bool lockDskSync;
    bool autoDskSync;
}
DiskControllerConfig;

inline bool isValidDriveSpeed(isize speed)
{
    switch (speed) {
            
        case -1: case 1: case 2: case 4: case 8: return true;
    }
    return false;
}

typedef struct
{
    isize selectedDrive;
    DriveState state;
    i32 fifo[6];
    u8 fifoCount;

    u16 dsklen;
    u16 dskbytr;
    u16 dsksync;
    u8 prb;
}
DiskControllerInfo;
