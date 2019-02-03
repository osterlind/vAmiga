// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#ifndef _TOD_INC
#define _TOD_INC

#include "HardwareComponent.h"

class CIA;

//! @brief    Increments a BCD number by one.
inline uint8_t incBCD(uint8_t bcd) {
    return ((bcd & 0x0F) == 0x09) ? (bcd & 0xF0) + 0x10 : (bcd & 0xF0) + ((bcd + 0x01) & 0x0F);
}

/*! @brief    Time of day clock (TOD)
 *  @details  Each CIA chip contains a time of day clock, counting hours,
 *            minutes, seconds and tenth of a second. Every TOD clock features
 *            an alarm mechanism. When the alarm time is reached, an interrupt
 *            is initiated.
 */
class TOD : public HardwareComponent {
    
    friend CIA;

private:
    
    //! @brief    Reference to the connected CIA
    CIA *cia;
    
    //! @brief    Time of day clock
	TimeOfDay tod;

    //! @brief    Time of day clock latch
    TimeOfDay latch;

    //! @brief    Alarm time
	TimeOfDay alarm;
	
	/*! @brief    Indicates if the TOD registers are frozen
	 *  @details  The CIA chip freezes the registers when the hours-part is read
     *            and reactivates them, when the 1/10th part is read. Although
     *            the values stay constant, the internal clock continues to run.
     *            Purpose: If you start reading with the hours-part, the clock
     *            won't change until you have read the whole time.
     */
	bool frozen;
	
	/*! @brief    Indicates if the TOD clock is halted.
	 *  @details  The CIA chip stops the TOD clock when the hours-part is
     *            written and restarts it, when the 1/10th part is written.
     *            Purpose: The clock will only start running when the time is
     *            completely set.
     */
	bool stopped;
	
    /*! @brief    Indicates if tod time matches the alarm time
     *  @details  This value is read in checkForInterrupt() for edge detection.
     */
    bool matching;
        
public:
    
    //
    //! @functiongroup Creating and destructing
    //
    
	//! @brief    Constructor
	TOD(CIA *cia);
	
    
    //
    //! @functiongroup Methods from HardwareComponent
    //

    void _powerOn() override;
	void _dump() override;

    
    //
    //! @functiongroup Configuring the component
    //
 
    //! @brief    Returns the current configuration.
    TODInfo getInfo();
    

    //
    //! @functiongroup Running the component
    //
    
private:
    
    //! @brief    Freezes the time of day clock.
    void freeze() { if (!frozen) { latch.value = tod.value; frozen = true; } }
    
    //! @brief    Unfreezes the time of day clock.
    void defreeze() { frozen = false; }
    
    //! @brief    Stops the time of day clock.
    void stop() { stopped = true; }
    
    //! @brief    Starts the time of day clock.
    void cont() { stopped = false; }

    //! @brief    Returns the hours digits of the time of day clock.
    uint8_t getTodHours() { return (frozen ? latch.hours : tod.hours) & 0x9F; }

    //! @brief    Returns the minutes digits of the time of day clock.
    uint8_t getTodMinutes() { return (frozen ? latch.minutes : tod.minutes) & 0x7F; }

    //! @brief    Returns the seconds digits of the time of day clock.
    uint8_t getTodSeconds() { return (frozen ? latch.seconds : tod.seconds) & 0x7F; }

    //! @brief    Returns the tenth-of-a-second digits of the time of day clock.
    uint8_t getTodTenth() { return (frozen ? latch.tenth : tod.tenth) & 0x0F; }

    //! @brief    Returns the hours digits of the alarm time.
    uint8_t getAlarmHours() { return alarm.hours & 0x9F; }

    //! @brief    Returns the minutes digits of the alarm time.
    uint8_t getAlarmMinutes() { return alarm.minutes & 0x7F; }

    //! @brief    Returns the seconds digits of the alarm time.
    uint8_t getAlarmSeconds() { return alarm.seconds & 0x7F; }

    //! @brief    Returns the tenth-of-a-second digits of the alarm time.
    uint8_t getAlarmTenth() { return alarm.tenth & 0x0F; }
    

	//! @brief    Sets the hours digits of the time of day clock.
    void setTodHours(uint8_t value) { tod.hours = value & 0x9F; checkForInterrupt(); }
	
	//! @brief    Sets the minutes digits of the time of day clock.
    void setTodMinutes(uint8_t value) {
        tod.minutes = value & 0x7F; checkForInterrupt(); }
	
	//! @brief    Sets the seconds digits of the time of day clock.
    void setTodSeconds(uint8_t value) {
        tod.seconds = value & 0x7F; checkForInterrupt(); }
	
	//! @brief    Sets the tenth-of-a-second digits of the time of day clock.
	void setTodTenth(uint8_t value) {
        tod.tenth = value & 0x0F; checkForInterrupt(); }
	
	//! @brief    Sets the hours digits of the alarm time.
    void setAlarmHours(uint8_t value) {
        alarm.hours = value & 0x9F; checkForInterrupt(); }
	
	//! @brief    Sets the minutes digits of the alarm time.
    void setAlarmMinutes(uint8_t value) {
        alarm.minutes = value & 0x7F; checkForInterrupt(); }
	
	//! @brief    Sets the seconds digits of the alarm time.
    void setAlarmSeconds(uint8_t value) {
        alarm.seconds = value & 0x7F; checkForInterrupt(); }
	
	//! @brief    Sets the tenth-of-a-second digits of the time of day clock.
    void setAlarmTenth(uint8_t value) {
        alarm.tenth = value & 0x0F; checkForInterrupt(); }
	
	/*! @brief    Increments the TOD clock by one tenth of a second.
     */
	void increment();

    /*! @brief    Updates variable 'matching'
     *  @details  If a positive edge occurs, the connected CIA will be requested
     *            to trigger an interrupt.
     */
    void checkForInterrupt();
};

#endif


