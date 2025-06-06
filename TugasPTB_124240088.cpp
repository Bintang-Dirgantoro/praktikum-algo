#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

struct Mahasiswa {
    int nim;
    char nama[100];
};

struct PtbNode {
    Mahasiswa data;
    PtbNode *left;
    PtbNode *right;

    PtbNode(Mahasiswa mhs1) {
        data = mhs1;
        left = nullptr;
        right = nullptr;
    }
};

void clear()
{
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
    system("cls");
}

void tulisKeFile(const Mahasiswa &mhs) {
    FILE *file = fopen("data_mahasiswa.dat", "ab");
    if (file != nullptr) {
        fwrite(&mhs, sizeof(Mahasiswa), 1, file);
        fclose(file);
    } else {
        cout << "Gagal menyimpan data ke file.\n";
    }
}

void insert(PtbNode *&root, Mahasiswa mhs) {
    if (root == nullptr) {
        root = new PtbNode(mhs);
        tulisKeFile(mhs); 
        return;
    }

    PtbNode *bantu = root;
    while (true) {
        if (mhs.nim < bantu->data.nim) {
            if (bantu->left == nullptr) {
                bantu->left = new PtbNode(mhs);
                tulisKeFile(mhs); 
                return;
            }
            bantu = bantu->left;
        } else if (mhs.nim > bantu->data.nim) {
            if (bantu->right == nullptr) {
                bantu->right = new PtbNode(mhs);
                tulisKeFile(mhs); 
                return;
            }
            bantu = bantu->right;
        } else {
            cout << "NIM sudah ada!\n";
            return;
        }
    }
    clear();
}

bool searching(PtbNode *root, int target) {
    PtbNode *bantu = root;
    while (bantu) {
        if (bantu->data.nim == target) {
            cout << bantu->data.nim << " ditemukan: " << bantu->data.nama << endl;
            return true;
        }
        if (target < bantu->data.nim)
            bantu = bantu->left;
        else
            bantu = bantu->right;
    }
    return false;
    clear();
}

void inorder(PtbNode *root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->data.nim << " - " << root->data.nama << endl;
        inorder(root->right);
    }
    clear();
}

void hapus(PtbNode *&root, int target) {
    if (root == nullptr) return;

    if (target < root->data.nim) {
        hapus(root->left, target);
    } else if (target > root->data.nim) {
        hapus(root->right, target);
    } else {
        PtbNode *bantu = root;
        if (root->left == nullptr) {
            root = root->right;
        } else if (root->right == nullptr) {
            root = root->left;
        } else {
            PtbNode *successor = root->right;
            while (successor->left != nullptr)
                successor = successor->left;
            root->data = successor->data;
            hapus(root->right, successor->data.nim);
            return;
        }
        delete bantu;
    }
    clear();
}

void loadDariFile(PtbNode *&root) {
    FILE *file = fopen("data_mahasiswa.dat", "rb");
    if (file == nullptr) return;

    Mahasiswa mhs;
    while (fread(&mhs, sizeof(Mahasiswa), 1, file)) {
        insert(root, mhs); 
    }
    fclose(file);
}

void tampilMenu() {
    cout << "\n===== MENU =====\n";
    cout << "1. Tambah data mahasiswa\n";
    cout << "2. Cari NIM\n";
    cout << "3. Tampilkan semua (inorder)\n";
    cout << "4. Hapus data mahasiswa\n";
    cout << "5. Keluar\n";
    cout << "================\n";
}

int main() {
    PtbNode *root = nullptr;
    loadDariFile(root); 

    int pilihan;
    do {
        tampilMenu();
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
        case 1: {
            Mahasiswa mhs;
            cout << "Masukkan NIM: ";
            cin >> mhs.nim;
            cin.ignore();
            cout << "Masukkan Nama: ";
            cin.getline(mhs.nama, 100);
            insert(root, mhs);
            break;
        }
        case 2: {
            int nim;
            cout << "Masukkan NIM yang dicari: ";
            cin >> nim;
            if (!searching(root, nim))
                cout << "NIM tidak ditemukan!\n";
            break;
        }
        case 3:
            cout << "Data Mahasiswa (urut NIM):\n";
            inorder(root);
            break;
        case 4: {
            int nim;
            cout << "Masukkan NIM yang ingin dihapus: ";
            cin >> nim;
            hapus(root, nim);
            break;
        }
        case 5:
            cout << "Keluar...\n";
            break;
        default:
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 5);

    return 0;
}
