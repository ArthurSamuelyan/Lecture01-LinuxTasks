## How to build and run

1. `$ make`

2. `$ 03-reparenting-process` - see the output.
Remember <child PID> and <parent PID>.

3. Open a new terminal (later referred as second).

4. From the second terminal kill parent process.
`$ kill -SIGKILL <parent PID>`

5. See the output from  the first terminal. 
Notice the <new parent PID>.
You may checkout the new parent process with 
`$ ps -p <new parent PID>` typed in the second terminal

6. Finish the demonstration `kill -SIGKILL <child PID>`

