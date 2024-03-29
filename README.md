### About the Project
CHAT was designed and developed as a class-wide data communications and inter-networking project. Led by me, the class
worked together to create the specifications. I wrote a Request for Comments based on these specifications. The class 
reviewed the RFC and I made necessary changes. Then, the class was split into six groups, three to each write a Server implementation and three to each
write a Client implementation. My responsibility was to write a testing saddle, seen in this repository, for the Servers and Clients to ensure they matched the protocol.

### Running
1. Start from directory `chat-server-test-saddle/`
2. Build the library you want to run:
    1. `cd ./test-client-source` or `cd ./benchmark-server-source` 
    2. `cmake -S . -B build`
    3. `cmake --build build` 
3. Build the saddle:
   1. `cd ..`
   2. `cmake -S . -B build`
   3. `cmake --build build`
4. Run the test saddle.

```
usage: ./chat-test-saddle -l <server | client> -i <ip address> -p <port number> [-t]
   -l <server | client>, whether to run the test server or test client
   -i <ip address>, if server selected for -l, will run the server at this ip address.
       if client selected for -l, will connect to a server at this ip address.
   -p <port number>, if server selected for -l, will run the server at this port number.
       if client selected for -l, will connect to a server at this port number.   
   [-t], optionally trace the execution of the program.
```