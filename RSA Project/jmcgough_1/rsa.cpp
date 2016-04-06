/********************************************
* Jason McGough								*
* Algorithms: RSA Encryption				*
* File - rsa.cpp							*
********************************************/

#include "RSA.H"

 //complete
mpz_class RSA::randPrime(int num_digits)
{
	//cout << "Entering randPrime function" << endl;
	 // Temporary home for generated number
	char* tempNum = new char[num_digits+1];
	
	 // Generated prime will end up here
	mpz_class prime;
	
	do
	{
		 // Ensures generated number will be odd, i.e. not divisible by 2!
		tempNum[0] = (rand()%5)*2+1+'0';
		
		 // Generate random digits
		for(int k = 1; k < num_digits-1; k++)
		{
			tempNum[k] = (rand()%9)+'0';
		}
		
		 // Makes sure leading digit is not a zero
		tempNum[num_digits-1] = (rand()%9)+'0';
		 
		 // Denotes end of the big integer
		tempNum[num_digits] = 0;
		
		prime = tempNum;
		
	}while(!isPrime(prime));	// Repeat process if result is not actually prime
	
	delete [] tempNum;			// House-keeping!
	//cout << "returning prime" << endl;
	return prime;
}
 //complete
bool RSA::isPrime(mpz_class p)
{
	//cout << "checking if prime" << endl;
	for(int h = 0; h < 3; h++)
	{
		int n;
		
		if( p > 32768)
		{
			n = rand()%32767 + 1;
		}
		else
		{
			 // n = rand%(n-1)+1
			n = rand()%(mpz_get_ui(p.get_mpz_t()-1)) + 1;
		}
		
		if(modPow(n, p-1, p) != 1)
		{
			//cout << "Not prime" << endl;
			return false;
		}
	}
	//cout << "Prime" << endl;
	return true;
}
 //complete
mpz_class RSA::gcd(mpz_class a, mpz_class b)
{
	//cout << "Entering gcd function" << endl;
	mpz_class c;			//stores remainder
	
	if(b == 0) return a;		//base case
	
	 // "mpz_class mod(remainder, numerator, denominator)" sets c 
	 // as remainder of the mpz_class modulus operation between a and b
	mpz_mod(c.get_mpz_t(), a.get_mpz_t(), b.get_mpz_t());
	
	return gcd(b, c);
}
 //complete
Pair RSA::gcdEx(mpz_class x, mpz_class y)
{
	//cout << "Entering gcdEx function" << endl;
	if(y==0) return Pair(1, 0);	//base case
	
	//cout << "x: " << x << endl;
	//cout << "y: " << y << endl;
	
	mpz_class quo = x/y;
	mpz_class rem = x-(y*quo);
	
	//cout << "Quotient and Remainder found" << endl;
	//cout << "quo: " << quo << endl;
	//cout << "rem: " << rem << endl;
	
	Pair ab = gcdEx(y, rem);
	
	//cout << "Returning Pair, exiting gcdEx" << endl;
	
	return Pair(ab.b, (ab.a - (quo*ab.b)));
}
 //complete - needs comments
mpz_class RSA::modPow(mpz_class b, mpz_class e, mpz_class m)
{
	mpz_class x = 1;

	while(e > 0)
	{
		if(mpz_odd_p(e.get_mpz_t()))
		{
			x *= b;
		}
		
		mpz_mod(x.get_mpz_t(), x.get_mpz_t(), m.get_mpz_t());
		
		e /= 2;
		b *= b;
		
		mpz_mod(b.get_mpz_t(), b.get_mpz_t(), m.get_mpz_t());
	}
	
	return x;
}
 //complete
void RSA::genKeys(bool flag)
{
	//cout << "Entering genKeys" << endl;
	//Find n and phi
	n = p*q;
	phi = (p-1)*(q-1);
	
	//Flag whether or not we need to find e
	if(flag)
	{
		//cout << "Flag is true" << endl;
		for(mpz_class k = 3; k < phi; k += 2)
		{
			//cout << "Inside e finding loop" << endl;
			if(gcd(phi, k) == 1)
			{
				e = k;
				//cout << "e selected" << endl;
				break;
			}
		}
	}
	//cout << "Exiting flag check" << endl;
	//cout << "e: " << e << endl;
	//cout << "phi: " << phi << endl;
	
	Pair find_d = gcdEx(e, phi);
	
	//Ensure d is positive
	if(find_d.a < 0)
	{
		find_d.a += phi;
	}
	//cout << "d found and set" << endl;
	d = find_d.a;
}
 //complete
void RSA::randKeys(int n, int m)
{
	//cout << "Entering randKeys" << endl;
	p = randPrime(n);
	//cout << "p value selected" << endl;
	q = randPrime(m);
	//cout << "q value selected" << endl;
	genKeys(true);
	//cout << "Leaving randKeys" << endl;
}
 //complete
void RSA::setKeys(mpz_class x, mpz_class y, mpz_class z)
{
	//User sets keys here
	p = x;
	q = y;
	e = z;
	
	 //e supplied, don't need to generate
	genKeys(false);
}
 //complete
string RSA::encrypt(string msg, int group)
{
	int length = msg.length();
	string encrypted_msg;
	mpz_class mult;
	mpz_class num;
	
	//Loop increments by group size
	for(int k = 0; k < length; k += group)
	{
		mult = 1;
		num = 0;
		
		 // Separates hexed character groups
		if(k != 0) encrypted_msg += "r";
		
		for(int h = 0; (h < group) && ((k+h) < length); h++)
		{
			mult *= 256;
			num += msg[k+h] * mult;
		}
		
		 // Converts message group to encrypted form
		mpz_class convert = modPow(num, e, n);
		
		 // Adds group to total encrypted message
		char* str = mpz_get_str(NULL, 16, convert.get_mpz_t());
		encrypted_msg += str;
		
		delete [] str;
	}
	
	return encrypted_msg;
}
 //complete - needs comments
string RSA::decrypt(string msg)
{
	//cout << "Entering decrypt function" << endl;
	string decrypted_msg;
	string group;

	int mLength = msg.length();
	int gLength = group.length();

	for(int i = 0; i < mLength; i++)
	{
		if(msg[i] == 'r' || i == mLength-1)
		{
			if((i == mLength-1) && (msg[i] != 'r'))
			{
				group += msg[i];
			}
			
			if(gLength % 2 != 0)
			{
				group = "0" + group;
			}
			
			mpz_class g = (mpz_class)("0x" + group);
			
			mpz_class revert = modPow(g, d, n);
			
			//cout << "Creating temporary group" << endl;
			char* str = mpz_get_str(NULL, 16, revert.get_mpz_t());
			
			for(int l = strlen(str)-2; l >= 0; l -= 2)
			{
				unsigned char ch = 0;
				
				if(isalpha(str[l]))
				{
					ch += str[l] - 'a' + 10;
				}
				else
				{
					ch += str[l] - '0';
				}
			
				ch<<=4;
				
				if(isalpha(str[l+1]))
				{
					ch += str[l+1] - 'a' + 10;
				}
				else
				{
					ch += str[l+1] - '0';
				}
				
				decrypted_msg += ch;
			}
			
			delete [] str;
			group = "";
		}
		else
		{
			group += msg[i];
		}
	}
	
	return decrypted_msg;
}

/****************************************************************************************/

int input_an_int(){
	char str[20];
	
	gets(str);
	int new_num = atoi(str);
	
	return new_num;
}

mpz_class input_a_big_int(){
	char str[4196];
	
	gets(str);
	
	return (mpz_class)((string)str);
}

int main()
{	
	RSA rsa;
	string message;
	string encrypted;
	bool done = false;
	
	 //Intro title, only shows on initial start-up
	cout << " |_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|" << endl;
	cout << " |  ___    ___     _       ___    ___           ___   _____   ___  |" << endl;
	cout << " | |   |  |   |   | |     |   |  |   |  |   |  |   |    |    |   | |" << endl;
	cout << " | |___|  |___   |___|    |      |___|   |_|   |___|    |    |   | |" << endl;
	cout << " | |  |       |  |   |    |      |  |     |    |        |    |   | |" << endl;
	cout << " | |   |  |___|  |   |    |___|  |   |    |    |        |    |___| |" << endl;
	cout << " |                                                                 |" << endl;
	cout << " |*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|" << endl;
	cout << "  ---By Jason McGough--- " << endl << endl;
	
	cout << "  This program uses Fermat's test 3 times to generate primes with\n"
			"  reasonable confidence." << endl << endl << endl;
			
	
	 //Program loop
	while(!done)
	{
		srand(time(NULL));
		
		 //The main menu
		cout << " |_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_| " << endl;
		cout << endl << "   Welcome to the main menu!" << endl << endl;
		cout << " |*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*|_|*| " << endl;
		cout << endl << "   Here are your options..." << endl << endl;
		
		cout << "     [1] Randomly generate some keys" << endl;
		cout << "     [2] Manually enter the keys" << endl;
		cout << "     [3] View current key data" << endl;
		cout << "     [4] Input a message" << endl;
		cout << "     [5] View current messages" << endl;
		cout << "     [6] Clear current messages" << endl;
		cout << "     [7] Encrypt message" << endl;
		cout << "     [8] Decrypt message" << endl;
		cout << "     [9] Exit the program" << endl << endl;
		
		cout << "   Option: ";
		int option;
		option = input_an_int();
		cout << endl;
		
		switch(option)
		{
			 //randomly generate some keys
			case 1:
			{
				int p_digits;
				int q_digits;
				int digits_choice;
			
				cout << "   How many digits?" << endl;
				cout << "    [1] Don't care (10 to 100 digits)" << endl;
				cout << "    [2] Let me pick" << endl << endl;
				cout << "   Option: ";
				digits_choice = input_an_int();
				cout << endl;
				
				 //Pick digits randomly for user
				if(digits_choice == 1)
				{
					p_digits = rand()%91 + 10;
					q_digits = rand()%91 + 10;
					
					cout << "   OK, we got " << p_digits << " digits for p..." << endl;
					cout << "   ... and " << q_digits << " digits for q." << endl << endl;
				}
				 //Let user pick digits
				else if(digits_choice == 2)
				{
					cout << "   How many digits would you like?" << endl;
					cout << "   P Digits: ";
					p_digits = input_an_int();
					cout << "   Q Digits: ";
					q_digits = input_an_int();
					cout << endl;
				}
				
				 //Generating random keys...
				rsa.randKeys(p_digits, q_digits);
					
				cout << "   Key data established!" << endl << endl;
				
				break;
			}
			 //User manually assigns keys here
			case 2:
			{
				mpz_class p;
				mpz_class q;
				mpz_class e;
			
				cout << "   All right then, please enter your..." << endl;
				cout << "   P: ";
				p = input_a_big_int();
				cout << "   Q: ";
				q = input_a_big_int();
				cout << "   E: ";
				e = input_a_big_int();
				
				 //Sets keys based on user input
				rsa.setKeys(p, q, e);
				
				cout << endl << "   Key data established!" << endl << endl;
				
				break;
			}
			 //Outputs current key data
			case 3:
			{
				cout << "   Current key data..." << endl << endl;
				cout << "     P: " << rsa.getP() << endl << endl;
				cout << "     Q: " << rsa.getQ() << endl << endl;
				cout << "     N: " << rsa.getN() << endl << endl;
				cout << "     Phi: " << rsa.getPhi() << endl << endl;
				cout << "     E: " << rsa.getE() << endl << endl;
				cout << "     D: " << rsa.getD() << endl << endl;
				
				break;
			}
			 //User option to enter encrypted or decrypted messages
			case 4:
			{
				int msg_choice;
			
				cout << "   Are you entering an unencrypted or encrypted message?" << endl;
				cout << "     [1] Unencrypted" << endl;
				cout << "     [2] Encrypted" << endl << endl;
				cout << "   Option: ";
				msg_choice = input_an_int();
				cout << endl;
				
				 //User enters unencrypted message
				if(msg_choice == 1)
				{
					cout << "   OK, please enter your unencrypted message: ";
					char mystr[4096];
					gets(mystr);
					message = (string)mystr;
					cout << endl;
					
					cout << "   So the unencrypted message is set as '" << message << "' then.\n\n"; 
				}
				 //User enters encrypted message
				else if(msg_choice == 2)
				{
					cout << "   OK, please enter your encrypted message: ";
					char mystr[4096];
					gets(mystr);
					encrypted = (string)mystr;
					cout << endl;
					
					cout << "   So the encrypted message is set as '" << encrypted << "' then.\n\n";
				}
				
				break;
			}
			 //Outputs current message status
			case 5:
			{
				cout << "   Listed below is the current message status:" << endl << endl;
				cout << " Unencrypted message: " << message << endl << endl;
				cout << " Encrypted message: " << encrypted << endl << endl;
				
				break;
			}
			 //Allows user to clear messages without having to start over
			case 6:
			{
				 //Messages not cleared yet
				int msg_clear;
			
				cout << "   Clear current messages... are you sure?" << endl;
				cout << "     [1] Yes" << endl;
				cout << "     [2] No" << endl << endl;
				cout << "   Option: ";
				msg_clear = input_an_int();
				cout << endl;
				
				 //Messages clear here
				if(msg_clear == 1)
				{
					message = "";
					encrypted = "";
					
					cout << "   Messages cleared!\n\n";
				}
				
				break;
			}
			 //Encrypt message option
			case 7:
			{
				int encrypt_option;
				int groups;
				
				cout << "   So, encrypt the message then?" << endl;
				cout << "     [1] Yes" << endl;
				cout << "     [2] No" << endl << endl;
				cout << "   Option: ";
				encrypt_option = input_an_int();
				cout << endl;
				
				 //User wants to encrypt message...
				if(encrypt_option == 1)
				{
					cout << "   How would you like the characters grouped?" << endl;
					cout << "   In groups of... ";
					groups = input_an_int();
					cout << endl;
				
					encrypted = rsa.encrypt(message, groups);
					
					cout << "   Message encrypted!" << endl << endl;
				}
				
				//In case of user not wanting to encrypt, simply exits option
				
				break;
			}
			 //Decrypt message option
			case 8: 
			{
				int decrypt_option;
				
				cout << "   So, decrypt the message then?" << endl;
				cout << "     [1] Yes" << endl;
				cout << "     [2] No" << endl << endl;
				cout << "   Option: ";
				decrypt_option = input_an_int();
				cout << endl;
				
				 //User want to decrypt message...
				if(decrypt_option == 1)
				{
					message = rsa.decrypt(encrypted);
					
					cout << "   Message decrypted!" << endl << endl;
				}
				
				//In case of user not wanting to decrypt, simply exits option
				
				break;
			}
			 //Closes the program
			case 9:
			{
				done = true;
				break;
			}
			 //Only bit of validation you will find... :)
			default:
			{
				cout << "   Invalid menu choice, returning to main menu...\n\n";
			}
		}
	}
	
	cout << "Exiting Program..." << endl;
	
	return 0;
}

