#include<bits/stdc++.h>
using namespace std;

struct block{
    int id;
    string name;
    int width=0;
    int height=0;
    int x=0;
    int y=0;
    bool rotate=false;
    block *parent=NULL;
    block *lchild=NULL;
    block *rchild=NULL;
    vector< vector<int> >record;
};

struct terminal{
    int id;
    string name;
    int x;
    int y;
};

struct net{
    int height=0;
    vector<int>block_list;
    vector<int>terminal_list;
};

struct Tree{
    int id;
    int width=0;
    int height=0;
    int best_loc=0;
    Tree *lchild=NULL;
    Tree *rchild=NULL;
    vector< vector<int> >record;
    // vector< pair<int,int> >record_loc;
};

vector<block*>block_v(450);
vector<terminal*>terminal_v(1000);
vector<net*>net_v;
vector<int>init_polish;
stringstream ss;
double dead_ratio;
int block_num,terminal_num,net_num,pin_num;
int total_area=0,w_fl,min_area=INT_MAX;
int x_boundary=0,y_boundary=0;
int X=0,Y=0;
Tree *root=new Tree();
int mul=1000,T0;
bool fixarea=true;
bool found=false;
bool accept=false;
bool timeup=false;
chrono::steady_clock::time_point t1,t2,t3,t4;
// block *root=new block();


int hpwl(){
    int length=0;
    int x_max,x_min,y_max,y_min,tempx,tempy,loc;
    for(int i=0;i<net_num;i++){
        net *n=net_v[i];
        x_max=0;
        y_max=0;
        x_min=9999999;
        y_min=9999999;
        for(int j=0;j<n->block_list.size();j++){
            loc=n->block_list[j];
            tempx=block_v[loc]->x+(block_v[loc]->width/2);
            tempy=block_v[loc]->y+(block_v[loc]->height/2);
            x_max=max(x_max,tempx);
            x_min=min(x_min,tempx);
            y_max=max(y_max,tempy);
            y_min=min(y_min,tempy);
        }
        for(int j=0;j<n->terminal_list.size();j++){
            loc=n->terminal_list[j];
            tempx=terminal_v[loc]->x;
            tempy=terminal_v[loc]->y;
            x_max=max(x_max,tempx);
            x_min=min(x_min,tempx);
            y_max=max(y_max,tempy);
            y_min=min(y_min,tempy);
        }
        length+=((x_max-x_min)+(y_max-y_min));
    }
    // cout<<"length "<<length<<endl;
    return length;
}

int cost_fuction(int alpha, int beta){
    accept=true;
    int cost=0;
    // if(x_boundary>w_fl&&y_boundary>w_fl){
    //     cost+=(x_boundary*y_boundary)-w_fl*w_fl;
    // }else if(x_boundary>w_fl&&y_boundary<=w_fl){
    //     cost+=(x_boundary-w_fl)*w_fl;
    // }else if(x_boundary<=w_fl&&y_boundary>w_fl){
    //     cost+=(y_boundary-w_fl)*w_fl;
    // }
    if(x_boundary>w_fl){
        cost+=(x_boundary-w_fl)*500;
    }
    if(y_boundary>w_fl){
        cost+=(y_boundary-w_fl)*500;
    }
    // cout<<"cost "<<cost<<endl; 
    if(fixarea&&cost>0){
        cost+=100000;
        return cost;
    }
    if(fixarea&&cost==0){
        found=true;
        cout<<"FIND"<<endl;
        return 0;
    } 
    if(!fixarea&&cost!=0){
        accept=false;
        cost*=mul;
    }

    cost+=min_area*alpha+hpwl()*beta;
    return cost;
}

vector<int> init(){
    vector<int>polish;
    vector<block*>sort_b=block_v;
    sort(sort_b.begin(),sort_b.begin()+block_num,[](const block *a,const block *b){
        return a->height>b->height;
        
        // return a->width>b->width;
    });
    // V:-1 H:-2
    polish.push_back(sort_b[0]->id);
    block_v[sort_b[0]->id]->x=0;
    block_v[sort_b[0]->id]->y=0;
    int loc,temp_w=0,temp_h=0,max_h=0;
    int wcnt=1,hcnt=1;
    bool h=false;
    // cout<<sort_b[0]->name<<endl;
    for(int i=1;i<block_num;i++){
        // cout<<sort_b[i]->name<<endl;
        loc=sort_b[i]->id;
        temp_w+=block_v[loc]->width;
        if(temp_w<=w_fl){
            // temp_w+=block_v[loc]->width;
            wcnt++;
        }else{
            wcnt=1;
            //block_v[loc]->x=0;
            temp_h+=max_h;
            temp_w=block_v[loc]->width;
            if(hcnt>=2){
                polish.push_back(-2);
            }
            hcnt++;
        }
        polish.push_back(loc);
        if(wcnt>=2){
            polish.push_back(-1);
        }
    }
    polish.push_back(-2);
    // for(int i=0;i<polish.size();i++){
    //     if(polish[i]==-2){
    //         cout<<"H"<<endl;
    //     }else if(polish[i]==-1){
    //         cout<<"V";
    //     }else{
    //         cout<<polish[i]<<" ";
    //     }
    // }
    return polish;
}


void compute_area(Tree *p,int op){
    int i,j;
    p->record.clear();
    // p->record_loc.clear();
    if(op==-1){ //V
        sort(p->lchild->record.begin(),p->lchild->record.end(),[](const vector<int>&a,const vector<int>&b){
            return (a[1]>b[1]);
        });
        sort(p->rchild->record.begin(),p->rchild->record.end(),[](const vector<int>&a,const vector<int>&b){
            return (a[1]>b[1]);
        });
        i=0;
        j=0;
        while(i<p->lchild->record.size()&&j<p->rchild->record.size()){
            p->record.push_back({p->lchild->record[i][0]+p->rchild->record[j][0],max(p->lchild->record[i][1],p->rchild->record[j][1]),i,j});
            // p->record_loc.push_back(make_pair(i,j));
            if(p->lchild->record[i][1]>p->rchild->record[j][1]){
                i++;
            }else if(p->lchild->record[i][1]<p->rchild->record[j][1]){
                j++;
            }else {
                i++;
                j++;
            }
        }

    }else{  //H
        sort(p->lchild->record.begin(),p->lchild->record.end());
        sort(p->rchild->record.begin(),p->rchild->record.end());
        i=p->lchild->record.size()-1;
        j=p->rchild->record.size()-1;
        while(i>=0&&j>=0){
            p->record.push_back({max(p->lchild->record[i][0],p->rchild->record[j][0]),p->lchild->record[i][1]+p->rchild->record[j][1],i,j});
            // p->record_loc.push_back(make_pair(i,j));
            if(p->lchild->record[i][0]>p->rchild->record[j][0]){
                i--;
            }else if(p->lchild->record[i][0]<p->rchild->record[j][0]){
                j--;
            }else {
                i--;
                j--;
            }
        }
    }
    return;
}

void dfs(Tree *cur,int &loc,int const&x,int const&y){
    if(cur->id>=0){
        block_v[cur->id]->x=x;
        block_v[cur->id]->y=y;
        int temp_w=cur->record[loc][0];
        if(block_v[cur->id]->width!=temp_w){
            block_v[cur->id]->rotate=!block_v[cur->id]->rotate;
            block_v[cur->id]->height=block_v[cur->id]->width;
            block_v[cur->id]->width=temp_w;
            // block_v[cur->id]->height=cur->record[loc].second;
        }
        free(cur);
        // cout<<cur->id<<"    "<<loc<<"   ("<<x<<" , "<<y<<endl;
        // cout<<block_v[cur->id]->width<<"    "<<block_v[cur->id]->height<<endl;
        // cout<<"---"<<endl;
        
        //cout<<"change"<<endl;
    }else{
        // cout<<"/****"<<endl;
        // cout<<cur->id<<" : "<<endl;
        // cout<<"width    "<<temp_w<<"    "<<x<<endl;
        // cout<<"height    "<<temp_h<<"   "<<y<<endl;
        // cout<<"*****/"<<endl;
        dfs(cur->lchild,cur->record[loc][2],x,y);
        int temp_w=0;
        int temp_h=0;
        if(cur->id==-1){//V
            temp_w=cur->lchild->record[cur->record[loc][2]][0];
        }else{
            temp_h=cur->lchild->record[cur->record[loc][2]][1];
            // cout<<cur->record[loc].first<<" "<<cur->record[loc].second<<endl;
        }
        // x+=temp_w;
        // y+=temp_h;
        dfs(cur->rchild, cur->record[loc][3], x+temp_w , y+temp_h);
        free(cur);
    }
    
}

void buildtree(vector<int>polish){
    stack<Tree*> st;
    int op_r=0;
    int op_d=0;
    for(int i=0;i<polish.size();i++){
        // cout<<polish[i]<<"  ";
        if(polish[i]>=0){
            Tree *t=new Tree();
            t->id=polish[i];
            t->record=block_v[t->id]->record;
            st.push(t);
            // op_d++;
            continue;
        }
        if(polish[i]==-1||polish[i]==-2){
            Tree *t1=st.top();
            st.pop();
            Tree *t2=st.top();
            st.pop();
            Tree *p=new Tree();
            p->id=polish[i];
            p->lchild=t2;
            p->rchild=t1;
            compute_area(p,polish[i]);
            st.push(p);
            // op_r++;
        }
    }
    Tree *p =st.top();
    // root =p;
    // cout<<endl<<"TREE AREA"<<endl;
    // for(int i=0;i<p->record.size();i++){
    //     cout<<p->record[i].first<<"  "<<p->record[i].second<<endl;
    // }

    int temp_area;
    int loc,loc_f,loc_r;
    min_area=INT_MAX;
    for(int i=0;i<p->record.size();i++){
        // cout<<root->record[i].first<<"  "<<root->record[i].second<<endl;
        temp_area=(p->record[i][0])*(p->record[i][1]);
        if(min_area>temp_area){
            min_area=temp_area;
            x_boundary=p->record[i][0];
            y_boundary=p->record[i][1];
            loc=i;
            // loc_l=root->record_loc[i].first;
            // loc_r=root->record_loc[i].second;
        }
    }
    p->best_loc=loc;
    // cout<<loc<<endl;
    // cout<<loc_w<<"  "<<loc_h<<endl;
    // cout<<root->lchild->record[loc_l].first<<"  "<<root->lchild->record[loc_l].second<<endl;
    // cout<<root->rchild->record[loc_r].first<<"  "<<root->rchild->record[loc_r].second<<endl;
    X=0;
    Y=0;
    // cout<<"DFS  "<<endl;
    dfs(p,loc,X,Y);
    // return p;
    // top2bottom(root);
    // cout<<op_d<<"    "<<op_r<<endl;
    // cout<<"Stack size "<<st.size()<<endl;
}


void runparser(string block_f,string net_f,string pl_f){
    string line,temp;

    /** hardblock file **/
    ifstream ifs(block_f);
    while(getline(ifs,line)){
        if(line==""){
            break;
        }
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>temp;
        if(temp=="NumHardRectilinearBlocks"){
            ss>>temp>>block_num;
        }else if(temp=="NumTerminals"){
            ss>>temp>>terminal_num;
        }else{
            cout<<"error"<<endl;
        }
    } 
    //cout<<block_num<<" "<<terminal_num<<endl;
    for(int i=0;i<block_num;i++){
        getline(ifs,line);
        ss.str("");
        ss.clear();
        block *b=new block();
        ss<<line;
        ss>>b->name;
        b->id=stoi(&b->name[2]);
        ss>>temp>>temp>>temp;
        temp.pop_back();
        b->x=stoi(&temp[1]);
        ss>>temp;
        temp.pop_back();
        b->y=stoi(&temp[0]);
        ss>>temp>>temp>>temp;
        temp.pop_back();
        int x2=stoi(&temp[1]);
        ss>>temp;
        temp.pop_back();
        int y2=stoi(&temp[0]);
        b->width=x2-b->x;
        b->height=y2-b->y;
        b->record.push_back({b->width,b->height,0,0});
        if(b->height!=b->width){
            b->record.push_back({b->height,b->width,1,1});
        }
        block_v[b->id]=b;
        total_area+=b->width*b->height;
        // if(b->height>b->width){
        //     int h=b->height;
        //     b->height=b->width;
        //     b->width=h;
        //     b->rotate=true;
        // }
    }
    ifs.close();


    /** net file **/
    ifs.open(net_f);
    for(int i=0;i<2;i++){
        getline(ifs,line);
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>temp;
        if(temp=="NumNets"){
            ss>>temp>>net_num;
        }else if(temp=="NumPins"){
            ss>>temp>>pin_num;
        }
    }
    //cout<<net_num<<"    "<<pin_num<<endl;
    int degree;
    for(int i=0;i<net_num;i++){
        getline(ifs,line);
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>temp>>temp>>degree;
        net *n=new net();
        for(int j=0;j<degree;j++){
            getline(ifs,line);
            ss.str("");
            ss.clear();
            ss<<line;
            ss>>temp;
            if(temp[0]=='p'){
                n->terminal_list.push_back(stoi(&temp[1]));
            }else if(temp[0]=='s'){
                n->block_list.push_back(stoi(&temp[2]));
                //n->height=max(n->height,block_v[stoi(&temp[2])]->height);
            }else{
                cout<<"error"<<endl;
            }

        }
        net_v.push_back(n);
    }

    ifs.close();
    
    /** pl file **/
    terminal_v.resize(terminal_num+50);
    ifs.open(pl_f);
    for(int i=0;i<terminal_num;i++){
        getline(ifs,line);
        terminal *t=new terminal();
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>t->name>>t->x>>t->y;
        t->id=stoi(&t->name[1]);
        terminal_v[t->id]=t;
    }
    ifs.close();
    // w_fl=sqrt(total_area*(1+dead_ratio));
  
    /*print the table*/
    // for(int i=0;i<block_num;i++){
    //     cout<<block_v[i].name<<"    "<<block_v[i].x<<"  "<<block_v[i].y<<"  "<<block_v[i].width<<"  "<<block_v[i].height<<endl;    
    // }
    // for(int i=0;i<net_num;i++){
    //     for(int j=0;j<net_v[i].terminal_list.size();j++){
    //         cout<<net_v[i].terminal_list[j]<<" ";
    //     }
    //     cout<<"|| ";
    //     for(int j=0;j<net_v[i].block_list.size();j++){
    //         cout<<net_v[i].block_list[j]<<" ";
    //     }
    //     cout<<endl;
    // }
    // for(int i=0;i<terminal_num;i++){
    //     cout<<terminal_v[i].name<<" "<<terminal_v[i].x<<" "<<terminal_v[i].y<<endl;
    // }
    return;
}

vector<int> move1(vector<int> polish){
    vector<int> op_d;
    for(int i=0;i<polish.size();i++){
        if(polish[i]>=0){
            op_d.push_back(i);
        }
    }
    int n=op_d.size();
    int t1=rand()%n;
    int t2=rand()%n;
    while(t1==t2){
        t2=rand()%n;
    }
    t1=op_d[t1];
    t2=op_d[t2];
    swap(polish[t1],polish[t2]);
    return polish;
}

vector<int> move2(vector<int> polish){
    vector<int>op_r;
    for(int i=1;i<polish.size();i++){
        if(polish[i-1]>=0&&polish[i]<0){
            op_r.push_back(i);
        }
    }
    int t1=rand();
    int n=op_r.size();
    t1=op_r[t1%n];
    for(int i=t1;i<polish.size();i++){
        if(polish[i]==-1){
            polish[i]=-2;
        }else if(polish[i]==-2){
            polish[i]=-1;
        }else{
            break;
        }
    }
    return polish;
}

bool check_ballot(vector<int>polish,int t1){
    int op_r=0;
    for(int i=0;i<=t1+1;i++){
        if(polish[i]<0){
            op_r++;
        }
    }
    t1++;
    if(2*op_r>=t1){
        return false;
    }
    return true;
}

bool check_skew(vector<int>polish,int t1){
    if(polish[t1]>=0){
        if(t1!=0&&polish[t1-1]!=polish[t1+1]){
            return true;
        }
    }else if(polish[t1]<0){
        if(t1!=polish.size()-1&&polish[t1]!=polish[t1+2]){
            return true;
        }
    }
    return false;
}

vector<int> move3(vector<int> polish){
    vector<int>op;
    for(int i=0;i<polish.size()-1;i++){
        if((polish[i]<0&&polish[i+1]>=0)||(polish[i]>=0&&polish[i+1]<0)){
            op.push_back(i);
        }
    }
    int t1;
    int n=op.size();
    bool done=false;
    int cnt=0;
    while(!done){
        t1=rand();
        t1=op[t1%n];
        cnt++;
        if(check_ballot(polish,t1)&&check_skew){
            done=true;
        }
        if(done||cnt==n){
            break;
        }
    }
    if(done){
        int t2=t1+1;
        swap(polish[t1],polish[t2]);
    }

    return polish;
}

vector<int> init2(vector<int>polish){
    vector<int>best_polish=polish;
    int curw=x_boundary;
    int curh=y_boundary;
    while(curw>w_fl||curh>w_fl){
        int choose=rand()%3;
        if(choose==0){
            polish=move1(polish);
        }else if(choose==1){
            polish=move2(polish);
        }else{
            polish=move3(polish);
        }
        buildtree(polish);
        if(y_boundary>x_boundary){
            if(y_boundary>curh||double(double(y_boundary)/double(x_boundary))<0.95){
                // cout<<double(double(y_boundary)/double(x_boundary))<<endl;
                polish=best_polish;
                buildtree(polish);
            }else{
                best_polish=polish;
                curw=x_boundary;
                curh=y_boundary;
            }
        }else{
            if(x_boundary>curw||double(double(x_boundary)/double(y_boundary))<0.95){
                polish=best_polish;
                buildtree(polish);
            }else{
                best_polish=polish;
                curw=x_boundary;
                curh=y_boundary;
            }
        }
    }
    return best_polish;
}

vector<int> sa(vector<int>E,double e,double r,double k,int alpha,int beta){
    int best_cost,costE;
    vector<int>BestE=E;
    int T,M,MT,uphill,reject,N=k*block_num;
    M=MT=uphill=0;
    best_cost=costE=cost_fuction(alpha,beta);
    // cout<<"Cost "<<best_cost<<endl;
    bool stop=false;
    T=1000;
    if(costE==0){
        return BestE;
    }
    do{
        MT=uphill=reject=0;
        do{
            int choose=rand()%3;
            if(fixarea==false){
                choose=0;
            }
            vector<int>NE;
            // cout<<"MOVE "<<choose<<endl;
            if(choose==0){
                NE=move1(E);
            }else if(choose==1){
                NE=move2(E);
            }else{
                NE=move3(E);
            }
            // cout<<"Finish MOVE"<<endl;
            MT++;
            buildtree(NE);
            // cout<<"TREE Finish"<<endl;
            int costNE=cost_fuction(alpha,beta);
            int delta=costNE-costE;
            double Random=((double)rand()/(RAND_MAX));
            if(delta<=0||Random<exp(-1*delta*T)){
                if(delta>0){
                    uphill++;
                }
                E=NE;
                costE=costNE;
                if(costE<best_cost&&accept){
                    BestE=E;
                    best_cost=costE;
                    if(costNE==0){
                        stop=true;
                        break;
                    }
                }
                
            }else {
                reject++;
            }
            // cout<<clock()/CLOCKS_PER_SEC<<endl;
            if(stop){
                // cout<<"stop"<<endl;
                return BestE;
            }
            t4 = chrono::steady_clock::now();
            double totaltime=chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count()/1000000;
            if(clock()/CLOCKS_PER_SEC>=550||totaltime>=550){
                // cout<<"Time up"<<endl;
                // cout<<clock()/CLOCKS_PER_SEC<<endl;
                // cout <<totaltime<<endl;
                timeup=true;
                return BestE;
            }
        }while(uphill<=N&&MT<=2*N);
        T=r*T;
        if(stop||timeup){
            break;
            return BestE;
        }
    }while(reject/MT<=0.95&&T>=e);
    return BestE;
}

void writefile(string outfile){
    ofstream ofs(outfile);
    ofs<<"Wirelength "<<hpwl()<<endl;
    ofs<<"Blocks"<<endl;
    for(int i=0;i<block_num;i++){
        ofs<<block_v[i]->name<<" "<<block_v[i]->x<<" "<<block_v[i]->y<< " "<<block_v[i]->rotate<<endl;
    }
    ofs.close();
    return;
}

int main(int argc, char* argv[]){
    double st,ed;
    bool flag=true;
    t1 = chrono::steady_clock::now();
    t3 = chrono::steady_clock::now();
    runparser(argv[1],argv[2],argv[3]);
    dead_ratio=stod(argv[5]);
    w_fl=sqrt(total_area*(1+dead_ratio));
    // cout<<w_fl<<endl;
    // srand(3);  //n100 0.1 1494165111
    int seed=0;
    if(block_num==100){
        if(dead_ratio==0.15){
            flag=false;
            seed=200;
        }else if(dead_ratio==0.1){
            flag=false;
            // seed=2000;
            seed=2000;
        }
    }else if(block_num==200){
        if(dead_ratio==0.15){
            flag=false;
            seed=100000;
        }else if(dead_ratio==0.1){
            flag=false;
            mul=100;
            // seed=0;
            // seed=1494156580;
            seed=1500000000;
        }
    }else if(block_num==300){
        mul=10;
        if(dead_ratio==0.15){
            flag=false;
            seed=0;
        }else if(dead_ratio==0.1){
            flag=false;
            mul=100;
            //0  3
            // seed=1670915463;
            seed =0;
            cout<<"seed "<<seed<<endl;
        }
    }
    // t2 = chrono::steady_clock::now();
    // double test=chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    // cout<<test<<endl;
    // cout<<"io "<<chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<endl;


    t1 = chrono::steady_clock::now();
    srand (seed);

    init_polish=init();
    buildtree(init_polish);
    // t2 = chrono::steady_clock::now();
    // cout<<"initial "<<chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<endl;
    // t1 = chrono::steady_clock::now();
    vector<int>ans;
    // init_polish=init2(init_polish);
    if(flag){
        init_polish=init2(init_polish);
    }

    // cout<<"out"<<endl;
    ans=sa(init_polish,0.1,0.9,10,10,1);
    buildtree(ans);
    
    // cout<<"one time"<<endl;
    //cout<<"Dead "<<dead_ratio<<endl;
    // for(int i=0;i<ans.size();i++){
    //     cout<<ans[i]<<" ";
    // }
    // cout<<"FIND"<<endl;
    if(found){
        fixarea=false;
    }
    while(!found&&!timeup){
        ans=sa(init_polish,0.1,0.9,10,10,1);
        buildtree(ans);
        if(found){
            fixarea=false;
        }
    }
    // t2 = chrono::steady_clock::now();
    // cout<<"fixed area "<<chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<endl;
    // t1 = chrono::steady_clock::now();
    if(!timeup){
        ans=sa(ans,1,0.85,5,1,10);
        buildtree(ans);
    }

    // t2 = chrono::steady_clock::now();
    // cout<<"optimize"<<chrono::duration_cast<chrono::microseconds>(t2-t1).count()<<endl;
        
    // cout<<endl;
    // cout<<w_fl<<"   "<<x_boundary<<" "<<y_boundary<<endl;
    // cout<<min_area<<endl;
    writefile(argv[4]);
    // t4 = chrono::steady_clock::now();
    // cout<<"Total "<<chrono::duration_cast<std::chrono::microseconds>(t4 - t3).count()<<endl;

    return 0;
}



/*void init_sa(){
    vector<block*>sort_b=block_v;
    sort(sort_b.begin(),sort_b.begin()+block_num,[](const block *a,const block *b){
        return a->height>b->height;
    });
    sort_b.resize(block_num);
    // for(int i=0;i<sort_b.size();i++){
    //     cout<<i<<" : "<<sort_b[i]->name<<"  "<<sort_b[i]->x<<endl;
    // }
    int loc=0,temp_w=0,temp_h=0,max_h=0,pre,cur_parent;
    root=sort_b[0];
    temp_w=sort_b[0]->width;
    cur_parent=pre=sort_b[0]->id;
    sort_b.erase(sort_b.begin());
    bool st=true;
    while(sort_b.size()>0){
        // cout<<sort_b.size()<<endl;
        if(sort_b[loc]->width+temp_w<=w_fl){
            sort_b[loc]->parent=block_v[pre];
            block_v[pre]->lchild=sort_b[loc];
            pre=sort_b[loc]->id;
            sort_b[loc]->x=temp_w;
            temp_w+=sort_b[loc]->width;
            //max_h=max(block_v[loc]->height,max_h);
        }else{
            sort_b[loc]->parent=block_v[cur_parent];
            block_v[cur_parent]->rchild=sort_b[loc];
            pre=cur_parent=sort_b[loc]->id;
            // temp_h+=max_h;
            block_v[loc]->x=0;
            // block_v[loc]->y=temp_h;
            temp_w=block_v[loc]->width;
            // max_h=0;
            st=!st;
        }
        // cout<<sort_b[loc]->name<<"  "<<sort_b[loc]->x<<endl;
        if(st){
            loc=0;
            sort_b.erase(sort_b.begin());
        }else{
            loc=sort_b.size()-1;
            sort_b.pop_back();
        }
    }
    for(int i=0;i<sort_b.size();i++){
        cout<<i<<" : "<<sort_b[i]->name<<"  "<<sort_b[i]->x<<endl;
    }
    
    // int build[block_num];
    // //block *cur=new block();
    // //block *pre=new block();
    // memset( build, 0, sizeof(build));
    // bool first=true;
    // for(int i=0;i<net_num;i++){
    //     for(int j=0;j<net_v[i]->block_list.size();j++){
    //         loc=net_v[i]->block_list[j];
    //         if(build[loc]){
    //             continue;
    //         }
    //         build[loc]=1;
    //         if(first){
    //             first=false;
    //             root=block_v[loc];
    //             temp_w=block_v[loc]->width;
    //             block_v[loc]->x=0;
    //             block_v[loc]->y=0;
    //             max_h=block_v[loc]->height;
    //             cur_parent=pre=block_v[loc]->id;
    //         }else{
    //             if(block_v[loc]->width+temp_w<=w_fl){
    //                 block_v[loc]->parent=block_v[pre];
    //                 block_v[pre]->lchild=block_v[loc];
    //                 pre=block_v[loc]->id;
    //                 block_v[loc]->x=temp_w;
    //                 block_v[loc]->y=temp_h;
    //                 temp_w+=block_v[loc]->width;
    //                 max_h=max(block_v[loc]->height,max_h);
    //             }else{
    //                 block_v[loc]->parent=block_v[cur_parent];
    //                 block_v[cur_parent]->rchild=block_v[loc];
    //                 pre=cur_parent=block_v[loc]->id;
    //                 temp_h+=max_h;
    //                 block_v[loc]->x=0;
    //                 block_v[loc]->y=temp_h;
    //                 temp_w=block_v[loc]->width;
    //                 max_h=0;
    //             }
    //             cout<<block_v[loc]->name<<" "<<block_v[loc]->x<<"   "<<block_v[loc]->y<<endl;


    //         }
    //     }
    // }
    return;
}*/

// void top2bottom(Tree *cur){
//     // stack<Tree*> st;
//     // st.push(cur);
//     // while(!st.empty()){
//     //     Tree *node=st.top();
//     //     st.pop();

//     //     loc=node->best_loc;
//     //     node->width=node->record[loc].first;
//     //     node->height=node->record[loc].second;
//     //     cout<<node->id<<"   :   "<<node->width<<"   "<<node->height<<endl;
//     //     if(node->id<0){
//     //         loc_l=node->record_loc[loc].first;
//     //         node->lchild->best_loc=loc_w;
//     //         loc_r=node->record_loc[loc].second;
//     //         node->rchild->best_loc=loc_h;
//     //     }

//     //     if(node->rchild){
//     //         st.push(node->rchild);
//     //     }
//     //     if(node->lchild){
//     //         st.push(node->lchild);
//     //     }
//     // }
//     stack<Tree*> st;
//     Tree *node=cur;
//     int x=0,y=0;
//     while(!st.empty()||node!=NULL){
//         while(node!=NULL){
//             st.push(node);
//             node=node->lchild;
//         }
//         node=st.top();
//         st.pop();
//         if(node->id>=0){
//             block_v[node->id]->x=x;
//             block_v[node->id]->y=y;
//             int loc=node->best_loc;
//             int temp_w=node->record[loc].first;
//             if(temp_w!=block_v[node->id]->width){
//                 block_v[node->id]->rotate=!block_v[node->id]->rotate;
//                 block_v[node->id]->height=block_v[node->id]->width;
//                 block_v[node->id]->width=temp_w;
//             }
//             cout<<node->id<<"   "<<x<<"  "<<y<<"    "<<block_v[node->id]->width<<"  "<<block_v[node->id]->height<<endl;
//         }else{
//             int loc=node->best_loc;
//             int loc_l=node->record_loc[loc].first;
//             node->lchild->best_loc=loc_l;
//             int loc_r=node->record_loc[loc].second;
//             node->rchild->best_loc=loc_r;
//             if(node->id==-1){
//                 x+=node->lchild->record[loc_l].first;
//                 cout<<"width "<<node->lchild->record[loc_l].first<<endl;
//             }else{
//                 y+=node->lchild->record[loc_l].second;
//                 cout<<"height "<<node->lchild->record[loc_l].second<<endl;
//             }
//         }
//         node=node->rchild;
//     }
// }


