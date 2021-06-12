# soal-shift-sisop-modul-4-IT15-2021

# Soal 1
Pada soal ini kita diminta untuk melakukan enkripsi maupun dekripsi nama file dan folder dengan Atbash cipher. Untuk mendapatkan nama file dan folder akan dilakukan looping untuk mengecek dimana posisi awal berupa slash (/) dan posisi akhir berupa titik (.), sebagai contoh pada path AtoZ_folder/DATA_PENTING/kucinglucu123.jpg yang akan diolah adalah DATA_PENTING/kucinglucu123 sehingga menjadi WZGZ_KVMGRMT/pfxrmtofxf123 sedangkan karakter yang lainnya tidak berubah dan menghasilkan path akhir AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg. Maka kita butuh 3 fungsi untuk mendapatkan index penanda awal dan akhir enkripsi dan dekripsi:
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

Untuk melakukan enkripsi dan dekripsi menggunakan Atbash cipher akan dibuat fungsi tersendiri.

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
Pemanggilan fungsi dekripsi dilakukan pada tiap utility functions getattr, mkdir, rename, rmdir, create, dan fungsi-fungsi lain yang menurut kami esensial dalam proses sinkronisasi FUSE dan mount folder. Fungsi dekripsi dan enkripsi dilakukan di utility function readdir karena FUSE akan melakukan dekripsi di mount folder lalu enkripsi di FUSE saat readdir. Pemanggilannya dilakukan dengan pengecekan apakah string AtoZ_ terdapat di string path di masing-masing utility function dengan menggunakan fungsi strstr(). Jika ya, maka fungsi enkripsi dan dekripsi akan dipanggil untuk string tersebut dengan AtoZ_ sebagai starting point string yang diteruskan. Untuk pencatatan log akan dijelaskan pada soal nomor 4.

## Hasil run
- Kondisi awal
![121061825-8a37f900-c7ee-11eb-9924-65a0bf4f5102](https://user-images.githubusercontent.com/71550384/121785379-b8994800-cbe3-11eb-9bd6-5e513de296e2.png)

- Setelah di enkripsi
![121061746-6f658480-c7ee-11eb-86a9-8d875f998fb4](https://user-images.githubusercontent.com/71550384/121785397-c8189100-cbe3-11eb-8fb0-5379845347b4.png)

- Setelah di dekripsi
![121061825-8a37f900-c7ee-11eb-9924-65a0bf4f5102 (1)](https://user-images.githubusercontent.com/71550384/121785410-d5ce1680-cbe3-11eb-8e32-f64b1ea15f3d.png)

