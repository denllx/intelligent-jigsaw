#include "logic.h"

//初始化行、列以及为开始和结束节点申请内存
void Solver::setScale(int _m,int _n){
    m=_m;
    n=_n;
    //初始化矩阵
    Start.matrix=new int*[m];
    End.matrix=new int*[n];
    for (int i=0;i<m;i++){
        Start.matrix[i]=new int[n];
        End.matrix[i]=new int[n];
    }
}

int Solver::calNow2end(Node *node){
    //当前节点到目标节点，所有数字所在格子的曼哈顿距离
    int nodeX,nodeY,endX,endY;
    int dis=0;//距离
    for (int index=0;index<m*n-1;index++){//index=0到7是合法编号
        for (int i=0;i<m;i++){
            for (int j=0;j<n;j++){
                if (node->matrix[i][j]==index){
                    nodeX=i;
                    nodeY=j;
                }//定位编号为index的小图在node中的位置
                if (End.matrix[i][j]==index){
                    endX=i;
                    endY=j;
                }//定位编号为index的小图在目标节点中的位置
            }
        }
        dis+=abs(nodeX-endX)+abs(nodeY-endY);//曼哈顿距离
    }
    return dis;
}

//为开始和结束节点的九宫格赋值
void Solver::setNode(int **idx){//idx为MainWindow::idx，即小图编号矩阵
    //九宫格的数字
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            Start.matrix[i][j]=idx[i][j];
            if (Start.matrix[i][j]==-1){
                Start.r=i;
                Start.c=j;
            }
        }
    }
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            End.matrix[i][j]=i*n+j;
        }
    }
    End.matrix[m-1][n-1]=-1;
    //启发函数
    Start.start2now=0;
    Start.now2end=calNow2end(&Start);
    Start.total=Start.now2end+Start.start2now;
}

bool Solver::isAble(){
    //可行的充要条件是逆序数之和同奇偶，证明见实验报告
    int* arrayStart=new int[m*n];
    int* arrayEnd=new int[m*n];
    int numStart=0,numEnd=0;//两个数组的正序数（正序数同奇偶等价于逆序数同奇偶）
    int k=0;
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            arrayStart[k]=Start.matrix[i][j];
            arrayEnd[k]=End.matrix[i][j];
            k++;
        }
    }//将matrix展开为一维数组

    for (int i=0;i<m*n;i++){
        for (int j=0;j<m*n;j++){
            if (arrayStart[i]>arrayStart[j]&&arrayStart[j]!=-1) numStart++;
            if (arrayEnd[i]>arrayEnd[j]&&arrayEnd[j]!=-1) numEnd++;
            //空格不参加数列的构成，因此在计数时大于空格(-1)的不计入正序对
        }
    }
    return (numStart-numEnd)%2==0;
}

//返回从start到node的可行路径，路径上每个节点是一个小图编号矩阵
vector<int**> Solver::path(Node *node){
    vector<int**> ret;
    if (node==&Start) {
        ret.push_back(Start.matrix);
        return ret;
    };//递归边界条件
    ret=path(node->father);
    ret.push_back(node->matrix);
    return ret;
}

//判断是否为目标状态
bool Solver::isEnd(int **idx){
    for (int i=0;i<m;i++){
        for (int j=0;j<n;j++){
            if (idx[i][j]!=End.matrix[i][j])
                return false;
        }
    }
    return true;
}

bool Solver::isEnd(Node *node){
    return isEnd(node->matrix);
}

//将开节点中total最小的节点放到首位
void Solver::sortOpen(){
    int min=open[0].total,minIdx=0;
    for (int i=1;i<=openCloseNum;i++){
        if (min>open[i].total&&open[i].total>-1){
            min=open[i].total;
            minIdx=i;
        }
    }
    OpenNode tmp;
    tmp=open[minIdx];
    open[minIdx]=open[0];
    open[0]=tmp;
}

void Solver::calFuncs(Node *f, Node *s){
    s->start2now=f->start2now+1;
    s->now2end=calNow2end(s);
    s->total=s->now2end+s->start2now;
}

void Solver::addToOpen(Node *node){
    openCloseNum++;//开节点和闭节点的总数增加
    openNum++;//开节点总数的增加
    open[openCloseNum].node=node;
    open[openCloseNum].total=node->total;
}

void Solver::copyMatrix(Node *f, Node *s){
    if (s->matrix=nullptr){
        s->matrix=new int*[m];
        for (int i=0;i<m;i++){
            s->matrix[i]=new int[n];
            for (int j=0;j<n;j++){
                s->matrix[i][j]=f->matrix[i][j];
            }
        }
    }
}

//父节点中的空格根据dir规定的方向移动，产生一个子节点
void Solver::move(Direction dir, Node *f){
    if (dir==LEFT&&f->c>0){
        Node* son=new Node;
        //复制九宫格
        copyMatrix(f,son);

        //移动空格
        son->matrix[f->r][f->c]=f->matrix[f->r][f->c-1];
        son->matrix[f->r][f->c-1]=-1;//空格左移

        //空格位置
        son->c=f->c-1;
        son->r=f->r;

        //禁止移动的方向、父亲
        son->parentdir=RIGHT;//不能向右移动
        son->father=f;

        //启发函数
        calFuncs(f,son);

        //添加到open表
        addToOpen(son);
    }

    else if (dir==RIGHT && f->c<n-1){
        Node* son=new Node;
        //复制九宫格
        copyMatrix(f,son);

        son->matrix[f->r][f->c]=f->matrix[f->r][f->c+1];
        son->matrix[f->r][f->c+1]==-1;

        //空格位置
        son->r=f->r;
        son->c=f->c+1;

        //禁止移动的方向、父亲
        son->parentdir=LEFT;
        son->father=f;

        //启发函数
        calFuncs(f,son);

        //加入open表
        addToOpen(son);
    }

    else if (dir==UP&&f->r>0){//向上
        Node* son=new Node;
        copyMatrix(f,son);
        son->matrix[f->r][f->c]=f->matrix[f->r-1][f->c];
        son->matrix[f->r-1][f->c]=-1;
        son->r=f->r-1;
        son->c=f->c;
        son->parentdir=DOWN;
        son->father=f;
        calFuncs(f,son);
        addToOpen(son);
    }

    else if (dir==DOWN&&f->r<m-1){//向下
        Node* son=new Node;
        copyMatrix(f,son);
        son->matrix[f->r][f->c]=f->matrix[f->r+1][f->c];
        son->matrix[f->r+1][f->c]=-1;
        son->r=f->r+1;
        son->c=f->c;
        son->parentdir=UP;
        son->father=f;
        calFuncs(f,son);
        addToOpen(son);
    }
}

void Solver::extend(Node* node){
    for (int i=0;i<4;i++){
        if (i!=node->parentdir){
            move(Direction(i),node);
        }
    }
}

vector<int**> Solver::solve(){
    open[0].node=&Start;//将start放入open表
    openNum=1;
    while(1){
        if (isEnd(open[0].node)){
            vector<int**> ret=path(open[0].node);
            return ret;
        }
        extend(open[0].node);
        open[0].node=nullptr;
        open[0].total=-1;
        openNum--;
        sortOpen();
    }
}
