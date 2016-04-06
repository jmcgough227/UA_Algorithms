
                    ----Readme!----                    
-------------------------------------------------------

Algorithms Project 1: RSA Cryptography
Name: Jason McGough
Contact: jrm43@zips.uakron.edu

-------------------------------------------------------

-----Welcome to the RSA Crytopgraphy readme!

This program was designed with big interger libraries (mpz_class) in mind.
These libraries require that Cygwin be used to compile properly!
This should be 32-bit cygwin, not 64-bit.

A make file is included, so once you are in the proper directory, the
project can be built by simply typing in "make." Thereafter, you can
run the program by typing in "./RSA".

When you run the program, an intro title will be the first thing to display,
followed by the program's main menu. The main menu lists 9 options, which are
selected by entering in the corresponding integer next to the option:

	-Option [1] generates random keys used in the encryption process. There
	 are two sub-options here:
		-A random number generator may be used to provide the number of
		 digits used in the keys.
		-The user may assign the number of digits for p and q themselves.
	-Option [2] allows the user to assign the key data used.
		-This will ask the user for values for p, q and e. 
	-Option [3] outputs the current key data to the display.
	-Option [4] is where the user will input messages. There are 2
	 sub-options here:
		-The user may input an unencrypted message
		-The user may input an encrypted message
	-Option [5] outputs both the current unencrypted message as well as
	 the encrypted message. These will be blank if no message has been
	 generated yet.
	-Option [6] will clear all current messages from their respective
	 variables. This can be used to make sure that the encryption/decryption
	 process works both ways without having to re-enter p,q and e. There is
	 a confirmation prompt here, so as to avoid accidentally clearing messages!
	-Option [7] Will first confirm the encryption action before committing to
	 it. If the user confirms, the current unencrypted message will be run
	 through the encrypt function and then placed into the encrypted message
	 variable. Remember, these messages can be viewed with option [5].
	-Option [8] will first confirm the decryption action before committing to
	 it. If the user confirms, the current encrypted message will be run
	 through the decryption function and then placed into the unencrypted
	 message.
		-This may not appear to do anything if you do not first clear the
		 current messages and then re-enter the encrypted message.
	-Option [9] will end the current session, closing the program.

-----Source explained

Classes have been used in order to encapsulate data and to prevent unwanted changes
to the more sensitive variables that RSA encryption uses.
	-RSA encapsulates the bulk of the features implemented in this program.
	-Pair has been implemented to provide an easy way to return 2 values
	 from a function, i.e, the extended Euclidean algorithm.

Functions have been implemented to:
	-Randomly generate very large prime numbers
		-Generates digits into a string before putting them into an mpz_class
		 object.
	-Test these very large numbers to make sure they are prime 
	 	-Via Fermat's Litte Theorem.
	-Find the greatest common divisor
		-Euclid's
	-Execute the extended version of the gcd function
		-Euclid's extended
	-Perform modular exponentiation
	-Generate encryption keys randomly
	-Set encryption keys manually
	-Encrypt a message
		-Broken up into groups of hexidecimal digits, size defined by user.
	-Decrypt a message
		-Translates hex back into the original message, provided the user
		 has the proper keys.

Two smaller functions have been implemented have been implemented to take input from
the user in the form of integers and mpz_class objects (big integers).

The entirety of the UI is contained within the main() function, which uses a switch
statement inside of a while loop to stay within the program until finished. The
main menu is a nexus to the options, always returning there before executing another
command.
	
