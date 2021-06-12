# soal-shift-sisop-modul-4-IT15-2021

# Soal 1
- Pada soal ini kita diminta untuk melakukan enkripsi maupun dekripsi nama file dan folder dengan Atbash cipher. Untuk mendapatkan nama file dan folder akan dilakukan looping untuk mengecek dimana posisi awal berupa slash (/) dan posisi akhir berupa titik (.), sebagai contoh pada path AtoZ_folder/DATA_PENTING/kucinglucu123.jpg yang akan diolah adalah DATA_PENTING/kucinglucu123 sehingga menjadi WZGZ_KVMGRMT/pfxrmtofxf123 sedangkan karakter yang lainnya tidak berubah dan menghasilkan path akhir AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg. Maka kita butuh 3 fungsi untuk mendapatkan index penanda awal dan akhir enkripsi dan dekripsi:
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

- Untuk melakukan enkripsi dan dekripsi menggunakan Atbash cipher akan dibuat fungsi tersendiri.

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
- Pemanggilan fungsi dekripsi dilakukan pada tiap utility functions getattr, mkdir, rename, rmdir, create, dan fungsi-fungsi lain yang menurut kami esensial dalam proses sinkronisasi FUSE dan mount folder. Fungsi dekripsi dan enkripsi dilakukan di utility function readdir karena FUSE akan melakukan dekripsi di mount folder lalu enkripsi di FUSE saat readdir. Pemanggilannya dilakukan dengan pengecekan apakah string AtoZ_ terdapat di string path di masing-masing utility function dengan menggunakan fungsi strstr(). Jika ya, maka fungsi enkripsi dan dekripsi akan dipanggil untuk string tersebut dengan AtoZ_ sebagai starting point string yang diteruskan. Untuk pencatatan log akan dijelaskan pada soal nomor 4.

## Hasil run
- Kondisi awal
![121061825-8a37f900-c7ee-11eb-9924-65a0bf4f5102](https://user-images.githubusercontent.com/71550384/121785379-b8994800-cbe3-11eb-9bd6-5e513de296e2.png)

- Setelah di enkripsi
![121061746-6f658480-c7ee-11eb-86a9-8d875f998fb4](https://user-images.githubusercontent.com/71550384/121785397-c8189100-cbe3-11eb-8fb0-5379845347b4.png)

- Setelah di dekripsi
![121061825-8a37f900-c7ee-11eb-9924-65a0bf4f5102 (1)](https://user-images.githubusercontent.com/71550384/121785410-d5ce1680-cbe3-11eb-8e32-f64b1ea15f3d.png)

## Kendala yang Dihadapi
- Kebingungan saat menentukan utility functions mana saja yang akan dipakai.
- Terdapat kesalahan saat enkripsi dan dekripsi pada fungsi readdir sehingga file tidak tertampil pada FUSE.

# Soal 2
- Pada utility functions RENAME dilakukan pengecekan apakah direktori direname dengan menambahkan RX_ atau menghilangkan RX_ dengan fungsi strstr().
dalam soal ini kita akan mengecek apakah sebuah path terdapat string RX_ jika iya maka akan dienkripsi file file yang ada didalamnya. Disini kami menggunakan atbash dan rot13 cipher sesuai perintah soal. Dimana atbash sudah dijelaskan pada soal 1. Selanjutnya sebuah file yang sudah terenkripsi dengan atbash akan di enkripsi dengan rot13. Begitu juga untuk melakukan dekripsi pada pencarian path aslinya. kami perlu menggabungkan dua atbash dan rot13 cipher tadi, namun karena keduanya masih symetric cipher jadi tidak perlu membuat fungsi decode baru.

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
- Jika terdeteksi RX_ pada path tujuan berarti direktori direname dengan menambahkan RX_ dan dilanjutkan dengan memecahkan file pada fungsi enkripsi2.
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

- Jika terdeteksi RX_ pada path asal dan tidak terdeteksi RX_ pada path tujuan berarti direktori direname dengan menghilangkan RX_ dan dilanjutkan dengan menggabungkan file pada fungsi dekripsi2.
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

- Untuk melakukan enkripsi dan dekripsi menggunakan ROT13 cipher akan dibuat fungsi tersendiri.
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
- Pemanggilan fungsi dekripsi dilakukan pada tiap utility functions getattr, mkdir, rename, rmdir, create, dan fungsi-fungsi lain yang menurut kami esensial dalam proses sinkronisasi FUSE dan mount folder. Fungsi dekripsi dan enkripsi dilakukan di utility function readdir karena FUSE akan melakukan dekripsi di mount folder lalu enkripsi di FUSE saat readdir. Pemanggilannya dilakukan dengan pengecekan apakah string RX_ terdapat di string path di masing-masing utility function dengan menggunakan fungsi strstr(). Jika ya, maka fungsi enkripsi dan dekripsi akan dipanggil untuk string tersebut dengan RX_ sebagai starting point string yang diteruskan. Untuk pencatatan log akan dijelaskan pada soal nomor 4.

## Hasil run

- Kondisi awal
![121061783-7d1b0a00-c7ee-11eb-876a-0b3200353e29](https://user-images.githubusercontent.com/71550384/121785748-d9629d00-cbe5-11eb-9d13-361013cb6c4f.png)


- Setelah enkripsi
![121061821-8906cc00-c7ee-11eb-853b-1eadf17b0cbc](https://user-images.githubusercontent.com/71550384/121785756-e41d3200-cbe5-11eb-90f9-2c478a1b1f2a.png)


- Setelah deksipsi
![121061783-7d1b0a00-c7ee-11eb-876a-0b3200353e29 (1)](https://user-images.githubusercontent.com/71550384/121785770-f39c7b00-cbe5-11eb-8607-9f5063769ca1.png)

## Kendala yang dihadapi
- Pada Vigenere cipher saat dicek dengan printf terlihat sudah menghasilkan path yang benar tapi file tersebut tidak muncul pada FUSE sehingga sementara kami memakai ROT13 cipher semua.
- Sulit untuk melakukan split dan join karena tidak boleh memakai exec*(). Pada direktori asli file sudah terpecah, tetapi pada FUSE juga tertampil file yang terpecah kecuali RX_ dihilangkan dan file tergabung kembali.


# Soal 3

- Pada utility functions RENAME dilakukan pengecekan apakah direktori direname dengan menambahkan A_is_a_ atau menghilangkan A_is_a_ dengan fungsi strstr().
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
- Jika terdeteksi A_is_a_ pada path tujuan berarti direktori direname dengan menambahkan A_is_a_. Maka dilanjutkan dengan mengubah nama file menjadi lowercase dan menambahkan nilai desimalnya sebagai ekstensi yang baru pada fungsi encryptBinary.
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
- Jika terdeteksi A_is_a_ pada path asal dan tidak terdeteksi A_is_a_ pada path tujuan berarti direktori direname dengan menghilangkan A_is_a_. Maka dilanjutkan dengan mengubah nama file menjadi semula dengan bantuan nilai desimalnya pada fungsi decryptBinary.
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
Untuk pencatatan log akan dijelaskan pada soal nomor 4.

## Hasil run

- Kondisi awal
 ![121061895-a50a6d80-c7ee-11eb-9924-bd254073db3e](https://user-images.githubusercontent.com/71550384/121785909-d0260000-cbe6-11eb-9dac-0f32cb13db2c.png)
 
- Setelah di enkripsi
![121061900-a5a30400-c7ee-11eb-930f-c0d20c4193f4](https://user-images.githubusercontent.com/71550384/121785916-e0d67600-cbe6-11eb-8918-c9f87aa09073.png)

- Setelah di dekripsi
![121061895-a50a6d80-c7ee-11eb-9924-bd254073db3e (1)](https://user-images.githubusercontent.com/71550384/121785921-eaf87480-cbe6-11eb-8b1f-7df66686e352.png)


## Kendala yang dialami
- Waktu pengerjaan tidak cukup untuk mengerjakannya
- Sulit untuk memahami maksud dari soal dengan adanya direktori spesial 
- bingung saat melakukan dekripsi karena perlu mengambil nilai desimalnya yang ada di paling belakang nama file.

# Soal 4
- Untuk soal ini kita diminta untuk membuat log system yang bertujuan untuk memudahkan dalam memonitor kegiatan pada file system. Disini kita membuat dua fungsi dalam pembuatan log system ini yaitu fungsi tulisLog dan tulisLog2 perbedaannya terdapat pada DESC (informasi dan parameter tambahan) yang perlu dicantumkan dalam format untuk loggingnya. Dalam menuliskan log system sesuai format yang ada kita perlu mencari waktu sekarang untuk nanti dicantumkan dalam log systemnya. Dalam fungsi tulisLog kita juga memasukkan parameter char *nama yang mana adalah System Call dan char *fpath adalah deskripsi mengenai file yang ada.

```c
void tulisLog(char *nama, char *fpath)
{
	time_t rawtime;
	struct tm *timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
```

- Selanjutnya kita inisialisasi sebuah array of char atau string haha untuk nanti menyimpan perintah system call yang telah dijalankan pada filesystem dan mencatatnya dalam file SinSeiFS.log. Lalu kita perlu membuka file SinSeiFS.log pada direktori home pengguna dengan mode a (append) agar nanti bisa dituliskan log yang baru dan jika file belum ada maka akan dibuat file yang baru.
```c
	char haha[1000];
	
	FILE *file;
	file = fopen("/home/aldo/SinSeiFS.log", "a");
```

- Selanjutnya kita bisa melakukan pengecekan pada syscall yang ada pada parameter. Jika syscall adalah RMDIR atau UNLINK maka log levelnya akan dicatat WARNING. Namun jika tidak, maka log levelnya akan dicatat INFO. Dan akan dicatat juga waktu sekarang beserta keterangan lainnya.
```c
	if (strcmp(nama, "RMDIR") == 0 || strcmp(nama, "UNLINK") == 0)
		sprintf(haha, "WARNING::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath);
	else
		sprintf(haha, "INFO::%.2d%.2d%d-%.2d:%.2d:%.2d::%s::%s\n", timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, nama, fpath);
```

- Langkah terakhir kita tuliskan log yang ada kedalam file SinSeiFS.log dan kita tutup filenya.
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

- Lalu untuk implementasinya kita masukkan fungsi-fungsi ini kedalam setiap fungsi system call yang ada.

## Hasil run

![121061886-a2a81380-c7ee-11eb-87d7-e15ea1e5b1aa](https://user-images.githubusercontent.com/71550384/121786054-bb963780-cbe7-11eb-9c6a-605ca7ab8389.png)


## Kendala yang dialami
-  kesulitan dalam mengambil waktu sekarang (real time) dan pada pencatatannya.
-  bingung untuk mencatat System Call karena ada yang punya 1 atau 2 argumen. Akhirnya kami pisahkan menjadi 2 fungsi untut mencatat log.
