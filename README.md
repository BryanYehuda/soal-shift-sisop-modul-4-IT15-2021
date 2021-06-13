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

Pada bagian ini kami diminta untuk melakukan enkripsi maupun dekripsi terhadap nama file dan nama folder menggunakan Atbash cipher. Untuk mendapatkan nama file dan nama folder yang diinginkan, kelompok kami melakukan looping untuk melakukan pengecekkan posisi awal berupa slash (/) dan posisi akhir berupa titik (.). Disini kami menerapkan 3 fungsi untuk mendapatkan index awal berupa slash (/) dan berupa titik (.) enkripsi dan dekripsi:  
- slash_id : Mengembalikan index slash
- ext_id : Mengembalikan index file extension
- split_ext_id : Mengembalikan index file extension pada file yang displit

```c 
int split_ext_id(char *path)
{
	int ada = 0;
	for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.'){
			if(ada == 1) return i;
			else ada = 1;
		}
	}
	return strlen(path);
}

int ext_id(char *path)
{
	for(int i=strlen(path)-1; i>=0; i--){
		if (path[i] == '.') return i;
	}
	return strlen(path);
}

int slash_id(char *path, int mentok)
{
	for(int i=0; i<strlen(path); i++){
		if (path[i] == '/') return i + 1;
	}
	return mentok;
}
```

- Untuk melakukan enkripsi dan dekripsi dengan Atbash cipher akan dibuat fungsi berikut ini:

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
- Pemanggilan fungsi dekripsi dilakukan pada tiap utility functions seperti getattr, mkdir, rename, rmdir, create, dan fungsi-fungsi lain yang menurut kelompok kami sering digunakan dalam proses sinkronisasi FUSE dan mount folder. Fungsi dekripsi dan enkripsi akan dilakukan di utility function readdir karena FUSE akan melakukan dekripsi di mount folder lalu dilakukan enkripsi di FUSE saat readdir. Pemanggilannya dilakukan dengan pengecekan apakah string AtoZ_ terdapat di string path di masing-masing utility function dengan menggunakan fungsi strstr(). Jika terdeteksi ada string AtoZ_, maka fungsi enkripsi dan dekripsi akan dipanggil untuk string tersebut dengan AtoZ_ sebagai starting point string yang diteruskan. Untuk pencatatan hasil running log akan dijelaskan pada bagian nomor 4.

## Hasil run
- Folder Awal

![image](https://user-images.githubusercontent.com/73151866/121809272-678e6000-cc86-11eb-8e11-5a1c0261e786.png)

- Setelah di enkripsi

![image](https://user-images.githubusercontent.com/73151866/121809281-6d844100-cc86-11eb-92d3-a49e677f1a73.png)

- Setelah di dekripsi

![image](https://user-images.githubusercontent.com/73151866/121809300-7f65e400-cc86-11eb-9f5a-f7297cdbffdf.png)

## Kendala yang Dihadapi


# Soal 2
- Pada utility functions RENAME akan kami lakukan pengecekan apakah direktori dilakukan rename dengan menambahkan RX_ ataupun menghilangkan RX_ dengan fungsi strstr().
Kemudian kami akan melakukan pengecekkan apakah di dalam path tersebut terdapat string RX_. Jika ditemukan terdapat string RX_ maka akan dienkripsi file file yang ada di dalam direktorinya. Disini kelompok kami menggunakan atbash dan rot13 cipher sesuai perintah di dalam soal, dimana untuk penggunaan atbash sudah dijelaskan pada bagian 1. Selanjutnya  file tadi yang sudah terenkripsi dengan atbash akan dilakukan enkripsi lebih lanjut dengan rot13. Begitu juga dengan fungsi dekripsi pada pencarian path aslinya. Kelompok kami perlu melakukan dekripsi menggunakan atbash dan rot13 cipher tadi.  

```c
static int xmp_rename(const char *from, const char *to)
{
	int res;
	char frompath[1000], topath[1000];
	
	char *a = strstr(to, atoz);
	if (a != NULL) decryptAtbash(a);
	
	char *b = strstr(from, rx);
	if (b != NULL){
		decryptRot13(b);
		decryptAtbash(b);
	}
	
	char *c = strstr(to, rx);
	if (c != NULL){
		decryptRot13(c);
		decryptAtbash(c);
	}

	sprintf(frompath, "%s%s", dirPath, from);
	sprintf(topath, "%s%s", dirPath, to);

	res = rename(frompath, topath);
	if (res == -1) return -errno;

	tulisLog2("RENAME", frompath, topath);
	
	if (c != NULL){
		enkripsi2(topath);
		tulisLog2("ENCRYPT2", from, to);
	}

	if (b != NULL && c == NULL){
		dekripsi2(topath);
		tulisLog2("DECRYPT2", from, to);
	}
	
	...

	return 0;
}
```
- Jika terdeteksi adanya string RX_ pada path tujuan berarti direktori direname dengan menambahkan RX_ dan dilanjutkan dengan melakukan pemecahan file pada fungsi enkripsi2.
```c
void enkripsi2(char *fpath)
{
	chdir(fpath);
	DIR *dp;
	struct dirent *dir;
	
	dp = opendir(".");
	if(dp == NULL) return;
	struct stat lol;
	char dirPath[1000];
	char filePath[1000];
	
	while ((dir = readdir(dp)) != NULL){
		printf("dirname %s\n", dir->d_name);
		printf("%s/%s\n", fpath, dir->d_name);
		if (stat(dir->d_name, &lol) < 0);
		else if (S_ISDIR(lol.st_mode)){
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;
			sprintf(dirPath,"%s/%s",fpath, dir->d_name);
			enkripsi2(dirPath);
			printf("dirpath %s\n", dirPath);
		}
		else{
			sprintf(filePath,"%s/%s",fpath,dir->d_name);
			FILE *input = fopen(filePath, "r");
			if (input == NULL) return;
			int bytes_read, count = 0;
			void *buffer = malloc(1024);
			while((bytes_read = fread(buffer, 1, 1024, input)) > 0){
				char newFilePath[1000];
				sprintf(newFilePath, "%s.%04d", filePath, count);
				count++;
				FILE *output = fopen(newFilePath, "w+");
				if(output == NULL) return;
				fwrite(buffer, 1, bytes_read, output);
				fclose(output);
				memset(buffer, 0, 1024);
			}
			fclose(input);
			printf("filepath %s\n", filePath);
			remove(filePath);
		}
	}
	closedir(dp);
}
```

- Jika terdeteksi string RX_ pada path asal dan tidak terdeteksi string RX_ pada path tujuan berarti direktori direname dengan menghilangkan RX_ sehingga akan dilanjutkan dengan menggabungkan file pada fungsi dekripsi2.
```c
void dekripsi2(char *dir)
{
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
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0) continue;
			sprintf(dirPath, "%s/%s", dir, de->d_name);
			dekripsi2(dirPath);
		}
		else{
			sprintf(filePath, "%s/%s", dir, de->d_name);
			filePath[strlen(filePath) - 5] = '\0';
			FILE *check = fopen(filePath, "r");
			if (check != NULL) return;
			FILE *file = fopen(filePath, "w");
			int count = 0;
			char topath[1000];
			sprintf(topath, "%s.%04d", filePath, count);
			void *buffer = malloc(1024);
			while (1){
				FILE *op = fopen(topath, "rb");
				if (op == NULL) break;
				size_t readSize = fread(buffer, 1, 1024, op);
				fwrite(buffer, 1, readSize, file);
				fclose(op);
				remove(topath);
				count++;
				sprintf(topath, "%s.%04d", filePath, count);
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
void encryptRot13(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	printf("encrypt path ROT13: %s\n", path);
	
	int endid = split_ext_id(path);
	int startid = slash_id(path, 0);
	
	for (int i = startid; i < endid; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char tmp = path[i];
			if(isupper(path[i])) tmp -= 'A';
			else tmp -= 'a';
			tmp = (tmp + 13) % 26; //ROT13 cipher
			if(isupper(path[i])) tmp += 'A';
			else tmp += 'a';
			path[i] = tmp;
		}
	}
}

void decryptRot13(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, "..")) return;
	
	printf("decrypt path ROT13: %s\n", path);
	
	int endid = split_ext_id(path);
	int startid = slash_id(path, endid);
	
	for (int i = startid; i < endid; i++){
		if (path[i] != '/' && isalpha(path[i])){
			char tmp = path[i];
			if(isupper(path[i])) tmp -= 'A';
			else tmp -= 'a';
			tmp = (tmp + 13) % 26; //ROT13 cipher
			if(isupper(path[i])) tmp += 'A';
			else tmp += 'a';
			path[i] = tmp;
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

## Kendala yang dihadapi


# Soal 3

- Pada utility functions RENAME akan dilakukan pengecekan apakah direktori direname dengan menambahkan A_is_a_ atau menghilangkan A_is_a_ dengan fungsi strstr().
```c
static int xmp_rename(const char *from, const char *to)
{
	int res;
	char frompath[1000], topath[1000];
	
	...

	sprintf(frompath, "%s%s", dirPath, from);
	sprintf(topath, "%s%s", dirPath, to);

	res = rename(frompath, topath);
	if (res == -1) return -errno;

	tulisLog2("RENAME", frompath, topath);
	
	...
	
	if (strstr(to, aisa) != NULL){
		encryptBinary(topath);
		tulisLog2("ENCRYPT3", from, to);
	}
	
	if (strstr(from, aisa) != NULL && strstr(to, aisa) == NULL){
		decryptBinary(topath);
		tulisLog2("DECRYPT3", from, to);
	}

	return 0;
}
```
- Jika terdeteksi A_is_a_ pada path tujuan berarti direktori direname dengan menambahkan A_is_a_. Maka akan dilanjutkan dengan mengubah nama file menjadi lowercase dan menambahkan nilai desimalnya sebagai ekstensi yang baru pada fungsi encryptBinary.
```c
void getBinary(char *fname, char *bin, char *lowercase){
	int endid = ext_id(fname);
	int startid = slash_id(fname, 0);
	int i;
	
	for(i=startid; i<endid; i++){
		if(isupper(fname[i])){
			bin[i] = '1';
			lowercase[i] = fname[i] + 32;
		}
		else{
			bin[i] = '0';
			lowercase[i] = fname[i];
		}
	}
	bin[endid] = '\0';
	
	for(; i<strlen(fname); i++){
		lowercase[i] = fname[i];
	}
	lowercase[i] = '\0';
}

int bin_to_dec(char *bin){
	int tmp = 1, res = 0;
	for(int i=strlen(bin)-1; i>=0; i--){
		if(bin[i] == '1') res += tmp;
		tmp *= 2;
	}
	return res;
}

void encryptBinary(char *fpath)
{
	chdir(fpath);
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
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;
			sprintf(dirPath,"%s/%s",fpath, dir->d_name);
			encryptBinary(dirPath);
		}
		else{
			sprintf(filePath,"%s/%s",fpath, dir->d_name);
			char bin[1000], lowercase[1000];
			getBinary(dir->d_name, bin, lowercase);
			int dec = bin_to_dec(bin);
			sprintf(filePathBinary,"%s/%s.%d",fpath,lowercase,dec);
			rename(filePath, filePathBinary);
		}
	}
	closedir(dp);
}

```
- Jika terdeteksi A_is_a_ pada path asal dan tidak terdeteksi adanya A_is_a_ pada path tujuan berarti direktori direname dengan menghilangkan A_is_a_. Maka akan dilanjutkan dengan mengubah nama file menjadi semula dengan bantuan nilai desimalnya pada fungsi decryptBinary.
```c
int convertDec(char *ext){
	int dec = 0, pengali = 1;
	for(int i=strlen(ext)-1; i>=0; i--){
		dec += (ext[i]-'0')*pengali;
		pengali *= 10;
	}
	return dec;
}

void dec_to_bin(int dec, char *bin, int len){
	int idx = 0;
	while(dec){
		if(dec & 1) bin[idx] = '1';
		else bin[idx] = '0';
		idx++;
		dec /= 2;
	}
	while(idx < len){
		bin[idx] = '0';
		idx++;
	}
	bin[idx] = '\0';
	
	for(int i=0; i<idx/2; i++){
		char tmp = bin[i];
		bin[i] = bin[idx-1-i];
		bin[idx-1-i] = tmp;
	}
}

void getDecimal(char *fname, char *bin, char *normalcase){
	int endid = ext_id(fname);
	int startid = slash_id(fname, 0);
	int i;
	
	for(i=startid; i<endid; i++){
		if(bin[i-startid] == '1') normalcase[i-startid] = fname[i] - 32;
		else normalcase[i-startid] = fname[i];
	}
	
	for(; i<strlen(fname); i++){
		normalcase[i-startid] = fname[i];
	}
	normalcase[i-startid] = '\0';
}

void decryptBinary(char *fpath)
{
	chdir(fpath);
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
			if (strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,"..") == 0) continue;
			sprintf(dirPath,"%s/%s",fpath, dir->d_name);
			decryptBinary(dirPath);
		}
		else{
			sprintf(filePath,"%s/%s",fpath, dir->d_name);
			char fname[1000], bin[1000], normalcase[1000], clearPath[1000];
			
			strcpy(fname, dir->d_name);
			char *ext = strrchr(fname, '.');
			int dec = convertDec(ext+1);
			for(int i=0; i<strlen(fname)-strlen(ext); i++) clearPath[i] = fname[i];
			
			char *ext2 = strrchr(clearPath, '.');
			dec_to_bin(dec, bin, strlen(clearPath)-strlen(ext2));
			getDecimal(clearPath, bin, normalcase);
			sprintf(filePathDecimal,"%s/%s",fpath,normalcase);
			rename(filePath, filePathDecimal);
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

## Kendala yang dialami
- Waktu pengerjaan tidak cukup untuk mengerjakannya
- Sulit untuk memahami maksud dari soal dengan adanya direktori spesial 
- bingung saat melakukan dekripsi karena perlu mengambil nilai desimalnya yang ada di paling belakang nama file.

# Soal 4
- Untuk soal ini kelompok kami diminta untuk membuat sebuah log system yang bertujuan untuk melakukan pencatatan aktivitas dalam file. Disini kelompok kami membuat dua fungsi dalam pembuatan log system ini yaitu fungsi tulisLog dan tulisLog2 dimana perbedaannya terdapat pada DESC (informasi dan parameter tambahan) yang perlu dicantumkan dalam format untuk menuliskan lognya. Agar bisa menuliskan log system sesuai format yang ada, maka kelompok kami perlu mencari waktu sekarang untuk nanti dicantumkan dalam log systemnya. Dalam fungsi tulisLog kita juga memasukkan parameter char *nama yang mana adalah System Call dan char *fpath yang mana adalah deskripsi mengenai file yang ada.

```c
void tulisLog(char *nama, char *fpath)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
```

- Selanjutnya kelompok kami melakukan inisialisasi sebuah array of char untuk menyimpan perintah system call yang telah dijalankan pada filesystem dan kemudian mencatatnya dalam file SinSeiFS.log. Lalu dibukalah file SinSeiFS.log pada direktori home pengguna dengan mode a (append) agar nanti bisa dituliskan log yang baru dan jika file belum ada maka akan dibuat file yang baru.
```c
	char haha[1000];
	
	FILE *file;
	file = fopen("/home/aldo/SinSeiFS.log", "a");
```

- Selanjutnya kelompok kami melakukan pengecekan pada syscall yang ada pada parameter. Jika syscall adalah RMDIR atau UNLINK maka log levelnya akan dicatat WARNING. Namun jika tidak, maka log levelnya akan dicatat INFO. Dan akan dilakukan juga pencatatan waktu sekarang beserta keterangan lainnya.
```c
	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(haha, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath);
	else
		sprintf(haha, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath);
```

- Langkah terakhir, kami tuliskan log yang ada kedalam file SinSeiFS.log dan kami tutup filenya.
```c
	fputs(haha, file);
	fclose(file);
	return;
}
```

- Untuk fungsi yang kedua tulisLog2 sebenarnya kurang lebih sama dengan fungsi tulisLog yang sudah dijelaskan sebelumnya, namun terdapat perbedaan pada parameter yang diberikan dan pada pencatatannya. Untuk parameternya ada char *nama yang merupakan syscall, const char *from adalah keterangan file sebelum dilakukannya perintah system call yang dijalankan oleh file system dan const char *to keterangan file setelah dilakukannya perintah system call yang dijalankan oleh file system. Lalu pada pencatatannya kurang lebih sama dengan yang sebelumnya namun ditambahkan keterangan sesuai dengan parameter yang diberikan.
```c
void tulisLog2(char *nama, const char *from, const char *to)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char haha[1000];

	FILE *file;
	file = fopen("/home/aldo/SinSeiFS.log", "a");

	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(haha, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, from, to);
	else
		sprintf(haha, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, from, to);

	fputs(haha, file);
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
