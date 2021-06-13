# soal-shift-sisop-modul-4-IT15-2021

# Soal 1
```c
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
```

struktur fuse yang kami gunakan adalah seperti diatas :

getattr = Untuk mengembalikan attribute dari suatu file  
readdir = Untuk melakukan pembacaan directory  
read = untuk melakukan pembacaan file  
mkdir = untuk pembuatan suatu directory baru  
rmdir = untuk menghapus directory  
rename = untuk menganti nama sebuah directory ataupun file  
truncate = perpanjang file yang diberikan sehingga ukurannya tepat beberapa byte  
write = penulisan ke suatu file atau directory  
open = membuka file  
statfs = mengembalikan statistic dari file yang digunakan  
unlink = menghapus simbolik link dan file yang diberikan  
readlink = Jika jalur adalah tautan simbolik, isi buf dengan targetnya, hingga ukuran tertentu.  
mknod = Buat file (perangkat) khusus, FIFO, atau soket.  
symlink = Buat tautan simbolik bernama "dari" yang, ketika dievaluasi, akan mengarah ke "ke".  
link = Buat hardlink antara "dari" dan "ke".  
chmod = memodifikasi izin  
chown = mengganti kepemilikan file

a. Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.  

```c
static int xmp_mkdir(const char *path, mode_t mode){
	int result;
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);

	//kodelain...

	result = mkdir(filepath, mode);
	writeTheLog("MKDIR", filepath);

	if (result == -1) return -errno;
	return 0;
}
```
Ketika menggunakan mkdir, akan dibandingkan dengan strstr apakah direktory skrng terdapat variabel atoz(isinya AtoZ). jika terdapat panggil fungsi dekripsiMenjadiAtbash(a) lalu masukkan ke log dengan memanggil fungsi writeTheLog("MKDIR", filepath)

b. Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.  
c. Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.  
```c
static int xmp_rename(const char *from, const char *to){
	int result;
	char dariDir[1000], keDir[1000];
	
    //kodelain

	char *a = strstr(to, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
	//kodelain

	sprintf(dariDir, "%s%s", dirPath, from);
	sprintf(keDir, "%s%s", dirPath, to);

	result = rename(dariDir, keDir);
	if (result == -1) return -errno;

	writeTheLog2("RENAME", dariDir, keDir);
	
	if (c != NULL){lakukanEnkripsi(keDir);writeTheLog2("ENCRYPT2", from, to);}
	//kodelain

	return 0;
}
```
```c
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi){
	char filepath[1000];
	
	char *a = strstr(path, atoz);
	if (a != NULL) dekripsiMenjadiAtbash(a);
	
    //kodelain

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
		//kodelain
		result = (filler(buf, de->d_name, &st, 0));
		if (result != 0) break;
	}

	closedir(dp);
	return 0;
}
```
Ketika dipanggil rename, dia akan membandingkan file apakah pada nama file terdapat "AtoZ" jika terdapat lakukan deskripsi atbash dengan memanggil fungsi lakukanEnkripsi(keDir) lalu tulis ke dalam log RENAME dan dari posisi mana ke mana
jika direname kembali dengan menghilangkan AtoZ maka dia akan kembali kefungsi semula dengan memanggil fungsi deskripsiAtbash


Berikut adalah penjelasan bagaimana fungsi enkripsi dan deskripsi bekerja

Pada bagian ini kami diminta untuk melakukan enkripsi maupun dekripsi terhadap nama file dan nama folder menggunakan Atbash cipher. Untuk mendapatkan nama file dan nama folder yang diinginkan, kelompok kami melakukan looping untuk melakukan pengecekkan posisi awal berupa slash (/) dan posisi akhir berupa titik (.). Disini kami menerapkan 3 fungsi untuk mendapatkan index awal berupa slash (/) dan berupa titik (.) enkripsi dan dekripsi:
```c
int split_ext_id(char *path){
	int ada = 0;
	for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.'){
			if(ada == 1) return i;
			else ada = 1;
		}
	}
	return strlen(path);
}
```
Fungsi diatas dipanggil untuk mengembalikan index file extension pada file yang sudah dipisahkan. Pada program tersebut akan dilakukan perulangan sepanjang length dari path yang dipassing. setelah ketemu "." maka akan direturn titiknya berada pada posisi berapa
```c
int ext_id(char *path){
	for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.') return i;
	}
	return strlen(path);
}
```
fungsi diatas dipanggil untuk Mengembalikan index file extension berada pada posisi keberapa
```c
int slash_id(char *path, int mentok){
	for(int i=0; i<strlen(path); i++){
		if (path[i] == '/') return i + 1;
	}
	return mentok;
}
```
fungsi diatas dipanggil untuk Mengembalikan index slash berada pada posisi ke berapa. Inti dari program ini adalah melakukan perulangan pada path yang diterima. jika slash terdeteksi akan dikembalikan posisi slash+1

untuk melakukan enkripsi dan deskripsi seperti yang sudah dijelaskan diatas, saya menggunakan kode berikut ini :
```c
void encryptAtbash(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	printf("encrypt path Atbash: %s\n", path);
	
	int endid = split_ext_id(path);
	if(endid == strlen(path)) endid = ext_id(path);
	int startid = slash_id(path, 0);
	
	for (int i = startid; i < endid; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char tmp = path[i];
			if(isupper(path[i])) tmp -= 'A';
			else tmp -= 'a';
			tmp = 25 - tmp; //Atbash cipher
			if(isupper(path[i])) tmp += 'A';
			else tmp += 'a';
			path[i] = tmp;
		}
	}
}

void decryptAtbash(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	printf("decrypt path Atbash: %s\n", path);
	
	int endid = split_ext_id(path);
	if(endid == strlen(path)) endid = ext_id(path);
	int startid = slash_id(path, endid);
	
	for (int i = startid; i < endid; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char tmp = path[i];
			if(isupper(path[i])) tmp -= 'A';
			else tmp -= 'a';
			tmp = 25 - tmp; //Atbash cipher
			if(isupper(path[i])) tmp += 'A';
			else tmp += 'a';
			path[i] = tmp;
		}
	}
}
```

Untuk soal penulisan log akan lebih dijelaskan pada soal nomor 4. Untuk setiap fungsi rename, mkdir, getattr yang dijalankan oleh system sudah akan tercatat pada log. Dan untuk enkripsi dan deskripsi sudah dilakukan secara rekursi

## Hasil run
- Folder Awal

![image](https://user-images.githubusercontent.com/73151866/121809272-678e6000-cc86-11eb-8e11-5a1c0261e786.png)

- Setelah di enkripsi

![image](https://user-images.githubusercontent.com/73151866/121809281-6d844100-cc86-11eb-92d3-a49e677f1a73.png)

- Setelah di dekripsi

![image](https://user-images.githubusercontent.com/73151866/121809300-7f65e400-cc86-11eb-9f5a-f7297cdbffdf.png)

# Soal 2
- Menggunakan fungsi strstr(), akan dilakukan pengecekkan apakah suatu direktori dilakukan rename dengan menambahkan RX_ ataupun penghapusan RX_. Setelah kelompok kami lakukan pengecekkan apakah di dalam path tersebut ada string RX_. Jika ditemukan adanya string tersebut, maka akan dilakukan enkripsi filenya menggunakan atbash dan rot13 cipher sesuai dengan perintah soal. Hal yang sama akan diterapkan pada fungsi dekripsinya yaitu jika pada awalnya ada RX_ kemudian dilakukan penghapusan string RX_, maka akan dilakukan dekripsi pada filenya menggunakan atbash dan rot13 cipher sesuai dengan perintah soal.  

```c
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
```
- Jika terdeteksi adanya string RX_ pada path tujuan berarti telah dilakukan rename dengan menambahkan RX_ sehingga akan dilakukan dekripsi pada file menggunakan atbash dan rot13 cipher yang kemudian akan dilanjutkan dengan melakukan pemecahan file menjadi berukuran 1024 byte sesuai dengan perintah pada soal.
```c
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
```

- Jika terdeteksi string RX_ pada path asal dan tidak terdeteksi string RX_ pada path tujuan berarti telah dilakukan rename dengan menghilangkan RX_ sehingga akan dilakukan penggabungan file menjadi satu kesatuan dan kemudian dilakukan dekripsi menggunakan atbash dan rot13 cipher pada file tersebut.    
```c
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
```

- Untuk melakukan enkripsi dan dekripsi menggunakan ROT13 cipher akan dilakukan oleh fungsi berikut:
```c
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
```
- Pemanggilan fungsi dekripsi dilakukan pada tiap utility functions seperti getattr, mkdir, rename, rmdir, create, dan fungsi-fungsi lain yang menurut kelompok kami sering digunakan dalam proses sinkronisasi FUSE dan mount folder. Fungsi dekripsi dan enkripsi dilakukan di utility function readdir karena FUSE akan melakukan dekripsi di mount folder lalu enkripsi di FUSE saat readdir. Pemanggilannya dilakukan dengan melakukan pengecekan apakah string RX_ terdapat di string path di masing-masing utility function dengan menggunakan fungsi strstr(). Jika ditemukan adanya string RX_, maka fungsi enkripsi dan dekripsi akan dipanggil untuk string tersebut dengan RX_ sebagai starting point string yang diteruskan. Untuk pencatatan running log akan dijelaskan pada bagian nomor 4.

## Hasil run

- Folder Awal

![image](https://user-images.githubusercontent.com/73151866/121809317-8a207900-cc86-11eb-8c09-5b795ac6cc98.png)

- Setelah enkripsi

![image](https://user-images.githubusercontent.com/73151866/121809321-8ee52d00-cc86-11eb-8d76-456fd8d641af.png)

- Setelah file terpecah

![image](https://user-images.githubusercontent.com/73151866/121809817-8beb3c00-cc88-11eb-85e7-cd26bd2370f3.png)

- File di dalam filesystem

![image](https://user-images.githubusercontent.com/73151866/121809847-ad4c2800-cc88-11eb-8f27-e0dc0cbd58a5.png)

# Soal 3

- Untuk soal ini kelompok kami membuat utility functions RENAME direktri yang akan dilakukan pengecekan apakah direktori direname dengan menambahkan A_is_a_ dan menjadi direktori spesial atau menghilangkan A_is_a_ dan menjadi direktori biasa dengan fungsi strstr() yang dipanggil.
```c
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
```
- Selanjutnya terdeteksi direktori bernama A_is_a_ pada path tujuan berarti direktori direname dengan menambahkan A_is_a_ pada nama direktori . Maka akan dilanjutkan dengan mengubah nama file menjadi lowercase dan menambahkan nilai desimalnya sebagai ekstensi yang baru pada fungsi encryptBinary pada bagian file.
```c
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
```
- Selanjutnya apabila terdeteksi direktori A_is_a_ pada path asal dan tidak terdeteksi adanya A_is_a_ pada path tujuan berarti direktori direname dengan menghilangkan A_is_a_ maka direktori tersebut akan kembali menjadi direktori awal. Maka akan dilanjutkan dengan mengubah nama file menjadi semula dengan bantuan nilai desimalnya pada fungsi decryptBinary.
```c
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
```
Untuk pencatatan running log akan dijelaskan pada bagian nomor 4.

## Hasil run

- Kondisi awal

![image](https://user-images.githubusercontent.com/73151866/121809370-ba681780-cc86-11eb-8be3-0fd741093915.png)
 
- Setelah di enkripsi

![image](https://user-images.githubusercontent.com/73151866/121809374-bcca7180-cc86-11eb-8158-354b838b699c.png)

- Setelah di dekripsi

![image](https://user-images.githubusercontent.com/73151866/121809383-c358e900-cc86-11eb-9c41-24846334d4eb.png)

# Soal 4
- Untuk soal ini kami membuat sebuah log system untuk melakukan pencatatan aktivitas dalam file. Disini kelompok kami membuat dua fungsi dalam pembuatan log system ini yaitu fungsi writeTheLog dan writeTheLog2 dimana perbedaannya terdapat pada deskripsi atau informasi dan parameter tambahan yang perlu dicantumkan dalam format untuk menuliskan lognya.

Agar bisa menuliskan log system sesuai format yang ada, maka kelompok kami perlu mencari waktu saat ini untuk nanti dicantumkan dalam log systemnya. Dalam fungsi tulisLog kita juga memasukkan parameter char *nama yang mana adalah System Call dan char *fpath yang mana adalah deskripsi mengenai file yang ada.

```c
void writeTheLog(char *nama, char *filepath)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
```

- Selanjutnya kelompok kami melakukan inisialisasi sebuah array of char untuk menyimpan perintah system call yang telah dijalankan pada filesystem dan kemudian mencatatnya dalam file SinSeiFS.log.
```c
	char infoWriteLog[1000];
	
	FILE *file;
	file = fopen("/home/ianfelix/SinSeiFS.log", "a");
```

- Selanjutnya kelompok kami melakukan pengecekan pada syscall yang ada pada parameter. Jika syscall adalah RMDIR atau UNLINK maka log levelnya akan dicatat WARNING. Namun jika tidak, maka log levelnya akan dicatat INFO. Dan akan dilakukan juga pencatatan waktu sekarang beserta keterangan lainnya.
```c
	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(infoWriteLog, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, filepath);
	else
		sprintf(infoWriteLog, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, filepath);
```

- terakhir, kami tuliskan log yang ada kedalam file SinSeiFS.log dan kami tutup filenya.
```c
	fputs(infoWriteLog, file);
	fclose(file);
	return;
}
```

- Untuk fungsi yang kedua writeTheLog2 sebenarnya kurang lebih sama dengan fungsi writeTheLog yang sudah dijelaskan sebelumnya, namun terdapat perbedaan pada parameter yang diberikan dan pada pencatatannya. Untuk parameternya ada char *nama yang merupakan syscall, const char *from adalah keterangan file sebelum dilakukannya perintah system call yang dijalankan oleh file system dan const char *to keterangan file setelah dilakukannya perintah system call yang dijalankan oleh file system. Lalu pada pencatatannya kurang lebih sama dengan yang sebelumnya namun ditambahkan keterangan sesuai dengan parameter yang diberikan.
```c
void writeTheLog2(char *nama, const char *from, const char *to)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char infoWriteLog[1000];

	FILE *file;
	file = fopen("/home/kali/SinSeiFS.log", "a");

	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(infoWriteLog, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, from, to);
	else
		sprintf(infoWriteLog, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, from, to);

	fputs(infoWriteLog, file);
	fclose(file);
	return;
}
```
- Lalu untuk implementasinya kami masukkan fungsi-fungsi ini kedalam setiap fungsi system call yang ada.

## Hasil run

![image](https://user-images.githubusercontent.com/73151866/121809442-06b35780-cc87-11eb-9d85-c021a9ef5626.png)

## Kendala yang dialami
-  kesulitan dalam mengambil waktu sekarang (real time) dan pada pencatatannya.
-  bingung untuk mencatat System Call karena ada yang punya 1 atau 2 argumen. Akhirnya kami pisahkan menjadi 2 fungsi untut mencatat log.
-  Terkadang file file yang ada di Downloads tidak muncul semuanya ketika sudah dimount
-  Contoh kode dari asisten di github, sangat sederhana sehingga kami tidak punya acuan bagaimana cara mengoding dengan baik dan benar. Di internet juga referensinya sangat sedikit
-  Kebingungan memaknai folder special / directory special itu seperti apa
