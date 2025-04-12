# Take home exercise

## Algorithm

* Clients send to the server random integer numbers from 0 to 1023.
* Server receives and stores these numbers into a container as unique values.
* Upon a client's request, the average of squares of the numbers including the newly received number is
calculated and sent back to the client as a response.
* A client receives this value, generates a new random number and then again sends it to the server and so
on.
* Every N seconds the server makes a dump - it stores all numbers from the container into a file in the binary
format

## Details
* User has to be able to normally stop clients and server (for example, using ESC button)
* Server has to make a dump in the separate thread - not in the thread(s) which is used for communication
with clients
* Output all the processes into console and the application's log file
* When developing, you must use the Boost Asio library

## Notes
* Boost is required in the path ..\boost\include\boost-1_87
* Displays the progress of the test on screen
* Logs are also written to a file
* Dumps data to a file every N seconds (use -N <value> to override)
* Posts tasks to the I/O context to avoid additional locking
* Client does not support reconnection (not required from the description)
* Units tests are included only for logic

## Design
UML
![UML Diagram.](/UML/design.drawio.svg)

## How to run

* Compile all
* Run run_10_clients_1_server.bat

![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](/images/Capture.PNG)


## License

Copyright (c) 2025 Michael Alvanos Services LTD All rights reserved.

This software is provided "as is" without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

This software is the property of Michael Alvanos Services LTD.
Redistribution or reproduction, in whole or in part, is not permitted
without explicit prior written permission from the owner.
