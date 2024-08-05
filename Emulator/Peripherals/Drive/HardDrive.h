// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the Mozilla Public License v2
//
// See https://mozilla.org/MPL/2.0 for license information
// -----------------------------------------------------------------------------

#pragma once

#include "HardDriveTypes.h"
#include "Drive.h"
#include "AgnusTypes.h"
#include "HdControllerTypes.h"
#include "HDFFile.h"
#include "MemUtils.h"

namespace vamiga {

class HardDrive : public Drive, public Inspectable<HardDriveInfo> {
    
    Descriptions descriptions = {
        {
            .name           = "HardDrive0",
            .description    = "Hard Drive 0",
            .shell          = "hd0"
        },
        {
            .name           = "HardDrive1",
            .description    = "Hard Drive 1",
            .shell          = "hd1"
        },
        {
            .name           = "HardDrive2",
            .description    = "Hard Drive 2",
            .shell          = "hd2"
        },
        {
            .name           = "HardDrive3",
            .description    = "Hard Drive 3",
            .shell          = "hd3"
        }
    };

    ConfigOptions options = {

        OPT_HDR_TYPE,
        OPT_HDR_WRITE_THROUGH, 
        OPT_HDR_PAN,
        OPT_HDR_STEP_VOLUME
    };
    
    friend class HDFFile;
    friend class HdController;

    // Write-through storage files
    static std::fstream wtStream[4];
    
    // Current configuration
    HardDriveConfig config = {};
    
    // Product information
    string diskVendor;
    string diskProduct;
    string diskRevision;
    string controllerVendor;
    string controllerProduct;
    string controllerRevision;

    // Hard disk geometry
    GeometryDescriptor geometry;
    
    // Partition table
    std::vector <PartitionDescriptor> ptable;

    // Loadable file system drivers
    std::vector <DriverDescriptor> drivers;

    // Disk data
    Buffer<u8> data;
    
    // Keeps track of modified blocks (to update the run-ahead instance)
    Buffer<bool> dirty;

    // Current position of the read/write head
    DriveHead head;

    // Current drive state
    HardDriveState state = HDR_IDLE;
    
    // Disk state flags
    DiskFlags flags = 0;
    optional <bool> bootable;

    
    //
    // Initializing
    //

public:

    HardDrive(Amiga& ref, isize nr);
    ~HardDrive();
    
    HardDrive& operator= (const HardDrive& other);

    // Creates a hard drive with a certain geometry
    void init(const GeometryDescriptor &geometry);

    // Creates a hard drive with a certain capacity in bytes
    void init(isize size);

    // Creates a hard drive with the contents of a file system
    void init(const MutableFileSystem &fs) throws;

    // Creates a hard drive with the contents of a media file
    void init(const MediaFile &file) throws;

    // Creates a hard drive with the contents of an HDF
    void init(const HDFFile &hdf) throws;

    // Creates a hard drive with the contents of an HDF file
    void init(const std::filesystem::path &path) throws;

    const HardDriveTraits &getTraits() const {

        static HardDriveTraits traits;

        traits.nr = objid;
        
        traits.diskVendor = diskVendor.c_str();
        traits.diskProduct = diskProduct.c_str();
        traits.diskRevision = diskRevision.c_str();
        traits.controllerVendor = controllerVendor.c_str();
        traits.controllerProduct = controllerProduct.c_str();
        traits.controllerRevision = controllerRevision.c_str();

        traits.cylinders = geometry.cylinders;
        traits.heads = geometry.heads;
        traits.sectors = geometry.sectors;
        traits.bsize = geometry.bsize;

        traits.tracks = geometry.numTracks();
        traits.blocks = geometry.numBlocks();
        traits.bytes = geometry.numBytes();
        traits.upperCyl = geometry.upperCyl();
        traits.upperHead = geometry.upperHead();
        traits.upperTrack = geometry.upperTrack();

        return traits;
    }

    const PartitionTraits &getPartitionTraits(isize nr) const {

        static PartitionTraits traits;

        auto descr = getPartitionDescriptor(nr);
        traits.nr = nr;
        traits.name = descr.name.c_str();
        traits.lowerCyl = descr.lowCyl;
        traits.upperCyl = descr.highCyl;

        return traits;
    }

private:

    // Restors the initial state
    void init();

    
    //
    // Methods from CoreObject
    //
    
private:
    
    void _dump(Category category, std::ostream& os) const override;
    
    
    //
    // Methods from CoreComponent
    //
    
private:
    
    void _initialize() override;
    
    template <class T>
    void serialize(T& worker)
    {
        if (isSoftResetter(worker)) return;

        worker

        << head.cylinder
        << head.head
        << head.offset
        << state;

        if (isResetter(worker)) return;

        worker

        << config.type
        << config.pan
        << config.stepVolume
        << diskVendor
        << diskProduct
        << diskRevision
        << controllerVendor
        << controllerProduct
        << controllerRevision
        << geometry
        << ptable
        << drivers
        << data
        << flags
        << bootable;

    } SERIALIZERS(serialize);

    void _didReset(bool hard) override;
    void _didLoad() override;

public:

    const Descriptions &getDescriptions() const override { return descriptions; }

    
    //
    // Methods from Drive
    //
    
public:
    
    string getDiskVendor() const override { return diskVendor; }
    string getDiskProduct() const override { return diskProduct; }
    string getDiskRevision() const override { return diskRevision; }
    string getControllerVendor() const override { return controllerVendor; }
    string getControllerProduct() const override { return controllerProduct; }
    string getControllerRevision() const override { return controllerRevision; }

    bool isConnected() const override;

    Cylinder currentCyl() const override { return head.cylinder; }
    Head currentHead() const override { return head.head; }
    isize currentOffset() const override { return head.offset; }

    bool getFlag(DiskFlags mask) const override;
    void setFlag(DiskFlags mask, bool value) override;

    bool hasDisk() const override;
    bool hasModifiedDisk() const override;
    bool hasProtectedDisk() const override;
    void setModificationFlag(bool value) override;
    void setProtectionFlag(bool value) override;

    
    //
    // Methods from Configurable
    //

public:
    
    const HardDriveConfig &getConfig() const { return config; }
    const ConfigOptions &getOptions() const override { return options; }
    i64 getOption(Option option) const override;
    void checkOption(Option opt, i64 value) override;
    void setOption(Option option, i64 value) override;
    
private:
    
    void connect();
    void disconnect();

    
    //
    // Analyzing
    //

public:

    // Returns information about the disk
    void cacheInfo(HardDriveInfo &info) const override;

    // Returns information about a specific partition
    // void getPartitionInfo(isize nr) const;

    const PartitionDescriptor &getPartitionDescriptor(isize nr) const;

    // Returns the disk geometry
    const GeometryDescriptor &getGeometry() const { return geometry; }

    // Returns the number of partitions
    isize numPartitions() const { return isize(ptable.size()); }

    // Returns the number of loadable file system drivers
    isize numDrivers() const { return isize(drivers.size()); }

    // Returns the current drive state
    HardDriveState getState() const { return state; }
    
    // Gets or sets the 'modification' flag
    bool isModified() const { return flags & FLAG_MODIFIED; }
    void setModified(bool value) { value ? flags |= FLAG_MODIFIED : flags &= ~FLAG_MODIFIED; }

    // Returns the current controller state
    HdcState getHdcState() const;

    // Checks whether the drive will work with the currently installed Rom
    bool isCompatible() const;
    
    
    //
    // Formatting
    //
    
    // Returns a default volume name
    string defaultName(isize partition = 0) const;

    // Formats the disk
    void format(FSVolumeType fs, string name) throws;

    // Change the drive geometry
    void changeGeometry(isize c, isize h, isize s, isize b = 512) throws;
    void changeGeometry(const GeometryDescriptor &geometry) throws;
    
    
    //
    // Reading and writing
    //
    
public:
    
    // Reads a data block from the hard drive and copies it into RAM
    i8 read(isize offset, isize length, u32 addr);
    
    // Reads a data block from RAM and writes it onto the hard drive
    i8 write(isize offset, isize length, u32 addr);
    
    // Reads a loadable file system
    void readDriver(isize nr, Buffer<u8> &driver);
    
private:

    // Checks the given argument list for consistency
    i8 verify(isize offset, isize length, u32 addr);

    // Moves the drive head to the specified block
    void moveHead(isize lba);
    void moveHead(isize c, isize h, isize s);
    
    
    //
    // Importing and exporting
    //
    
public:
    
    // Restores a disk (called on connect)
    bool restoreDisk() throws;

    // Exports the disk in HDF format
    void writeToFile(const std::filesystem::path &path) throws;

    
    //
    // Managing write-through mode
    //
    
    void enableWriteThrough() throws;
    void disableWriteThrough();

private:
    
    // Return the path to the write-through storage file
    string writeThroughPath();
    
    // Creates or updates the write-through storage file
    void saveWriteThroughImage() throws;
    
    
    //
    // Scheduling and serving events
    //
    
public:
    
    // Schedules an event to revert to idle state
    void scheduleIdleEvent();
    
    // Services a hard drive event
    template <EventSlot s> void serviceHdrEvent();
    
};

}
