/*
	Astrology Password
	Description: Uses your computer's uptime and system name as a "sign" and mushes them together to create a semi-random password
	Arguments: Takes a single argument "system name" or any cipher you wish to use
	Author: Damean Murphy-Short
	Version: 1.0

*/
#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

using std::cout;
using std::cin;
using std::string;
using std::endl;

#define MAXLEN 255


		//Determine OS
		//Use OS-calls to determine uptime
		#ifdef _WIN32
			#include <windows.h>
			long s_uptime(){
				long uptime = GetTickCount64();
				return uptime;
			}	

			string getsysname(string sysname){
				#define INFO_BUFFER_SIZE 32767
				
				TCHAR infoBuf[INFO_BUFFER_SIZE];
				DWORD bufCharCount = INFO_BUFFER_SIZE;
				string placeholder = "System_name";	
				
				if( GetComputerName(infoBuf, &bufCharCount) ){
					sysname += infoBuf;
					return sysname;
				}else{
					sysname += placeholder;
					return sysname;
				}
			}
		#elif __linux__
			#include <linux/unistd.h>
			#include <linux/kernel.h> 
			#include <sys/sysinfo.h>
			#include <limits.h>
			#include <unistd.h>

			long s_uptime(){
				struct sysinfo info;
				if(sysinfo(&info)){
					cout << "Bad sysinfo" << endl;
					exit(0);
				}
				return info.uptime;
			}
		
			//This gets hostname from linux and puts in c++ string 
			string getsysname(string sysname){
				char hostname[MAXLEN];
				gethostname(hostname, MAXLEN);
				cout << "system hostname is " << hostname << endl;
				sysname += hostname;
				
				return sysname;
			}	
			


/* fuck apple		#elif __APPLE__
			#include <sys/sysctl.h>

	*/			
		#elif BSD
			long s_uptime(){

				struct timespec ts;
				if(!clock_gettime(CLOCK_UPTIME_PRECISE, &ts)){
					cout << "Bad bsd time" << endl;
					exit(0);
				}
			
				long uptime = ts.tv_sec;
			}	

		#endif	
		
		using namespace std;

		//An array of strings for actual sign names
		string realSign[12] = {
		"Aquarius",
		"Pisces",
		"Aries",
		"Taurus",
		"Gemini",
		"Cancer",
		"Leo",
		"Virgo",
		"Libra",
		"Scorpio",
		"Sagittarius",
		"Capricorn"
		};


		//Get the passed argument into a string
		string getcipher(char *input){
			string dest;

			return dest;
		}


		//A switch to determine the sun sign of the computer
		int getsun(int monthday, int month){
			int sun;
			
			switch(month){
//January
				case 1:
					if(monthday < 20){
						sun = 0;
					}else{
						sun = 1;
					}
					break;
//February			
				case 2:
					if(monthday < 19){
						sun = 1;
					}else{
						sun = 2;
					}
					break;
//March			
				case 3:
					if(monthday < 21){
						sun = 2;
					}else{
						sun = 3;
					}
					break;
//April			
				case 4:
					if(monthday < 21){
						sun = 3;
					}else{
						sun = 4;
					}
					break;
//May			
				case 5:
					if(monthday < 21){
						sun = 4;
					}else{
						sun = 5;
					}
					break;
//June			
				case 6:
					if(monthday < 21){
						sun = 5;
					}else{
						sun = 6;
					}
					break;
//July			
				case 7:
					if(monthday < 23){
						sun = 6;
					}else{
						sun = 7;
					}
					break;
//August			
				case 8:
					if(monthday < 23){
						sun = 7;
					}else{
						sun = 8;
					}
					break;
//September			
				case 9:
					if(monthday < 23){
						sun = 8;
					}else{
						sun = 9;
					}
					break;
//October			
				case 10:
					if(monthday < 23){
						sun = 9;
					}else{
						sun = 10;
					}
					break;
//November			
				case 11:
					if(monthday < 23){
						sun = 10;
					}else{
						sun = 11;
					}
					break;
//December			
				case 12:
					if(monthday < 23){
						sun = 11;
					}else{
						sun = 0;
					}
					break;
				default:
					cout << "Error determining sun" << endl;
					exit(0);
					break;
			}

			return sun;
		}

		int getmoon(int uptime){
			int moon;
			const int INTERVAL = 720; //Number of minutes to determine sign ie. every 2 hours
			
			uptime /= INTERVAL;
			//Divide the number of INTERVAL hours into 12
			uptime %= 12;
			
			moon = uptime;

			return moon;
		}
		

/*
33 !                         1
35-38 # $ % &                5
42 *                         6
48-57 0 1 2 3 4 5 6 7 8 9   16
64 @                        17
65-90 UPPER                 43
94 ^                        44
97-122 lower                70
*/
		string use_cipher(string cipher, string password){
			int y = 0;

			for(int i = 0; i < (password.length() - 1) ; i++){
				y = 0;
				cout << "scrambling.." << endl;
				password[i] += cipher[i];
				//Make sure password is not above approved range
				if(password[i] > 122){
					password[i] %= 122;
				}
				if(password[i] < 33){
					while(password[i] < 33){
						y++;
						password[i] += (cipher[i] + y);
						if(password[i] > 122){
							password[i] %= 122;
						}
					}
				}
				if(password[i] == 34){
					password[i]--;
				}
				if( (password[i] > 38 && password[i] < 48 && !(password[i] == 42)) || (password[i] > 57 && password[i] < 64) || (password[i] < 97 && password[i] > 90 && !(password[i] == 94)) ){
					int ucase = 0;
					int letter = 0;
					letter = password[i] % 26;
					ucase = password[i] % 2;
					if(ucase){
						password[i] = letter + 65;
					}else{
						password[i] = letter + 97;
					}
				}
				cout << password[i] << endl << cipher[i] << endl;
			}
						return password;
		}
		

		string constraint_special(string password){
			char temp;
			//Ensure first character is a letter, as is required by some passwords
			while( password[0] < 65 || (password[0] > 90 && password[0] <97) ){
				temp = password[0];
				password = password.substr(1);
				password += temp;	
			}
			
			return password;	
		}


		int main(int argc, char **argv){
			long uptime;
			int sun;
			int moon;
			string cipher;
			string readingStr;
			string readingNum;
		
			//If user passes an argument, use it as a cipher, else use the host name
			if(argc > 1){	
				cipher += argv[1];
			}else{
				string sysname;
				sysname = getsysname(sysname);

				cipher += sysname;
			}
			cout << "Cipher is " << cipher << endl;

			//get uptime in seconds
			uptime = s_uptime();

			cout << uptime << endl;
			
			//Get current time since 1970
			time_t seconds;
			seconds = time(NULL);
			
			cout << seconds << endl;
			
			//Get date at uptime	
			seconds -= uptime;
			struct tm signdate;
			signdate = *localtime(&seconds);
			
			//Make month 1-12 instead of 0-11
			int realMonth = signdate.tm_mon + 1;	
			int realMonthday = signdate.tm_mday;	
			
			sun = getsun(realMonthday, realMonth);
			cout << "Sun sign is: " << realSign[sun] << endl;
			cout << realMonth << " " << realMonthday << endl;

			moon = getmoon(uptime);

			cout << "Moon is: " << realSign[moon] << endl;
				
			stringstream uppies;
			uppies << uptime;
			readingStr += realSign[sun];
			
			readingStr = use_cipher(uppies.str(), readingStr);

			stringstream stream;
			stream << sun << realMonth << realMonthday << moon;
			
			readingNum += stream.str();
			readingNum = use_cipher(cipher, readingNum);	
			
			readingStr = constraint_special(readingStr);

			cout << readingStr << readingNum << endl;
			
			cout << "Enter to continue...";
			getchar();

			return 0;
		}		
		//Compare uptime month, day, and hour into zodiac signs

	
