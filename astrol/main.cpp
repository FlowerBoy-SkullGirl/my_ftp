#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

using namespace std;


		//Determine OS
		//Use OS-calls to determine uptime
		#ifdef _WIN32
			#include <windows>
			long s_uptime(){
				long uptime = GetTickCount64();
				return uptime;
			}	
		#elif __linux__
			#include <linux/unistd.h>
			#include <linux/kernel.h> 
			#include <sys/sysinfo.h>

			long s_uptime(){
				struct sysinfo info;
				if(sysinfo(&info)){
					cout << "Bad sysinfo" << endl;
					exit(0);
				}
				return info.uptime;
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
		
		int main(int argc, char **argv){
			long uptime;
			int sun;
			int moon;
			string cipher;
		
			//If user passes an argument, use it as a cipher, else use the host name
			if(argc > 1){	
				cipher += getcipher(argv[1]);
			}else{
				cipher += getsysname();
			}

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
			cout << "Sun sign is: " << sun << endl;
			cout << realMonth << " " << realMonthday << endl;

			moon = getmoon(uptime);

			cout << "Moon is: " << moon << endl;

			return 0;
		}		
		//Compare uptime month, day, and hour into zodiac signs

	
