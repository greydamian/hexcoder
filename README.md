hexcoder
========

Overview
--------

Hexadecimal codec utility.

Compatibility
-------------

* Linux
* Unix
* OS X

Installation
------------

Full installation of hexcoder is a 2 step process. However, the second step is 
optional.

Firstly, navigate to the root directory of the project and run the following 
command:

    $ bash build.sh

Once this has completed, you can run hexcoder by navigating to the bin 
directory and running the following command:

    $ ./hexcoder

Secondly, and optionally, you can install hexcoder system-wide by copying the 
contents of the bin directory into your path. For example, you could execute a 
command such as:

    # cp bin/* /usr/local/bin/

Examples
--------

Hexcoder can be used to encode the binary data of a file (e.g. file.bin) as 
hexadecimal characters and then store the output in another file (e.g. 
file.hex) by executing the following command:

    $ hexcoder <file.bin >file.hex

Hexcoder can be used to decode the hexadecimal characters of a file (e.g. 
file.hex) as binary data and then store the output in another file (e.g. 
file.bin) by executing the following command:

    $ hexcoder -d <file.hex >file.bin

Hexcoder can be used to "beautify" the hexadecimal characters of a file (e.g. 
file.hex) and then store the output in another file (e.g. beauty.hex) by 
executing the following command:

    $ hexcoder -b <file.hex >beauty.hex

License
-------

Copyright (c) 2014 Damian Jason Lapidge

Licensing terms and conditions can be found within the file LICENSE.txt.

