// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

let fmt8  = MyFormatter.init(radix: 16, min: 0, max: 0xFF)
let fmt16 = MyFormatter.init(radix: 16, min: 0, max: 0xFFFF)
let fmt24 = MyFormatter.init(radix: 16, min: 0, max: 0xFFFFFF)
let fmt32 = MyFormatter.init(radix: 16, min: 0, max: 0xFFFFFFFF)

class Inspector: NSWindowController {

    // Debug panel (Commons)
    @IBOutlet weak var debugPanel: NSTabView!
    
    // Debug panel (CPU)
    @IBOutlet weak var instrTableView: InstrTableView!
    @IBOutlet weak var traceTableView: TraceTableView!
    @IBOutlet weak var breakTableView: BreakTableView!
    @IBOutlet weak var watchTableView: WatchTableView!
    @IBOutlet weak var cpuPC: NSTextField!
    @IBOutlet weak var cpuD0: NSTextField!
    @IBOutlet weak var cpuD1: NSTextField!
    @IBOutlet weak var cpuD2: NSTextField!
    @IBOutlet weak var cpuD3: NSTextField!
    @IBOutlet weak var cpuD4: NSTextField!
    @IBOutlet weak var cpuD5: NSTextField!
    @IBOutlet weak var cpuD6: NSTextField!
    @IBOutlet weak var cpuD7: NSTextField!
    @IBOutlet weak var cpuA0: NSTextField!
    @IBOutlet weak var cpuA1: NSTextField!
    @IBOutlet weak var cpuA2: NSTextField!
    @IBOutlet weak var cpuA3: NSTextField!
    @IBOutlet weak var cpuA4: NSTextField!
    @IBOutlet weak var cpuA5: NSTextField!
    @IBOutlet weak var cpuA6: NSTextField!
    @IBOutlet weak var cpuA7: NSTextField!
    @IBOutlet weak var cpuUSP: NSTextField!
    @IBOutlet weak var cpuSSP: NSTextField!

    @IBOutlet weak var cpuT: NSButton!
    @IBOutlet weak var cpuS: NSButton!
    @IBOutlet weak var cpuI2: NSButton!
    @IBOutlet weak var cpuI1: NSButton!
    @IBOutlet weak var cpuI0: NSButton!
    @IBOutlet weak var cpuX: NSButton!
    @IBOutlet weak var cpuN: NSButton!
    @IBOutlet weak var cpuZ: NSButton!
    @IBOutlet weak var cpuV: NSButton!
    @IBOutlet weak var cpuC: NSButton!

    @IBOutlet weak var cpuStopAndGoButton: NSButton!
    @IBOutlet weak var cpuStepIntoButton: NSButton!
    @IBOutlet weak var cpuStepOverButton: NSButton!
    @IBOutlet weak var cpuTraceStopAndGoButton: NSButton!
    @IBOutlet weak var cpuTraceStepIntoButton: NSButton!
    @IBOutlet weak var cpuTraceStepOverButton: NSButton!
    @IBOutlet weak var cpuTraceClearButton: NSButton!

    // Debug panel (Memory)
    @IBOutlet weak var memSearchField: NSSearchField!
    @IBOutlet weak var memBankTableView: BankTableView!
    @IBOutlet weak var memTableView: MemTableView!
    @IBOutlet weak var memLayoutButton: NSButton!
    @IBOutlet weak var memLayoutSlider: NSSlider!
    @IBOutlet weak var memChipRamButton: NSButton!
    @IBOutlet weak var memChipRamText: NSTextField!
    @IBOutlet weak var memSlowRamButton: NSButton!
    @IBOutlet weak var memSlowRamText: NSTextField!
    @IBOutlet weak var memFastRamButton: NSButton!
    @IBOutlet weak var memFastRamText: NSTextField!
    @IBOutlet weak var memRomButton: NSButton!
    @IBOutlet weak var memRomText: NSTextField!
    @IBOutlet weak var memWomButton: NSButton!
    @IBOutlet weak var memWomText: NSTextField!
    @IBOutlet weak var memExtButton: NSButton!
    @IBOutlet weak var memExtText: NSTextField!
    @IBOutlet weak var memCIAButton: NSButton!
    @IBOutlet weak var memRTCButton: NSButton!
    @IBOutlet weak var memOCSButton: NSButton!
    @IBOutlet weak var memAutoConfButton: NSButton!

    var bank = 0
    var memSrc = MEM_CHIP
    var selected = -1

    // Counter to avoid Moire effects in state machine images
    var state0 = 0
    var state1 = 0
    var state2 = 0
    var state3 = 0

    // Debug panel (CIA)
    @IBOutlet weak var ciaSelector: NSSegmentedControl!
    
    @IBOutlet weak var ciaPRA: NSTextField!
    @IBOutlet weak var ciaPRAbinary: NSTextField!
    @IBOutlet weak var ciaDDRA: NSTextField!
    @IBOutlet weak var ciaDDRAbinary: NSTextField!
    @IBOutlet weak var ciaPA7: NSButton!
    @IBOutlet weak var ciaPA6: NSButton!
    @IBOutlet weak var ciaPA5: NSButton!
    @IBOutlet weak var ciaPA4: NSButton!
    @IBOutlet weak var ciaPA3: NSButton!
    @IBOutlet weak var ciaPA2: NSButton!
    @IBOutlet weak var ciaPA1: NSButton!
    @IBOutlet weak var ciaPA0: NSButton!

    @IBOutlet weak var ciaPRB: NSTextField!
    @IBOutlet weak var ciaPRBbinary: NSTextField!
    @IBOutlet weak var ciaDDRB: NSTextField!
    @IBOutlet weak var ciaDDRBbinary: NSTextField!
    @IBOutlet weak var ciaPB7: NSButton!
    @IBOutlet weak var ciaPB6: NSButton!
    @IBOutlet weak var ciaPB5: NSButton!
    @IBOutlet weak var ciaPB4: NSButton!
    @IBOutlet weak var ciaPB3: NSButton!
    @IBOutlet weak var ciaPB2: NSButton!
    @IBOutlet weak var ciaPB1: NSButton!
    @IBOutlet weak var ciaPB0: NSButton!
    
    @IBOutlet weak var ciaTA: NSTextField!
    @IBOutlet weak var ciaTAlatch: NSTextField!
    @IBOutlet weak var ciaTArunning: NSButton!
    @IBOutlet weak var ciaTAtoggle: NSButton!
    @IBOutlet weak var ciaTApbout: NSButton!
    @IBOutlet weak var ciaTAoneShot: NSButton!
    
    @IBOutlet weak var ciaTB: NSTextField!
    @IBOutlet weak var ciaTBlatch: NSTextField!
    @IBOutlet weak var ciaTBrunning: NSButton!
    @IBOutlet weak var ciaTBtoggle: NSButton!
    @IBOutlet weak var ciaTBpbout: NSButton!
    @IBOutlet weak var ciaTBoneShot: NSButton!
    
    @IBOutlet weak var ciaICR: NSTextField!
    @IBOutlet weak var ciaICRbinary: NSTextField!
    @IBOutlet weak var ciaIMR: NSTextField!
    @IBOutlet weak var ciaIMRbinary: NSTextField!
    @IBOutlet weak var ciaIntLineLow: NSButton!

    @IBOutlet weak var ciaCntHi: NSTextField!
    @IBOutlet weak var ciaCntMid: NSTextField!
    @IBOutlet weak var ciaCntLo: NSTextField!
    @IBOutlet weak var ciaCntIntEnable: NSButton!
    @IBOutlet weak var ciaAlarmHi: NSTextField!
    @IBOutlet weak var ciaAlarmMid: NSTextField!
    @IBOutlet weak var ciaAlarmLo: NSTextField!

    @IBOutlet weak var ciaSDR: NSTextField!
    @IBOutlet weak var ciaSDRbinary: NSTextField!
    
    @IBOutlet weak var ciaIdleCycles: NSTextField!
    @IBOutlet weak var ciaIdleLevelText: NSTextField!
    @IBOutlet weak var ciaIdleLevel: NSLevelIndicator!
    
    // Debug panel (Agnus)
    @IBOutlet weak var dmaVPOS: NSTextField!
    @IBOutlet weak var dmaHPOS: NSTextField!

    @IBOutlet weak var dmaDMACON: NSTextField!
    @IBOutlet weak var dmaBPL0CON: NSTextField!
    @IBOutlet weak var dmaDDFSTRT: NSTextField!
    @IBOutlet weak var dmaDDFSTOP: NSTextField!
    @IBOutlet weak var dmaDIWSTRT: NSTextField!
    @IBOutlet weak var dmaDIWSTOP: NSTextField!

    @IBOutlet weak var dmaBLTAMOD: NSTextField!
    @IBOutlet weak var dmaBLTBMOD: NSTextField!
    @IBOutlet weak var dmaBLTCMOD: NSTextField!
    @IBOutlet weak var dmaBLTDMOD: NSTextField!
    @IBOutlet weak var dmaBPL1MOD: NSTextField!
    @IBOutlet weak var dmaBPL2MOD: NSTextField!

    @IBOutlet weak var dmaBPL1PT: NSTextField!
    @IBOutlet weak var dmaBPL2PT: NSTextField!
    @IBOutlet weak var dmaBPL3PT: NSTextField!
    @IBOutlet weak var dmaBPL4PT: NSTextField!
    @IBOutlet weak var dmaBPL5PT: NSTextField!
    @IBOutlet weak var dmaBPL6PT: NSTextField!
    @IBOutlet weak var dmaBPL1Enable: NSButton!
    @IBOutlet weak var dmaBPL2Enable: NSButton!
    @IBOutlet weak var dmaBPL3Enable: NSButton!
    @IBOutlet weak var dmaBPL4Enable: NSButton!
    @IBOutlet weak var dmaBPL5Enable: NSButton!
    @IBOutlet weak var dmaBPL6Enable: NSButton!

    @IBOutlet weak var dmaAUD0PT: NSTextField!
    @IBOutlet weak var dmaAUD1PT: NSTextField!
    @IBOutlet weak var dmaAUD2PT: NSTextField!
    @IBOutlet weak var dmaAUD3PT: NSTextField!
    @IBOutlet weak var dmaAUD0Enable: NSButton!
    @IBOutlet weak var dmaAUD1Enable: NSButton!
    @IBOutlet weak var dmaAUD2Enable: NSButton!
    @IBOutlet weak var dmaAUD3Enable: NSButton!

    @IBOutlet weak var dmaBLTAPT: NSTextField!
    @IBOutlet weak var dmaBLTBPT: NSTextField!
    @IBOutlet weak var dmaBLTCPT: NSTextField!
    @IBOutlet weak var dmaBLTDPT: NSTextField!
    @IBOutlet weak var dmaBLTAEnable: NSButton!
    @IBOutlet weak var dmaBLTBEnable: NSButton!
    @IBOutlet weak var dmaBLTCEnable: NSButton!
    @IBOutlet weak var dmaBLTDEnable: NSButton!
    @IBOutlet weak var dmaBLTPRI: NSButton!
    @IBOutlet weak var dmaBLS: NSButton!

    @IBOutlet weak var dmaCOPPC: NSTextField!
    @IBOutlet weak var dmaCOPEnable: NSButton!

    @IBOutlet weak var dmaSPR0PT: NSTextField!
    @IBOutlet weak var dmaSPR1PT: NSTextField!
    @IBOutlet weak var dmaSPR2PT: NSTextField!
    @IBOutlet weak var dmaSPR3PT: NSTextField!
    @IBOutlet weak var dmaSPR4PT: NSTextField!
    @IBOutlet weak var dmaSPR5PT: NSTextField!
    @IBOutlet weak var dmaSPR6PT: NSTextField!
    @IBOutlet weak var dmaSPR7PT: NSTextField!
    @IBOutlet weak var dmaSPR0Enable: NSButton!
    @IBOutlet weak var dmaSPR1Enable: NSButton!
    @IBOutlet weak var dmaSPR2Enable: NSButton!
    @IBOutlet weak var dmaSPR3Enable: NSButton!
    @IBOutlet weak var dmaSPR4Enable: NSButton!
    @IBOutlet weak var dmaSPR5Enable: NSButton!
    @IBOutlet weak var dmaSPR6Enable: NSButton!
    @IBOutlet weak var dmaSPR7Enable: NSButton!

    @IBOutlet weak var dmaDSKPT: NSTextField!
    @IBOutlet weak var dmaDSKEnable: NSButton!
    
    // Debug panel (Copper)
    @IBOutlet weak var copActive: NSButton!
    @IBOutlet weak var copCOPPC: NSTextField!
    @IBOutlet weak var copCOPINS1: NSTextField!
    @IBOutlet weak var copCOPINS2: NSTextField!
    @IBOutlet weak var copCDANG: NSButton!

    @IBOutlet weak var copCOP1LC: NSTextField!
    @IBOutlet weak var copPlus1: NSButton!
    @IBOutlet weak var copMinus1: NSButton!
    @IBOutlet weak var copList1: CopperTableView!

    @IBOutlet weak var copCOP2LC: NSTextField!
    @IBOutlet weak var copPlus2: NSButton!
    @IBOutlet weak var copMinus2: NSButton!
    @IBOutlet weak var copList2: CopperTableView!

    // Debug panel (Denise)
    @IBOutlet weak var deniseBPLCON0: NSTextField!
    @IBOutlet weak var deniseHIRES: NSButton!
    @IBOutlet weak var deniseBPU: NSTextField!
    @IBOutlet weak var deniseHOMOD: NSButton!
    @IBOutlet weak var deniseDBPLF: NSButton!
    @IBOutlet weak var deniseLACE: NSButton!

    @IBOutlet weak var deniseBPLCON1: NSTextField!
    @IBOutlet weak var deniseP1H: NSTextField!
    @IBOutlet weak var deniseP2H: NSTextField!

    @IBOutlet weak var deniseBPLCON2: NSTextField!
    @IBOutlet weak var denisePF2PRI: NSButton!
    @IBOutlet weak var denisePF2P2: NSButton!
    @IBOutlet weak var denisePF2P1: NSButton!
    @IBOutlet weak var denisePF2P0: NSButton!
    @IBOutlet weak var denisePF1P2: NSButton!
    @IBOutlet weak var denisePF1P1: NSButton!
    @IBOutlet weak var denisePF1P0: NSButton!

    @IBOutlet weak var deniseDIWSTRT: NSTextField!
    @IBOutlet weak var deniseDIWSTRTText: NSTextField!
    @IBOutlet weak var deniseDIWSTOP: NSTextField!
    @IBOutlet weak var deniseDIWSTOPText: NSTextField!

    @IBOutlet weak var deniseCLXDAT: NSTextField!

    @IBOutlet weak var deniseCol0: NSColorWell!
    @IBOutlet weak var deniseCol1: NSColorWell!
    @IBOutlet weak var deniseCol2: NSColorWell!
    @IBOutlet weak var deniseCol3: NSColorWell!
    @IBOutlet weak var deniseCol4: NSColorWell!
    @IBOutlet weak var deniseCol5: NSColorWell!
    @IBOutlet weak var deniseCol6: NSColorWell!
    @IBOutlet weak var deniseCol7: NSColorWell!
    @IBOutlet weak var deniseCol8: NSColorWell!
    @IBOutlet weak var deniseCol9: NSColorWell!
    @IBOutlet weak var deniseCol10: NSColorWell!
    @IBOutlet weak var deniseCol11: NSColorWell!
    @IBOutlet weak var deniseCol12: NSColorWell!
    @IBOutlet weak var deniseCol13: NSColorWell!
    @IBOutlet weak var deniseCol14: NSColorWell!
    @IBOutlet weak var deniseCol15: NSColorWell!
    @IBOutlet weak var deniseCol16: NSColorWell!
    @IBOutlet weak var deniseCol17: NSColorWell!
    @IBOutlet weak var deniseCol18: NSColorWell!
    @IBOutlet weak var deniseCol19: NSColorWell!
    @IBOutlet weak var deniseCol20: NSColorWell!
    @IBOutlet weak var deniseCol21: NSColorWell!
    @IBOutlet weak var deniseCol22: NSColorWell!
    @IBOutlet weak var deniseCol23: NSColorWell!
    @IBOutlet weak var deniseCol24: NSColorWell!
    @IBOutlet weak var deniseCol25: NSColorWell!
    @IBOutlet weak var deniseCol26: NSColorWell!
    @IBOutlet weak var deniseCol27: NSColorWell!
    @IBOutlet weak var deniseCol28: NSColorWell!
    @IBOutlet weak var deniseCol29: NSColorWell!
    @IBOutlet weak var deniseCol30: NSColorWell!
    @IBOutlet weak var deniseCol31: NSColorWell!

    @IBOutlet weak var sprSelector: NSSegmentedControl!
    @IBOutlet weak var sprVStart: NSTextField!
    @IBOutlet weak var sprVStop: NSTextField!
    @IBOutlet weak var sprHStart: NSTextField!
    @IBOutlet weak var sprPtr: NSTextField!
    @IBOutlet weak var sprTableView: SpriteTableView!
    @IBOutlet weak var sprAttach: NSButton!

    // Debug panel (Paula)
    @IBOutlet weak var paulaIntena: NSTextField!
    @IBOutlet weak var paulaIntreq: NSTextField!
    @IBOutlet weak var paulaEna14: NSButton!
    @IBOutlet weak var paulaEna13: NSButton!
    @IBOutlet weak var paulaEna12: NSButton!
    @IBOutlet weak var paulaEna11: NSButton!
    @IBOutlet weak var paulaEna10: NSButton!
    @IBOutlet weak var paulaEna9: NSButton!
    @IBOutlet weak var paulaEna8: NSButton!
    @IBOutlet weak var paulaEna7: NSButton!
    @IBOutlet weak var paulaEna6: NSButton!
    @IBOutlet weak var paulaEna5: NSButton!
    @IBOutlet weak var paulaEna4: NSButton!
    @IBOutlet weak var paulaEna3: NSButton!
    @IBOutlet weak var paulaEna2: NSButton!
    @IBOutlet weak var paulaEna1: NSButton!
    @IBOutlet weak var paulaEna0: NSButton!
    @IBOutlet weak var paulaReq14: NSButton!
    @IBOutlet weak var paulaReq13: NSButton!
    @IBOutlet weak var paulaReq12: NSButton!
    @IBOutlet weak var paulaReq11: NSButton!
    @IBOutlet weak var paulaReq10: NSButton!
    @IBOutlet weak var paulaReq9: NSButton!
    @IBOutlet weak var paulaReq8: NSButton!
    @IBOutlet weak var paulaReq7: NSButton!
    @IBOutlet weak var paulaReq6: NSButton!
    @IBOutlet weak var paulaReq5: NSButton!
    @IBOutlet weak var paulaReq4: NSButton!
    @IBOutlet weak var paulaReq3: NSButton!
    @IBOutlet weak var paulaReq2: NSButton!
    @IBOutlet weak var paulaReq1: NSButton!
    @IBOutlet weak var paulaReq0: NSButton!

    @IBOutlet weak var audioLen0: NSTextField!
    @IBOutlet weak var audioPer0: NSTextField!
    @IBOutlet weak var audioVol0: NSTextField!
    @IBOutlet weak var audioDat0: NSTextField!
    @IBOutlet weak var audioImg0: NSButton!
    @IBOutlet weak var audioLen1: NSTextField!
    @IBOutlet weak var audioPer1: NSTextField!
    @IBOutlet weak var audioVol1: NSTextField!
    @IBOutlet weak var audioDat1: NSTextField!
    @IBOutlet weak var audioImg1: NSButton!
    @IBOutlet weak var audioLen2: NSTextField!
    @IBOutlet weak var audioPer2: NSTextField!
    @IBOutlet weak var audioVol2: NSTextField!
    @IBOutlet weak var audioDat2: NSTextField!
    @IBOutlet weak var audioImg2: NSButton!
    @IBOutlet weak var audioLen3: NSTextField!
    @IBOutlet weak var audioPer3: NSTextField!
    @IBOutlet weak var audioVol3: NSTextField!
    @IBOutlet weak var audioDat3: NSTextField!
    @IBOutlet weak var audioImg3: NSButton!

    @IBOutlet weak var dskStateText: NSTextField!
    @IBOutlet weak var dskSelectDf0: NSButton!
    @IBOutlet weak var dskSelectDf1: NSButton!
    @IBOutlet weak var dskSelectDf2: NSButton!
    @IBOutlet weak var dskSelectDf3: NSButton!
    @IBOutlet weak var dskDsklen: NSTextField!
    @IBOutlet weak var dskDmaen: NSButton!
    @IBOutlet weak var dskWrite: NSButton!
    @IBOutlet weak var dskDskbytr: NSTextField!
    @IBOutlet weak var dskByteready: NSButton!
    @IBOutlet weak var dskDmaon: NSButton!
    @IBOutlet weak var dskDiskwrite: NSButton!
    @IBOutlet weak var dskWordequal: NSButton!
    @IBOutlet weak var dskAdkconHi: NSTextField!
    @IBOutlet weak var dskPrecomp1: NSButton!
    @IBOutlet weak var dskPrecomp0: NSButton!
    @IBOutlet weak var dskMfmprec: NSButton!
    @IBOutlet weak var dskUartbrk: NSButton!
    @IBOutlet weak var dskWordsync: NSButton!
    @IBOutlet weak var dskMsbsync: NSButton!
    @IBOutlet weak var dskFast: NSButton!
    @IBOutlet weak var dskDsksync: NSTextField!
    @IBOutlet weak var dskFifo0: NSTextField!
    @IBOutlet weak var dskFifo1: NSTextField!
    @IBOutlet weak var dskFifo2: NSTextField!
    @IBOutlet weak var dskFifo3: NSTextField!
    @IBOutlet weak var dskFifo4: NSTextField!
    @IBOutlet weak var dskFifo5: NSTextField!

    // Debug Panel (Ports)
    @IBOutlet weak var po0JOYDAT: NSTextField!
    @IBOutlet weak var po0M0V: NSButton!
    @IBOutlet weak var po0M0H: NSButton!
    @IBOutlet weak var po0M1V: NSButton!
    @IBOutlet weak var po0M1H: NSButton!
    @IBOutlet weak var po0POTDAT: NSTextField!

    @IBOutlet weak var po1JOYDAT: NSTextField!
    @IBOutlet weak var po1M0V: NSButton!
    @IBOutlet weak var po1M0H: NSButton!
    @IBOutlet weak var po1M1V: NSButton!
    @IBOutlet weak var po1M1H: NSButton!
    @IBOutlet weak var po1POTDAT: NSTextField!

    @IBOutlet weak var poPOTGO: NSTextField!
    @IBOutlet weak var poOUTRY: NSButton!
    @IBOutlet weak var poDATRY: NSButton!
    @IBOutlet weak var poOUTRX: NSButton!
    @IBOutlet weak var poDATRX: NSButton!
    @IBOutlet weak var poOUTLY: NSButton!
    @IBOutlet weak var poDATLY: NSButton!
    @IBOutlet weak var poOUTLX: NSButton!
    @IBOutlet weak var poDATLX: NSButton!

    @IBOutlet weak var poPOTGOR: NSTextField!
    @IBOutlet weak var poDATRYR: NSButton!
    @IBOutlet weak var poDATRXR: NSButton!
    @IBOutlet weak var poDATLYR: NSButton!
    @IBOutlet weak var poDATLXR: NSButton!

    @IBOutlet weak var poTXD: NSButton!
    @IBOutlet weak var poRXD: NSButton!
    @IBOutlet weak var poCTS: NSButton!
    @IBOutlet weak var poDSR: NSButton!
    @IBOutlet weak var poCD: NSButton!
    @IBOutlet weak var poDTR: NSButton!

    @IBOutlet weak var poRecShift: NSTextField!
    @IBOutlet weak var poRecBuffer: NSTextField!
    @IBOutlet weak var poTransShift: NSTextField!
    @IBOutlet weak var poTransBuffer: NSTextField!

    @IBOutlet weak var poSerialIn: NSTextView!
    @IBOutlet weak var poSerialOut: NSTextView!

    // Debug Panel (Events)
    @IBOutlet weak var evCpuProgress: NSTextField!
    @IBOutlet weak var evCpuProgress2: NSTextField!
    @IBOutlet weak var evDmaProgress: NSTextField!
    @IBOutlet weak var evDmaProgress2: NSTextField!
    @IBOutlet weak var evCiaAProgress: NSTextField!
    @IBOutlet weak var evCiaAProgress2: NSTextField!
    @IBOutlet weak var evCiaBProgress: NSTextField!
    @IBOutlet weak var evCiaBProgress2: NSTextField!

    @IBOutlet weak var evTableView: EventTableView!

    // Cached state of all Amiga components
    var cpuInfo: CPUInfo?
    var ciaInfo: CIAInfo?
    var agnusInfo: AgnusInfo?
    var copperInfo: CopperInfo?
    var deniseInfo: DeniseInfo?
    var spriteInfo: SpriteInfo?
    var paulaInfo: PaulaInfo?
    var audioInfo: AudioInfo?
    var diskInfo: DiskControllerInfo?
    var port1Info: ControlPortInfo?
    var port2Info: ControlPortInfo?
    var serInfo: SerialPortInfo?
    var uartInfo: UARTInfo?
    var eventInfo: EventInfo?
    var isRunning = true

    // The document this inspector is bound to
    var parent: MyController?

    // Timer for triggering continous update
    var timer: Timer?

    // Used to determine the items that should be refreshed
    var refreshCnt = 0

    // Returns the number of the currently inspected sprite
    var selectedSprite: Int { return sprSelector.indexOfSelectedItem }

    // Factory method
    static func make(parent: MyController) -> Inspector? {

        let inspector = Inspector.init(windowNibName: "Inspector")
        inspector.parent = parent
        return inspector
    }
    
    override func awakeFromNib() {
        
        track()
        
        // Create and assign binary number formatter
        let bF = MyFormatter.init(radix: 2, min: 0, max: 255)
        ciaPRAbinary.formatter = bF
        ciaDDRAbinary.formatter = bF
        ciaPRBbinary.formatter = bF
        ciaDDRBbinary.formatter = bF
        ciaSDRbinary.formatter = bF
        ciaICRbinary.formatter = bF
        ciaIMRbinary.formatter = bF
    }
    
    override func showWindow(_ sender: Any?) {

        track()

        lockAmiga()

        super.showWindow(self)
        amiga?.enableDebugging()
        updateInspectionTarget()
        
        timer = Timer.scheduledTimer(withTimeInterval: inspectionInterval, repeats: true) { _ in

            lockAmiga()
            if amiga != nil {
                if self.isRunning || self.refreshCnt == 0 {
                    self.refresh(count: self.refreshCnt)
                }
                self.isRunning = amiga!.isRunning()
                self.refreshCnt += 1
            }
            unlockAmiga()
        }

        unlockAmiga()
    }

    // Assigns a number formatter to a control
    func assignFormatter(_ formatter: Formatter, _ control: NSControl) {
        
        control.abortEditing()
        control.formatter = formatter
        control.needsDisplay = true
    }

    func needsRefresh() {

        // We force a refresh by setting the refresh counter to 0. This causes
        // all refresh methods to be invoked inside the timer handler.
        refreshCnt = 0
    }

    func refresh(count: Int) {
        
        if window?.isVisible == false { return }

        if let id = debugPanel.selectedTabViewItem?.label {

            switch id {

            case "CPU": refreshCPU(count: count)
            case "CIA": refreshCIA(count: count)
            case "Memory": refreshMemory(count: count)
            case "Agnus": refreshAgnus(count: count)
            case "Copper": refreshCopper(count: count)
            case "Denise": refreshDenise(count: count)
            case "Paula": refreshPaula(count: count)
            case "Ports": refreshPorts(count: count)
            case "Events": refreshEvents(count: count)
            default: break
            }
        }
    }
}

extension Inspector: NSWindowDelegate {
    
    func windowWillClose(_ notification: Notification) {

        lockAmiga()

        track("Closing inspector")
        timer?.invalidate()
        amiga?.disableDebugging()
        amiga?.clearInspectionTarget()

        unlockAmiga()
    }
}

extension Inspector: NSTabViewDelegate {

    func updateInspectionTarget() {

        if let id = debugPanel.selectedTabViewItem?.label {

            switch id {

            case "CPU":    parent?.amiga.setInspectionTarget(INS_CPU)
            case "CIA":    parent?.amiga.setInspectionTarget(INS_CIA)
            case "Memory": parent?.amiga.setInspectionTarget(INS_MEM)
            case "Agnus":  parent?.amiga.setInspectionTarget(INS_AGNUS)
            case "Copper": parent?.amiga.setInspectionTarget(INS_AGNUS)
            case "Denise": parent?.amiga.setInspectionTarget(INS_DENISE)
            case "Paula":  parent?.amiga.setInspectionTarget(INS_PAULA)
            case "Ports":  parent?.amiga.setInspectionTarget(INS_PORTS)
            case "Events": parent?.amiga.setInspectionTarget(INS_EVENTS)
            default:       break
            }

            needsRefresh()
        }
    }

    func tabView(_ tabView: NSTabView, didSelect tabViewItem: NSTabViewItem?) {

        lockAmiga()
        updateInspectionTarget()
        unlockAmiga()
    }
}
