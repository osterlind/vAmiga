// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "AmigaTypes.h"
#include "Agnus.h"
#include "ControlPort.h"
#include "CIA.h"
#include "CPU.h"
#include "Denise.h"
#include "Drive.h"
#include "Keyboard.h"
#include "Memory.h"
#include "MsgQueue.h"
#include "Paula.h"
#include "RegressionTester.h"
#include "RetroShell.h"
#include "RTC.h"
#include "SerialPort.h"
#include "Thread.h"
#include "ZorroManager.h"

void threadTerminated(void *thisAmiga);
void *threadMain(void *thisAmiga);

/* A complete virtual Amiga. This class is the most prominent one of all. To
 * run the emulator, it is sufficient to create a single object of this type.
 * All subcomponents are created automatically. The public API gives you
 * control over the emulator's behaviour such as running and pausing the
 * emulation. Please note that most subcomponents have their own public API.
 * E.g., to query information from Paula, you need to invoke a public method on
 * amiga.paula.
 */
class Amiga : public Thread {

    /* Result of the latest inspection. In order to update the GUI inspector
     * panels, the emulator schedules events in the inspector slot (SLOT_INS in
     * the secondary table) on a periodic basis. Inside the event handler, the
     * current state is recorded. When the GUI updates the inspector panels, it
     * displays the result of the latest inspection.
     */
    AmigaInfo info = {};

     
    //
    // Sub components
    //
    
public:
    
    // The thread manager
    // Thread thread = Thread(*this);

    // Core components
    CPU cpu = CPU(*this);
    CIAA ciaA = CIAA(*this);
    CIAB ciaB = CIAB(*this);
    Memory mem = Memory(*this);
    Agnus agnus = Agnus(*this);
    Denise denise = Denise(*this);
    Paula paula = Paula(*this);
    
    // Logic board
    RTC rtc = RTC(*this);
    ZorroManager zorro = ZorroManager(*this);
    
    // Ports
    ControlPort controlPort1 = ControlPort(*this, PORT_1);
    ControlPort controlPort2 = ControlPort(*this, PORT_2);
    SerialPort serialPort = SerialPort(*this);

    // Peripherals
    Keyboard keyboard = Keyboard(*this);

    // Floppy drives
    Drive df0 = Drive(*this, 0);
    Drive df1 = Drive(*this, 1);
    Drive df2 = Drive(*this, 2);
    Drive df3 = Drive(*this, 3);
    
    // Shortcuts to all four drives
    Drive *df[4] = { &df0, &df1, &df2, &df3 };
    
    // Command console
    RetroShell retroShell = RetroShell(*this);
    
    // Communication channel to the GUI
    MsgQueue msgQueue = MsgQueue(*this);

    // Regression test manager
    RegressionTester regressionTester;
    
    
    //
    // Emulator thread
    //
    
private:
        
    /* Run loop flags. This variable is checked at the end of each runloop
     * iteration. Most of the time, the variable is 0 which causes the runloop
     * to repeat. A value greater than 0 means that one or more runloop control
     * flags are set. These flags are flags processed and the loop either
     * repeats or terminates depending on the provided flags.
     */
    RunLoopFlags flags = 0;
    
    // The invocation counter for implementing suspend() / resume()
    isize suspendCounter = 0;
            

    //
    // Snapshot storage
    //
    
private:
    
    class Snapshot *autoSnapshot = nullptr;
    class Snapshot *userSnapshot = nullptr;

    
    //
    // Initializing
    //
    
public:
    
    Amiga();
    ~Amiga();

    const char *getDescription() const override { return "Amiga"; }

    void prefix() const override;

    void reset(bool hard);
    void hardReset() { reset(true); }
    void softReset() { reset(false); }

private:
    
    void _reset(bool hard) override;

    
    //
    // Configuring
    //
    
public:
        
    // Gets a single configuration item
    i64 getConfigItem(Option option) const;
    i64 getConfigItem(Option option, long id) const;
    
    // Sets a single configuration item and informs the GUI
    void configure(Option option, i64 value) throws;
    void configure(Option option, long id, i64 value) throws;

    // Sets a single configuration item without informing the GUI
    void _configure(Option option, i64 value) throws;
    void _configure(Option option, long id, i64 value) throws;

    // Prepares the Amiga for regression testing
    void configure(ConfigScheme scheme);
    
    
    //
    // Analyzing
    //
    
public:
    
    AmigaInfo getInfo() { return AmigaComponent::getInfo(info); }
    
    EventID getInspectionTarget() const;
    void setInspectionTarget(EventID id);
    void setInspectionTarget(EventID id, Cycle trigger);
    void removeInspectionTarget();
    
private:
    
    void _inspect() override;
    void _dump(dump::Category category, std::ostream& os) const override;
    
    
    //
    // Controlling
    //
    
private:
    
    void _powerOn() override;
    void _powerOff() override;
    void _run() override;
    void _pause() override;
    void _halt() override;
    void _warpOn() override;
    void _warpOff() override;
    
    
    //
    // Serializing
    //
    
private:
    
    template <class T>
    void applyToPersistentItems(T& worker)
    {
    }

    template <class T>
    void applyToHardResetItems(T& worker)
    {
    }

    template <class T>
    void applyToResetItems(T& worker)
    {
    }

    isize _size() override { COMPUTE_SNAPSHOT_SIZE }
    isize _load(const u8 *buffer) override { LOAD_SNAPSHOT_ITEMS }
    isize _save(u8 *buffer) override { SAVE_SNAPSHOT_ITEMS }

    
    //
    // Methods from Thread class
    //
    
    bool readyToPowerOn() override;
    void execute() override;
    
    
    //
    // Working with the emulator thread
    //
    
public:
                
    /* Pauses the emulation thread temporarily. Because the emulator is running
     * in a separate thread, the GUI has to pause the emulator before changing
     * it's internal state. This is done by embedding the code inside a
     * suspend / resume block:
     *
     *            suspend();
     *            do something with the internal state;
     *            resume();
     *
     *  It it safe to nest multiple suspend() / resume() blocks.
     */
    void suspend();
    void resume();
    
    /* Sets or clears a run loop control flag. The functions are thread-safe
     * and can be called from inside or outside the emulator thread.
     */
    void setControlFlag(u32 flags);
    void clearControlFlag(u32 flags);
    
    // Convenience wrappers for controlling the run loop
    void signalStop() { setControlFlag(RL::STOP); }
    void signalInspect() { setControlFlag(RL::INSPECT); }
    void signalWarpOn() { setControlFlag(RL::WARP_ON); }
    void signalWarpOff() { setControlFlag(RL::WARP_OFF); }
    void signalAutoSnapshot() { setControlFlag(RL::AUTO_SNAPSHOT); }
    void signalUserSnapshot() { setControlFlag(RL::USER_SNAPSHOT); }

    //
    // Running the emulator
    //
    
public:
    
    // Runs or pauses the emulator
    void stopAndGo();
    
    /* Executes a single instruction. This function is used for single-stepping
     * through the code inside the debugger. It starts the execution thread and
     * terminates it after the next instruction has been executed.
     */
    void stepInto();
    
    /* Runs the emulator until the instruction following the current one is
     * reached. This function is used for single-stepping through the code
     * inside the debugger. It sets a soft breakpoint to PC+n where n is the
     * length bytes of the current instruction and starts the emulator thread.
     */
    void stepOver();
        
    
    //
    // Handling snapshots
    //
    
public:
    
    /* Requests a snapshot to be taken. Once the snapshot is ready, a message
     * is written into the message queue. The snapshot can then be picked up by
     * calling latestAutoSnapshot() or latestUserSnapshot(), depending on the
     * requested snapshot type.
     */
    void requestAutoSnapshot();
    void requestUserSnapshot();
     
    // Returns the most recent snapshot or nullptr if none was taken
    Snapshot *latestAutoSnapshot();
    Snapshot *latestUserSnapshot();

    /* Loads the current state from a snapshot file. There is an thread-unsafe
     * and thread-safe version of this function. The first one can be unsed
     * inside the emulator thread or from outside if the emulator is halted.
     * The second one can be called any time.
     */
    void loadFromSnapshotUnsafe(Snapshot *snapshot);
    void loadFromSnapshotSafe(Snapshot *snapshot);
};
