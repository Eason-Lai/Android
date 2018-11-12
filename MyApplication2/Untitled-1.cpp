class NFA_Node;
class Trans//trans类：接收某个字母（incept），可以到某个状态结点(des)
{
public:
    char incept;
    NFA_Node* des;
    Trans(char incept, NFA_Node* des)
    {
        this->incept = incept;
        this->des = des;
    }
};

class NFA_Node//NFA_Node类：每个状态结点的ID（state_ID）,该结点的trans,是否被访问过（visit）
{
public:
    int stateID;
    vector<Trans*> t;
    bool visit;
    NFA_Node(int stateID)
    {
        visit = false;
        this->stateID = stateID;
    }
    void AddTrans(Trans* tt)
    {
        t.push_back(tt);
    }

};

class NFA//NFA类：整个NFA图的开始状态结点（start），结束状态结点（end）
{
public:
    NFA_Node* start;
    NFA_Node* end;
    NFA() {}
    NFA(int SID, char c)
    {
        NFA_Node* s1 = new NFA_Node(SID);
        NFA_Node* s2 = new NFA_Node(SID + 1);
        Trans* tt = new Trans(c, s2);
        s1->AddTrans(tt);
        start = s1;
        end = s2;
    }

};

class Converter  //正则转换为NFA
{
public:
    int S_ID;//状态结点ID数
    Converter(string str)
    {
        pretreat(str);//添加连接运算符
        Houzhui(this->lamb);//转化为后缀表达式
        S_ID = 1;//初始状态ID为1
    }
    Converter() { S_ID = 1; }

    void show()//显示后缀表达式
    {
        cout << this->lamb << endl;
    }


    NFA ToNFA()//后缀正则表达式转换为NFA
    {
        //stNFA.Clear();
        //Operator_Stack.Clear();
        NFA tempb, tempb1, tempb2;

        char tempc1;
        for (int i = 0;i < lamb.size();i++)//如果是运算符，出NFA栈顶的两个元素进行运算
        {
            tempc1 = lamb[i];
            if (isOperator(tempc1))
            {
                switch (tempc1)
                {
                case '|':
                    tempb1 = stNFA.top();
                    stNFA.pop();
                    tempb2 = stNFA.top();
                    stNFA.pop();
                    tempb1 = Union(tempb2, tempb1);
                    stNFA.push(tempb1);
                    break;
                case '&':
                    tempb1 = stNFA.top();
                    stNFA.pop();
                    tempb2 = stNFA.top();
                    stNFA.pop();
                    tempb2 = Connect(tempb1, tempb2);
                    stNFA.push(tempb2);
                    break;
                case '*':
                    tempb1 = stNFA.top();
                    stNFA.pop();
                    tempb1 = Closure(tempb1);
                    stNFA.push(tempb1);
                    break;
                }
            }
            else//如果是可接受的状态（字母），则新增一个NFA，将NFA入栈
            {
                tempb = NFA(S_ID, tempc1);
                S_ID += 2;
                stNFA.push(tempb);
            }
        }
        tempb = stNFA.top();
        stNFA.pop();
        return tempb;//返回栈顶，NFA的起始状态
    }


private:
    stack<NFA> stNFA;
    stack<char> Operator_Stack;
    string lamb;//lamb为str经处理转换成的字符串
    bool isOperator(char c)//是否为运算符
    {
        switch (c)
        {
        case '|':
        case '&':
        case '(':
        case ')':
        case '!':
        case '*': return true;
        default: return false;
        }
    }

    int getOperatorNumber(char t1)//得到运算符优先级
    {
        switch (t1)
        {
        case '$': return 0;
        case '!': return 1;
        case ')': return 2;
        case '|': return 3;
        case '&': return 4;
        case '*': return 5;
        case '(': return 6;
        default: return 7;
        }
    }

    bool Operator_Less_Than(char t1, char t2)//判断运算符优先级别，返回1为A的优先级小于等于B
    {
        int temp1 = getOperatorNumber(t1);
        int temp2 = getOperatorNumber(t2);
        if (temp1 <= temp2)
            return true;
        return false;
    }


    void pretreat(string str)//输入正则表达式 使ab变成a&b（显示表达连接运算符）
    {
        int i = 0,n;
        char c, pc;

        pc = str[i];
        c = str[++i];
        //while (str[i] != '\0')
        qDebug()<<str.length()<<endl;
        n = str.length();
        //while ( i != str.length() )
        while (str[i] != '\0')
        {
            if (((pc == ')'&&c == '(')) || (!isOperator(pc) && !isOperator(c)) || (!isOperator(pc) && c == '(') || (pc == ')'&&!isOperator(c)) || (pc == '*'&&!isOperator(c)) || (pc == '*'&&c == '('))//四种情况需要加&
                str.insert(i, "&");
            if ( i+1 == str.length() ) break;
            pc = str[i++];
            c = str[i];
            qDebug()<<i<<endl;
            qDebug()<<str[i]<<endl;
            qDebug()<<str.length()<<endl;
        }
        str += "!";//结束标志
        qDebug()<<str.length()<<endl;
        this->lamb = str;
    }


    void Houzhui(string lamb)//将中缀表达式转化为后缀表达式
    {
        string l = "";
        Operator_Stack.push('$');//起始标志
        char tempc, tempc2;

        for (int i = 0;i < lamb.size();i++)
        {
            tempc = lamb[i];
            if (isOperator(tempc))
            {
                switch (tempc)
                {
                case '(': Operator_Stack.push(tempc); break;
                case ')':
                    while (Operator_Stack.top() != '(')
                    {
                        tempc2 = Operator_Stack.top();
                        Operator_Stack.pop();
                        l += tempc2;
                    }
                    Operator_Stack.pop();
                    break;
                default:
                    tempc2 = Operator_Stack.top();//和栈顶的元素比较优先级
                    while (tempc2 != '('&&Operator_Less_Than(tempc, tempc2))//当前运算符优先级比栈顶运算符优先级小，则栈顶出栈，当前入栈
                    {
                        tempc2 = Operator_Stack.top();
                        Operator_Stack.pop();
                        l += tempc2;
                        tempc2 = Operator_Stack.top();
                    }
                    Operator_Stack.push(tempc);
                    break;
                }
            }
            else
                l += tempc;
        }
        this->lamb = l;
    }

    NFA& Connect(NFA G1, NFA G2) //把G1连在G2后面
    {
        Trans* t = new Trans('@', G1.start);
        G2.end->AddTrans(t);
        G2.end = G1.end;
        return G2;//返回一个NFA,G2的end改变了
    }
    NFA& Union(NFA G1, NFA G2) //G1|G2
    {
        NFA_Node* n1 = new NFA_Node(S_ID++);
        Trans* t1 = new Trans('@', G1.start);
        Trans* t2 = new Trans('@', G2.start);
        n1->AddTrans(t1);
        n1->AddTrans(t2);

        NFA_Node* n2 = new NFA_Node(S_ID++);
        Trans* t3 = new Trans('@', n2);
        Trans* t4 = new Trans('@', n2);
        G1.end->AddTrans(t3);
        G2.end->AddTrans(t4);

        NFA* G = new NFA();
        G->start = n1;
        G->end = n2;

        return *G;//返回一个NFA
    }
    NFA& Closure(NFA G2) //G2*
    {
        Trans* t = new Trans('#', G2.start);//#表示是重复的空输入
        G2.end->AddTrans(t);

        NFA_Node* n1 = new NFA_Node(S_ID++);
        Trans* t1 = new Trans('@', n1);
        G2.end->AddTrans(t1);

        NFA_Node* n2 = new NFA_Node(S_ID++);
        Trans* t2 = new Trans('@', G2.start);
        n2->AddTrans(t2);


        Trans* t3 = new Trans('@', n1);
        n2->AddTrans(t3);

        NFA* G = new NFA();
        G->start = n2;
        G->end = n1;

        return *G;
    }
};

void Display(NFA &G, set<char> &S, vector<NFA_Node*> &V)//输出NFA
{
    qstr = QString("正则表达式转NFA+\n");
    //cout << "正则表达式转NFA" << endl;
    queue<NFA_Node*> MyQueue;
    MyQueue.push(G.start);
    qstr.append("开始状态"+QString::number(G.start->stateID)+"\n");
    //cout << "开始状态" << G.start->stateID << endl;
    char tt;
    while (!MyQueue.empty())
    {
        NFA_Node* tmp = MyQueue.front();
        MyQueue.pop();
        V.push_back(tmp);//放入向量
        tmp->visit = true;
        if (tmp->t.size() > 0)
            qstr.append("从状态"+QString::number(tmp->stateID)+"出发\n");
            //cout << "从状态" << tmp->stateID << "出发" << endl;
        if (tmp->t.size() > 0)
            for (int i = 0;i < tmp->t.size();i++)//该结点的所有trans
            {
                tt = tmp->t[i]->incept;
                if (tt != '@'&&tt != '#')
                    S.insert(tt);//放入SET有卵用？？！！！
                if (tt == '@')
                {
                    qstr.append(QString::number(tmp->stateID)+"---epsilon--->"+QString::number(tmp->t[i]->des->stateID));
                    //cout << tmp->stateID << "---" << "epsilon" << "--->" << tmp->t[i]->des->stateID;
                    if (tmp->t[i]->des->t.size() == 0) qstr.append("接收状态");//cout << "接收状态";//判断改trans是否为终止状态
                    qstr.append("\n");//cout << endl;
                    if (tmp->t[i]->des->visit == false)//广度遍历
                    {
                        MyQueue.push(tmp->t[i]->des);tmp->t[i]->des->visit = true;
                    }
                }
                else if (tt == '#')
                {
                    qstr.append(QString::number(tmp->stateID)+"---epsilon回到--->"+QString::number(tmp->t[i]->des->stateID)+"\n");
                    //cout << tmp->stateID << "---" << "epsilon回到" << "--->" << tmp->t[i]->des->stateID << endl;
                }
                else
                {
                    qstr.append(QString::number(tmp->stateID)+"---"+QString(tt)+"--->"+QString::number(tmp->t[i]->des->stateID));
                    //cout << tmp->stateID << "---" << tt << "--->" << tmp->t[i]->des->stateID;
                    if (tmp->t[i]->des->t.size() == 0) qstr.append("接收状态");//cout << "接收状态";
                    qstr.append("\n");//cout << endl;
                    if (tmp->t[i]->des->visit == false)
                    {
                        MyQueue.push(tmp->t[i]->des);tmp->t[i]->des->visit = true;
                    }
                }
            }
    }
    qstr.append("接收状态"+QString::number(G.end->stateID)+"\n");
    //cout << "接收状态" << G.end->stateID << endl;
}

class DFA_Edge;

class DFA_Node//DFA结点，状态编号stateID,每个点的边t(向量Edge)
{
public:
    int stateID;
    vector<DFA_Edge*> t;
    vector<int> ori;//?
    bool flag;//判断是否能到达终止状态
    DFA_Node(int s)
    {
        flag = false;
        stateID = s;
    }
};

class DFA_Edge//DFA的边，可接受的字母（incept）,该边的终点（des）,是否访问过（visit）
{
public:
    char incept;
    DFA_Node* des;
    bool visit;
    DFA_Edge(char a, DFA_Node* &b)
    {
        incept = a;
        des = b;
        visit = false;
    }
};

/*class Union
{
public:
vector<int> data;
int unionID;
int flag;//=0时该集合已被分裂
Union(Union* p)
{
for (int i = 0;i < p->data.size();i++)
this->data[i] = p->data[i];
}
Union(int id)
{
this->unionID = id;
this->flag = 1;
}
};*/

const int MAX = 100;
class NFA_To_DFA
{
public:
    int MaxStatus;//DFA的状态数总量
    NFA_Node* start;//起始结点
    int cando;//存NFA的接收状态 !!!!!!!!

    int T[MAX][MAX];//二维数组存储，T的表竖的表示DNF状态，横的表示NFA状态。T[2][1]=1表示，在DFA的状态2可以到达NFA的状态1
    int visit[MAX];//该状态是否被访问过
    vector<NFA_Node*> tmp;//向量——NFA结点，用于转化时临时存储
    vector<NFA_Node*> nfa;//向量——NFA结点
    vector<DFA_Node*> dfa;//向量——DFA结点
    set<char> Alpha;



    NFA_To_DFA(int max, NFA_Node* &S, int cando)//初始化
    {
        this->MaxStatus = max;
        this->start = S;
        this->cando = cando;
        Init();
        for (int i = 0;i <= MAX;i++)
            for (int j = 0;j <= MAX;j++)
                T[i][j] = 0;
    }

    void Init()
    {
        for (int i = 0;i <= this->MaxStatus;i++)
        {
            this->visit[i] = 0;
        }
        this->tmp.clear();
    }

    NFA_Node*& findnfa(int st)//通过状态st,找到nfa结点
    {
        for (int i = 0;i < nfa.size();i++)
            if (nfa[i]->stateID == st) return nfa[i];
    }
    DFA_Node*& finddfa(int st)
    {
        for (int i = 0;i < dfa.size();i++)
            if (dfa[i]->stateID == st) return dfa[i];
    }

    void findclosure(NFA_Node* p)//找出该点的epsilon闭包
    {
        visit[p->stateID] = 1;
        visit[0]++;//visit[0]记录该点是否可经过epsilon到达其他状态
        if (p->t.size() == 0) return;
        for (int i = 0;i < p->t.size();i++)
        {
            if (!visit[p->t[i]->des->stateID] && (p->t[i]->incept == '#' || p->t[i]->incept == '@'))//该结点没有被访问过，且接受状态为空
                findclosure(p->t[i]->des);
        }
    }

    void moveto(int st, char c)
    {
        //	if(st==2) cout<<T[st][0]<<endl;
        for (int i = 1;i <= MaxStatus;i++)
        {
            if (T[st][i])
            {
                NFA_Node* p = findnfa(i);
                if (p->t.size()>0)
                    for (int j = 0;j < p->t.size();j++)
                        if (p->t[j]->incept == c)
                        {
                            tmp.push_back(p->t[j]->des);
                        }//cout<<c<<" "; cout<<p->t[j]->des->stateID<<endl;}
            }
        }
    }

    void closure()
    {
        for (int i = 0;i < tmp.size();i++)
        {
            findclosure(tmp[i]);
        }
    }

    int AddStatus()//T[0][0]当前状态集合数 初始为0 （当前DFA的节点数）
    {
        if (visit[0] == 0)//表示没有状态
            return -2;

        for (int i = 1;i <= T[0][0];i++)
        {
            if (visit[0] != T[i][0])//此集合状态数不一样 则跳过
                continue;
            int j = 1;
            for (;j <= MaxStatus;j++)
                if (visit[j] != T[i][j])//有一个状态不一样 则跳过。结束循环，新增状态
                    break;
            if (j == (MaxStatus + 1))  //说明矩阵T中已有此集合,并返回位置
                return i;
        }

        T[0][0]++;//当前DFA的节点数
        for (int i = 0;i <= MaxStatus;i++)
            T[T[0][0]][i] = visit[i];
        //	cout<<T[T[0][0]][0];
        return -1;

    }//返回-1表示插入了新状态集合

    void Convert()
    {
        int i, j;

        findclosure(start);//找出起始点的epsilon闭包
        AddStatus();//更新T[][]数组，（该状态的编号为NFA的）
        DFA_Node* s1 = new DFA_Node(1);//新增DFA的起始结点
        if (visit[cando]) s1->flag = true;//起始结点是否能到达结束状态

        for (i = 1;i <= MaxStatus;i++)
            if (visit[i]) s1->ori.push_back(i);//存入该点经过epsilon可达的所有状态（该状态的编号为NFA的）

        dfa.push_back(s1);//DFA向量存入起始点

        Init();
        qDebug()<<11111<<endl;

        //cout<<Alpha.size();

        for (i = 1;i <= T[0][0];i++)//i目前的状态集合号 （该状态编号为DFA的）
        {
            for (set<char>::iterator t1 = Alpha.begin();t1 != Alpha.end();t1++)//set可排序,先找经过a的，再找经过b,c,d...
            {
                moveto(i, *t1);//从DFA状态i,经过字符t1可到达的状态号存入向量tmp(该状态的编号为NFA的)
                closure();//找出向量tmp的所有epsilon闭包，即状态i经过字符t1的epsilon闭包


                if ((j = AddStatus()) >= 0)//没有新状态产生
                {
                    DFA_Edge* e1 = new DFA_Edge(*t1, finddfa(j));
                    finddfa(i)->t.push_back(e1);

                }
                else if (j == -1)//产生了新状态
                {
                    DFA_Node* d1 = new DFA_Node(T[0][0]);//新增DFA结点
                    if (visit[cando]) d1->flag = true;//是否能到达结束状态
                    for (int tt = 1;tt <= MaxStatus;tt++)
                        if (visit[tt]) d1->ori.push_back(tt);//存入该点经过epsilon可达的所有状态（该状态的编号为NFA的）
                    dfa.push_back(d1);//DFA向量存入新增点

                    DFA_Edge* e1 = new DFA_Edge(*t1, finddfa(T[0][0]));//新增DFA边
                    finddfa(i)->t.push_back(e1);
                    //		cout<<T[0][0]<<endl;
                }

                Init();
            }
        }
    }

    void show(DFA_Node* p)
    {
        if (p->ori.size() == 0) return;
        qstr.append("(原状态");
        //cout << "(原状态";
        for (int i = 0;i < p->ori.size();i++) qstr.append(QString::number(p->ori[i]));
            //cout << p->ori[i];
        qstr.append(")");
        //cout << ")";
    }

    void Display()
    {
        qstr.append("NFA转DFA\n");
        //cout << "NFA转DFA" << endl;
        for (int i = 0;i < dfa.size();i++)
            dfs(dfa[i]);
    }

    void dfs(DFA_Node* &p)
    {
        if (p->t.size()>0)
            for (int i = 0;i < p->t.size();i++)
                if (p->t[i]->visit == false)
                {
                    qstr.append(QString::number(p->stateID));
                    //cout << p->stateID;
                    if (p->flag) qstr.append("(接收状态)");//cout << "(接收状态)";
                    show(p);
                    qstr.append("---"+QString(p->t[i]->incept)+"--->"+QString::number(p->t[i]->des->stateID));
                    //cout << "---" << p->t[i]->incept << "--->" << p->t[i]->des->stateID;
                    show(p->t[i]->des);
                    if (p->t[i]->des->flag)  qstr.append("(接收状态)");//cout << "(接收状态)";
                    qstr.append("\n");//cout << endl;
                    p->t[i]->visit = true;
                    dfs(p->t[i]->des);
                }
    }

    /*vector<Union*> u;//dfa的状态集合向量
    vector<Union*> temp;//temp[i]:某状态经某字符可到达状态i
    int count = 2;//当前集合数
    int id;
    int ifincept;
    int endid;
    int endunionid;
    int hasincept;//这个集合的所有元素能否接受该字符

    DFA_Node* tempdfa;

    void init()
    {
    u.clear();
    Union *fstart = new Union(1);
    Union *fend = new Union(2);
    for (int i = 0;i < dfa.size();i++)//遍历所以DFA结点，找出非终态和终态，划分集合
    {
    if (dfa[i]->flag)  fend->data.push_back(dfa[i]->stateID);
    else fstart->data.push_back(dfa[i]->stateID);
    }
    u.push_back(fstart);
    u.push_back(fend);

    for (int i = 0;i < u[0]->data.size();i++)
    cout << u[0]->data[i];
    }

    int findendunionid(int id)
    {
    for (int i = 0;i < u.size();i++)
    {
    for (int j = 0;j < u[i]->data.size();j++)
    {
    if (id == u[i]->data[j])
    {
    return u[i]->unionID;
    }
    }

    }

    }

    Union* findbelongunion(int unionid)
    {
    for (int i = 0;i < temp.size();i++)
    {
    if (unionid == temp[i]->unionID)
    return temp[i];
    }
    Union *newunion = new Union(unionid);
    temp.push_back(newunion);
    return newunion;
    }

    void test()
    {
    for (set<char>::iterator t1 = Alpha.begin();t1 != Alpha.end();t1++)
    {
    for (int i = 0;i < u.size();i++)//当前集合
    {
    temp.clear();
    hasincept = 0;
    for (int j = 0;j < u[i]->data.size();j++)//当前集合元素
    {
    id = u[i]->data[j];
    ifincept = 0;
    tempdfa = finddfa(id);
    for (int k = 0;k < tempdfa->t.size();k++)//找到当前元素的终止状态ID
    {
    if ((tempdfa->t[k])->incept == *t1)
    {
    ifincept = 1;
    endid = tempdfa->t[k]->des->stateID;
    break;
    }

    }
    if (ifincept == 0)endunionid = 0;//0表示无接受
    else endunionid = findendunionid(endid);

    Union *nowunion = findbelongunion(endunionid);//找出在temp里属于哪个集合
    nowunion->data.push_back(id);//将id放入该集合
    }
    if (temp.size() > 1)//大于1表示有新状态产生
    {
    for (int t = 0;t < temp.size();t++)
    {
    if (temp[t]->unionID != u[i]->unionID)
    {
    cout << temp[t]->unionID << endl;
    u.push_back(temp[i]);
    }
    }
    }

    }//遍历集合结束
    cout << "num" << u.size();
    for (int test = 0;test < u.size();test++)
    cout << u[test]->unionID;
    }//遍历set结束
    cout << endl;
    }
    */


    int state;			        //状态 1 ~ state
    map<char, int> inSignInt;	//输入符
    vector<char>	inSign;		//输入符集合
    int inSize;					//输入符个数
    set<int> final;		     	//终态
    int	  S;				    //起始符
    int stateMap[101][101];  	//图
    bool reach[101][101];		//可达性
    multimap<int, int> nextState; //直接后继 (貌似没用到)
    bool useless[101];			   //标记为无用状态
    vector<set<int> > que;			//状态队列
                                    //int def = 1000000;


    void Input()		//输入
    {
        state = MaxStatus;//DFA的当前状态数
        int index = 1;
        for (set<char>::iterator t1 = Alpha.begin();t1 != Alpha.end();t1++)
        {
            inSignInt[*t1] = index++;//inSignInt(a)=1,inSignInt(b)=2；把字符变成数字，易于存储
            inSign.push_back(*t1);//可接受字符的向量
        }
        inSize = inSign.size();//可接受字符的个数

        for (int i = 0;i < dfa.size();i++)
        {
            if (dfa[i]->flag)
            {
                final.insert(dfa[i]->stateID);//存放终态的set
            }
        }
        S = 1;//起始状态为1

        int a, b, c;//a为开始状态，b为经过的字符，c为到达状态
        for (int i = 0;i < dfa.size();i++)
        {
            a = dfa[i]->stateID;
            for (int j = 0;j < dfa[i]->t.size();j++)
            {
                b = dfa[i]->t[j]->incept;
                c = dfa[i]->t[j]->des->stateID;
                stateMap[a][inSignInt[b]] = c;		 //如： f(1,a)=2;   stateMap[i][j]=x,i状态经过编号为j的字符可到达x状态
                reach[a][c] = 1;//reach[i][j]表示i,j两个状态是可达的
                nextState.insert(make_pair(a, c));
            }
        }
    }

    void floyd()			//任意两状态间可达性 reach[][]
    {
        for (int k = 1; k <= state; k++) {
            for (int i = 1; i <= state; i++) {
                for (int j = 1; j <= state; j++) {

                    if (reach[i][k] == 1 && reach[k][j] == 1) {
                        reach[i][j] = 1;
                    }
                }
            }
        }

    }

    bool startTo(int n)			//起始态S 能否 到达 n
    {
        if (S == n)         //本身是起始态
            return true;

        return reach[S][n];
    }

    bool toFinal(int n)			//n 能否到达 终态
    {
        if (find(final.begin(), final.end(), n) != final.end())  //本身是终态
            return true;

        for (set<int>::iterator iter = final.begin(); iter != final.end(); ++iter) {
            int j = *iter;
            if (reach[n][j])
                return true;
        }

        return false;
    }

    void markUseless()		//标记无用状态
    {
        for (int i = 1; i <= state; i++) {
            if ((startTo(i) == false || toFinal(i) == false)) { //&&find(final.begin(),final.end(),i)==final.end()&&S!=i) {
                useless[i] = true;
            }
        }
    }

    int findFromQue(int x)		//x是队列que中第几个划分中的元素
    {
        int i;
        for (i = 0; i<que.size(); i++) {
            if (find(que[i].begin(), que[i].end(), x) != que[i].end())
                break;
        }
        //	cout<<i<<"-"<<endl;					//
        return i;
    }

    bool divit(int i, int j)	//根据输入符j划分队列中第i个集合
    {
        multimap<int, int> m;
        set<int> s;
        int x, f;
        for (set<int>::iterator iter = que[i].begin(); iter != que[i].end(); ++iter) {
            x = stateMap[*iter][j];
            f = findFromQue(x);
            m.insert(make_pair(f, *iter));
            s.insert(f);
        }
        //	cout<<i<<" "<<s.size()<<"*"<<endl;							//
        //	return 0;
        if (s.size() <= 1) {
            return false;		//没被划分
        }
        else {
            que.erase(que.begin() + i);
            for (set<int>::iterator sIter = s.begin();sIter != s.end();++sIter) {
                multimap<int, int>::iterator mIter = m.find(*sIter);
                int cnt = m.count(*sIter);
                set<int> newSet;
                for (int k = 0; k != cnt; k++, ++mIter) {
                    newSet.insert(mIter->second);
                }
                que.push_back(newSet);
            }
        }
        return true;
    }

    void division()				//划分所有集合
    {
        for (set<int>::iterator iter = final.begin(); iter != final.end(); ++iter) {     //消除终态集合中的无用状态
            if (useless[*iter]) {
                final.erase(iter);
                iter = final.begin();
            }
        }
        que.push_back(final);
        set<int> s;
        for (int i = 1; i <= state; i++) {
            if (find(final.begin(), final.end(), i) == final.end()) {
                if (!useless[i]) {					//无用状态不入队
                    s.insert(i);
                }
            }
        }
        que.push_back(s);        //先将 非终态、终态划分

                                 //	cout<<*(que[0].begin()+1)<<" "<<*(que[1].end()-1);
                                 //	return;
        bool flag = true;				//能否继续划分的标志
        for (int i = 0; ; i++) {
            //	cout<<"*";
            if (i >= que.size()) {
                if (flag == true)
                    break;
                else {
                    i = 0;
                    flag = true;
                }
            }
            for (int j = 1; j <= inSize; j++) {
                //	cout<<"*";
                if (divit(i, j)) {
                    flag = false;
                    i--;
                    break;
                }
            }
        }
    }
    void Output()
    {
        qstr.append("\n无用状态有：\n");
        //cout << endl << "无用状态有：" << endl;
        for (int i = 1; i <= state; i++) {
            if (useless[i]) qstr.append(QString::number(i)+" ");
                //cout << i << " ";
        }
        qstr.append("\n");
        //cout << endl;
        qstr.append("\n最终的划分是：\n");
        //cout << endl << "最终的划分是：" << endl;
        qstr.append("P = (");
        //cout << "P = (";
        for (int i = 0; i<que.size(); i++) {
            qstr.append(" { ");
            //cout << " { ";
            for (set<int>::iterator iter = que[i].begin(); iter != que[i].end(); iter++) {
                if (iter != que[i].begin())
                    qstr.append(",");
                    //cout << ",";
                qstr.append(QString::number(*iter));
                //cout << *iter;
            }
            qstr.append(" } ");
            //cout << " } ";
        }
        qstr.append(")");
        //cout << ")" << endl;
        qstr.append("\n最小化的NFA表:\n\t");
        //cout << endl << "最小化的NFA表:" << endl;
        //cout << " \t";
        for (int i = 0; i<inSign.size(); i++) {
            qstr.append(QString(inSign[i])+"\t");
            //cout << inSign[i] << "\t";
        }
        qstr.append("\n");
        //cout << endl;

        vector<string > strQ;
        for (int i = 0; i<que.size(); i++) {
            string s;
            for (set<int>::iterator iter = que[i].begin(); iter != que[i].end(); ++iter) {
                s.push_back(*iter + '0');
            }
            strQ.push_back(s);
        }
        int out[100][100];
        for (int i = 0; i<que.size(); i++) {
            if (!strQ[i].empty())
                qstr.append(QString::number(*strQ[i].begin()));
                //cout << *strQ[i].begin();
            char a= *strQ[i].begin();
            qstr.append("\t");
            //cout << "\t";
            for (int j = 0; j<inSign.size(); j++) {
                int x = stateMap[*(que[i].begin())][inSignInt[inSign[j]]];
                int f = findFromQue(x);
                if (f < strQ.size() && !strQ[f].empty())
                {
                    qstr.append(QString::number(*strQ[f].begin()));
                    //cout << *strQ[f].begin();
                    out[*strQ[i].begin()-48][j] = *strQ[f].begin()-48;
                }
                else out[*strQ[i].begin() - 48][j] =0;
                qstr.append("\t");
                //cout << "\t";
            }
            qstr.append("\n");
            //cout << endl;
        }
        qstr.append("switch(state)\n{\n");
        //cout << "switch(state)" << endl;
        //cout << "{" << endl;
        for (int i = 1; i < que.size(); i++)
        {
            qstr.append(" case "+QString::number(*strQ[i].begin() - 48)+":\n");
            //cout << " case " << *strQ[i].begin() - 48 << ":" << endl;
            for (int j = 0; j < inSign.size(); j++)
            {
                if (out[*strQ[i].begin() - 48][j] != 0)
                {
                    qstr.append( "   if(input=="+QString(inSign[j])+")\n{");
                    qstr.append(  "   state="+QString::number(out[*strQ[i].begin() - 48][j])+";\n"+"     break;\n}\n");
//                    cout << "   if(input==" << inSign[j] << ")" <<endl;
//                    cout << " {";
//                    cout << "   state=" << out[*strQ[i].begin() - 48][j] << ";" << endl;
//                    cout << "     break;" << endl;
//                    cout << " }" << endl;

                }

            }
            qstr.append( "  error();\n");
            //cout << "  error();" << endl;
        }
        qstr.append("default: error();\n}\n" );
        cout << "default: error();" << endl;
        cout << "}" << endl;

    }



};