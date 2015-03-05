# HTTP Server

## **Step 1:** run *init_code* `make.py`

**Description:** 

Generates LMEM and CRC index tables initialization files

**Inputs:** 

Folder with files to be stored in the LMEM, `cdir` variable should contain the folder path.

**Outputs:** 

    /results
	    - lmem_generated_file.html
	    - romCrcIndex1_init.html
	    - romCrcIndex2_init.html

Step 1 needs to executed only when `files` folder has been updated. 

## **Step 2**: build *bitstream* 

**Description:** Generates `.max` and `.h` project files
**Outputs:** 
	- `httpServer.max`
	- `httpServer.h`

Import `bitstream` folder in MaxIDE as Old/Non MaxCompiler project and click on the menu Run\Run (Ctrl + F11) to generate `.ax` and `.h` files.

## **Step 3:** run *hostcode* `run.sh`

**Description:** Initializes and configures DFE 
	
This script will create and run executable hostcode program. 	

## **Step 4:** use HTTP Web-Server

Current settings of the *bitstream* and *hostname* have been set for a simulation.

**Usage:** 

    $ telnet ip_address port 

**Example:** 

    $ telnet 169.254.10.4 80

System generated output:

    Trying 169.254.10.4...
    Connected to 169.254.10.4.
    Escape character is '^]'.

Enter the following text and press Enter

    GET /files/Test.txt 12345

The following output will be generated:

    01234567891011121314151617181920212223242526272829303132333435363738394041424344
    ...
    10011002100310041005100610071008100910101011101210131014101510161017101810191020102110221023Connection closed by foreign host.