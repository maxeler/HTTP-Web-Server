# Getting started:

### How to clone project

To clone the project, type in the terminal window
    git clone --recursive <repository-url>

### How to compile project:
      
In terminal type **./make** and press Enter. 
Binary file **httpServer** should be created in current directory. 

### How to run project:

Step 1. Before running binary, hosted files should be put in the **/files** folder.   
Step 2. In terminal run binary file: 

    ./httpServer <port_number>

where `<port_number>` is network port on which server is running. 

# Usage:

Once project is run, to access server and hosted files follow one of the following URLs:

	http://0.0.0.0:10080/path/to/file_name.ext
	http://localhost:10080/path/to/file_name.ext

# Example of use:

	http://0.0.0.0:10080/files/app.png
