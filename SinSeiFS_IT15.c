#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

static const char *dirPath = "/home/ianfelix/Downloads";

char *key = "SISOP";
char *atoz = "AtoZ_";
char *rx = "RX_";
char *aisa = "A_is_a_";
int x = 0;

char upper_case[]={'Z', 'Y', 'X', 'W', 'V', 'U',
                   'T', 'S', 'R', 'Q', 'P', 'O',
                   'N', 'M', 'L', 'K', 'J', 'I', 
                   'H', 'G', 'F', 'E', 'D', 'C', 'B', 'A'};
               
char lower_case[]={'z', 'y', 'x', 'w', 'v', 'u',
                    't', 's', 'r', 'q', 'p', 'o',
                    'n', 'm', 'l', 'k', 'j', 'i',
                    'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};    
char atbash[1000] = "~!@#$%^&*(~!@#$%^&*()-=_+,./?>)-=_+,./?><;:[]}{\|\<;:[]}{|\0123457890123456789ABCDEFGHIJZYXWVUTSRQABCDEFGHIJKLMNOPQRSTPONMLKJIHGKLMNOPQRSTabcdefghijzyxwvutsrqabcdefghijklmnopqrstponmlkjihgklmnopqrst";
char rot[1000] = "~!@#$%^&*(~!@#$%^&*()-=_+,./?>)-=_+,./?><;:[]}{\|\<;:[]}{|\0123457890123456789ABCDEFGHIJNOPQRSTUVWABCDEFGHIJKLMNOPQRSTXYZABCDEFGKLMNOPQRSTabcdefghijnopqrstuvwabcdefghijklmnopqrstxyzabcdefgklmnopqrst";

void enc(char* encrypt)
{
  if(!strcmp(encrypt,".") || !strcmp(encrypt,"..")) return;
  char temp[1000];
  sprintf(temp, "%s",encrypt);
    int a = strlen(encrypt);
    for(int i=0; i<a; i++){
      encrypt[i]=temp[a-i-1];
    }
}
 
void dec(char* decrypt)
{
  if(!strcmp(decrypt,".") || !strcmp(decrypt,"..")) return;
  char temp[1000];
  sprintf(temp, "%s",decrypt);
    int a = strlen(decrypt);
    for(int i=0; i<a; i++){
      decrypt[i]=temp[a-i-1];
    }
}

void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}

void cobaAtbashGagal(char *message)
{
    int len = strlen(message);                   // Calculating lenght of the string
    // printf("len %d\n", len);
    char cipher[1024];
    int ascii_char;
    char result[1024];
    for(int i=0;i<len;i++)
    {
        ascii_char = message[i];                         // Storing ASCII value of alphabet
        
        if(ascii_char>='A' && ascii_char<='Z')
        message[i] = upper_case[ascii_char-65];    // If character is in upper case(minus 65 from its ASCII value to get reversed character)
        else if(ascii_char>='a' && ascii_char<='z')
        message[i] = lower_case[ascii_char-97];    // If character is in upper case(minus 97 from its ASCII value to get reversed character)
        else if(ascii_char == 46) break;
        else continue;
        // printf("msg %d\n", message[i]);
    }
    
}
char *log_path = "/home/ianfelix/SinSeiFS.log";
void masukkanLog(char *tingkatan, char *infonih){
    char waktu[100];
    char log[100];
    time_t now = time(NULL);
    struct tm *info = localtime(&now);
    FILE * files;
    files = fopen (log_path, "a+");
    strftime(waktu, 100, "%y%m%d-%H:%M:%S", info);
    snprintf(log, sizeof log ,"%s::%s::%s\n", tingkatan, time, infonih);
    fputs(log, files);
    fclose(files);
}

void vignereChiper(){
     int i,j,k,numstr[100],numkey[100],numcipher[100];
    char str[100],key[100];
    printf("Enter a string\n");
    gets(str);
    //converting entered string to Capital letters
    for(i=0,j=0;i<strlen(str);i++){
        if(str[i]!=' '){
            str[j]=toupper(str[i]);   
            j++;
        }
    }
    str[j]='\0';
    printf("Entered string is : %s \n",str);
    //Storing string in terms of ascii
    for(i=0;i<strlen(str);i++){
        numstr[i]=str[i]-'A';
    }
    printf("Enter a key\n");
    gets(key);
    //converting entered key to Capital letters
    for(i=0,j=0;i<strlen(key);i++){
        if(key[i]!=' '){
            key[j]=toupper(key[i]);   
            j++;
        }
    }
    key[j]='\0';
        //Assigning key to the string
    for(i=0;i<strlen(str);){
        for(j=0;(j<strlen(key))&&(i<strlen(str));j++){
            numkey[i]=key[j]-'A';
            i++;
        }
    }
    
    for(i=0;i<strlen(str);i++){
        numcipher[i]=numstr[i]+numkey[i];
    }
    for(i=0;i<strlen(str);i++){
        if(numcipher[i]>25){
            numcipher[i]=numcipher[i]-26;
        }
    }
    printf("Vigenere Cipher text is\n");   
    for(i=0;i<strlen(str);i++){
        printf("%c",(numcipher[i]+'A')); 
    }
        
    printf("\n");
}

int cekLength(char *plain, int i){
    int length = i;
    while(i>=0){
        if(plain[i] == '/') break;
    
        if(plain[i] == '.'){
            length = i - 1; break;
        }
        i--;
    }
    return length;
}

int cekMulai(char *plain, int j, int length){
    int mulai = 0;
    while(j < length){
        if (plain[j] == '/') mulai = j;
        j++;
    }
    return mulai;
}

void getEnc(char *plain,int k,int length, char *kodeEnkripsi){
    while(k <= length){
        if(plain[k] == '/') continue;
    
        int urutanKode; 
        urutanKode = 0;
        while(1){
            if(strcmp(kodeEnkripsi, "atbash")==0 && plain[k] == atbash[urutanKode]){
                plain[k] = atbash[urutanKode+10]; break;
            }else if(strcmp(kodeEnkripsi, "atbash")==0 && plain[k] != atbash[urutanKode]){
                plain[k]=plain[k];
            }else if(strcmp(kodeEnkripsi, "rot")==0 && plain[k] == rot[urutanKode]){
                plain[k] = rot[urutanKode+10]; break;
            }else if(strcmp(kodeEnkripsi, "rot")==0 && plain[k] == rot[urutanKode]){
                plain[k]=plain[k];
            }
            urutanKode++;
        }
        k++;
    }
}

void encAtbash(char *plain){
    int length = strlen(plain);
    int mulai;
    int i = length;
    length = cekLength(plain, i);
    int j=1;
    mulai = cekMulai(plain, j, length);
    int k= mulai;
    getEnc(plain,k, length, "atbash");
}

void rot13(char *plain){
    int length = strlen(plain);
    int mulai;
    int i = length;
    length = cekLength(plain, i);

    int k = 1;
    mulai = cekMulai(plain, k, length);
    int j= mulai;
    getEnc(plain,j, length, "rot");
}

int cekMulaiDec(int i, int length, char *plain){
    int mulai = 0;
    while(i<length){
        if(plain[i + 1] == '\0' || plain[i] == '/'  ){
            mulai = i + 1; break;
        }
        i++;
    }
    return mulai;
}

int cekLengthDec(char *plain, int i){
    int length = i;
    while(i>=0){    
        if(plain[i] == '.'){
            length = i - 1; break;
        }
        i--;
    }
    return length;
}

void getDec(char *plain, int j, char *kodeEnkripsi, int urutanKode){
    while(1)
    {
        if(strcmp(kodeEnkripsi, "atbash")==0 && plain[j] == atbash[urutanKode]){
            plain[j] = atbash[urutanKode-10]; break;
        }else if(strcmp(kodeEnkripsi, "rot")==0 && plain[j] == rot[urutanKode]){
            plain[j] = rot[urutanKode-10]; break;
        }
        urutanKode--;
    }
}

void decAtbash1(char *plain){
    int length, i, mulai,k,j;
    i = 1;
    length = strlen(plain);
    mulai = cekMulaiDec(i,length, plain);
    k = length;
    length = cekLengthDec(plain,k);
    j = mulai;
    int urutanKode = strlen(atbash) - 1;
    // getDec(plain, j, length, "atbash", urutanKode);
}

void decRot1(char *plain){
    int length, i, mulai,k,j;
    i = 1;
    length = strlen(plain);
    mulai = cekMulaiDec(i,length, plain);
    k = length;
    length = cekLengthDec(plain,k);
    j = mulai;
    int urutanKode = strlen(rot) - 1;
    // getDec(plain, j, length, "rot", urutanKode);
}

void decAtbash(char *plain){
    int length = strlen(plain);
    int mulai = 0;

    int i;
    i = 1;
    while(i<length){
        if(plain[i + 1] == '\0' || plain[i] == '/'  ){mulai = i + 1; break;}i++;
    }

    for (int j = strlen(plain); j >= 0; j--){
        if (plain[j] == '.'){length = j - 1; break;}
    }
 
    for (int k = mulai; k <= length; k++){
        if(plain[k] == '/') continue;
        
        int kodeUrutan; 
        kodeUrutan = strlen(atbash) - 1;
        getDec(plain, k, "atbash", kodeUrutan);
    }
}

void decRot(char *plain){
    int length = strlen(plain);
    int mulai = 0;

    int i;
    i = 1;
    while(i<length){
        if(plain[i + 1] == '\0' || plain[i] == '/'  ){mulai = i + 1; break;}i++;
    }

    for (int j = strlen(plain); j >= 0; j--){
        if (plain[j] == '.'){length = j - 1; break;}
    }
 
    for (int k = mulai; k <= length; k++){
        if(plain[k] == '/')continue;
        
        int kodeUrutan; 
        kodeUrutan = strlen(atbash) - 1;
        getDec(plain, k, "rot", kodeUrutan);
    }
}

static int xmp_readdirGagal(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;
    char fpath[1000];
    sprintf (fpath,"%s%s",dirPath,path);
    
    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;
        // printf("INI DE : %s\n", de->d_name);
        char namafiles[10000];
        snprintf(namafiles, sizeof namafiles, "%s", de->d_name);
        if(strstr(namafiles, "AtoZ_")){
            chdir("/home/ianfelix/Downloads");
            // system("pwd");
            char lokasi[10000];
            snprintf(lokasi, sizeof lokasi, "./%s", de->d_name); 
            // listFilesRecursively(lokasi);
            // printf("%s\n",lokasi);
            // opendir(lokasi);
        }
        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(filler(buf, de->d_name, &st, 0)) break;
    }
    closedir(dp);
    return 0;
}

void writeTheLog(char *nama, char *filepath)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
 
	char infoWriteLog[1000];

	FILE *file;
	file = fopen("/home/ianfelix/SinSeiFS.log", "a");

	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(infoWriteLog, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, filepath);
	else
		sprintf(infoWriteLog, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, filepath);

	fputs(infoWriteLog, file);
	fclose(file);
	return;
}

void writeTheLog2(char *nama, const char *from, const char *to)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char infoWriteLog[1000];

	FILE *file;
	file = fopen("/home/ianfelix/SinSeiFS.log", "a");

	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(infoWriteLog, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, from, to);
	else
		sprintf(infoWriteLog, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, from, to);

	fputs(infoWriteLog, file);
	fclose(file);
	return;
}

int pisahkanExt(char *path){
	int flag = 0;
	for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.'){if(flag == 1) return i; else flag = 1;}
	}
	return strlen(path);
}

int extensionId(char *path){
	for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.') return i;
	}
	return strlen(path);
}

int pemisahId(char *path, int hasil){
	for(int i=0; i<strlen(path); i++){
		if (path[i] == '/') return i + 1;
	}
	return hasil;
}

void enkripsiMenjadiAtbash(char *path){
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	int idAkhir = pisahkanExt(path);
	if(idAkhir == strlen(path)){
        idAkhir = extensionId(path);
    }
	int idAwal = pemisahId(path, 0);
	
	for (int i = idAwal; i < idAkhir; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char sementara = path[i];
			if(isupper(path[i])) sementara -= 'A'; else sementara -= 'a'; sementara = 25 - sementara; 
			if(isupper(path[i])) sementara += 'A'; else sementara += 'a'; path[i] = sementara;
		}
	}
}

void dekripsiMenjadiAtbash(char *path){
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	int idAkhir = pisahkanExt(path);
	if(idAkhir == strlen(path)){
        idAkhir = extensionId(path);
    }
	int idAwal = pemisahId(path, idAkhir);
	
	for (int i = idAwal; i < idAkhir; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char sementara = path[i];
			if(isupper(path[i])) sementara -= 'A'; else sementara -= 'a'; sementara = 25 - sementara;
			if(isupper(path[i])) sementara += 'A'; else sementara += 'a'; path[i] = sementara;
		}
	}
}

void enkripsiMenjadiRot13(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	
	int idAkhir = pisahkanExt(path);
	int idAwal = pemisahId(path, 0);
	
	for (int i = idAwal; i < idAkhir; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char sementara = path[i];
			if(isupper(path[i])) sementara -= 'A'; else sementara -= 'a'; sementara = (sementara + 13) % 26;
			if(isupper(path[i])) sementara += 'A'; else sementara += 'a';
			path[i] = sementara;
		}
	}
}

void dekripsiMenjadiRot13(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	
	int idAkhir = pisahkanExt(path);
	int idAwal = pemisahId(path, idAkhir);
	
	for (int i = idAwal; i < idAkhir; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char sementara = path[i];
			if(isupper(path[i])) sementara -= 'A'; else sementara -= 'a'; sementara = (sementara + 13) % 26;
			if(isupper(path[i])) sementara += 'A'; else sementara += 'a';
			path[i] = sementara;
		}
	}
}

void enkripsiMenjadiVigenere(char *path){
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	
	int idAkhir = pisahkanExt(path);
	int idAwal = pemisahId(path, 0);
	
	for (int i = idAwal; i < idAkhir; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char sementara1 = path[i]; char sementara2 = key[(i-idAwal) % strlen(key)];
			if(isupper(path[i])){sementara1 -= 'A'; sementara2 -= 'A'; sementara1 = (sementara1 + sementara2) % 26; sementara1 += 'A';}
			else{ sementara1 -= 'a'; sementara2 = tolower(sementara2) - 'a'; sementara1 = (sementara1 + sementara2) % 26; sementara1 += 'a';}
			path[i] = sementara1;
		}
	}
}

void desripsiMenjadiVigenere(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	
	int idAkhir = pisahkanExt(path);
	int idAwal = pemisahId(path, idAkhir);
	
	for (int i = idAwal; i < idAkhir; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char sementara1 = path[i];
			char sementara2 = key[(i-idAwal) % strlen(key)];
			if(isupper(path[i])){sementara1 -= 'A'; sementara2 -= 'A';sementara1 = (sementara1 - sementara2 + 26) % 26; sementara1 += 'A';}
			else{sementara1 -= 'a'; sementara2 = tolower(sementara2) - 'a'; sementara1 = (sementara1 - sementara2 + 26) % 26; sementara1 += 'a';}
			path[i] = sementara1;
		}
	}
}

void lakukanEnkripsi(char *filepath){
	chdir(filepath);
	DIR *dp;
	struct dirent *dir;
	
	dp = opendir(".");
	if(dp == NULL) return;
	struct stat lol;
	char dirPath[1000];
	char filePath[1000];
	
    while ((dir = readdir(dp)) != NULL){
		if (stat(dir->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;sprintf(dirPath,"%s/%s",filepath, dir->d_name);lakukanEnkripsi(dirPath);
		}
		else{
			sprintf(filePath,"%s/%s",filepath,dir->d_name);
			FILE *input = fopen(filePath, "r");
			if (input == NULL) return;
			int baca_byte, hitung = 0;
			void *buffer = malloc(1024);
			while((baca_byte = fread(buffer, 1, 1024, input)) > 0){
				char newFilePath[1000];
				sprintf(newFilePath, "%s.%04d", filePath, hitung);
				hitung++;
				FILE *keluaranEnc = fopen(newFilePath, "w+");
				if(keluaranEnc == NULL) return;
				fwrite(buffer, 1, baca_byte, keluaranEnc);
				fclose(keluaranEnc);
				memset(buffer, 0, 1024);
			}
			fclose(input);
			remove(filePath);
		}
	}
    closedir(dp);
}

void lakukanDekripsi(char *dir){
	chdir(dir);
	DIR *dp;
	struct dirent *de;
	struct stat lol;
	dp = opendir(".");
	if (dp == NULL) return;
	
	char dirPath[1000];
	char filePath[1000];
	
	while ((de = readdir(dp)) != NULL){
		if (stat(de->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;sprintf(dirPath, "%s/%s", dir, de->d_name);lakukanDekripsi(dirPath);
		}
		else{
			sprintf(filePath, "%s/%s", dir, de->d_name);
			filePath[strlen(filePath) - 5] = '\0';
			FILE *check = fopen(filePath, "r");
			if (check != NULL) return;
			FILE *file = fopen(filePath, "w");
			int hitung = 0;
			char keDir[1000];
			sprintf(keDir, "%s.%04d", filePath, hitung);
			void *buffer = malloc(1024);
			while (1){
				FILE *op = fopen(keDir, "rb");
				if (op == NULL) break;
				size_t readSize = fread(buffer, 1, 1024, op);
				fwrite(buffer, 1, readSize, file);
				fclose(op);
				remove(keDir);
				hitung++;
				sprintf(keDir, "%s.%04d", filePath, hitung);
			}
			free(buffer);
			fclose(file);
		}
	}
	closedir(dp);
}

void ambilBiner(char *fname, char *bin, char *lowercase){
	int idAkhir = extensionId(fname);
	int idAwal = pemisahId(fname, 0);
	int i;
	
	for(i=idAwal; i<idAkhir; i++){
		if(isupper(fname[i])){
			bin[i] = '1';lowercase[i] = fname[i] + 32;
		}
		else{
			bin[i] = '0';lowercase[i] = fname[i];
		}
	}
	bin[idAkhir] = '\0';
	
	for(; i<strlen(fname); i++){
		lowercase[i] = fname[i];
	}
	lowercase[i] = '\0';
}

int bin2dec(char *bin){
	int sementara = 1, result = 0;
	for(int i=strlen(bin)-1; i>=0; i--){if(bin[i] == '1') result += sementara; sementara *= 2;}
	return result;
}

void encryptBinary(char *filepath){
	chdir(filepath);
	DIR *dp;
	struct dirent *dir;
	struct stat lol;
	dp = opendir(".");
	if(dp == NULL) return;
	
	char dirPath[1000];
	char filePath[1000];
	char filePathBinary[1000];
	
    while ((dir = readdir(dp)) != NULL){
		if (stat(dir->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;sprintf(dirPath,"%s/%s",filepath, dir->d_name);encryptBinary(dirPath);
		}else{
			sprintf(filePath,"%s/%s",filepath, dir->d_name);
			char bin[1000], lowercase[1000]; ambilBiner(dir->d_name, bin, lowercase);
			int dec = bin2dec(bin);
			sprintf(filePathBinary,"%s/%s.%d",filepath,lowercase,dec); rename(filePath, filePathBinary);
		}
	}
    closedir(dp);
}

int convertDec(char *ext){
	int dec = 0, pengali = 1;
	for(int i=strlen(ext)-1; i>=0; i--){dec += (ext[i]-'0')*pengali;pengali *= 10;}
	return dec;
}

void dec2bin(int dec, char *bin, int len){
	int idx = 0;
	while(dec){
		if(dec & 1) bin[idx] = '1';
		else bin[idx] = '0';idx++;
		dec /= 2;
	}
	while(idx < len){
		bin[idx] = '0'; idx++;
	}
	bin[idx] = '\0';
	
	for(int i=0; i<idx/2; i++){
		char sementara = bin[i];bin[i] = bin[idx-1-i];bin[idx-1-i] = sementara;
	}
}

void getDecimal(char *fname, char *bin, char *normalcase){
	int idAkhir = extensionId(fname);
	int idAwal = pemisahId(fname, 0);
	int i;
	
	for(i=idAwal; i<idAkhir; i++){
		if(bin[i-idAwal] == '1') normalcase[i-idAwal] = fname[i] - 32;
		else normalcase[i-idAwal] = fname[i];
	}
	
	for(; i<strlen(fname); i++){
		normalcase[i-idAwal] = fname[i];
	}
	normalcase[i-idAwal] = '\0';
}

void decryptBinary(char *filepath){
	chdir(filepath);
	DIR *dp;
	struct dirent *dir;
	struct stat lol;
	dp = opendir(".");
	if(dp == NULL) return;
	
	char dirPath[1000];
	char filePath[1000];
	char filePathDecimal[1000];
	
    while ((dir = readdir(dp)) != NULL){
		if (stat(dir->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;sprintf(dirPath,"%s/%s",filepath, dir->d_name);decryptBinary(dirPath);
		}
		else{
			sprintf(filePath,"%s/%s",filepath, dir->d_name);
			char fname[1000], bin[1000], normalcase[1000], clearPath[1000];
			
			strcpy(fname, dir->d_name);
			char *ext = strrchr(fname, '.');
			int dec = convertDec(ext+1);
			for(int i=0; i<strlen(fname)-strlen(ext); i++) clearPath[i] = fname[i];
			
			char *ext2 = strrchr(clearPath, '.');
			dec2bin(dec, bin, strlen(clearPath)-strlen(ext2));getDecimal(clearPath, bin, normalcase);sprintf(filePathDecimal,"%s/%s",filepath,normalcase);rename(filePath, filePathDecimal);
		}
	}
    closedir(dp);
}

static int xmp_getattr(const char *path, struct stat *stbuf){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = lstat(filepath, stbuf);
	if (result == -1) return -errno;
	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	if (x != 24) x++;
	else writeTheLog("READDIR", filepath);

	int result = 0;
	DIR *dp;
	struct dirent *de;

	(void)offset;
	(void)fi;

	dp = opendir(filepath);
	if (dp == NULL) return -errno;

	while ((de = readdir(dp)) != NULL){
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;

		struct stat st;
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;

		if (a != NULL) enkripsiMenjadiAtbash(de->d_name);
		if (b != NULL){enkripsiMenjadiAtbash(de->d_name);enkripsiMenjadiRot13(de->d_name);}

		result = (filler(buf, de->d_name, &st, 0));
		if (result != 0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b); dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath; sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	int result = 0;
	int fd = 0;

	(void)fi;
	writeTheLog("READ", filepath);

	fd = open(filepath, O_RDONLY);
	if (fd == -1) return -errno;

	result = pread(fd, buf, size, offset);
	if (result == -1) result = -errno;

	close(fd);
	return result;
}

static int xmp_mkdir(const char *path, mode_t mode){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = mkdir(filepath, mode);
	writeTheLog("MKDIR", filepath);

	if (result == -1) return -errno;
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){
		dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);
	}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = rmdir(filepath);
	writeTheLog("RMDIR", filepath);

	if (result == -1) return -errno;
	return 0;
}

static int xmp_rename(const char *from, const char *to){
	int result;
	char dariDir[1000], keDir[1000];
	
	char *a = strstr(to, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(from, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}
	
	char *c = strstr(to, rx);
	if (c != NULL){dekripsiMenjadiRot13(c);dekripsiMenjadiAtbash(c);}

	sprintf(dariDir, "%s%s", dirPath, from);
	sprintf(keDir, "%s%s", dirPath, to);

	result = rename(dariDir, keDir);
	if (result == -1) return -errno;

	writeTheLog2("RENAME", dariDir, keDir);
	
	if (c != NULL){lakukanEnkripsi(keDir);writeTheLog2("ENCRYPT2", from, to);}
	if (b != NULL && c == NULL){lakukanDekripsi(keDir);writeTheLog2("DECRYPT2", from, to);}
	if (strstr(to, aisa) != NULL){encryptBinary(keDir);writeTheLog2("ENCRYPT3", from, to);}
	if (strstr(from, aisa) != NULL && strstr(to, aisa) == NULL){decryptBinary(keDir);writeTheLog2("DECRYPT3", from, to);}

	return 0;
}

static int xmp_truncate(const char *path, off_t size){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	writeTheLog("TRUNC", filepath);
	result = truncate(filepath, size);
	
	if (result == -1) return -errno;
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
	int fd;
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	(void)fi;
	writeTheLog("WRITE", filepath);

	fd = open(filepath, O_WRONLY);
	if (fd == -1) return -errno;

	result = pwrite(fd, buf, size, offset);
	if (result == -1) result = -errno;

	close(fd);
	return result;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	(void)fi;
	writeTheLog("CREATE", filepath);
	
	result = creat(filepath, mode);	
	if (result == -1) return -errno;

	close(result);
	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	result = utimes(filepath, tv);
	if (result == -1) return -errno;
	return 0;
}

static int xmp_access(const char *path, int mask){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = access(filepath, mask);
	if (result == -1) return -errno;
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = open(filepath, fi->flags);

	writeTheLog("OPEN", filepath);
	if (result == -1) return -errno;

	close(result);
	return 0;
}

static int xmp_unlink(const char *path)
{
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = unlink(filepath);
	writeTheLog("UNLINK", filepath);

	if (result == -1) return -errno;
	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = readlink(filepath, buf, size - 1);
	writeTheLog("READLINK", filepath);
	if (result == -1) return -errno;

	buf[result] = '\0';
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	writeTheLog("MKNOD", filepath);
	
	if (S_ISREG(mode)){
		result = open(filepath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (result >= 0) result = close(result);
	}else if (S_ISFIFO(mode)) result = mkfifo(filepath, mode);
	else result = mknod(filepath, mode, rdev);
	
	if (result == -1) return -errno;
	return 0;
}

static int xmp_symlink(const char *from, const char *to){
	int result;
	char dariDir[1000], keDir[1000];
	
	char *a = strstr(to, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(from, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}
	
	char *c = strstr(to, rx);
	if (c != NULL){dekripsiMenjadiRot13(c);dekripsiMenjadiAtbash(c);}

	sprintf(dariDir, "%s%s", dirPath, from);
	sprintf(keDir, "%s%s", dirPath, to);

	result = symlink(dariDir, keDir);
	writeTheLog2("SYMLINK", dariDir, keDir);
	
	if (result == -1) return -errno;
	return 0;
}

static int xmp_link(const char *from, const char *to)
{
	int result;
	char dariDir[1000], keDir[1000];
	
	char *a = strstr(to, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(from, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}
	
	char *c = strstr(to, rx);
	if (c != NULL){dekripsiMenjadiRot13(c);dekripsiMenjadiAtbash(c);}

	sprintf(dariDir, "%s%s", dirPath, from);
	sprintf(keDir, "%s%s", dirPath, to);

	result = link(dariDir, keDir);
	writeTheLog2("LINK", dariDir, keDir);

	if (result == -1) return -errno;
	return 0;
}

static int xmp_chmod(const char *path, mode_t mode){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = chmod(filepath, mode);
	writeTheLog("CHMOD", filepath);

	if (result == -1) return -errno;
	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = lchown(filepath, uid, gid);
	writeTheLog("CHOWN", filepath);
	
	if (result == -1) return -errno;
	return 0;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	char *b = strstr(path, rx);
	if (b != NULL){dekripsiMenjadiRot13(b);dekripsiMenjadiAtbash(b);}

	if (strcmp(path, "/") == 0){path = dirPath;sprintf(filepath, "%s", path);}
	else{sprintf(filepath, "%s%s", dirPath, path);}

	result = statvfs(filepath, stbuf);
	writeTheLog("STATFS", filepath);
	
	if (result == -1) return -errno;
	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.write = xmp_write,
	.create = xmp_create,
	.utimens = xmp_utimens,
	.access = xmp_access,
	.open = xmp_open,
	.unlink = xmp_unlink,
	.readlink = xmp_readlink,
	.mknod = xmp_mknod,
	.symlink = xmp_symlink,
	.link = xmp_link,
	.chmod = xmp_chmod,
	.chown = xmp_chown,
	.statfs = xmp_statfs,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
