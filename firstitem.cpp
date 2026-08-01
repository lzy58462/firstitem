#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
// 图书
typedef struct book {
    char isbn[20];          // ISBN（主键）身份证uid
    char title[100];        // 书名
    char author[50];        // 作者
    char publisher[50];     // 出版社
    int total;              // 馆藏总数
    int borrowed;           // 已借出数量
    int borrowCount;        // 累计借阅次数
    struct book* next;
} Book;

// 读者
typedef struct reader {
    char id[20];            // 借阅证号（主键）
    char name[50];          // 姓名
    char phone[15];         // 电话
    int borrowedCount;      // 当前已借数量（上限5本）
    struct reader* next;
} Reader;

// 借阅记录
typedef struct record {
    char readerId[20];
    char isbn[20];
    time_t borrowDate;      // 借书日期
    int returned;           // 0未还 1已还
    struct record* next;
} Record;

//工具函数
int timecheck(Record* rec) {
    if (time(NULL) - rec->borrowDate > 60) {
        return 1;
    }
    return 0;
}

int findbook(Book* head,char* isbn) {
    Book* p = head;
    while (p) {
        if (strcmp(p->isbn, isbn) == 0) {
            return 1;
        }
        p=p->next;
    }
    return 0;
}
int findreader(Reader* head, char* id) {
    Reader* p = head;
    while (p) {
        if (strcmp(p->id, id) == 0) {
            return 1;
        }
        p = p->next;
    }
    return 0;
}
//根据id找读者
Reader* getReader(Reader* head, char* id) {
    Reader* p = head;
    while (p) {
        if (strcmp(p->id, id) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
//根据isbn找书
Book* getBook(Book* head, char* isbn) {
    Book* p = head;
    while (p) {
        if (strcmp(p->isbn, isbn) == 0) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}
//book
void addBook(Book** head) {
    Book* p = (Book*)malloc(sizeof(Book));
    printf("isbn:\n");
    scanf_s("%s", p->isbn, (unsigned)sizeof(p->isbn));
    if (findbook(*head, p->isbn)) {
        printf("error isbn can't been same\n");
        free(p);
        return;
    }
    printf("请输入书名：\n");
    scanf_s("%s", p->title, (unsigned)sizeof(p->title));
    printf("请输入作者：\n");
    scanf_s("%s", p->author,(unsigned)sizeof(p->author));
    printf("请输入出版社：\n");
    scanf_s("%s", p->publisher,(unsigned)sizeof(p->publisher));
    printf("请输入馆藏总数：\n");
    scanf_s("%d", &p->total);
    if (p->total < 1) {
        printf("error number of total must higher than 1\n");
        free(p);
        return;
    }
    p->borrowCount = 0;
    p->borrowed = 0;
    p->next = *head;
    *head = p;
    printf("以存入\n");
}
void displayBooks(Book* head) {
    if (!head) {
        printf("图书为空");
        return;
    }
    Book* p = head;
    while (p) {
        printf("isbn:%s\n",p->isbn);
        printf("书名:%s\n", p->title);
        printf("作者:%s\n", p->author);
        printf("出版社:%s\n", p->publisher);
        printf("馆藏总量:%d\n", p->total);
        p = p->next;
    }
}
void searchBook(Book* head) {
    Book* p = head;
    char key[20];
    printf("输入isbn");
    scanf_s("%s", key, (unsigned)sizeof(key));
    while (p) {
        if (strcmp(p->isbn,key)==0) {
            printf("isbn:%s\n", p->isbn);
            printf("书名:%s\n", p->title);
            printf("作者:%s\n", p->author);
            printf("出版社:%s\n", p->publisher);
            printf("馆藏总量:%d\n", p->total);
            return;
        }
        p=p->next;
    }
    printf("not exist");
    return;
}
void deleteBook(Book** head) {
    Book* p = *head;
    Book* prev = NULL;
    char isbn[20];
    printf("isbn删除");
    scanf_s("%s", isbn, (unsigned)sizeof(isbn));
    while (p) {
        if (strcmp(p->isbn,isbn)==0) {
            if (p->borrowed != 0) {
                printf("以借阅不可以删除");
                return;
            }
            if (prev == NULL) {
                *head = p->next;
            }
            else {
                prev->next = p->next;
            }
            free(p);//为什么要free(p)
            printf("删除成功");
            return;
        }
        prev = p;
        p = p->next;
    }
}
void modifyBook(Book* head) {
    printf("isbn:");
    char isbn[20];
    scanf_s("%s", isbn,(unsigned)sizeof(isbn));
    Book* p = head;
    while (p) {
        if (strcmp(p->isbn, isbn) == 0) {
            printf("modifing");
            printf("请输入书名：\n");
            scanf_s("%s", p->title, (unsigned)sizeof(p->title));
            printf("请输入作者：\n");
            scanf_s("%s", p->author, (unsigned)sizeof(p->author));
            printf("请输入出版社：\n");
            scanf_s("%s", p->publisher, (unsigned)sizeof(p->publisher));
            printf("请输入馆藏总数：\n");
            scanf_s("%d", &p->total);
            if (p->total < 1) {
                printf("error number of total must higher than 1\n");
                return;
            }
            if (p->total < p->borrowed) {
                printf("error number of total must higher than borrowed\n");
                return;
            }
            return;
        }
        p = p->next;
    }
}
//reader
void addReader(Reader** head1) {
    Reader* p = (Reader*)malloc(sizeof(Reader));
    printf("输入id值");
    scanf_s("%s",p->id,(unsigned)sizeof(p->id));
    if (findreader(*head1, p->id)) {
        printf("error");
        free(p);
        return;
    }
    printf("输入姓名");
    scanf_s("%s", p->name, (unsigned)sizeof(p->name));
    printf("输入电话");
    scanf_s("%s", p->phone, (unsigned)sizeof(p->phone));
    p->borrowedCount = 0;
    p->next = *head1;
    *head1 = p;
    return;
}
void deleteReader(Reader** head1) {
    Reader* p = *head1;
    reader* prev = NULL;
    printf("id:\n");
    char id[20];
    scanf_s("%s",id,(unsigned)sizeof(id));
    while (p) {
        if (strcmp(id, p->id)==0) {
            if (prev == NULL) {
                *head1 = p->next;
            }
            else {
                prev->next = p->next;
            }
            free(p);
            return;
        }
        prev = p;
        p = p->next;
    }
    return;
}
void modifyReader(Reader* head1) {
    printf("id:");
    char id[20];
    //
    scanf_s("%s", id, (unsigned)sizeof(id));
    Reader* p = head1;
    while (p) {
        if (strcmp(p->id, id) == 0) {
            printf("modifing");
            printf("name：\n");
            scanf_s("%s", p->name, (unsigned)sizeof(p->name));
            printf("phone：\n");
            scanf_s("%s", p->phone, (unsigned)sizeof(p->phone));
            return;
        }
        p = p->next;
    }
    return;
}
void displayReaders(Reader* head1) {
    if (!head1) {
        printf("图书为空");
        return;
    }
    Reader* p = head1;
    while (p) {
        printf("id:%s\n", p->id);
        printf("name:%s\n", p->name);
        printf("phone:%s\n", p->phone);
        printf("borrowedcount:%s\n", p->borrowedCount);
        p = p->next;
    }
}
void searchReader(Reader* head1) {
    Reader* p = head1;
    char key[20];
    printf("输入id");
    scanf_s("%s", key, (unsigned)sizeof(key));
    while (p) {
        if (strcmp(p->id, key) == 0) {
            printf("name:%s\n", p->name);
            printf("phone:%s\n", p->phone);
            printf("borrowedcount:%s\n", p->borrowedCount);
            return;
        }
        p = p->next;
    }
    printf("not exist");
    return;
}
//borrow
void borrowBook(Book* bHead, Reader* rHead, Record** recHead) {
    char readerId[20];
    char isbn[20];
    printf("id:");
    scanf_s("%s", readerId,(unsigned)sizeof(readerId));
    //读者是否存在
    Reader* reader = getReader(rHead, readerId);
    if (reader == NULL) {
        printf("未登记");
        return;
    }
    printf("isbn:");
    //书书否存在
    scanf_s("%s", isbn, (unsigned)sizeof(isbn));
    Book* book = getBook(bHead, isbn);
    if (book == NULL) {
        printf("无此书");
        return;
    }
    //判断读者借了5本
    if (reader->borrowedCount >= 5) {
        printf("can't more than 5");
        return;
    }
    //图书是否有库存
    if (book->borrowed >= book->total) {
        printf("该书已被借阅");
        return;
    }
    reader->borrowedCount++;
    book->borrowCount++;
    book->borrowed++;
    Record* rec = (Record*)malloc(sizeof(Record));
    rec->borrowDate = time(NULL);
    struct tm localTime;
    localtime_s(&localTime, &rec->borrowDate);
    printf("borrowdate:year:%d,month:%d,day:%d\n", localTime.tm_year + 1900, localTime.tm_mon + 1, localTime.tm_mday);
    strcpy_s(rec->isbn, isbn); 
    strcpy_s(rec->readerId, readerId);
    rec->returned = 0;
    rec->next = *recHead;
    *recHead = rec;
}
void returnBook(Book* bHead, Reader* rHead, Record** recHead) {
    char readerId[20];
    char isbn[20];
    printf("id:");
    scanf_s("%s", readerId, (unsigned)sizeof(readerId));
    //读者是否存在
    Reader* reader = getReader(rHead, readerId);
    if (reader == NULL) {
        printf("未登记");
        return;
    }
    printf("isbn:");
    //书书否存在
    scanf_s("%s", isbn, (unsigned)sizeof(isbn));
    Book* book = getBook(bHead, isbn);
    if (book == NULL) {
        printf("无此书");
        return;
    }
    Record* rec = *recHead;
    while (rec) {
        if (strcmp(rec->readerId, readerId) == 0 && strcmp(rec->isbn, isbn) == 0 && rec->returned == 0) {
            rec->returned = 1;
            reader->borrowedCount--;
            book->borrowed--;
            printf("还书成功\n");
            return;
        }
        rec = rec->next;
    }
    printf("没有找到未归还的借阅记录\n");
}
void viewRecords(Record* recHead, Reader* rHead, Book* bHead) {
    if (!recHead) {
        printf("暂无借阅记录\n");
        return;
    }
    Record* record = recHead;
    while (record) {
        Reader* reader = getReader(rHead, record->readerId);
        Book* book = getBook(bHead, record->isbn);
        struct tm localTime;
        localtime_s(&localTime, &record->borrowDate);
        printf("--------------------\n");
        printf("读者ID:%s\n", record->readerId);
        printf("读者姓名:%s\n", reader ? reader->name : "未知读者");
        printf("ISBN:%s\n", record->isbn);
        printf("书名:%s\n", book ? book->title : "未知图书");
        printf("借书日期:%d-%02d-%02d %02d:%02d:%02d\n", localTime.tm_year + 1900,
            localTime.tm_mon + 1, localTime.tm_mday, localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
        printf("状态:%s\n", record->returned ? "已归还" : "未归还");
        record = record->next;
    }
    printf("--------------------\n");
}
void viewOverdue(Record* recHead, Reader* rHead, Book* bHead) {
    if (!recHead) {
        printf("不能为空");
        return;
    }
    Record* p = recHead;
    while (p) {
        if (timecheck(p)) {
            if (p->returned == 0) {
                printf("id:%s\n", p->readerId);
                printf("isbn:%s\n", p->isbn);
                printf("borrowdate:%d\n", p->borrowDate);
                printf("returned:%d\n", p->returned);
            }
        }
        p = p->next;
    }
}
void saveBooks(Book* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("图书文件保存失败\n");
        return;
    }
    while (head) {
        fprintf(fp, "%s %s %s %s %d %d %d\n", head->isbn, head->title, head->author,
            head->publisher, head->total, head->borrowed, head->borrowCount);
        head = head->next;
    }
    fclose(fp);
}

void saveReaders(Reader* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("读者文件保存失败\n");
        return;
    }
    while (head) {
        fprintf(fp, "%s %s %s %d\n", head->id, head->name, head->phone, head->borrowedCount);
        head = head->next;
    }
    fclose(fp);
}

void saveRecords(Record* head, const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("借阅记录文件保存失败\n");
        return;
    }
    while (head) {
        fprintf(fp, "%s %s %lld %d\n", head->readerId, head->isbn,
            (long long)head->borrowDate, head->returned);
        head = head->next;
    }
    fclose(fp);
}

void saveAll(Book* books, Reader* readers, Record* records) {
    saveBooks(books, "books.txt");
    saveReaders(readers, "readers.txt");
    saveRecords(records, "records.txt");
}

void loadBooks(Book** head, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    Book* p;
    while ((p = (Book*)malloc(sizeof(Book))) != NULL &&
        fscanf(fp, "%19s %99s %49s %49s %d %d %d", p->isbn, p->title, p->author,
            p->publisher, &p->total, &p->borrowed, &p->borrowCount) == 7) {
        p->next = *head;
        *head = p;
    }
    free(p);
    fclose(fp);
}
void loadReaders(Reader** head, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    Reader* p;
    while ((p = (Reader*)malloc(sizeof(Reader))) != NULL &&
        fscanf(fp, "%19s %49s %14s %d", p->id, p->name, p->phone, &p->borrowedCount) == 4) {
        p->next = *head;
        *head = p;
    }
    free(p);
    fclose(fp);
}
void loadRecords(Record** head, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    Record* p;
    long long borrowDate;
    while ((p = (Record*)malloc(sizeof(Record))) != NULL &&
        fscanf(fp, "%19s %19s %lld %d", p->readerId, p->isbn, &borrowDate, &p->returned) == 4) {
        p->borrowDate = (time_t)borrowDate;
        p->next = *head;
        *head = p;
    }
    free(p);
    fclose(fp);
}
int main() {
    Book* head = NULL;
    Reader* head1 = NULL;
    Record* head2 = NULL;
    int choice;
    loadBooks(&head, "books.txt");
    loadReaders(&head1, "readers.txt");
    loadRecords(&head2, "records.txt");

    while (1) {
        printf("0.保存并退出\n");
        printf("1.添加图书\n");
        printf("2.显示所有图书\n");
        printf("3.测试isbn是否可用\n");
        printf("4.isbn查询\n");
        printf("5.isbn删除\n");
        printf("6.修改\n");
        printf("7.add reader\n");
        printf("8.displayreaders\n");
        printf("9.search reader\n");
        printf("10.delet reader\n");
        printf("11.modify\n");
        printf("12.借书\n");
        printf("13.显示借阅记录\n");
        printf("14.显示逾期记录\n");
        printf("15.还书\n");
        scanf_s("%d", &choice);
        switch (choice) {
        case 0:
            saveAll(head, head1, head2);
            printf("数据已保存\n");
            return 0;
        case 1:
            addBook(&head);
            break;
        case 2:
            displayBooks(head);
            break;
        case 3: {
            char isbn[20];
            printf("isbn:\n");
            scanf_s("%s", isbn, (unsigned)sizeof(isbn));
            if (findbook(head, isbn)) {
                printf("重复\n");
            }
            else {
                printf("正确\n");
            }
            break;
        }
        case 4:
            printf("isbn:\n");
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
            printf("id:\n");
            searchReader(head1);
            break;
        case 10:
            deleteReader(&head1);
            break;
        case 11:
            modifyReader(head1);
            break;
        case 12:
            borrowBook(head,head1,&head2);
            break;
        case 13:
            viewRecords(head2,head1,head);
            break;
        case 14:
            viewOverdue(head2,head1,head);
            break;
        case 15:
            returnBook(head,head1,&head2);
            break;
        default:
            printf("无效选项\n");
            break;
        }
        saveAll(head, head1, head2);
    }
}
