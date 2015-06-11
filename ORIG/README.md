### How to compile project:  

In terminal type **./make** and press Enter. 
Binary file **httpServer** should be created in current directory.

### How to run project:

To run project, type in terminal: 

    ./httpServer [--port=PORT] [--path=PATH]

where `[--port=PORT]` is optional argument that specifies network port on which server will be running and `[--path=PATH]` is also optional argument which specifies hosting folder.  If optional arguments are not set, it is assumed that default server port is **8080** and that default hosting folder is **./files**.   

Before running the project hosted files should be put in the hosting folder.

### Usage:

Once project is run, to access server and hosted files follow one of the following URLs:

	http://0.0.0.0:8080/path/to/file_name.ext
	http://localhost:8080/path/to/file_name.ext

### Example of use:
If server is set to listen on the port **8080**, to open hosted file e.g. **app.png** type in a browser:

        http://0.0.0.0:8080/files/app.png
