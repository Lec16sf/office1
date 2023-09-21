#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//存储文字的结构
typedef struct Literal {
    int num;
    int flag;
    struct Literal *next;
} Literal;

//存储子句的结构
typedef struct Clause {
    int num;
    int flag;
    Literal *first;
    struct Clause *next;

} Clause;

typedef struct me {
    int num;
    struct me *next;
} me;

char filename[100]; //存储文件的名称
int argumentnum, clausenum; //存储变元和子句数量
int  times[1000005], flag[100005];
Clause *head;
Clause *test;   //存储子句的链表的表头

void createClause();
void destroyClause();
int evaluateClause();
void backtracking(int num);
int caltimes();
int DPLL(int count);
int dispose3(int num, int count);


int check()
{
    Clause *tepc1 = head->next;
    Literal *tepl1;
    int cal = 0, num, mark;
    while(tepc1) {
        tepl1 = tepc1->first;
        while(tepl1) {
            if(tepl1->num > 0) {
                num = tepl1->num;
                mark = 1;
            }
            else {
                num = tepl1->num * (-1);
                mark = 0;
            }
            if(mark) {
                if(flag[num] == 1) {
                    cal++;
                    break;
                }
            }
            else {
                if(flag[num] == -1) {
                    cal++;
                    break;
                }
            }
            tepl1 = tepl1->next;
        }
        tepc1 = tepc1->next;
    }
    if(cal == clausenum) {
        return 1;
    }
    else {
        return 0;
    }


}

void p()
{
    int t = 0;
    Clause *tepc1;
    Literal *tepl1;
    tepc1 = head->next;
    while(tepc1) {
        if(tepc1->flag > 0){
            tepl1 = tepc1->first;
            while(tepl1) {
                if(tepl1->flag > 0) {
                    //printf("%d ", tepl1->num);
                }
                tepl1 = tepl1->next;
            }
            t++;
            //printf("\n");
        } 
        tepc1 = tepc1->next;
    }
    //printf("\n\n%d\n\n\n", t);
}

int main()
{
    
    time_t t1, t2;
    t1 = clock();
    head =(Clause *) malloc (sizeof(Clause));
    head->next = NULL;
    scanf("%s", filename);
    createClause();
    if(DPLL(clausenum)) {
        printf("Y\n");
    }
    else {
        printf("N\n");
    }
    t2 = clock();
    for(int i = 1; i <= argumentnum; i++) {
        printf("%4d ", flag[i] * i);
        if(i%10 == 0) {
            printf("\n");
        }
    }
    printf("%ld", t2 - t1);
    return 0;
}

//读取cnf文件，并对文件内容进行处理和存储
void createClause() 
{
    char tep[100];
    FILE *fp;
    int i, num;
    Clause *lastc = head;
    Literal *lastl = NULL;
    if(!(fp = fopen(filename, "r"))) {
        printf("文件打开失败!\n");
    }
    fscanf(fp, "%s", tep);
    while(tep[0] != 'p' || tep[1] != 0) {
        fscanf(fp,"%s", tep);
    }
    fscanf(fp, "%s", tep);
    fscanf(fp, "%d %d", &argumentnum, &clausenum);
    for(i = 0; i < clausenum; i++) {
        lastc->next = (Clause *) malloc (sizeof(Clause));
        lastc = lastc->next;
        lastc->next = NULL;
        lastc->num = 0;
        lastc->flag = 1;
        fscanf(fp, "%d", &num);
        while(num) {
            if(!lastl) {
                lastc->first = (Literal *) malloc (sizeof(Literal));
                lastl = lastc->first;
            }
            else {
                lastl->next = (Literal *) malloc (sizeof(Literal));
                lastl = lastl->next; 
            }
            lastl->next = NULL;
            lastl->num = num;
            lastl->flag = 1;
            lastc->num++;
            fscanf(fp, "%d", &num);
        }
        lastl = NULL;
    }
    fclose(fp);
}

void destroyClause ()
{
    Clause *tepc1 = head->next, *tepc2;
    Literal *tepl1, *tepl2;
    while(tepc1) {
        tepl1 = tepc1->first;
        while(tepl1) {
            tepl2 = tepl1;
            tepl1 = tepl1->next;
            free(tepl2);
        }
        tepc2 = tepc1;
        tepc1 = tepc1->next;
        free(tepc2);
    }
}

void backtracking(int num)
{
    Clause *tepc2;
    Literal *tepl1;
    tepc2 = head->next;
    while(tepc2) {
        tepl1 = tepc2->first;
        if(tepc2->flag <= 0) {
            tepc2->flag += num;
            if(tepc2->flag > 1) {
                tepc2->flag = 1;
            }
        }
        while(tepl1) {
            if(tepl1->flag <= 0) {
                tepl1->flag += num;
                if(tepl1->flag >= 1) {
                    tepl1->flag = 1;
                    tepc2->num++;
                }
            }
            tepl1 = tepl1->next;
        }
        tepc2 = tepc2->next;
    }
}

int DPLL(int count)
{
    Clause *tepc1 = head->next;
    Literal *tepl1;
    int maxnum, tepcount, sinnum = 0;
    while(tepc1) {
        if(tepc1->num == 1 && tepc1->flag > 0) {
            tepl1 = tepc1->first;
            while(tepl1->flag != 1) {
                tepl1 = tepl1->next;
            }
            sinnum ++;
            if(tepl1->num > 0) {
                flag[tepl1->num] = 1;
            }
            else {
                flag[(-1) * tepl1->num] = -1;
            }
            count = dispose3(tepl1->num, count);
            if(count == -1) {
                backtracking(sinnum);
                return 0;
            }
            else if(count == 0 ) {
                backtracking(sinnum);
                return 1;
            }
        }
        tepc1 = tepc1->next;
    }
    maxnum = 0;
    tepc1 = head->next;
    while(tepc1) {
        if(tepc1->flag > 0) {
            tepl1 = tepc1->first;
            while(tepl1) {
                if(tepl1->flag > 0) {
                    maxnum = tepl1->num;
                    break;
                }
                tepl1 = tepl1->next;
            }
        }
        if(maxnum) {
            break;
        }
        tepc1 = tepc1->next;
    }
    if(maxnum > 0){
        flag[maxnum] = 1;
    }
    else {
        flag[(-1) * maxnum] = -1;
    }
    tepcount = dispose3(maxnum, count);
    if(tepcount == 0) {
        backtracking(1 + sinnum);
        return 1;
    }
    else if(tepcount > 0) {
        if(DPLL(tepcount)) {
            backtracking(1 + sinnum);
            return 1;
        }
        else {
            backtracking(1);
        }
    }
    else {
        backtracking(1);
    }
    maxnum *= -1;
    if(maxnum > 0){
        flag[maxnum] = 1;
    }
    else {
        flag[(-1) * maxnum] = -1;
    }
    tepcount = dispose3(maxnum, count);
    if(tepcount == 0) {
        backtracking(1 + sinnum);
        return 1;
    }
    else if(tepcount > 0) {
        if(DPLL(tepcount)) {
            backtracking(1 + sinnum);
            return 1;
        }
    }
    backtracking(1 + sinnum);
    return 0;
}


int dispose3(int num, int count)
{
    Clause *tepc1;
    Literal *tepl1;
    tepc1 = head->next;
    int flag = 1; 
    while(tepc1) {
        tepl1 = tepc1->first;
        if(tepc1->flag <= 0) {
            tepc1->flag--;
        }
        while(tepl1) {
            if(tepl1->flag <= 0) {
                tepl1->flag--;
            }
            else if(tepl1->num == num) {
                if(tepc1->flag == 1) {
                    count --; 
                    tepc1->flag = 0;  
                }
            }
            else if(tepl1->num + num == 0) {
                tepl1->flag = 0;
                tepc1->num --;
                if(tepc1->num == 0 && tepc1->flag > 0) {
                    flag = -1;
                }
            }
            tepl1 = tepl1->next;
        }
        tepc1 = tepc1->next;
    }
    if(count == 0) {
        return 0;
    }
    else  if(flag == -1) {
        return -1;
    }
    else {
        return count;
    }
}

int  caltimes()
{
    Clause *tepc1;
    Literal *tepl1;
    tepc1 = head->next;
    int num, max = 0, maxnum;
    while(tepc1) {
        if(tepc1->flag > 0) {
            tepl1 = tepc1->first;
            while(tepl1){ 
                if(tepl1->flag) {
                    if(tepl1->num < 0) {
                        num = (-1) * tepl1->num;
                    }
                    else {
                        num = tepl1->num;
                    }
                    times[num]++;
                    if(times[num] > max) {
                        max = times[num];
                        maxnum = num;
                    }
                }
                tepl1 = tepl1->next;
            }
        }
        tepc1 = tepc1->next;
    }
    return maxnum;
}
