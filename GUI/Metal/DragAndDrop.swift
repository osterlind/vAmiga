// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

// import AppKit

extension NSPasteboard.PasteboardType {
    static let compatibleFileURL = NSPasteboard.PasteboardType(kUTTypeFileURL as String)
}

public extension MetalView {
    
    // Returns a list of supported drag and drop types
    func acceptedTypes() -> [NSPasteboard.PasteboardType] {
        
        return [.compatibleFileURL, .string, .fileContents]
    }
    
    // Registers the supported drag and drop types
    func setupDragAndDrop() {
    
        registerForDraggedTypes(acceptedTypes())
    }

    override func draggingEntered(_ sender: NSDraggingInfo) -> NSDragOperation {
        
        let pasteBoard = sender.draggingPasteboard
        guard let type = pasteBoard.availableType(from: acceptedTypes()) else {
            return NSDragOperation()
        }
        
        switch type {
            
        case .string:
            return NSDragOperation.copy
        
        case .fileContents:
            return NSDragOperation.copy
            
        case .compatibleFileURL:
            if let url = NSURL.init(from: pasteBoard) as URL? {
            
                // Unpack the file if it is compressed
                draggedUrl = url.unpacked(maxSize: 2048 * 1024)

                // Analyze the file type
                let type = AmigaFileProxy.type(of: draggedUrl)
                
                // Open the drop zone layer
                parent.renderer.dropZone.open(type: type, delay: 0.25)
            }

            return NSDragOperation.copy
            
        default:
            return NSDragOperation()
        }
    }
    
    override func draggingUpdated(_ sender: NSDraggingInfo) -> NSDragOperation {
        
        parent.renderer.dropZone.draggingUpdated(sender)
        return NSDragOperation.copy
    }

    override func draggingExited(_ sender: NSDraggingInfo?) {
    
        parent.renderer.dropZone.close(delay: 0.25)
    }
    
    override func prepareForDragOperation(_ sender: NSDraggingInfo) -> Bool {
        
        parent.renderer.dropZone.close(delay: 0.25)
        return true
    }
    
    override func performDragOperation(_ sender: NSDraggingInfo) -> Bool {
        
        let pasteBoard = sender.draggingPasteboard
        
        guard
            let type = pasteBoard.availableType(from: acceptedTypes()),
            let document = parent.mydocument
        else { return false }
        
        switch type {
            
        case .string:
            
            // Type text on virtual keyboard
            guard let text = pasteBoard.string(forType: .string) else {
                return false
            }
            parent.keyboard.autoTypeAsync(text)
            return true
            
        case .fileContents:
            
            // Check if we got another virtual machine dragged in
            let fileWrapper = pasteBoard.readFileWrapper()
            let fileData = fileWrapper?.regularFileContents
            let length = fileData!.count
            let nsData = fileData! as NSData
            let rawPtr = nsData.bytes
            
            let snapshot: SnapshotProxy? = try? Proxy.make(buffer: rawPtr, length: length)
            if snapshot == nil { return false }
            
            if document.proceedWithUnexportedDisk() {
                DispatchQueue.main.async {
                    try? self.parent.amiga.loadSnapshot(snapshot!)
                }
                return true
            }
            
        case .compatibleFileURL:
            
            if let url = draggedUrl {
                
                // Check if the file is a snapshot or a script
                do {
                    let types: [FileType] = [ .SNAPSHOT, .SCRIPT, .HDF ]
                    try document.createAttachment(from: url, allowedTypes: types)
                    try document.mountAttachment()
                    return true
                } catch { }

                do {
                    // Check drop zones
                    for i in 0...3 {
                        if parent.renderer.dropZone.isInside(sender, zone: i) {
                            
                            let types: [FileType] = [ .ADF, .EXT, .IMG, .DMS, .EXE, .DIR ]
                            try document.createAttachment(from: url, allowedTypes: types)
                            try document.mountAttachment(drive: i)
                            return true
                        }
                    }

                    // Create attachment
                    try document.createAttachment(from: url)
                    
                    // Run the import dialog
                    document.runImportDialog()
                    return true
                    
                } catch {
                    (error as? VAError)?.cantOpen(url: url, async: true)
                }
            }
                        
        default:
            break
        }
        
        return false
    }
    
    override func concludeDragOperation(_ sender: NSDraggingInfo?) {
    }
}
