Dustin Franklin
df59@uakron.edu
University of Akron Computer Science
Algorithms Spring 2023

To build the key generation program, inside the key_generation directory, run:

make clean
make

To run this program:

simply execute the executable ./rsa435



This program will:

generate two random 200 decimal digit numbers and verify they are prime numbers
using fermat's test, then store these two random prime numbers into a single
file separated by a new line. That file will be named p_q.txt

generate a private key and store it into a file named d_n.txt

generate a public key and store it into a file named e_n.txt




To build the sign executable program, inside the sign_verify directory run:

make clean
make

This program is executed in the command line in one of two forms:

./sign s file.xyz

where s indicates the program will sign file.xyz and save the signature of the
sha256 hash of file.xyz to file.xyz.signature. It is required to have a private
key pair file named d_n.txt in the working directory.

or

./sign v file.xyz file.xyz.signature

where v indicates the program will verify that the hash of file.xyz matches
file.xyz.signature. It is required to have a public key pair file named e_n.txt
in the working directory.






All code involving the BigInteger and BigUnsigned classes were written by Matt
McCutchen and can be found at https://mattmccutchen.net/bigint/
