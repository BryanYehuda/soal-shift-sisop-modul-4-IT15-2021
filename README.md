# soal-shift-sisop-modul-4-IT15-2021

# Soal 1
Pada soal ini kita diminta untuk melakukan enkripsi maupun dekripsi nama file dan folder dengan Atbash cipher. Untuk mendapatkan nama file dan folder akan dilakukan looping untuk mengecek dimana posisi awal berupa slash (/) dan posisi akhir berupa titik (.), sebagai contoh pada path AtoZ_folder/DATA_PENTING/kucinglucu123.jpg yang akan diolah adalah DATA_PENTING/kucinglucu123 sehingga menjadi WZGZ_KVMGRMT/pfxrmtofxf123 sedangkan karakter yang lainnya tidak berubah dan menghasilkan path akhir AtoZ_folder/WZGZ_KVMGRMT/pfxrmtofxf123.jpg. Maka kita butuh 3 fungsi untuk mendapatkan index penanda awal dan akhir enkripsi dan dekripsi:
- slash_id : Mengembalikan index slash
- ext_id : Mengembalikan index file extension
- split_ext_id : Mengembalikan index file extension pada file yang displit
