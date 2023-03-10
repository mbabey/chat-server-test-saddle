### Running
1. Start from directory `chat-server-test-saddle/`
2. Build the library you want to run:
    1. `cd ./client-test-saddle-source` or `cd ./server-test-saddle-source` 
    2. `cmake -S . -B build`
    3. `cmake --build build` 
3. Build the saddle:
   1. `cd ..`
   2. `cmake -S . -B build`
   3. `cmake --build build`
4. Run the test saddle.


      usage: ./chat-test-saddle -l <server | client> -i <ip address> -p <port number> [-t]    
      -l <server | client>, whether to run server or client tests                             
      -i <ip address>, if server selected for -l, will run the server at this ip address.     
          if client selected for -l, will connect to a server at this ip address.             
      -p <port number>, if server selected for -l, will run the server at this port number.   
          if client selected for -l, will connect to a server at this port number.            
      [-t], optionally trace the execution of the program.