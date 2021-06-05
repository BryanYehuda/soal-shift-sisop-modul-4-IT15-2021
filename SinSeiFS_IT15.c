#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
 
char *dir_path = "/home/ianfelix/Downloads";
char *log_path = "/home/ianfelix/SinSeiFS.log";
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
    sprintf (fpath,"%s%s",dir_path,path);
    
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

static int xmp_mkdir(const char *path, mode_t mode){
    int hasil;
    
    char newPath[12000];
    
    if (strcmp(path, "/") == 0)
    {
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        char dinamis[12000];
        strcpy(dinamis, path);
        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    
    hasil = mkdir(newPath, mode);
    if (hasil == -1) return -errno;
    
    char deskripsi[12000];
    snprintf(deskripsi,sizeof deskripsi ,"MKDIR::%s", newPath);
    masukkanLog("INFO", deskripsi);
    
    return 0;
}

static int xmp_getattr(const char *path, struct stat *stbuf){
    int hasil;
    char sementara[12000];
    
    strcpy(sementara, path);

    if(strncmp(path, "/RX_", 4) == 0){
        decRot(sementara);decAtbash(sementara);
    }
    else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(sementara);}
    
    char newPath[12000];
    snprintf(newPath, sizeof newPath ,"%s%s", dir_path, sementara);
    
    hasil = lstat(newPath, stbuf);
    
    if(hasil == -1) return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
    char newPath[12000];
    char dinamis[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        strcpy(dinamis, path);
        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
 
    int success = 0;
    
    DIR *dp;
    struct dirent *de;
    
    (void)offset;
    (void)fi;
    
    dp = opendir(newPath);
 
    if(dp == NULL) return -errno;
    
    while ((de = readdir(dp)) != NULL){
    if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
    
    struct stat st;
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;
    
    char sementara[1000];
    strcpy(sementara, de->d_name);
    if (strncmp(path, "/AtoZ_", 5) == 0){encAtbash(sementara);
    }else if(strncmp(path, "/RX_", 4) == 0){encAtbash(sementara);rot13(sementara);}
    success = (filler(buf, sementara, &st, 0));
    if (success != 0) break;}
    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        char dinamis[1000];
        strcpy(dinamis, path);
        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    
    int ifSuccess = 0;
    int filDir = 0;
    
    (void)fi;
    
    filDir = open(newPath, O_RDONLY);
    if (filDir == -1) return -errno;
    ifSuccess = pread(filDir, buf, size, offset);
    if (ifSuccess == -1) ifSuccess = -errno;
    close(filDir);
    
    return ifSuccess;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev){
    int ifSuccess;
    
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        char dinamis[1000];
        strcpy(dinamis, path);
        
        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
 
    if (S_ISREG(mode)){
        ifSuccess = open(newPath, O_CREAT | O_EXCL | O_WRONLY, mode);
        if(ifSuccess >= 0)
            ifSuccess = close(ifSuccess);
        }else if (S_ISFIFO(mode)) ifSuccess = mkfifo(newPath, mode);
        else ifSuccess = mknod(newPath, mode, rdev);
    
    if (ifSuccess == -1) return -errno;
    
    char deskripsi[12000];
    snprintf(deskripsi,sizeof deskripsi ,"CREATE::%s", newPath);
    masukkanLog("INFO", deskripsi);
    
    return 0;
}

static int xmp_rename(const char *from, const char *to){
    int hasil;
    
    char lokasiSebelum[12000];
    if (strcmp(from, "/") == 0){
        from = dir_path;
        snprintf(lokasiSebelum, sizeof lokasiSebelum ,"%s", from);
    }else{
        char dinamis[12000];
        strcpy(dinamis, from);
        
        if(strncmp(from, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(from, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        
        snprintf(lokasiSebelum, sizeof lokasiSebelum ,"%s%s", dir_path, dinamis);
    }
    
    char lokasiSesudah[1000];
    if (strcmp(to, "/") == 0){
        to = dir_path;
        snprintf(lokasiSesudah, sizeof lokasiSesudah ,"%s", to);
    }else{
        char dinamis[1000];
        strcpy(dinamis, to);
    
        if(strncmp(to, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(to, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
    
        snprintf(lokasiSesudah, sizeof lokasiSesudah ,"%s%s", dir_path, dinamis);
    }
    
    hasil = rename(lokasiSebelum, lokasiSesudah);
    if (hasil == -1) return -errno;
    
    char deskripsi[12000];
    snprintf(deskripsi,sizeof deskripsi ,"RENAME::%s::%s", lokasiSebelum, lokasiSesudah);
    masukkanLog("INFO", deskripsi);
    
    return 0;
}

static int xmp_unlink(const char *path){
    int hasil;
    
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        char dinamis[12000];
        strcpy(dinamis, path);

        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
    
        snprintf(newPath,sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    
    hasil = unlink(newPath);
    if (hasil == -1) return -errno;
    
    char deskripsi[12000];
    snprintf(deskripsi, sizeof deskripsi ,"REMOVE::%s", newPath);
    masukkanLog("WARNING", deskripsi);
    return 0;
}

static int xmp_rmdir(const char *path){
    int hasil;
    
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        char dinamis[1000];
        strcpy(dinamis, path);
        
        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
    
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    hasil = rmdir(newPath);
    if (hasil == -1)
    return -errno;
    
    char deskripsi[12000];
    snprintf(deskripsi, sizeof deskripsi ,"RMDIR::%s", newPath);
    masukkanLog("WARNING", deskripsi);
    
    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int hasil;
    
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath,sizeof newPath ,"%s", path);
    }else{
        char dinamis[1000];
        strcpy(dinamis, path);

        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    
    hasil = open(newPath, fi->flags);
    if (hasil == -1) return -errno;
    
    close(hasil);
    return 0;
}

static int xmp_truncate(const char *path, off_t size){
    int hasil;
    
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath, sizeof newPath ,"%s", path);
    }else{
        char dinamis[1000];
        strcpy(dinamis, path);
        
        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    
    hasil = truncate(newPath, size);
    if (hasil == -1) return -errno;
    
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi){
    char newPath[12000];
    
    if (strcmp(path, "/") == 0){
        path = dir_path;
        snprintf(newPath,sizeof newPath ,"%s", path);
    }else{
        char dinamis[12000];
        strcpy(dinamis, path);

        if(strncmp(path, "/RX_", 4) == 0){
            decRot(dinamis);decAtbash(dinamis);
        }
        else if(strncmp(path, "/AtoZ_", 5) == 0){decAtbash(dinamis);}
        
        snprintf(newPath, sizeof newPath ,"%s%s", dir_path, dinamis);
    }
    
    int fileDir;
    int hasil;
    
    (void) fi;
    fileDir = open(newPath, O_WRONLY);
    if (fileDir == -1) return -errno;
    
    hasil = pwrite(fileDir, buf, size, offset);
    if (hasil == -1) hasil = -errno;
    
    close(fileDir);
    
    char deskripsi[12000];
    snprintf(deskripsi,sizeof deskripsi ,"WRITE::%s", newPath);
    masukkanLog("INFO", deskripsi);
    
    return hasil;
}

static struct fuse_operations xmp_oper = {
    .getattr  = xmp_getattr,
    .read     = xmp_read,
    .readdir  = xmp_readdir,
    .unlink   = xmp_unlink,
    .rmdir    = xmp_rmdir,
    .rename   = xmp_rename,
    .truncate = xmp_truncate,
    .mknod    = xmp_mknod,
    .open     = xmp_open,
    .write    = xmp_write,
    .mkdir    = xmp_mkdir,
};
 
int main(int argc, char *argv[]){
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
 
