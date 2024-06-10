The control unit (Raspberry Pi) runs as a server. It controls the physical pins on the board based on received protocols. The Pi at this point in time is set to 192.168.10.100, constantly accepting clients. Client in this case refers to any attempt at making the control unit perform some action. Some user interface can act as a client, sending and receiving data, then closing. Some user interface can also create multiple clients, connecting concurrently.
# Server
When a client connects, the server expects a JSON formatted protocol (see [[#Formatting]]). It keeps the client socket open and puts it in a queue along with the protocol received. The main thread is constantly trying to pull from the queue. This allows for a queueing system of many protocols, and handles multiple clients even. However, this also complicates cancelation of said protocol, since by default, any attempts to reach the server will be put last in queue and will be stale when reached.
## Usage
### Initialize(int port)
Handles all setup needed for the server to run. Usage is simple:
```cpp
Server::ServerHandler server;

if(server.Initialize(8080)) {  
	// Server is running
}
```
### Listen()
Starts listening for client connections on a separate thread. Will run indefinitely on this thread until `Close()` is called. Whenever a client connects inside `Listen()`, it generates a thread to keep the connection and goes back to listening for new clients. Typical usage:
``` cpp
Server::ServerHandler server;

if(!server.Initialize(8080)) {
	std::cerr << "Server did not initialize correctly.\n";
    return -1;
}

server.Listen();
```
### Close()
Forces the server to close all threads associated with the server. This also closes the server itself, so usage might be rare.
``` cpp
void CleanupServer(Server::ServerHandler server) {
	std::cout << "Shutting down server.\n";
	server.Close();
}
```
### Respond(int clientSocket, const std::string &data)
This sends a given message to a given client. This message can be anything as long as it is a propper string and it is small enough to be sent over ethernet. Echo example:
``` cpp
void Echo(const std::string messageReceived, int clientSocket) {  
    std::string reply = messageReceived;
    Server::ServerHandler::Respond(clientSocket, reply);  
}
```
## Response
Whenever the control unit performs an action on the current protocol on the main thread, it sends an "update" to the client. This update is the protocol, modified, with indicators.

Because actions can be quite frequent, each message is delimited by a newline character. This lets the client separate responses dynamically and avoids most data bleeding.
Client example:
```CS
byte[] buffer = new byte[1];
StringBuilder messageBuilder = new();

while (true)
{
	// Reading response
	int bytesRead = await stream.ReadAsync(buffer, 0, buffer.Length);
	
	// Disconnected
	if (bytesRead == 0) break;

	// Get character
	char readChar = (char)buffer[0];

	// Keep reading
	if (readChar != '\n')
	{
		messageBuilder.Append(readChar);
		continue;
	}

	// Build string and process message as desired
	string completeMessage = messageBuilder.ToString();
	messageBuilder.Clear();
	ProcessMessage(completeMessage);
}
```
