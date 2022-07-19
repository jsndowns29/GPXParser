Jason Downs
Full Stack GPX Parser School Project

Backend: Node and Express.js (located in app.js file), Parser is written in C and uses libxml2 (located in the parser file)
Frontend: HTML, CSS, JavaScript, JQuery (located in public folder)
Database: MySQL

The Node and Express backend uses the parser written in C to parse the GPX files and convert them into a JavaScript readable JSON format. The backend stores and retrieves this data from the MySQL databases and serves it to the frontend. The user can also create and modify the GPX files from the user interface.

Before running you must use the make file provided in the parser folder to compile the C code. The code was written to compile on a Debian Linux virtual machine as per assignment requirements, it will not compile on mac. 