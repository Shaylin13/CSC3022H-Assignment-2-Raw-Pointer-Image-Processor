readme
PDYSHA009

VolImage

to run the executable, navigate to the folder (A2) in the terminal
type "make" to compile

there are 3 run types, for each functionality:
1) run the executable without optional parameters
    eg:
     ./volimage "MRI"
    
2)run executable with diffmap flag, two ints and ouput name
    eg:
     ./volimage "MRI" -d 1 2 "output"

3)run with extract flag
    eg:
     ./volimage "MRI" -x 1 "output"
     
Within the Makefile there are commands that will run the 3 different run types for testing
these have preset values (ones shown above).
command for those are:

1)make run
2)make diffmap
3)make extract

there are clean fucntions in makefile aswell
1)make clean
removes executable, user generated .raw and .dat from root folder

2)make tidy
removes any .raw and .dat so user can run again without compiling.
