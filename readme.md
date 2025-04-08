## Requirements
- grpc version 1.71.0
- Protobuf version 29.3
- Python 3.10-3.12 (Change line:4 in the cmake if the Python version is different from 3.12)

## Run Instructions

1.  `Go to the root directory`
2. `mkdir build`
3. `cd build`
4. `cmake ..`
5. `make`
6. Create 5 terminals for the 5 localhost server nodes and 1 terminal for the client code
7. For local testing, use the local_testing folder otherwise use distributed testing if on a switch
8. Navigate to build directory 
9. Run in each server terminal `./server ../configs/[local/distributed]_testing/config_X.json 2165868`
10. The csv file is renamed to `collision.csv` and put in the same directory as the python client.
11. In the client terminal, run `python client.py` in the `python/src` directory
