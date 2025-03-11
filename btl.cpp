#include <iostream>
#include <fstream>
#include <string>
#include <sstream> // chuyển đổi string sang số và ngược lại (dùng stringstream cho dữ liệu từ file students.txt)
#include <algorithm> // Cung cấp các thuật toán phổ biến như sắp xếp, tìm kiếm, đảo ngược mảng, loại bỏ phần tử trùng lặp,...
#include <vector> // sử dụng vector để lưu trữ mảng sinh viên từ file 
using namespace std;
const int MAX_STUDENTS = 200;  // Giới hạn số lượng sinh viên
struct Student {
    string id;
    string name;
    int birthYear;
    float averageScore;
};
// Hàm hiển thị danh sách sinh viên
void displayStudents(Student students[], int n) {
    for (int i = 0; i < n; i++) {
        cout << "ID: " << students[i].id
             << ", Ten: " << students[i].name
             << ", Nam sinh: " << students[i].birthYear
             << ", Diem trung binh: " << students[i].averageScore << endl;
    }
    cout << "-----------------------------------" << endl;
}
// Hàm đọc dữ liệu từ file
int readStudentsFromFile(const string& filename, Student students[]) {
    // Chuyển đổi string sang const char* để sử dụng với ifstream
    ifstream file(filename.c_str()); // Chuyển đổi string sang const char*
    // Kiểm tra xem file có mở được không
    if (!file) {
        cerr << "Khong the mo file: " << filename << endl;
        return 0;
    }
    string line;
    int count = 0;
    while (getline(file, line) && count < MAX_STUDENTS) { // Đọc từng dòng trong file
        if (line.empty()) continue;  // Bỏ qua dòng trống
        stringstream ss(line); // Chuyển dòng thành stringstream để đọc từng trường
        getline(ss, students[count].id, ',');
        getline(ss, students[count].name, ',');
        ss >> students[count].birthYear;
        ss.ignore(); // Bỏ qua ký tự phân cách
        ss >> students[count].averageScore;
        if (ss.fail()) {  // Kiểm tra lỗi đọc dữ liệu
            cerr << "Du lieu khong hop le tai dong " << count + 1 << ": " << line << endl;
            ss.clear(); // Xóa cờ lỗi và tiếp tục
            continue;
        }
        count++;
    }
    file.close();
    return count;
}
// Hàm sắp xếp chọn
void selectionSort(Student students[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i; // Giả sử phần tử đầu tiên là nhỏ nhất
        for (int j = i + 1; j < n; j++) { // Tìm phần tử nhỏ nhất trong mảng con
            if (students[j].name < students[minIdx].name) {
                minIdx = j;
            }
        }
        swap(students[i], students[minIdx]); // Đổi chỗ phần tử nhỏ nhất với phần tử đầu tiên
    }
}
// Hàm trộn hai mảng con
void merge(Student students[], int left, int mid, int right) {
    int n1 = mid - left + 1; // Kích thước mảng con bên trái
    int n2 = right - mid; // Kích thước mảng con bên phải
    Student leftArr[n1], rightArr[n2]; // Tạo mảng tạm thời
    for (int i = 0; i < n1; i++) 
        leftArr[i] = students[left + i];
    for (int i = 0; i < n2; i++) 
        rightArr[i] = students[mid + 1 + i];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) { // Trộn hai mảng con vào mảng chính
        if (leftArr[i].name <= rightArr[j].name) {
            students[k] = leftArr[i];
            i++;
        } else {
            students[k] = rightArr[j];
            j++;
        }
        k++;
    }
    while (i < n1) // Sao chép các phần tử còn lại của mảng bên trái
        students[k++] = leftArr[i++];
    while (j < n2) // Sao chép các phần tử còn lại của mảng bên phải
        students[k++] = rightArr[j++];
}
// Hàm sắp xếp trộn
void mergeSort(Student students[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2; // Tính chỉ số giữa
        mergeSort(students, left, mid); // Sắp xếp nửa đầu tiên
        mergeSort(students, mid + 1, right); // Sắp xếp nửa sau
        merge(students, left, mid, right); // Trộn hai nửa lại
    }
}
// Hàm phân hoạch cho sắp xếp nhanh
int partition(Student students[], int low, int high) {
    string pivot = students[high].name; // Chọn phần tử cuối làm chốt
    int i = low - 1;
    for (int j = low; j < high; j++) { // Di chuyển các phần tử nhỏ hơn chốt về bên trái
        if (students[j].name < pivot) {
            i++;
            swap(students[i], students[j]);
        }
    }
    swap(students[i + 1], students[high]); // Đổi chỗ phần tử chốt về đúng vị trí
    return i + 1;
}
// Hàm sắp xếp nhanh
void quickSort(Student students[], int low, int high) {
    if (low < high) {
        int pi = partition(students, low, high); // Phân hoạch mảng
        quickSort(students, low, pi - 1); // Sắp xếp mảng con bên trái
        quickSort(students, pi + 1, high); // Sắp xếp mảng con bên phải
    }
}
// Hàm tìm kiếm sinh viên theo ID
void searchStudentById(Student students[], int n, const string& id) {
    bool found = false;
    for (int i = 0; i < n; i++) {
        if (students[i].id == id) {
            cout << "Tim thay sinh vien:" << endl;
            cout << "ID: " << students[i].id
                 << ", Ten: " << students[i].name
                 << ", Nam sinh: " << students[i].birthYear
                 << ", Diem trung binh: " << students[i].averageScore << endl;
            found = true;
            break;
        }
    }
    if (!found) {
        cout << "Sinh vien voi ma " << id << " khong co trong danh sach!" << endl;
    }
}
// Hàm tìm kiếm sinh viên theo tên (có thể trùng khớp một phần)
void searchStudentByName(Student students[], int n, const string& name) {
    bool found = false;
    for (int i = 0; i < n; i++) {
        // Kiểm tra nếu tên nhập vào là một phần của tên sinh viên
        if (students[i].name.find(name) != string::npos) {
            cout << "Tim thay sinh vien:" << endl;
            cout << "ID: " << students[i].id
                 << ", Ten: " << students[i].name
                 << ", Nam sinh: " << students[i].birthYear
                 << ", Diem trung binh: " << students[i].averageScore << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Khong co sinh vien nao chua ten: " << name << endl;
    }
}
int main() {
    Student students[MAX_STUDENTS];
    int n = readStudentsFromFile("students.txt", students); // Đọc dữ liệu từ file
    if (n == 0) {
        cout << "Khong co sinh vien nao hoac loi file!" << endl;
        return 1;
    }
    int choice;
    string input;
    do {
        cout << "\nMenu:\n";
        cout << "1. Hien thi danh sach sinh vien\n";
        cout << "2. Sap xep danh sach sinh vien bang sap xep chon (Selection Sort)\n"; 
        cout << "3. Sap xep danh sach sinh vien bang sap xep tron (Merge Sort)\n"; 
        cout << "4. Sap xep danh sach sinh vien bang sap xep nhanh (Quick Sort)\n";
        cout << "5. Tim kiem sinh vien theo ID\n";
        cout << "6. Tim kiem sinh vien theo ten\n";
        cout << "0. Thoat\n";
        cout << "Nhap lua chon cua ban: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Danh sach sinh vien ban dau:" << endl;
                displayStudents(students, n); // Hiển thị danh sách sinh viên
                break;
            case 2:
                {
                    Student selectionSorted[MAX_STUDENTS];
                    copy(students, students + n, selectionSorted); // Sao chép mảng sinh viên
                    selectionSort(selectionSorted, n); // Sắp xếp bằng Selection Sort
                    cout << "Sau khi sap xep chon:" << endl;
                    displayStudents(selectionSorted, n); // Hiển thị danh sách sau khi sắp xếp
                }
                break;
            case 3:
                {
                    Student mergeSorted[MAX_STUDENTS];
                    copy(students, students + n, mergeSorted); // Sao chép mảng sinh viên
                    mergeSort(mergeSorted, 0, n - 1); // Sắp xếp bằng Merge Sort
                    cout << "Sau khi sap xep tron:" << endl;
                    displayStudents(mergeSorted, n); // Hiển thị danh sách sau khi sắp xếp
                }
                break;
            case 4:
                {
                    Student quickSorted[MAX_STUDENTS];
                    copy(students, students + n, quickSorted); // Sao chép mảng sinh viên
                    quickSort(quickSorted, 0, n - 1); // Sắp xếp bằng Quick Sort
                    cout << "Sau khi sap xep nhanh:" << endl;
                    displayStudents(quickSorted, n); // Hiển thị danh sách sau khi sắp xếp
                }
                break;
            case 5:
                cout << "Nhap ID sinh vien can tim: ";
                cin >> input;
                searchStudentById(students, n, input); // Tìm kiếm sinh viên theo ID
                break;
            case 6:
                cout << "Nhap ten sinh vien can tim: ";
                cin.ignore(); // Bỏ qua ký tự newline trước đó
                getline(cin, input); // Đọc cả dòng tên
                searchStudentByName(students, n, input); // Tìm kiếm sinh viên theo tên
                break;
            case 0:
                cout << "Thoat chuong trinh.\n";
                break;
            default:
                cout << "Lua chon khong hop le. Vui long thu lai.\n";
        }
    } while (choice != 0);
    return 0;
}
