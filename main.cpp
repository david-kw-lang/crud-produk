#include <iostream>
#include <string>
#include <fstream>
#include <limits>

using namespace std;

struct Produk {
    int pk;
    char nama [20];
    int harga;
};

int pilihMenu(); //deklarasi fungsi
void checkDatabase(fstream &data); //deklarasi fungsi

void writeData(fstream &data, int posisi, Produk &inputProduk)
{
    data.seekp((posisi-1)*sizeof(Produk), ios::beg); //mencari posisi
    data.write(reinterpret_cast<char*>(&inputProduk), sizeof(Produk));
}
int getDataSize(fstream &data)
{
    int start, end;
    data.seekg(0, ios::beg); //mencari posisi dlm data dan pindah corser didepan 0
    start = data.tellg();
    data.seekg(0, ios::end);
    end = data.tellg();

    return (end - start)/sizeof(Produk);
}

Produk readData(fstream &data, int posisi)
{
    Produk readProduk;
    data.seekp((posisi-1)*sizeof(Produk), ios::beg); //mencari posisi
    data.read(reinterpret_cast<char*>(&readProduk), sizeof(Produk));
    return readProduk;
}

void addDataProduk(fstream &data)
{
    Produk inputProduk, lastProduk;
    int size;
    size = getDataSize(data);
    if(size == 0){
        inputProduk.pk = 1;
    } else {
        lastProduk = readData(data, size);
        inputProduk.pk = lastProduk.pk + 1;
    }
    cout << "jumlah produk yang sudah diinput" << size << endl;
    cout << "pk sekarang adalah " << inputProduk.pk << endl;

    cout << "Nama Produk : ";
    cin.getline(inputProduk.nama, 20);
    cout << "Harga Produk: ";
    cin >> inputProduk.harga;

    writeData(data, size+1 , inputProduk);
}

void displayDataProduk(fstream &data)
{
    Produk showProduk;
    int size;

    size = getDataSize(data);
    cout << "No. \tpk \tNama \tHarga"<<endl;
    for(int i = 1; i<=size;i++)
    {
        showProduk = readData(data,i);
        cout << i << "\t";
        cout  << showProduk.pk<< "\t";
        cout  << showProduk.nama<< "\t";
        cout  << showProduk.harga<< endl;
    }
}

void updateDataProduk(fstream &data)
{
    int nomor;
    Produk updateDataProduk;

    cout << "masukan nomor Produk yang akan dirubah :";
    cin >> nomor;
     cin.ignore(numeric_limits<streamsize>::max(), '\n');
    updateDataProduk = readData(data,nomor);

    cout << "Produk yang dirubah :"<<endl;
    cout << "nama Produk :"<< updateDataProduk.nama << endl;
    cout << "harga Produk :"<< updateDataProduk.harga << endl;

    cout << "proses perubahan data : "<< endl;
    cout << "nama Produk";
    cin.getline(updateDataProduk.nama,20);
    cout<< "harga Produk:";
    cin>> updateDataProduk.harga;

    writeData(data,nomor,updateDataProduk);


}
void delateDataProduk(fstream&data)
{
    int nomor,size,offset;
    Produk blankProduk, temporaryProduk;
    fstream datasementara;

    size = getDataSize(data);
    cout << "pilih nama produk yang akan dihapus:";
    cin>> nomor;

    blankProduk.pk = 0;
    writeData(data,nomor,blankProduk);

    datasementara.open("temp.dat",ios::trunc | ios :: out | ios ::in | ios :: binary);

    offset = 0;
    for(int i = 1;i<=size;  i++)
    {
        temporaryProduk = readData(data,i);
        if(temporaryProduk.pk != 0)
        {
            writeData(datasementara,i-offset,temporaryProduk);
        }else{
           offset = offset+1;
        }
    }
    size = getDataSize(datasementara);
    data.close();
    data.open("data.bin",ios::trunc | ios :: out | ios ::in | ios :: binary);
    data.close();
    data.open("data.bin",ios :: out | ios ::in | ios :: binary);

     for(int i = 1;i<=size;  i++)
    {
         temporaryProduk = readData(data,i);
        writeData(data,i,temporaryProduk);
    }



}
int main ()
{
    fstream data;
    checkDatabase(data);

    int pilihan = pilihMenu();
    char is_continue;

    while (pilihan != 5)
    {
        switch (pilihan)
        {
            case 1 :
                cout <<  "Menu tambah produk" << endl;
                addDataProduk(data);
                break;
            case 2 :
                cout <<  "Menu lihat produk" << endl;
                displayDataProduk(data);
                break;
            case 3 :
                cout <<  "Menu ubah produk" << endl;
                displayDataProduk(data);
                updateDataProduk(data);
                displayDataProduk(data);
                break;
            case 4 :
                cout <<  "Menu hapus produk" << endl;
                displayDataProduk(data);
                delateDataProduk(data);
                displayDataProduk(data);
                break;
            default :
                cout << "menu tidak valid" << endl;
                break;
        }

        label_continue:
        cout << "lanjutkan?[y/n] :";
        cin >> is_continue;
        if(is_continue == 'y' | is_continue == 'Y') {
            pilihan = pilihMenu();
        } else if (is_continue == 'n' | is_continue == 'N') {
            break;
        } else {
            goto label_continue;
        }
    }


    return 0;
}

int pilihMenu()
{
    int input;
    cout << "Program CRUD Produk" << endl;
    cout << "====================" << endl;
    cout << "1. Tambah Produk" << endl;
    cout << "2. Lihat Produk" << endl;
    cout << "3. Ubah Produk" << endl;
    cout << "4. Hapus Produk" << endl;
    cout << "5. Selesai" << endl;
    cout << "====================" << endl;
    cout << "Masukkan pilihan? [1-5]:";
    cin >> input;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return input;

}

void checkDatabase(fstream &data)
{
    data.open("data.bin", ios::out | ios::in | ios::binary);
    if (data.is_open()) {
        cout << "database ditemukan!" << endl;
    } else {
        cout << "database belum ada, buat baru" << endl;
        data.close ();
        data.open("data.bin", ios::trunc | ios::out | ios::in | ios::binary);
    }

}

