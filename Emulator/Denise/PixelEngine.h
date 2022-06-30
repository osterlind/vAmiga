// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "PixelEngineTypes.h"
#include "SubComponent.h"
#include "ChangeRecorder.h"
#include "Constants.h"
#include "Buffer.h"

using util::Buffer;

struct FrameBuffer {

    // Checkerboard colors
    static constexpr u32 col1 = 0xFF222222; // 0xFF662222
    static constexpr u32 col2 = 0xFF444444; // 0xFFAA4444

    Buffer <u64> pixels;
    bool longFrame;
    
    FrameBuffer();

    // Initializes (a portion of) the frame buffer with a checkerboard pattern
    void clean();
    void clean(isize row);
    void clean(isize row, isize cycle);
};

class PixelEngine : public SubComponent {

    friend class Denise;

    // Current configuration
    PixelEngineConfig config = {};

public:

    // RGBA colors used to visualize the HBLANK and VBLANK area in the debugger
    static const u32 rgbaHBlank = 0xFF444444;
    static const u32 rgbaVBlank = 0xFF444444;

    //
    // Screen buffers
    //

private:

    /* The emulator utilizes double-buffering for the computed textures.
     * At any time, one of the two buffers is the "working buffer". The other
     * one is the "stable buffer". All drawing functions write to the working
     * buffer and the GPU reads from the stable buffer. Once a frame has
     * been completed, the working buffer and the stable buffer are swapped.
     */
    FrameBuffer emuTexture[2];

    // The currently active buffer
    isize activeBuffer = 0;

    // Mutex for synchronizing access to the stable buffer
    util::Mutex bufferMutex;
        
    // Buffer with background noise (random black and white pixels)
    Buffer<u32> noise;

    
    //
    // Color management
    //

    // The 32 Amiga color registers
    u16 colreg[32];

    // RGBA values for all possible 4096 Amiga colors
    u32 rgba[4096];

    /* The color register values translated to RGBA
     * Note that the number of elements exceeds the number of color registers:
     *  0 .. 31 : RGBA values of the 32 color registers
     * 32 .. 63 : RGBA values of the 32 color registers in halfbright mode
     *       64 : Pure black (used if the ECS BRDRBLNK bit is set)
     * 65 .. 72 : Additional colors used for debugging
     */
    static const int rgbaIndexCnt = 32 + 32 + 1 + 8;
    u64 indexedRgba[rgbaIndexCnt];
    
    // Indicates whether HAM mode is switched
    bool hamMode;
    
    
    //
    // Register change history buffer
    //

public:

    // Color register history
    RegChangeRecorder<128> colChanges;


    //
    // Initializing
    //
    
public:
    
    PixelEngine(Amiga& ref);
 
    // Initializes both frame buffers with a checkerboard pattern
    void clearAll();

    // Initializes (part of) the current frame buffer with a checkerboard pattern
    // void clear(isize line);
    // void clear(isize line, Pixel pixel);

private:

    // void clear(u32 *ptr, isize line, Pixel first = 0, Pixel last = HPOS_MAX);


    //
    // Methods from AmigaObject
    //
    
private:
    
    const char *getDescription() const override { return "PixelEngine"; }
    void _dump(Category category, std::ostream& os) const override { }

    
    //
    // Methods from AmigaComponent
    //
    
private:
    
    void _initialize() override;
    void _reset(bool hard) override;

    
    //
    // Configuring
    //

public:
    
    const PixelEngineConfig &getConfig() const { return config; }
    void resetConfig() override;

    i64 getConfigItem(Option option) const;
    void setConfigItem(Option option, i64 value);

    
    //
    // Serializing
    //
    
private:
    
    template <class T>
    void applyToPersistentItems(T& worker)
    {
        
    }

    template <class T>
    void applyToResetItems(T& worker, bool hard = true)
    {
        worker

        >> colChanges
        << colreg
        << hamMode;
    }

    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    u64 _checksum() override { COMPUTE_SNAPSHOT_CHECKSUM }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }
    isize didLoadFromBuffer(const u8 *buffer) override;

    
    //
    // Controlling
    //
    
private:

    void _powerOn() override;


    //
    // Accessing color registers
    //

public:

    // Performs a consistency check for debugging.
    static bool isRgbaIndex(isize nr) { return nr < rgbaIndexCnt; }
    
    // Changes one of the 32 Amiga color registers.
    void setColor(isize reg, u16 value);

    // Returns a color value in Amiga format or RGBA format
    u16 getColor(isize nr) const { return colreg[nr]; }
    u32 getRGBA(isize nr) const { return (u32)indexedRgba[nr]; }

    // Returns sprite color in Amiga format or RGBA format
    u16 getSpriteColor(isize s, isize nr) const { return getColor(16 + nr + 2 * (s & 6)); }
    u32 getSpriteRGBA(isize s, isize nr) const { return rgba[getSpriteColor(s,nr)]; }


    //
    // Using the color lookup table
    //

private:

    // Updates the entire RGBA lookup table
    void updateRGBA();

    // Adjusts the RGBA value according to the selected color parameters
    void adjustRGB(u8 &r, u8 &g, u8 &b);


    //
    // Working with frame buffers
    //

public:

    // Returns the working buffer or the stable buffer
    FrameBuffer &getWorkingBuffer();
    const FrameBuffer &getStableBuffer();

    // Return a pointer into the pixel storage
    u64 *workingPtr(isize row = 0, isize col = 0);
    u64 *stablePtr(isize row = 0, isize col = 0);
    
    // Swaps the working buffer and the stable buffer
    void swapBuffers();
    
    // Returns a pointer to randon noise
    u32 *getNoise() const;

    // Called after each line in the VBLANK area
    void endOfVBlankLine();

    // Called after each frame to switch the frame buffers
    void vsyncHandler();


    //
    // Working with recorded register changes
    //

public:

    // Applies a register change
    void applyRegisterChange(const RegChange &change);


    //
    // Synthesizing pixels
    //

public:
    
    /* Colorizes a rasterline. This function implements the last stage in the
     * graphics pipelile. It translates a line of color register indices into a
     * line of RGBA values in GPU format.
     */
    void colorize(isize line);
    
private:
    
    void colorize(u64 *dst, Pixel from, Pixel to);
    void colorizeHAM(u64 *dst, Pixel from, Pixel to, u16& ham);
    
    /* Hides some graphics layers. This function is an optional stage applied
     * after colorize(). It can be used to hide some layers for debugging.
     */
    
public:
    
    void hide(isize line, u16 layer, u8 alpha);
};
