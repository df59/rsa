/*
Dustin Franklin
df59@uakron.edu
University of Akron Computer Science
Algorithms Spring 2023

To build this program run:

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
McCutchen and found at https://mattmccutchen.net/bigint/


*/

#include "BigIntegerAlgorithms.hh"
#include "BigIntegerLibrary.hh"
#include "BigUnsigned.hh"
#include "BigUnsignedInABase.hh"
#include "sha256.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <string>

void save_signature_file(BigUnsigned signature, std::string filename) {
  std::string sig_file_name = filename + ".signature";
  std::ofstream signature_file = std::ofstream{sig_file_name};
  signature_file << signature;
  std::cout << "Signature written to " << sig_file_name << '\n';
  signature_file.close();
}

int main(int argc, char *argv[]) {
  // Second part of your project starts here
  if (argc > 4 || argc < 3 || (argv[1][0] != 's' && argv[1][0] != 'v'))
    std::cout << "wrong format! should be \"a.exe s filename\"";
  else {
    std::string filename = argv[2];

    // read the file
    std::streampos begin, end;
    std::ifstream myfile(filename.c_str(), std::ios::binary);
    begin = myfile.tellg();
    myfile.seekg(0, std::ios::end);
    end = myfile.tellg();
    std::streampos size = end - begin;

    myfile.seekg(0, std::ios::beg);
    char *memblock = new char[size];
    myfile.read(memblock,
                size); // read file; it's saved in the char array memblock
    myfile.close();

    std::string copyOFfile = filename + ".Copy";
    std::ofstream myfile2(copyOFfile.c_str(), std::ios::binary);
    myfile2.write(memblock, size); // write to a file
    myfile2.close();

    if (argv[1][0] == 's') {
      std::cout << "Signing document.\n";

      // sha256 hash of file stored as base 16 BigUnsigned
      BigUnsigned hash_16 = BigUnsignedInABase(sha256(memblock), 16);

      // get d_n.txt file, store to d and n
      std::ifstream d_n_file("d_n.txt");
      std::string line;
      BigUnsigned d, n;
      std::getline(d_n_file, line);
      d = stringToBigUnsigned(line);
      std::getline(d_n_file, line);
      n = stringToBigUnsigned(line);
      d_n_file.close();

      // decryption to sign, then save to signature file
      BigUnsigned signature = modexp(hash_16, d, n);
      save_signature_file(signature, filename);
    } else {

      std::string sig_file_name = argv[3];
      std::cout << "Verifying document.\n";
      // sha256 hash of file stored as base 16 BigUnsigned
      BigUnsigned hash_16 = BigUnsignedInABase(sha256(memblock), 16);

      // get e_n.txt file, store to e and n
      std::ifstream e_n_file("e_n.txt");
      std::string line;
      BigUnsigned e, n;
      std::getline(e_n_file, line);
      e = stringToBigUnsigned(line);
      std::getline(e_n_file, line);
      n = stringToBigUnsigned(line);
      e_n_file.close();

      // get signature file, store it as base 16 BigUnsigned
      std::ifstream sig_file(sig_file_name);
      std::getline(sig_file, line);
      BigUnsigned signature = BigUnsignedInABase(stringToBigUnsigned(line), 16);

      // encryption to verify
      BigUnsigned verification = modexp(signature, e, n);

      if (verification == hash_16) {
        std::cout << "authentic\n";
      } else {
        std::cout << "modified\n";
      }
    }
    delete[] memblock;
  }
  return 0;
}