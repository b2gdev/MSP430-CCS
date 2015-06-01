
This project contains the source code for the MSP430 companion processor's firmware on the Revision C b2g device. This source code is a "Code Composer Studio" (CCS) IDE project.

### Build configuration
* IDE
  * Code Composer Studio - Version 6.1.0.00104
* Compilers
  * GNU MSP430 GCC Tools 4.9.1
  * TI MSP430 Compiler Tools 4.4.4 (Optional)
* Target device
  * MSP430F2418
* Debugger
  * MSP-FET430UIF

### CCS installation instructions
* Download CCS from [link](http://processors.wiki.ti.com/index.php/Download_CCS)
* Install with the following minimum options selected
  * Processor support: 
   * MSP Ultra Low Power MCUs
     * MSP430 Ultra Low power MCUs
     * TI MSP430 Compiler (Optional)
     * GCC ARM Compiler
  * Debug Probes
     * MSP430 USB FET

### How to import this project to CCS
* Clone this project in to your CCS working directory
* Go to CCS and select Project menu and Import CCS Project option
* Press the Browse button and browse to the Workspace and press OK
* Make sure you tick the project you just cloned
* Do not check either “Automatically import referenced projects found in same directory” or “Copy projects into workspace”
* Press the Finish button
