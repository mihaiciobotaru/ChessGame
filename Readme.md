# Chess game

## Description

The project aimed to be accomplished is called ”ChessGame”. This
project consists of the client and server side of a chess game played across different
systems. The main requirements of this application are creating an attractive
interface for the user to play and making an efficient protocol to communicate
with the concurrent server. I have learned about C++, SFML and computer networking with the help of this project.

## Technologies used

For the graphical side of the project I decided to use SFML - Simple and Fast
Multimedia Library. SFML is a cross-platform library designed to provide simple
application programming interface (API) having bindings in C. For the transport
layer protocol, the best option is to use TCP - Transmission Control Protocol.
TCP is a connection-oriented transport protocol that send the information correctly
and keeps track of the packets‘s orders. This concept is essential because
the game relies on loyal sending and receiving of packets in correct order.More
than that, when sending a package, the protocol checks that the message was
sent correctly and ensures that the parties are still connected. As for the assets
used, I have displayed the pieces and the board‘s tiles with the help of John
Pablok‘s chess PNGs.

## Network protocol

### Server side
The server first creates a socket and binds it‘s address and port
to it in order for the clients to connect. After that the server enters in a loop
designed for listening to clients. When the server successfully accepted two clients
it creates a new thread for their communication and goes back at the start of
the loop to listen to new clients. All of these details described are presented in
the diagram above in the left and the other diagram depict the communication
between the clients. The protocol starts by sending to each client the color of
pieces he plays and, by sending these initial packages, lets the clients that there
was found another client to play with. The first player to connect is always the
white player and the other is the black player. After sending the initial packages,
it enters the loop of communication. First the server receives a package from the
first client and sends it back to the second. The second client sends a package
after that to the server and the server sends it back to the first client. This goes
on until the server receives an exit package.

### Client side 
Firstly the client creates it‘s socket and converts the server address
to binary form. After it has done that, it enters in a loop that it will exit only
when it would have successfully connected to the server. Once connected to the
server it waits for the initial package that it lets him know that it has found
another player to play with. It process the initial package in order to know
which color it is. If it is white, naturally, he will move first, then the other player
and so on until a player wins. After a client makes a move,it will send the board
info to the server and wait to receive board info back from the server in order to
move again. While waiting to receive the player can not move because it is not
their turn( their turn comes once again when the other players finishes makes
a move ). Once the game has finished, the client has two choices to exit or to
play another game. There also exists the possibility of exiting the game in the
middle of it. When exiting prematurely, the client sends the server a package in
which it lets him know. The server sends that package to the other client to let
him know that he was disconnected ( the server disconnects the other player and
exits communication when one of the players exited ). When this happens, the
client has the same possibility as before: to search for a new game or to exit.
Packages Packages have either 4 bytes or 256 bytes. Initial packages have 4
bytes representing each an int. The initial package sent to the first client is
number 2 which tells the client he is the color white and, for the second client,
the package contains the number 1 meaning the color black. Anytime the client
quits, he sends an exit package of 4 bytes size representing the number 0 as an
int to the server. When the server receives such a packages he sends the same
package with the same size to the other client and after that closes the client
sockets quitting the communication. When such a number is read by the client,
the player can not move any pieces anymore and a popup with the message
”Disconnected” appears. The popup offers the possibility to chose to search for
a new game or to exit the app through two buttons. There exists one last type of
package, the 256 bytes packages. This package is an 64 long int array encrypting
for each tile in the board what piece is on it. This package is sent to the server
after a client has been done. When the server receives such a package, he sends
it back without processing it further to the other client. When a board package
is received, the client knows the other player has finished it‘s move and it is his
turn now. Each client has a matrix of the board and when a board packages is
received, it replaces the matrix it has locally with the one received.


### Dependencies

* SFML 
* C++ compiler

## Authors

Ciobotaru Mihai (https://github.com/mihai2096)

## Acknowledgments

* [Multithreading](https://www.geeksforgeeks.org/multithreading-in-cpp/)
* [SFML documentation](https://www.sfml-dev.org/documentation/2.5.1/)
* [John Pablok - chess pieces and board assets](https://opengameart.org/content/chess-pieces-and-board-squares)
* [Jacob Sorber](https://www.youtube.com/c/JacobSorber)
* [Lenuta Alboaie, Andrei Panu - computer networking](https://profs.info.uaic.ro/~computernetworks/cursullaboratorul.php)
