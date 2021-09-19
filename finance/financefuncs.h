void my_charcat(char *dest, char *src){
	while(*dest != '\0'){
		*dest++;
	}
	*dest++ = *src;
	*dest = '\0';

}

void my_strcat(char *dest, char *src){
	while(*dest != '\0'){
		*dest++;
	}
	while(*src != '\0'){
		*dest++ = *src++;
	}
	*dest = '\0';
}
