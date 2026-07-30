#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <locale.h>

typedef struct book {
    char isbn[20];
    char title[100];
    char author[50];
    char publisher[50];
    int total;
    int borrowed;
    int borrowCount;
    struct book* next;
} Book;

typedef struct reader {
    char id[20];
    char name[50];
    char phone[15];
    int borrowedCount;
    struct reader* next;
} Reader;

typedef struct record {
    char readerId[20];
    char isbn[20];
    time_t borrowDate;
    time_t returnDate;
    int returned;
    struct record* next;
} Record;

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
    }
}

void readLine(char* buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        buffer[0] = '\0';
        return;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}

void printTime(time_t t) {
    if (t == 0) {
        printf("N/A");
        return;
    }
    struct tm* tm_info = localtime(&t);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("%s", buf);
}

int findBook(Book* head, const char* isbn) {
    Book* p = head;
    while (p) {
        if (strcmp(p->isbn, isbn) == 0) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

int findReader(Reader* head, const char* id) {
    Reader* p = head;
    while (p) {
        if (strcmp(p->id, id) == 0) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}

Reader* getReader(Reader* head, const char* id) {
    Reader* p = head;
    while (p) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

Book* getBook(Book* head, const char* isbn) {
    Book* p = head;
    while (p) {
        if (strcmp(p->isbn, isbn) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

void addBook(Book** head) {
    Book* p = (Book*)malloc(sizeof(Book));
    if (!p) {
        printf("内存分配失败。\n");
        return;
    }
    printf("请输入isbn：\n");
    scanf("%19s", p->isbn);
    clearInputBuffer();
    if (findBook(*head, p->isbn)) {
        printf("错误：isbn已存在。\n");
        free(p);
        return;
    }
    printf("请输入书名：\n");
    readLine(p->title, sizeof(p->title));
    printf("请输入作者：\n");
    readLine(p->author, sizeof(p->author));
    printf("请输入出版社：\n");
    readLine(p->publisher, sizeof(p->publisher));
    printf("请输入馆藏总数：\n");
    if (scanf("%d", &p->total) != 1) {
        printf("无效的数字。\n");
        clearInputBuffer();
        free(p);
        return;
    }
    clearInputBuffer();
    if (p->total < 1) {
        printf("错误：馆藏总数必须大于0。\n");
        free(p);
        return;
    }
    p->borrowCount = 0;
    p->borrowed = 0;
    p->next = *head;
    *head = p;
    printf("图书已保存。\n");
}

void displayBooks(Book* head) {
    if (!head) {
        printf("图书为空。\n");
        return;
    }
    Book* p = head;
    while (p) {
        printf("---------------------------\n");
        printf("ISBN：%s\n", p->isbn);
        printf("书名：%s\n", p->title);
        printf("作者：%s\n", p->author);
        printf("出版社：%s\n", p->publisher);
        printf("馆藏总数：%d\n", p->total);
        printf("已借出：%d\n", p->borrowed);
        printf("累计借阅次数：%d\n", p->borrowCount);
        p = p->next;
    }
}

void searchBook(Book* head) {
    char key[20];
    printf("输入isbn：");
    scanf("%19s", key);
    clearInputBuffer();
    Book* p = getBook(head, key);
    if (!p) {
        printf("未找到该图书。\n");
        return;
    }
    printf("ISBN：%s\n", p->isbn);
    printf("书名：%s\n", p->title);
    printf("作者：%s\n", p->author);
    printf("出版社：%s\n", p->publisher);
    printf("馆藏总数：%d\n", p->total);
    printf("已借出：%d\n", p->borrowed);
    printf("累计借阅次数：%d\n", p->borrowCount);
}

void deleteBook(Book** head) {
    char isbn[20];
    printf("输入要删除的isbn：");
    scanf("%19s", isbn);
    clearInputBuffer();
    Book* p = *head;
    Book* prev = NULL;
    while (p) {
        if (strcmp(p->isbn, isbn) == 0) {
            if (p->borrowed != 0) {
                printf("该书已被借出，无法删除。\n");
                return;
            }
            if (prev == NULL) {
                *head = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            printf("删除成功。\n");
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("未找到该图书。\n");
}

void modifyBook(Book* head) {
    char isbn[20];
    printf("输入要修改的isbn：");
    scanf("%19s", isbn);
    clearInputBuffer();
    Book* p = getBook(head, isbn);
    if (!p) {
        printf("未找到该图书。\n");
        return;
    }
    printf("请输入新书名（回车保留原值）：\n");
    char buffer[100];
    readLine(buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(p->title, buffer, sizeof(p->title) - 1);
        p->title[sizeof(p->title) - 1] = '\0';
    }
    printf("请输入新作者（回车保留原值）：\n");
    readLine(buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(p->author, buffer, sizeof(p->author) - 1);
        p->author[sizeof(p->author) - 1] = '\0';
    }
    printf("请输入新出版社（回车保留原值）：\n");
    readLine(buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(p->publisher, buffer, sizeof(p->publisher) - 1);
        p->publisher[sizeof(p->publisher) - 1] = '\0';
    }
    printf("请输入新的馆藏总数（0表示不修改）：\n");
    int total;
    if (scanf("%d", &total) == 1) {
        if (total > 0) {
            if (total < p->borrowed) {
                printf("错误：馆藏总数不能小于当前借出数量。\n");
            } else {
                p->total = total;
            }
        }
    }
    clearInputBuffer();
    printf("修改完成。\n");
}

void addReader(Reader** head1) {
    Reader* p = (Reader*)malloc(sizeof(Reader));
    if (!p) {
        printf("内存分配失败。\n");
        return;
    }
    printf("请输入读者id：");
    scanf("%19s", p->id);
    clearInputBuffer();
    if (findReader(*head1, p->id)) {
        printf("错误：该读者已存在。\n");
        free(p);
        return;
    }
    printf("请输入姓名：\n");
    readLine(p->name, sizeof(p->name));
    printf("请输入电话：\n");
    readLine(p->phone, sizeof(p->phone));
    p->borrowedCount = 0;
    p->next = *head1;
    *head1 = p;
    printf("读者已添加。\n");
}

void deleteReader(Reader** head1) {
    char id[20];
    printf("请输入要删除的读者id：");
    scanf("%19s", id);
    clearInputBuffer();
    Reader* p = *head1;
    Reader* prev = NULL;
    while (p) {
        if (strcmp(p->id, id) == 0) {
            if (prev == NULL) {
                *head1 = p->next;
            } else {
                prev->next = p->next;
            }
            free(p);
            printf("读者删除成功。\n");
            return;
        }
        prev = p;
        p = p->next;
    }
    printf("未找到该读者。\n");
}

void modifyReader(Reader* head1) {
    char id[20];
    printf("请输入要修改的读者id：");
    scanf("%19s", id);
    clearInputBuffer();
    Reader* p = getReader(head1, id);
    if (!p) {
        printf("未找到该读者。\n");
        return;
    }
    char buffer[50];
    printf("请输入新姓名（回车保留原值）：\n");
    readLine(buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(p->name, buffer, sizeof(p->name) - 1);
        p->name[sizeof(p->name) - 1] = '\0';
    }
    printf("请输入新电话（回车保留原值）：\n");
    readLine(buffer, sizeof(buffer));
    if (buffer[0] != '\0') {
        strncpy(p->phone, buffer, sizeof(p->phone) - 1);
        p->phone[sizeof(p->phone) - 1] = '\0';
    }
    printf("读者信息已更新。\n");
}

void displayReaders(Reader* head1) {
    if (!head1) {
        printf("读者列表为空。\n");
        return;
    }
    Reader* p = head1;
    while (p) {
        printf("---------------------------\n");
        printf("id：%s\n", p->id);
        printf("姓名：%s\n", p->name);
        printf("电话：%s\n", p->phone);
        printf("当前借书数量：%d\n", p->borrowedCount);
        p = p->next;
    }
}

void searchReader(Reader* head1) {
    char key[20];
    printf("输入id：");
    scanf("%19s", key);
    clearInputBuffer();
    Reader* p = getReader(head1, key);
    if (!p) {
        printf("未找到该读者。\n");
        return;
    }
    printf("id：%s\n", p->id);
    printf("姓名：%s\n", p->name);
    printf("电话：%s\n", p->phone);
    printf("当前借书数量：%d\n", p->borrowedCount);
}

Record* createRecord(const char* readerId, const char* isbn, time_t borrowDate, time_t returnDate, int returned) {
    Record* rec = (Record*)malloc(sizeof(Record));
    if (!rec) {
        return NULL;
    }
    strncpy(rec->readerId, readerId, sizeof(rec->readerId) - 1);
    rec->readerId[sizeof(rec->readerId) - 1] = '\0';
    strncpy(rec->isbn, isbn, sizeof(rec->isbn) - 1);
    rec->isbn[sizeof(rec->isbn) - 1] = '\0';
    rec->borrowDate = borrowDate;
    rec->returnDate = returnDate;
    rec->returned = returned;
    rec->next = NULL;
    return rec;
}

void borrowBook(Book* bHead, Reader* rHead, Record** recHead) {
    char readerId[20];
    char isbn[20];
    printf("请输入读者id：");
    scanf("%19s", readerId);
    clearInputBuffer();
    Reader* reader = getReader(rHead, readerId);
    if (!reader) {
        printf("未登记该读者。\n");
        return;
    }
    printf("请输入isbn：");
    scanf("%19s", isbn);
    clearInputBuffer();
    Book* book = getBook(bHead, isbn);
    if (!book) {
        printf("无此书。\n");
        return;
    }
    if (reader->borrowedCount >= 5) {
        printf("该读者已达到借书上限。\n");
        return;
    }
    if (book->borrowed >= book->total) {
        printf("该书暂无可借库存。\n");
        return;
    }
    reader->borrowedCount++;
    book->borrowCount++;
    book->borrowed++;
    Record* rec = createRecord(readerId, isbn, time(NULL), 0, 0);
    if (!rec) {
        printf("借阅记录创建失败。\n");
        return;
    }
    rec->next = *recHead;
    *recHead = rec;
    printf("借书成功。\n");
}

void returnBook(Book* bHead, Reader* rHead, Record* recHead) {
    char readerId[20];
    char isbn[20];
    printf("请输入读者id：");
    scanf("%19s", readerId);
    clearInputBuffer();
    Reader* reader = getReader(rHead, readerId);
    if (!reader) {
        printf("未登记该读者。\n");
        return;
    }
    printf("请输入isbn：");
    scanf("%19s", isbn);
    clearInputBuffer();
    Book* book = getBook(bHead, isbn);
    if (!book) {
        printf("无此书。\n");
        return;
    }
    Record* p = recHead;
    while (p) {
        if (strcmp(p->readerId, readerId) == 0 && strcmp(p->isbn, isbn) == 0 && p->returned == 0) {
            p->returned = 1;
            p->returnDate = time(NULL);
            reader->borrowedCount--;
            book->borrowed--;
            printf("还书成功。\n");
            return;
        }
        p = p->next;
    }
    printf("未找到该借阅记录，或该图书已归还。\n");
}

void viewRecords(Record* recHead) {
    if (!recHead) {
        printf("借阅记录为空。\n");
        return;
    }
    Record* p = recHead;
    while (p) {
        printf("---------------------------\n");
        printf("读者id：%s\n", p->readerId);
        printf("ISBN：%s\n", p->isbn);
        printf("借书时间：");
        printTime(p->borrowDate);
        printf("\n");
        printf("是否归还：%s\n", p->returned ? "是" : "否");
        if (p->returned) {
            printf("归还时间：");
            printTime(p->returnDate);
            printf("\n");
        }
        p = p->next;
    }
}

void viewOverdue(Record* recHead) {
    if (!recHead) {
        printf("借阅记录为空。\n");
        return;
    }
    time_t now = time(NULL);
    int found = 0;
    Record* p = recHead;
    while (p) {
        if (!p->returned) {
            double diff = difftime(now, p->borrowDate);
            if (diff > 30.0 * 24.0 * 60.0 * 60.0) {
                if (!found) {
                    printf("逾期记录：\n");
                }
                found = 1;
                printf("---------------------------\n");
                printf("读者id：%s\n", p->readerId);
                printf("ISBN：%s\n", p->isbn);
                printf("借书时间：");
                printTime(p->borrowDate);
                printf("\n");
            }
        }
        p = p->next;
    }
    if (!found) {
        printf("目前没有逾期记录。\n");
    }
}

void freeBooks(Book* head) {
    while (head) {
        Book* next = head->next;
        free(head);
        head = next;
    }
}

void freeReaders(Reader* head) {
    while (head) {
        Reader* next = head->next;
        free(head);
        head = next;
    }
}

void freeRecords(Record* head) {
    while (head) {
        Record* next = head->next;
        free(head);
        head = next;
    }
}

void saveData(Book* bookHead, Reader* readerHead, Record* recHead) {
    FILE* fBooks = fopen("books.txt", "w");
    FILE* fReaders = fopen("readers.txt", "w");
    FILE* fRecords = fopen("records.txt", "w");
    if (!fBooks || !fReaders || !fRecords) {
        printf("无法打开保存文件。\n");
        if (fBooks) fclose(fBooks);
        if (fReaders) fclose(fReaders);
        if (fRecords) fclose(fRecords);
        return;
    }
    for (Book* p = bookHead; p; p = p->next) {
        fprintf(fBooks, "%s\t%s\t%s\t%s\t%d\t%d\t%d\n", p->isbn, p->title, p->author, p->publisher, p->total, p->borrowed, p->borrowCount);
    }
    for (Reader* p = readerHead; p; p = p->next) {
        fprintf(fReaders, "%s\t%s\t%s\t%d\n", p->id, p->name, p->phone, p->borrowedCount);
    }
    for (Record* p = recHead; p; p = p->next) {
        fprintf(fRecords, "%s\t%s\t%lld\t%lld\t%d\n", p->readerId, p->isbn, (long long)p->borrowDate, (long long)p->returnDate, p->returned);
    }
    fclose(fBooks);
    fclose(fReaders);
    fclose(fRecords);
    printf("数据已保存到 books.txt、readers.txt、records.txt。\n");
}

void loadData(Book** bookHead, Reader** readerHead, Record** recHead) {
    freeBooks(*bookHead);
    freeReaders(*readerHead);
    freeRecords(*recHead);
    *bookHead = NULL;
    *readerHead = NULL;
    *recHead = NULL;

    FILE* fBooks = fopen("books.txt", "r");
    FILE* fReaders = fopen("readers.txt", "r");
    FILE* fRecords = fopen("records.txt", "r");
    if (!fBooks || !fReaders || !fRecords) {
        printf("无法打开数据文件，请确认 books.txt、readers.txt、records.txt 是否存在。\n");
        if (fBooks) fclose(fBooks);
        if (fReaders) fclose(fReaders);
        if (fRecords) fclose(fRecords);
        return;
    }
    char line[512];
    while (fgets(line, sizeof(line), fBooks)) {
        Book* p = (Book*)malloc(sizeof(Book));
        if (!p) break;
        char* token = strtok(line, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->isbn, token, sizeof(p->isbn) - 1);
        p->isbn[sizeof(p->isbn) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->title, token, sizeof(p->title) - 1);
        p->title[sizeof(p->title) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->author, token, sizeof(p->author) - 1);
        p->author[sizeof(p->author) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->publisher, token, sizeof(p->publisher) - 1);
        p->publisher[sizeof(p->publisher) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) { free(p); continue; }
        p->total = atoi(token);
        token = strtok(NULL, "\t\n");
        p->borrowed = token ? atoi(token) : 0;
        token = strtok(NULL, "\t\n");
        p->borrowCount = token ? atoi(token) : 0;
        p->next = *bookHead;
        *bookHead = p;
    }
    while (fgets(line, sizeof(line), fReaders)) {
        Reader* p = (Reader*)malloc(sizeof(Reader));
        if (!p) break;
        char* token = strtok(line, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->id, token, sizeof(p->id) - 1);
        p->id[sizeof(p->id) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->name, token, sizeof(p->name) - 1);
        p->name[sizeof(p->name) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) { free(p); continue; }
        strncpy(p->phone, token, sizeof(p->phone) - 1);
        p->phone[sizeof(p->phone) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        p->borrowedCount = token ? atoi(token) : 0;
        p->next = *readerHead;
        *readerHead = p;
    }
    while (fgets(line, sizeof(line), fRecords)) {
        char* token = strtok(line, "\t\n");
        if (!token) continue;
        char readerId[20];
        strncpy(readerId, token, sizeof(readerId) - 1);
        readerId[sizeof(readerId) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        if (!token) continue;
        char isbnValue[20];
        strncpy(isbnValue, token, sizeof(isbnValue) - 1);
        isbnValue[sizeof(isbnValue) - 1] = '\0';
        token = strtok(NULL, "\t\n");
        time_t borrowDate = token ? (time_t)atoll(token) : 0;
        token = strtok(NULL, "\t\n");
        time_t returnDate = token ? (time_t)atoll(token) : 0;
        token = strtok(NULL, "\t\n");
        int returned = token ? atoi(token) : 0;
        Record* rec = createRecord(readerId, isbnValue, borrowDate, returnDate, returned);
        if (!rec) continue;
        rec->next = *recHead;
        *recHead = rec;
    }
    fclose(fBooks);
    fclose(fReaders);
    fclose(fRecords);
    printf("数据已从 books.txt、readers.txt 和 records.txt 加载。\n");
}

void showCurrentTime(void) {
    time_t now = time(NULL);
    printf("当前系统时间：");
    printTime(now);
    printf("\n");
}

int main(void) {
    setlocale(LC_ALL, "");
    system("chcp 65001 > nul");

    Book* head = NULL;
    Reader* head1 = NULL;
    Record* head2 = NULL;
    int choice;
    while (1) {
        printf("\n=== 图书馆管理系统 ===\n");
        printf("1. 添加图书\n");
        printf("2. 显示所有图书\n");
        printf("3. isbn可用性检测\n");
        printf("4. 查询图书\n");
        printf("5. 删除图书\n");
        printf("6. 修改图书\n");
        printf("7. 添加读者\n");
        printf("8. 显示所有读者\n");
        printf("9. 查询读者\n");
        printf("10. 删除读者\n");
        printf("11. 修改读者\n");
        printf("12. 借书\n");
        printf("13. 还书\n");
        printf("14. 显示借阅记录\n");
        printf("15. 显示逾期记录\n");
        printf("16. 保存数据到文件\n");
        printf("17. 从文件加载数据\n");
        printf("18. 显示当前时间\n");
        printf("0. 退出\n");
        printf("请选择：");
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("输入无效。\n");
            continue;
        }
        clearInputBuffer();
        switch (choice) {
            case 1:
                addBook(&head);
                break;
            case 2:
                displayBooks(head);
                break;
            case 3: {
                char isbn[20];
                printf("请输入isbn：");
                scanf("%19s", isbn);
                clearInputBuffer();
                if (findBook(head, isbn)) {
                    printf("该ISBN已存在。\n");
                } else {
                    printf("该ISBN可以使用。\n");
                }
                break;
            }
            case 4:
                searchBook(head);
                break;
            case 5:
                deleteBook(&head);
                break;
            case 6:
                modifyBook(head);
                break;
            case 7:
                addReader(&head1);
                break;
            case 8:
                displayReaders(head1);
                break;
            case 9:
                searchReader(head1);
                break;
            case 10:
                deleteReader(&head1);
                break;
            case 11:
                modifyReader(head1);
                break;
            case 12:
                borrowBook(head, head1, &head2);
                break;
            case 13:
                returnBook(head, head1, head2);
                break;
            case 14:
                viewRecords(head2);
                break;
            case 15:
                viewOverdue(head2);
                break;
            case 16:
                saveData(head, head1, head2);
                break;
            case 17:
                loadData(&head, &head1, &head2);
                break;
            case 18:
                showCurrentTime();
                break;
            case 0:
                saveData(head, head1, head2);
                freeBooks(head);
                freeReaders(head1);
                freeRecords(head2);
                printf("退出程序。\n");
                return 0;
        }
    }
}
