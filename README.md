Arduino Web Server
==================

A project based on the Freetronics EtherMega 2560 to provide a web interface to
hardware.

The Freetronics EtherMega 2560 has:
* Ethernet
* MicroSD card reader
* Power over Ethernet


Intension
---------

The intension of this project is to have a HTTP server that will be able
to manipulate the IO ports based on input via HTTP requests.  The memory card
may be used to serve a nice looking web page or client-side JS application.

Especially with PoE, this would serve as a nice platform for home automation.
As it would be based on HTTP requests, this would also make it easily
accessible by a mobile app or a mobile browser, and (after analyzing the risks)
could also be port forwarded through a home router to make it accessible from
the internet.
