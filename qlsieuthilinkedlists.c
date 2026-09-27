#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Thêm con trỏ *next để chuyển cấu trúc thành Node của danh sách liên kết
struct product {
    char ten[20];
    int gia;
    int soLuong;
    int ID;
    struct product *next; // Con trỏ trỏ tới phần tử tiếp theo
};

// Thay thế mảng con trỏ bằng con trỏ head trỏ tới đầu danh sách
struct product *head = NULL;
int count = 0; // Vẫn giữ biến count để theo dõi tổng số lượng[cite: 3]

// Hàm hoán đổi dữ liệu giữa 2 node (hỗ trợ cho việc sắp xếp danh sách liên kết)
void swapData(struct product *a, struct product *b) {
    int tempID = a->ID; a->ID = b->ID; b->ID = tempID;
    int tempGia = a->gia; a->gia = b->gia; b->gia = tempGia;
    int tempSL = a->soLuong; a->soLuong = b->soLuong; b->soLuong = tempSL;
    char tempTen[20]; 
    strcpy(tempTen, a->ten); 
    strcpy(a->ten, b->ten); 
    strcpy(b->ten, tempTen);
}

// Duyệt qua danh sách liên kết để kiểm tra ID thay vì dùng vòng lặp mảng[cite: 3]
int idExists(int id) {
    struct product *current = head;
    while (current != NULL) {
        if (current->ID == id) return 1;
        current = current->next;
    }
    return 0;
}

void saveHistoryadded(const char *filename, struct product *newProduct) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Khong the mo file de ghi lich su\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file,"-------------------------------\n");
    fprintf(file, "Lich su them san pham:[%02d:%02d]Added\n", t->tm_hour, t->tm_min); 
    fprintf(file, "%d %s %d %d\n", newProduct->ID, newProduct->ten, newProduct->gia, newProduct->soLuong); 
    fclose(file);
}

void saveHistorydeleted(const char *filename, struct product deletedProduct) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Khong the mo file de ghi lich su\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file,"-------------------------------\n");
    fprintf(file, "Lich su xoa san pham:[%02d:%02d]Deleted\n", t->tm_hour, t->tm_min); 
    fprintf(file, "%d %s %d %d\n", deletedProduct.ID, deletedProduct.ten, deletedProduct.gia, deletedProduct.soLuong); 
    fclose(file);
}

void saveHistoryEdit(const char *filename, struct product *editedProduct) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        printf("Khong the mo file de ghi lich su\n");
        return;
    }
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    fprintf(file,"-------------------------------\n");
    fprintf(file, "Lich su sua san pham:[%02d:%02d]Edited\n", t->tm_hour, t->tm_min); 
    fprintf(file, "%d %s %d %d\n", editedProduct->ID, editedProduct->ten, editedProduct->gia, editedProduct->soLuong); 
    fclose(file);
}

// Duyệt danh sách liên kết từ đầu đến cuối để lưu vào file[cite: 3]
int saveProducts(const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Khong the mo file de ghi\n");
        return 0;
    }
    struct product *current = head;
    while (current != NULL) {
        fprintf(file, "%d %s %d %d\n", current->ID, current->ten, current->gia, current->soLuong); 
        current = current->next;
    }
    fclose(file);
    return 1;
}

// Cấp phát bộ nhớ cho Node mới thay vì dùng realloc[cite: 3]
int nhapProduct() {
    int newID;
    printf("Nhap ID san pham: ");
    if (scanf("%d", &newID) != 1 || newID < 0 || newID > 1000) { 
        printf("ID khong hop le\n");
        return 0;
    }
    if (idExists(newID)) {
        printf("ID da ton tai\n");
        return 0;
    }

    // Cấp phát động 1 Node mới cho danh sách liên kết
    struct product *newNode = (struct product *)malloc(sizeof(struct product));
    if (newNode == NULL) {
        printf("Khong the cap phat bo nho\n");
        return 0;
    }

    newNode->ID = newID;
    printf("Nhap ten san pham: ");
    scanf("%19s", newNode->ten); 
    printf("Nhap gia san pham: ");
    scanf("%d", &newNode->gia); 
    printf("Nhap so luong san pham: ");
    scanf("%d", &newNode->soLuong); 
    newNode->next = NULL;

    // Chèn Node vào cuối danh sách liên kết
    if (head == NULL) {
        head = newNode;
    } else {
        struct product *current = head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    
    count++; // Cập nhật biến đếm[cite: 3]
    if (saveProducts("products.txt")) {
        printf("Da luu san pham vao file\n");
    } else {
        printf("Khong the luu san pham vao file\n");
    }
    saveHistoryadded("history.txt", newNode);
    return 1;
}

// Duyệt tuần tự để in danh sách
void xuatProduct() {
    if (head == NULL) {
        printf("Danh sach trong!\n"); 
        return;
    }
    printf("\n+------+----------------------+----------+----------+\n"); 
    printf("| %-4s | %-20s | %-8s | %-8s |\n", "ID", "Ten san pham", "Gia", "So luong"); 
    printf("+------+----------------------+----------+----------+\n"); 

    struct product *current = head;
    while (current != NULL) {
        printf("| %-4d | %-20s | %-8d | %-8d |\n", current->ID, current->ten, current->gia, current->soLuong); 
        current = current->next;
    }
    printf("+------+----------------------+----------+----------+\n"); 
}

// Đọc file và chèn từng dòng thành một Node mới ở cuối danh sách[cite: 3]
int loadProducts(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Khong the mo file de doc\n");
        return 0;
    }
    
    // Xóa danh sách cũ nếu có trước khi load (tránh trùng lặp)
    struct product *current = head;
    while (current != NULL) {
        struct product *temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    count = 0;

    int tempID, tempGia, tempSL;
    char tempTen[20];

    while (fscanf(file, "%d %19s %d %d", &tempID, tempTen, &tempGia, &tempSL) == 4) { 
        struct product *newNode = (struct product *)malloc(sizeof(struct product));
        newNode->ID = tempID;
        strcpy(newNode->ten, tempTen);
        newNode->gia = tempGia;
        newNode->soLuong = tempSL;
        newNode->next = NULL;

        if (head == NULL) {
            head = newNode;
        } else {
            struct product *tail = head;
            while (tail->next != NULL) {
                tail = tail->next;
            }
            tail->next = newNode;
        }
        printf("Da doc san pham: ID=%d, Ten=%s, Gia=%d, So luong=%d\n", newNode->ID, newNode->ten, newNode->gia, newNode->soLuong); 
        count++;
    }
    fclose(file);
    return 1;
}

// Xóa node yêu cầu thao tác cập nhật con trỏ prev->next = current->next
void xoaproduct(char *keyword) { 
    int id = atoi(keyword); 
    struct product *current = head;
    struct product *prev = NULL;

    while (current != NULL) {
        if (current->ID == id || strcmp(current->ten, keyword) == 0) { 
            struct product deletedProduct = *current;
            
            // Nếu node cần xóa là node đầu tiên
            if (prev == NULL) {
                head = current->next;
            } else { // Nếu node nằm ở giữa hoặc cuối
                prev->next = current->next;
            }
            
            free(current); // Giải phóng vùng nhớ của node
            count--; 
            
            saveProducts("products.txt"); 
            saveHistorydeleted("history.txt", deletedProduct); 
            printf("Da xoa san pham voi ID %d\n", id); 
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("San pham voi ID %d hoac ten %s khong ton tai\n", id, keyword);
}

void searchProduct(char *keyword) {
    int gia = atoi(keyword); 
    int id = atoi(keyword); 
    int found = 0; 
    
    struct product *current = head;
    while (current != NULL) {
        if (current->ID == id || strcmp(current->ten, keyword) == 0 || current->gia == gia) { 
            printf("San pham tim thay: ID=%d, Ten=%s, Gia=%d, So luong=%d\n", current->ID, current->ten, current->gia, current->soLuong); 
            found = 1; 
        }
        current = current->next;
    }
    if (!found) {
        printf("Khong tim thay san pham voi ID %d hoac ten %s\n", id, keyword); 
    }
}

void editProduct(char *keyword) {
    int id = atoi(keyword); 
    struct product *current = head;

    while (current != NULL) {
        if (current->ID == id || strcmp(current->ten, keyword) == 0) { 
            printf("Nhap ten san pham moi: ");
            scanf("%19s", current->ten); 
            printf("Nhap gia san pham moi: ");
            scanf("%d", &current->gia); 
            printf("Nhap so luong san pham moi: ");
            scanf("%d", &current->soLuong); 
            
            saveProducts("products.txt"); 
            saveHistoryEdit("history.txt", current); 
            printf("Da cap nhat thong tin san pham %s\n", current->ten); 
            return;
        }
        current = current->next;
    }
    printf("San pham voi ID %d khong ton tai\n", id); 
}

/* Các hàm sắp xếp sử dụng thuật toán Bubble Sort thay vì Insertion Sort 
   để dễ dàng tráo đổi dữ liệu (swapData) trên danh sách liên kết */
void sortByIDtangdan() {
    if (head == NULL) return;
    int swapped;
    struct product *ptr1;
    struct product *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (ptr1->ID > ptr1->next->ID) { 
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Da sap xep danh sach theo ID tang dan.\n"); 
}

void sortByIDgiamdan() {
    if (head == NULL) return;
    int swapped;
    struct product *ptr1;
    struct product *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (ptr1->ID < ptr1->next->ID) { 
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Da sap xep danh sach theo ID giam dan.\n"); 
}

void sortByTenAtoZ() {
    if (head == NULL) return;
    int swapped;
    struct product *ptr1;
    struct product *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->ten, ptr1->next->ten) > 0) { 
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Da sap xep danh sach theo ten tu A den Z.\n"); 
}

void sortByTenZtoA() {
    if (head == NULL) return;
    int swapped;
    struct product *ptr1;
    struct product *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (strcmp(ptr1->ten, ptr1->next->ten) < 0) { 
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Da sap xep danh sach theo ten tu Z den A.\n"); 
}

void sortByGiaTangDan() {
    if (head == NULL) return;
    int swapped;
    struct product *ptr1;
    struct product *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (ptr1->gia > ptr1->next->gia) { 
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Da sap xep danh sach theo gia tang dan.\n"); 
}

void sortByGiaGiamDan() {
    if (head == NULL) return;
    int swapped;
    struct product *ptr1;
    struct product *lptr = NULL;
    do {
        swapped = 0;
        ptr1 = head;
        while (ptr1->next != lptr) {
            if (ptr1->gia < ptr1->next->gia) { 
                swapData(ptr1, ptr1->next);
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    printf("Da sap xep danh sach theo gia giam dan.\n"); 
}

void statistics() {
    int totalcost = 0; 
    struct product *current = head;
    while (current != NULL) {
        totalcost += current->gia * current->soLuong; 
        current = current->next;
    }
    printf("Tong gia tri cua tat ca san pham: %d\n", totalcost); 
}

int main(void) {
    int select; 
    int selectSort; 
    char keyword[20]; 
    do {
        printf("\nChon chuc nang:\n"); 
        printf("1. Nhap san pham\n"); 
        printf("2. Xuat san pham\n"); 
        printf("3. Xoa san pham\n"); 
        printf("4. Doc san pham tu file\n"); 
        printf("5. Tim kiem san pham\n"); 
        printf("6. Chinh sua san pham\n"); 
        printf("7. Sap xep san pham\n"); 
        printf("0. Thoat\n"); 
        scanf("%d", &select); 

        switch (select) { 
        case 1:
            nhapProduct();
            break;
        case 2:
            xuatProduct();
            break;
        case 3:
            printf("Nhap ID hoac ten san pham can xoa: "); 
            scanf("%19s", keyword); 
            xoaproduct(keyword); 
            break;
        case 4:
            loadProducts("products.txt"); 
            break;
        case 5:
            printf("Nhap ID hoac ten san pham can tim: "); 
            scanf("%19s", keyword); 
            searchProduct(keyword); 
            break;
        case 6:
            printf("Nhap ID hoac ten san pham can chinh sua: "); 
            scanf("%19s", keyword); 
            editProduct(keyword); 
            break;
        case 7:
            printf("Chon chuc nang sap xep:\n");
            printf("1. Sap xep theo ID tang dan\n");
            printf("2. Sap xep theo ID giam dan\n");
            printf("3. Sap xep theo ten tu A den Z\n");
            printf("4. Sap xep theo ten tu Z den A\n");
            printf("5. Sap xep theo gia tang dan\n");
            printf("6. Sap xep theo gia giam dan\n");
            scanf("%d", &selectSort);
            switch (selectSort) {
            case 1: sortByIDtangdan(); break;
            case 2: sortByIDgiamdan(); break; 
            case 3: sortByTenAtoZ(); break; 
            case 4: sortByTenZtoA(); break; 
            case 5: sortByGiaTangDan(); break; 
            case 6: sortByGiaGiamDan(); break; 
            default: printf("Chuc nang sap xep khong hop le\n"); 
            }
            xuatProduct();
            saveProducts("products.txt"); 
            break;
        case 0:
            break;
        default:
            printf("Chuc nang khong hop le\n"); 
            break;
        }
    } while (select != 0);

    // Giải phóng toàn bộ danh sách liên kết thay vì gọi free(products) một lần
    struct product *current = head;
    while (current != NULL) {
        struct product *temp = current;
        current = current->next;
        free(temp);
    }
    
    return 0;
}