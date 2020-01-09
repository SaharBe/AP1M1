Our program connects to flightgeer simulator and server as a server, 
which sends it commands, and at the same time, also serve a customer receiving commmands from it, to fly the plane.
The program will receive a text file and the desired commands for the flight. 

The program is built as a part of a programming course at Bar-Ilan University.


How it will work?
Here are the steps:

1. Download and install flight-geer : https://www.flightgear.org/download/
2. Enter to flight-gear setting thr following commands:

  --telnet=socket,in,10,127.0.0.1,5400,tcp --http=8080    
  
  --generic=socket,out,10,127.0.0.1,5402,tcp,generic_small
  
3. Plant the file 'generic_small.xml' in flightgeer/Protocol
4. start the program with fly.txt
5. Open the flight-geer and click Fly.

https://github.com/SaharBe/AP1M1
