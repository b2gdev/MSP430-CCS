Build tools
Code Composer Studio - Version 6.1.0.00104
MSP430-GCC-3.3.4.0

Use these project properties
compiler version = GNUv4.9.1(Red Hat) (Tested also for TIv4.4.3)
Device Variant = MSP430F2418

Debugging tool
MSP-FET430UIF

Notes

When using ==, != for SFR bit comparisons note that the mask does not seperate the bit value. Mask just make the other bits of the SFR 0

This GIT repo does not include the build files. Import the files to CCS workspace and bulid from it.