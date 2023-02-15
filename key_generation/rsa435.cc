/*
Dustin Franklin
df59@uakron.edu
University of Akron Computer Science
Algorithms Spring 2023



To build this program:

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




All code involving the BigInteger and BigUnsigned classes were written by Matt
McCutchen and found at https://mattmccutchen.net/bigint/

*/

// Standard libraries
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <string>

// 'BigIntegerLibrary.hh' includes all of the library headers.
#include "BigIntegerAlgorithms.hh"
#include "BigIntegerLibrary.hh"
#include "BigUnsigned.hh"

int big_int_size = 200; // desired base 10 digits for large primes

BigUnsigned get_random_bigunsigned() {
  BigUnsigned big1 = BigUnsigned(1);

  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dist(0, 9);
  big1 = dist(rng);

  for (int i = 0; i < big_int_size - 1; i++) { // ~199 base 10 digit number
    big1 = big1 * 10 + dist(rng);
  }
  return big1;
}

void amend_seven(BigUnsigned &number) {
  number = number * 10;
  number = number + 7;
}

bool fermat_test(BigUnsigned candidate_l) {
  BigUnsigned exponent = candidate_l - 1;
  if (candidate_l < 2) { // if this ever happens you should buy a lottery ticket
    std::cout << "fermat test failed with candidate being less than 2\n";
    return false;
  }

  for (int i = 1; i <= big_int_size; i++) {
    // std::cout << "fermat test at iteration " << i << '\n';
    BigUnsigned base = candidate_l - i;
    if (modexp(base, exponent, candidate_l) != 1) {
      // std::cout << "fermat test failed at iteration " << i << '\n';
      return false;
    }
  }

  return true;
}

BigUnsigned generate_fermat_prime() {
  bool prime = false;
  BigUnsigned random_candidate;

  while (!prime) {
    // std::cout << "generating new candidate\n";
    random_candidate = get_random_bigunsigned();
    amend_seven(random_candidate);
    // std::cout << random_candidate << '\n';

    if (fermat_test(random_candidate)) {
      prime = true;
      // std::cout << "fermat test passed with " << 1 << " iterations\n";
    } else {
      // std::cout << "fermat test failed with " << 1 << " iterations\n";
    }
  }

  return random_candidate;
}

void save_p_q_file(BigUnsigned p, BigUnsigned q) {
  std::ofstream p_q_file = std::ofstream{"p_q.txt"};
  p_q_file << p << '\n' << q;
  p_q_file.close();
}

void save_e_n_file(BigUnsigned e, BigUnsigned n) {
  std::ofstream e_n_file = std::ofstream{"e_n.txt"};
  e_n_file << e << '\n' << n;
  e_n_file.close();
}

void save_d_n_file(BigUnsigned d, BigUnsigned n) {
  std::ofstream d_n_file = std::ofstream{"d_n.txt"};
  d_n_file << d << '\n' << n;
  d_n_file.close();
}

int main() {
  // p and q generation of large fermat primes
  BigUnsigned p = generate_fermat_prime();
  std::cout << "p has been generated\n";
  BigUnsigned q = generate_fermat_prime();
  std::cout << "q has been generated\n";
  save_p_q_file(p, q);
  std::cout << "p and q written to p_q.txt\n";

  // n, e, d calculation of public and private keys
  BigUnsigned n, e, d, totient;
  n = p * q;
  std::cout << "n has been calculated\n" << n << '\n';
  e = 2316961; // public known prime number
  std::cout << "e is " << e << '\n';
  totient = (p - 1) * (q - 1) / gcd(p - 1, q - 1);
  d = modinv(e, totient); // extended euclidian implementation utilized in
                          // BigIntegerAlgorithms
  std::cout << "d has been calculated\n" << d << '\n';

  save_e_n_file(e, n);
  std::cout << "e and n written to e_n.txt\n";
  save_d_n_file(d, n);
  std::cout << "d and n written to d_n.txt\n";

  return 0;
}