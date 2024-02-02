// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Debugger.h"
#include "Amiga.h"
#include "IOUtils.h"
#include <sstream>

namespace vamiga {

const char *
Debugger::regName(u32 addr)
{
    return ChipsetRegEnum::key((addr >> 1) & 0xFF);
}

void
Debugger::_pause()
{

}

void
Debugger::stopAndGo()
{
    isRunning() ? amiga.pause() : amiga.run();
}

void
Debugger::stepInto()
{
    if (isRunning()) return;

    cpu.debugger.stepInto();
    amiga.run();

    // Inform the GUI
    msgQueue.put(MSG_STEP);
}

void
Debugger::stepOver()
{
    if (isRunning()) return;

    cpu.debugger.stepOver();
    amiga.run();

    // Inform the GUI
    msgQueue.put(MSG_STEP);
}

void
Debugger::jump(u32 addr)
{
    cpu.jump(addr);
}

template <Accessor A> const char *
Debugger::ascDump(u32 addr, isize bytes) const
{
    assert(bytes < 256);

    static char str[256];

    for (isize i = 0; i < bytes; i += 2) {

        u16 word = mem.spypeek16 <A> ((u32)(addr + i));
        str[i] = isprint(HI_BYTE(word)) ? HI_BYTE(word) : '.';
        str[i+1] = isprint(LO_BYTE(word)) ? LO_BYTE(word) : '.';
    }
    str[bytes] = 0;
    return str;
}

template <Accessor A> const char *
Debugger::hexDump(u32 addr, isize bytes, isize sz) const
{
    assert(sz == 1 || bytes % 2 == 0);
    assert(bytes <= 64);

    static char str[256];
    char *p = str;

    for (u32 i = 0; i < bytes; i++) {

        u8 byte = mem.spypeek8 <A> (addr + i);

        u8 digit1 = HI_NIBBLE(byte);
        u8 digit2 = LO_NIBBLE(byte);

        *p++ = digit1 < 10 ? '0' + digit1 : 'A' + digit1 - 10;
        *p++ = digit2 < 10 ? '0' + digit2 : 'A' + digit2 - 10;

        if ((i + 1) % sz == 0) *p++ = ' ';
    }
    *p = 0;

    /*
    for (isize i = 0; i < numBytes; i += 2, p += 5) {

        u16 word = mem.spypeek16 <A> ((u32)(addr + i));

        u8 digit1 = (word >> 12) & 0xF;
        u8 digit2 = (word >> 8) & 0xF;
        u8 digit3 = (word >> 4) & 0xF;
        u8 digit4 = (word >> 0) & 0xF;

        p[0] = digit1 < 10 ? '0' + digit1 : 'A' + digit1 - 10;
        p[1] = digit2 < 10 ? '0' + digit2 : 'A' + digit2 - 10;
        p[2] = digit3 < 10 ? '0' + digit3 : 'A' + digit3 - 10;
        p[3] = digit4 < 10 ? '0' + digit4 : 'A' + digit4 - 10;
        p[4] = i == numBytes - 2 ? char(0) : ' ';
    }
    */

    return str;
}

template <Accessor A> const char *
Debugger::memDump(u32 addr, isize bytes, isize sz) const
{
    assert(sz == 1 || bytes % 2 == 0);
    assert(bytes <= 32);
    
    static char str[256];

    strcpy(str, hexDump<A>(addr, bytes, sz));
    strcat(str, "  ");
    strcat(str, ascDump<A>(addr, bytes));

    return str;
}

template <Accessor A> void
Debugger::ascDump(std::ostream& os, u32 addr, isize lines)
{
    for (isize i = 0; i < lines; i++, addr += 64) {

        os << std::setfill('0') << std::hex << std::right << std::setw(6) << isize(addr);
        os << ":  ";
        os << hexDump<A>(addr, 64);
        os << std::endl;
    }
    current = addr;
}

template <Accessor A> void
Debugger::hexDump(std::ostream& os, u32 addr, isize lines, isize sz)
{
    if (sz != 1) addr &= ~0x1;

    for (isize i = 0; i < lines; i++, addr += 16) {

        os << std::setfill('0') << std::hex << std::right << std::setw(6) << isize(addr);
        os << ":  ";
        os << hexDump<A>(addr, 16);
        os << std::endl;
    }
    current = addr;
}

template <Accessor A> void
Debugger::memDump(std::ostream& os, u32 addr, isize lines, isize sz)
{
    if (sz != 1) addr &= ~0x1;

    for (isize i = 0; i < lines; i++, addr += 16) {

        os << std::setfill('0') << std::hex << std::right << std::setw(6) << isize(addr);
        os << ":  ";
        os << hexDump<A>(addr, 16, sz);
        os << "  ";
        os << ascDump<A>(addr, 16);
        os << std::endl;
    }
    current = addr;
}

bool
Debugger::isReadable(ChipsetReg reg) const
{
    switch (reg) {

        case REG_DMACONR:   case REG_VPOSR:     case REG_VHPOSR:    case REG_DSKDATR:
        case REG_JOY0DAT:   case REG_JOY1DAT:   case REG_CLXDAT:    case REG_ADKCONR:
        case REG_POT0DAT:   case REG_POT1DAT:   case REG_POTGOR:    case REG_SERDATR:
        case REG_DSKBYTR:   case REG_INTENAR:   case REG_INTREQR:

            return true;

        case REG_DENISEID:

            return denise.isECS();

        default:

            return false;
    }
}

bool
Debugger::isWritable(ChipsetReg reg) const
{
    switch (reg) {

        case REG_DSKPTH:    case REG_DSKPTL:    case REG_DSKLEN:    case REG_DSKDAT:
        case REG_REFPTR:    case REG_VPOSW:     case REG_VHPOSW:    case REG_COPCON:
        case REG_SERDAT:    case REG_SERPER:    case REG_POTGO:     case REG_JOYTEST:
        case REG_STREQU:    case REG_STRVBL:    case REG_STRHOR:    case REG_STRLONG:
        case REG_BLTCON0:   case REG_BLTCON1:   case REG_BLTAFWM:   case REG_BLTALWM:
        case REG_BLTCPTH:   case REG_BLTCPTL:   case REG_BLTBPTH:   case REG_BLTBPTL:
        case REG_BLTAPTH:   case REG_BLTAPTL:   case REG_BLTDPTH:   case REG_BLTDPTL:
        case REG_BLTSIZE:   case REG_BLTCMOD:   case REG_BLTBMOD:   case REG_BLTAMOD:
        case REG_BLTDMOD:   case REG_BLTCDAT:   case REG_BLTBDAT:   case REG_BLTADAT:
        case REG_DSKSYNC:   case REG_COP1LCH:   case REG_COP1LCL:   case REG_COP2LCH:
        case REG_COP2LCL:   case REG_COPJMP1:   case REG_COPJMP2:   case REG_COPINS:
        case REG_DIWSTRT:   case REG_DIWSTOP:   case REG_DDFSTRT:   case REG_DDFSTOP:
        case REG_DMACON:    case REG_CLXCON:    case REG_INTENA:    case REG_INTREQ:
        case REG_ADKCON:    case REG_AUD0LCH:   case REG_AUD0LCL:   case REG_AUD0LEN:
        case REG_AUD0PER:   case REG_AUD0VOL:   case REG_AUD0DAT:   case REG_AUD1LCH:
        case REG_AUD1LCL:   case REG_AUD1LEN:   case REG_AUD1PER:   case REG_AUD1VOL:
        case REG_AUD1DAT:   case REG_AUD2LCH:   case REG_AUD2LCL:   case REG_AUD2LEN:
        case REG_AUD2PER:   case REG_AUD2VOL:   case REG_AUD2DAT:   case REG_AUD3LCH:
        case REG_AUD3LCL:   case REG_AUD3LEN:   case REG_AUD3PER:   case REG_AUD3VOL:
        case REG_AUD3DAT:   case REG_BPL1PTH:   case REG_BPL1PTL:   case REG_BPL2PTH:
        case REG_BPL2PTL:   case REG_BPL3PTH:   case REG_BPL3PTL:   case REG_BPL4PTH:
        case REG_BPL4PTL:   case REG_BPL5PTH:   case REG_BPL5PTL:   case REG_BPL6PTH:
        case REG_BPL6PTL:   case REG_BPLCON0:   case REG_BPLCON1:   case REG_BPLCON2:
        case REG_BPL1MOD:   case REG_BPL2MOD:   case REG_BPL1DAT:   case REG_BPL2DAT:
        case REG_BPL3DAT:   case REG_BPL4DAT:   case REG_BPL5DAT:   case REG_BPL6DAT:
        case REG_SPR0PTH:   case REG_SPR0PTL:   case REG_SPR1PTH:   case REG_SPR1PTL:
        case REG_SPR2PTH:   case REG_SPR2PTL:   case REG_SPR3PTH:   case REG_SPR3PTL:
        case REG_SPR4PTH:   case REG_SPR4PTL:   case REG_SPR5PTH:   case REG_SPR5PTL:
        case REG_SPR6PTH:   case REG_SPR6PTL:   case REG_SPR7PTH:   case REG_SPR7PTL:
        case REG_SPR0POS:   case REG_SPR0CTL:   case REG_SPR0DATA:  case REG_SPR0DATB:
        case REG_SPR1POS:   case REG_SPR1CTL:   case REG_SPR1DATA:  case REG_SPR1DATB:
        case REG_SPR2POS:   case REG_SPR2CTL:   case REG_SPR2DATA:  case REG_SPR2DATB:
        case REG_SPR3POS:   case REG_SPR3CTL:   case REG_SPR3DATA:  case REG_SPR3DATB:
        case REG_SPR4POS:   case REG_SPR4CTL:   case REG_SPR4DATA:  case REG_SPR4DATB:
        case REG_SPR5POS:   case REG_SPR5CTL:   case REG_SPR5DATA:  case REG_SPR5DATB:
        case REG_SPR6POS:   case REG_SPR6CTL:   case REG_SPR6DATA:  case REG_SPR6DATB:
        case REG_SPR7POS:   case REG_SPR7CTL:   case REG_SPR7DATA:  case REG_SPR7DATB:
        case REG_COLOR00:   case REG_COLOR01:   case REG_COLOR02:   case REG_COLOR03:
        case REG_COLOR04:   case REG_COLOR05:   case REG_COLOR06:   case REG_COLOR07:
        case REG_COLOR08:   case REG_COLOR09:   case REG_COLOR10:   case REG_COLOR11:
        case REG_COLOR12:   case REG_COLOR13:   case REG_COLOR14:   case REG_COLOR15:
        case REG_COLOR16:   case REG_COLOR17:   case REG_COLOR18:   case REG_COLOR19:
        case REG_COLOR20:   case REG_COLOR21:   case REG_COLOR22:   case REG_COLOR23:
        case REG_COLOR24:   case REG_COLOR25:   case REG_COLOR26:   case REG_COLOR27:
        case REG_COLOR28:   case REG_COLOR29:   case REG_COLOR30:   case REG_COLOR31:
        case REG_NO_OP:

            return true;

        case REG_BLTCON0L:  case REG_BLTSIZV:   case REG_BLTSIZH:   case REG_SPRHDAT:
        case REG_BPLCON3:   case REG_HTOTAL:    case REG_HSSTOP:    case REG_HBSTRT:
        case REG_HBSTOP:    case REG_VTOTAL:    case REG_VSSTOP:    case REG_VBSTRT:
        case REG_VBSTOP:    case REG_BEAMCON0:  case REG_HSSTRT:    case REG_VSSTRT:
        case REG_HCENTER:

            return agnus.isECS();

        case REG_DENISEID:

            return denise.isECS();

        case REG_DIWHIGH:

            return agnus.isECS() || denise.isECS();

        default:

            return false;
    }
}

bool
Debugger::isUnused(ChipsetReg reg) const
{
    return !isReadable(reg) && !isWritable(reg);
}

u16
Debugger::readCs(ChipsetReg reg) const
{
    if (isUnused(reg)) throw VAError(ERROR_REG_UNUSED, ChipsetRegEnum::key(reg));
    if (isWritable(reg)) throw VAError(ERROR_REG_WRITE_ONLY, ChipsetRegEnum::key(reg));

    return mem.peekCustom16(u32(reg << 1));
}
void
Debugger::writeCs(ChipsetReg reg, u16 value)
{
    if (isUnused(reg)) throw VAError(ERROR_REG_UNUSED, ChipsetRegEnum::key(reg));
    if (isReadable(reg)) throw VAError(ERROR_REG_READ_ONLY, ChipsetRegEnum::key(reg));

    return mem.pokeCustom16<ACCESSOR_CPU>(u32(reg << 1), value);
}

void
Debugger::convertNumeric(std::ostream& os, isize value) const
{
    using namespace util;

    auto chr = [&](u8 v) { return std::isprint(v) ? (char)v : '.'; };

    os << std::setw(10) << std::right << std::setfill(' ') << dec(u32(value)) << " | ";
    os << hex(u32(value)) << " | ";
    os << bin(u32(value)) << " | ";
    os << "\"";
    os << chr(BYTE3(value)) << chr(BYTE2(value)) << chr(BYTE1(value)) << chr(BYTE0(value));
    os << "\"";
    os << std::endl;
}

void 
Debugger::convertNumeric(std::ostream& os, string s) const
{
    u8 bytes[4];

    bytes[0] = s.length() >= 4 ? (u8)s[s.length() - 1] : 0;
    bytes[1] = s.length() >= 3 ? (u8)s[s.length() - 2] : 0;
    bytes[2] = s.length() >= 2 ? (u8)s[s.length() - 3] : 0;
    bytes[3] = s.length() >= 1 ? (u8)s[s.length() - 4] : 0;

    convertNumeric(os, HI_HI_LO_LO(bytes[0], bytes[1], bytes[2], bytes[3]));
}

template const char *Debugger::ascDump <ACCESSOR_CPU> (u32, isize) const;
template const char *Debugger::ascDump <ACCESSOR_AGNUS> (u32, isize) const;
template const char *Debugger::hexDump <ACCESSOR_CPU> (u32, isize, isize) const;
template const char *Debugger::hexDump <ACCESSOR_AGNUS> (u32, isize, isize) const;
template const char *Debugger::memDump <ACCESSOR_CPU> (u32, isize, isize) const;
template const char *Debugger::memDump <ACCESSOR_AGNUS> (u32, isize, isize) const;
template void Debugger::ascDump <ACCESSOR_CPU> (std::ostream&, u32, isize);
template void Debugger::ascDump <ACCESSOR_AGNUS> (std::ostream&, u32, isize);
template void Debugger::hexDump <ACCESSOR_CPU> (std::ostream&, u32, isize, isize);
template void Debugger::hexDump <ACCESSOR_AGNUS> (std::ostream&, u32, isize, isize);
template void Debugger::memDump <ACCESSOR_CPU> (std::ostream&, u32, isize, isize);
template void Debugger::memDump <ACCESSOR_AGNUS> (std::ostream&, u32, isize, isize);
}
