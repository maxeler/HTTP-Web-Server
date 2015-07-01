# HTTP Server

### Requirements

The project uses Python **make** script which is located in the APP folder. To run script Python version 2.6+ needs to be installed.

### Configuration:
#####Set location for generated bitstream files
Before running build script, some parameters should be set in the make script first. 
This includes setting default location for generated .max and .h files and network parameters.
To set location of the .max and .h files for DFE and simulation, variables **MAXBUILDDIRECTORY_DFE** and **MAXBUILDDIRECTORY_SIM** need to be updated. Default values for these two variables are

    MAXBUILDDIRECTORY_DFE="/tmp/maxbuild/maxdc_builds/DFE/"
    MAXBUILDDIRECTORY_SIM="/tmp/maxbuild/maxdc_builds/DFE_SIM/"

#####Set network parameters
Network settings for **simulation** include configuring IP address of the HTTP Server (**IP1**), IP address of the client (**TAP1_IP**) and netmask (**TAP1_NETMASK**). Default values for these script variables are shown below.

    # ip addresses for simulation
    IP1="169.254.10.4"  
    TAP1_IP="169.254.10.48"  
    TAP1_NETMASK="255.255.255.0"

Network settings for **DFE** include configuring IP address of the HTTP Server (**IP**) and netmask (**NETMASK**). Value of the **IP** will be assigned to the QSFP_BOT_10G_PORT1 on the DFE. Default values for these script variables are shown below.

    # ip addresses for DFE
    IP="172.17.0.9"  
    NETMASK="255.255.255.224"
    
#####Prepare files
All files that need be hosted on the HTTP Server should be located in

    APP/CPU_SRC/HTTP-Web-Server/files
    
There are few example files already in this folder.    

### How to compile and run project:  
Project can be built for hardware (DFE) and simulation.  
To build and run project for **DFE**, navigate to the APP folder and type in terminal

     ./make dfe

If .max file alreads exists, it can be used for running, or it can be re-built.

To build project for **simulation**, type in terminal

     ./make sim

#####Usage
Once the compilation for DFE or simulation is done, script starts HTTP Server automatically.  
To test the HTTP Server open Firefox or Google Chrome browser.  
For example, if the server has IP address 172.17.0.9 and if **APP/CPU_SRC/HTTP-Web-Server/files** contains **file1.png**, **file2.png**, **file3.png** to get these files from the server enter in the browser address bar:

    http://172.17.0.9/files/file1.png
    http://172.17.0.9/files/file2.png
    http://172.17.0.9/files/file3.png

Upon request files should be displayed within the browser. 

### Cleaning build directory
 To clean build directory and to remove script generated files, type in terminal
 
     make clean
