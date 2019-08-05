# mdfh
Market data feed simulator and receiver

# Requirements
The build has been tested on
1. g++ 8.3 on Fedora Core 30 or g++ 9.1 on Mac
2. Boost (>= v1.66)

# How to build
1. Create a build directory on preferably on the top-level directory and _cd_ to it
2. Run cmake to create the Makefile
  On Linux and Mac this is:
  ```
  $ cmake .. -G "Unix Makefiles"
  ```
3. Then, build
  ```
  $ cmake --build .
  ```
  
  The output binaries should be available from the bin/ directory on the build dir.
  
# How to run
To run the client (market data receiver)
  ```
  $ ./client 0.0.0.0 239.255.0.1
  ```
  
To run the server (market data simulator)
  ```
  $ ./server 239.255.0.1
  ```
  
Extra: a python script is also included on the python/ directory. It is a UDP listener which just prints out all UDP traffic on the multicast group _239.255.0.1_ port 54321. This requres the *Twisted* library.
  ```
  $ python3 mdfh_client.py
  ```
