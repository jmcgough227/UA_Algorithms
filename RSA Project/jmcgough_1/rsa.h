/********************************************
* Jason McGough								*
* Algorithms: RSA Encryption				*
* File - rsa.h								*
********************************************/

#include <iostream>
#include <string>
#include <cctype>

 //Big integer libraries
#include "GMP.H"
#include "GMPXX.H"

using namespace std;

// Useful way to return multiple values from a function
struct Pair
{
	mpz_class a, b;
	
	Pair(mpz_class x, mpz_class y): a(x), b(y){}
};

class RSA
{
    private:
	mpz_class p, q;		// Prime numbers
	mpz_class n;		// n = p*q, used as modulus for public and private keys
	mpz_class phi;		// phi(n) = phi(p)*phi(q) = (p-1)*(q-1)
	mpz_class e;		// Public key exponent
	mpz_class d;		// Modular inverse of e
	
	 // Generate and check big primes
	mpz_class randPrime(int);
	bool isPrime(mpz_class);
	
	 // Finds greatest common divisor
	mpz_class gcd(mpz_class, mpz_class);
	 // ax + by = gcd(a, b), finds modular multiplicative inverses
	Pair gcdEx(mpz_class, mpz_class);
	
	 // Modular exponentiation here
	mpz_class modPow(mpz_class, mpz_class, mpz_class);
	
	 // Generates data required to establish keys
	void genKeys(bool);
	
	public:
	 // Generate keys randomly
	void randKeys(int, int);
	 // Assign keys manually
	void setKeys(mpz_class, mpz_class, mpz_class);
	
	mpz_class getP() {return p;}
	mpz_class getQ() {return q;}
	mpz_class getN() {return n;}
	mpz_class getPhi() {return phi;}
	mpz_class getE() {return e;}
	mpz_class getD() {return d;}
	
	string encrypt(string, int);	// Encrypt message in "chunks" of a size specified by the user
	string decrypt(string);			// Decrypts message
};

