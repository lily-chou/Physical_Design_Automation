#include<bits/stdc++.h>
using namespace std;

struct cell{
    string name;
    int id;
    int width=0;
    int height=0;
    int weight=1;
    int rbest=0;
    double x=0;
    double y=0;
    double newx=0;
    double newy=0;
};

struct cluster{
    double x=0,q=0;
    int width=0;
    int weight=0;
    vector<cell*> collection;
};

struct subrow{
    int x1=0;
    int x2=0;
    int y=0;
    int width=0;
    vector<cluster> cl_v;
};

struct row{
    int x=0;
    int y=0;
    int height=0;
    int site_width=0;
    int site_num=0;
    vector <subrow> sub_v;
};

vector<cell*>cell_v,terminal_v;
vector<row>row_v;
stringstream ss;
int maxdisplace,node_num,terminal_num=0,row_num;
int SETROW=4;

double displacement(cell *c,double newx,double newy){

    return (newx-c->x)*(newx-c->x)+(newy-c->y)*(newy-c->y);
}

void run_parser(string infile){
    size_t found = infile.find_last_of('/');
    string filepath=infile.substr(0,found);
    // cout<<"FilePath "<<filepath<<endl;
    found = filepath.find_last_of('/');
    string testcase=filepath.substr(found+1,filepath.length()-1);
    // cout<<"Testcase "<<testcase<<endl;

    unordered_map<string,cell*>cell_map;
    // cout<<filepath<<endl;
    string nodefile,plfile,sclfile;
    string line,temp;
    ifstream ifs(infile);
    while(getline(ifs,line)){
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>temp;
        // cout<<temp<<endl;
        if(temp=="RowBasedPlacement"){
            ss>>temp>>nodefile>>plfile>>sclfile;
            // cout<<nodefile<<"   "<<plfile<<"    "<<sclfile<<endl;
        }else if(temp=="MaxDisplacement"){
            ss>>temp>>maxdisplace;
        }else{
            cout<<"error"<<endl;
        }
    } 
    ifs.close();

    nodefile=filepath+'/'+nodefile;
    plfile=filepath+'/'+plfile;
    sclfile=filepath+'/'+sclfile;

    ifs.open(nodefile);
    while(getline(ifs,line)){
        if(line==""){
            break;
        }
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>temp;
        if(temp=="NumNodes"){
            ss>>temp>>node_num;
        }else if(temp=="NumTerminals"){
            ss>>temp>>terminal_num;
        }else{
            cout<<"error"<<endl;
        }
    }
    int n=node_num-terminal_num;
    for(int i=0;i<node_num;i++){
        getline(ifs,line);
        ss.str("");
        ss.clear();
        ss<<line;
        cell *c=new cell();
        ss>>c->name;
        c->id=i;
        ss>>c->width;
        ss>>c->height;
        // cout<<c->name<<"  "<<c->width<<"  "<<c->height<<endl;
        cell_map[c->name]=c;
        if(i<n){
            cell_v.push_back(c);
            // sort_cell.push_back(c);
        }else{
            terminal_v.push_back(c);
            // sort_.push_back(c);
        }
    }
    ifs.close();

    ifs.open(plfile);
    while(getline(ifs,line)){
        ss.str("");
        ss.clear();
        ss<<line;
        ss>>temp;
        ss>>cell_map[temp]->x;
        ss>>cell_map[temp]->y;
    }
    ifs.close();
    // for(int i=0;i<cell_v.size();i++){
    //     cout<<cell_v[i]->x<<"   "<<cell_v[i]->y<<endl;
    // }

    ifs.open(sclfile);
    getline(ifs,line);
    ss.str("");
    ss.clear();
    ss<<line;
    ss>>temp>>temp>>row_num;
    int x,y,h,site_num,site_width;
    for(int i=0;i<row_num;i++){
        while(getline(ifs,line)){
            ss.str("");
            ss.clear();
            ss<<line;
            ss>>temp;
            if(temp=="Coordinate"){
                ss>>temp>>y;
            }else if(temp=="Height"){
                ss>>temp>>h;
            }else if(temp=="Sitewidth"){
                ss>>temp>>site_width;
            }else if(temp=="NumSites"){
                ss>>temp>>site_num;
            }else if(temp=="SubrowOrigin"){
                ss>>temp>>x;
            }else if(temp=="End"){
                row r;
                r.x=x;
                r.y=y;
                r.height=h;
                r.site_width=site_width;
                r.site_num=site_num;
                //subrow
                subrow sr;
                sr.x1=r.x;
                sr.width=r.site_num*r.site_width;
                sr.x2=sr.x1+sr.width;
                sr.y=r.y;
                r.sub_v.push_back(sr);
                row_v.push_back(r);
            }
        }
    }
    ifs.close();

    if(testcase=="adaptec1"){
        SETROW=4;
    }else if(testcase=="adaptec3"){
        SETROW=3;
    }else if(testcase=="ibm01"){
        SETROW=3;
    }else if(testcase=="ibm07"){
        SETROW=4;
    }else if(testcase=="ibm09"){
        SETROW=5;
    }

    for(int i=0;i<cell_v.size();i++){
        cell_v[i]->rbest=(cell_v[i]->y-row_v[0].y)/row_v[0].height;
    }

    // DEBUG
    // for(int i=0;i<cell_v.size();i++){
    //     cout<<cell_v[i]->name<<"  "<<cell_v[i]->x<<"    "<<cell_v[i]->y<<endl;
    // }
}

void writefile(string outfile){
    ofstream ofs(outfile);
    for(int i=0;i<cell_v.size();i++){
        ofs<<cell_v[i]->name<<" "<<int(cell_v[i]->newx)<<" "<<int(cell_v[i]->newy)<<endl;
    }
    for(int i=0;i<terminal_v.size();i++){
        ofs<<terminal_v[i]->name<<" "<<terminal_v[i]->x<<" "<<terminal_v[i]->y<<endl;
    }
    ofs.close();
}

void spitrow(){
    // for(int i=0;i<row_num;i++){
    //     cout<<row_v[i]->sub_v.size()<<endl;
    // }
    if(terminal_num==0){
        return;
    }
    vector<cell*>sort_terminal=terminal_v;
    sort(sort_terminal.begin(),sort_terminal.end(),[](const cell *a,const cell *b){
        return a->x < b->x;        
    });
    int x1,x2;
    for(int i=0;i<terminal_num;i++){
        x1=sort_terminal[i]->x;
        x2=sort_terminal[i]->x+sort_terminal[i]->width;
        int j=(sort_terminal[i]->y-row_v[0].y)/row_v[0].height;
        int h=sort_terminal[i]->height/row_v[0].height;
        while(h>0){
            // cout<<"i: "<<i<<", h: "<<h<<endl;
            subrow *sr=&row_v[j].sub_v.back();
            // if(row_v[j].sub_v.back()->x1<=x1 && row_v[j].sub_v.back()->x2>x2){
            
            if(sr->x1<x1&&sr->x2>x2){
                subrow newsr;
                newsr.x1=x2;
                newsr.x2=sr->x2;
                newsr.width=newsr.x2-newsr.x1;
                newsr.y=sr->y;
                sr->x2=x1;
                sr->width=sr->x2-sr->x1;
                row_v[j].sub_v.push_back(newsr);
            }else if(x1<=sr->x1){
                sr->x1=x2;
                sr->width=sr->x2-sr->x1;
            }else if(x2>=sr->x2){
                sr->x2=x1;
                sr->width=sr->x2-sr->x1;
            }
            h--;
            j++;
        }
    }
    // for(int i=0;i<row_num;i++){
    //     cout<<row_v[i].sub_v.size()<<endl;
    //     cout<<"Y:   "<<row_v[i].y<<endl;
    //     // cout<<row_v[i]->sub_v[0]->x1<<" "<<row_v[i]->sub_v[0]->x2<<endl;
    //     // cout<<row_v[i]->sub_v[1]->x1<<" "<<row_v[i]->sub_v[1]->x2<<endl;
    //     // cout<<row_v[i]->sub_v[2]->x1<<" "<<row_v[i]->sub_v[2]->x2<<endl;
    //     // cout<<"----------------------"<<endl;
    // }
}

double placerow1(subrow sr,cell *c){
    // int subbest=insert2sub(rbest,c);
    // cout<<"subbest "<<subbest<<endl;
    // if(subbest==-1){
    //     c->newx=c->newy=DBL_MAX;
    //     return displacement(c);
    // }
    if(sr.width<c->width){
        return DBL_MAX;
    }

    // c->newy=sr.y;
    double newx=c->x;
    if(c->x<sr.x1){
        newx=sr.x1;
    }else if(c->x+c->width > sr.x2){
        newx=sr.x2-c->width;
    }
    // cout<<"left: "<<sr.x1<<" , right: "<<sr.x2<<" , newx: "<<newx<<" , original: "<<c->x<<endl;

    if(sr.cl_v.size()==0){
        c->newx=newx;
        
        if(newx<sr.x1){
                cout<<"ERROR left "<<endl;
                exit(0);
        }
        return displacement(c,newx,(double)sr.y);
    }else {
        // cluster cl=sr.cl_v.back();
        double xpos=sr.cl_v.back().x;
        double width=sr.cl_v.back().width;
        if(xpos+width<=newx){
            c->newx=newx;
            if(newx<sr.x1){
                cout<<"ERROR left "<<endl;
                exit(0);
            } 
            return displacement(c,newx,sr.y);
        }else {
            // cl.collection.push_back(c);
            double weight=sr.cl_v.back().weight+c->weight;
            double q=sr.cl_v.back().q+c->weight*(newx-width);
            // cl.weight+=c->weight;
            // cl.q+=c->weight*(newx-width);
            width+=c->width;
            int pos=sr.cl_v.size()-1;
            // collapse
            xpos=q/weight;
            if(xpos<sr.x1){
                xpos=sr.x1;
            }
            if(xpos>sr.x2-width){
                xpos=sr.x2-width;
            }
            double optimal_position=xpos;
            while(pos>0&&sr.cl_v[pos-1].x+sr.cl_v[pos-1].width>optimal_position){
                weight+=sr.cl_v[pos].weight;
                q+=sr.cl_v[pos].q-sr.cl_v[pos].weight*sr.cl_v[pos-1].width;
                optimal_position=q/weight;
                if(optimal_position<sr.x1){
                    optimal_position=sr.x1;
                }
                if(optimal_position>sr.x2-sr.cl_v[pos-1].width){
                    optimal_position=sr.x2-sr.cl_v[pos-1].width;
                }
                pos--;
            }
            if(optimal_position<sr.x1){
                cout<<"ERROR left "<<endl;
                exit(0);
            }
            double cost=0;
            for(int i=pos;i<sr.cl_v.size();i++){
                for(int j=0;j<sr.cl_v[i].collection.size();j++){
                    // cost+=(optimal_position-sr.cl_v[i].collection[j]->x)*(optimal_position-sr.cl_v[i].collection[j]->x)+(sr.y-sr.cl_v[i].collection[j]->y)*(sr.y-sr.cl_v[i].collection[j]->y);
                    cost+=displacement(sr.cl_v[i].collection[j],optimal_position,(double)sr.y);
                    optimal_position+=sr.cl_v[i].collection[j]->width;
                }
            }
            // cost+=(optimal_position-c->x)*(optimal_position-c->x)+(sr.y-c->y)*(sr.y-c->y);
            c->newx=optimal_position;
            cost+=displacement(c,optimal_position,(double)sr.y);
            return cost;
        }

    }
}

void placerow2(int rbest,int subbest,cell *c){
    if(row_v[rbest].sub_v[subbest].width<c->width){
        cout <<  "EEEEE" << endl;
        exit(0);
    }
    
    c->newy=row_v[rbest].y;
    // double newx=c->x;
    c->newx=c->x;
    if(c->x<row_v[rbest].sub_v[subbest].x1){
        c->newx=row_v[rbest].sub_v[subbest].x1;
    }else if(c->x+c->width > row_v[rbest].sub_v[subbest].x2){
        c->newx=row_v[rbest].sub_v[subbest].x2-c->width;
    }

    if(row_v[rbest].sub_v[subbest].cl_v.size()==0){
        cluster cl;
        cl.collection.emplace_back(c);
        cl.weight=c->weight;
        cl.q=c->weight*(c->newx-cl.width);
        cl.width=c->width;
        cl.x=c->newx;
        row_v[rbest].sub_v[subbest].cl_v.push_back(cl);
        // if(row_v[rbest].sub_v[subbest].cl_v.size()>0){
        //         cout<<"statement 1111111"<<endl;
        // }
        
    }else {
        // if(sr->cl_v.size()>0){
        //         cout<<"statement 222222"<<endl;
        // }
        if(row_v[rbest].sub_v[subbest].cl_v.back().x+row_v[rbest].sub_v[subbest].cl_v.back().width<=c->newx){
            // cout<<"in3333333"<<endl;
            cluster cl;
            cl.collection.push_back(c);
            cl.weight+=c->weight;
            cl.q+=c->weight*(c->newx-cl.width);
            cl.width+=c->width;
            cl.x=c->newx;
            row_v[rbest].sub_v[subbest].cl_v.push_back(cl);
            
            // if(sr->cl_v.size()>0){
            //     cout<<"statement 33333333"<<endl;
            // }
        }else {
            row_v[rbest].sub_v[subbest].cl_v.back().collection.push_back(c);
            row_v[rbest].sub_v[subbest].cl_v.back().weight+=c->weight;
            row_v[rbest].sub_v[subbest].cl_v.back().q+=c->weight*(c->newx-row_v[rbest].sub_v[subbest].cl_v.back().width);
            row_v[rbest].sub_v[subbest].cl_v.back().width+=c->width;

            int pos=row_v[rbest].sub_v[subbest].cl_v.size()-1;
            // collapse
            row_v[rbest].sub_v[subbest].cl_v.back().x=row_v[rbest].sub_v[subbest].cl_v.back().q/row_v[rbest].sub_v[subbest].cl_v.back().weight;
            if(row_v[rbest].sub_v[subbest].cl_v.back().x<row_v[rbest].sub_v[subbest].x1){
                row_v[rbest].sub_v[subbest].cl_v.back().x=row_v[rbest].sub_v[subbest].x1;
            }
            if(row_v[rbest].sub_v[subbest].cl_v.back().x>row_v[rbest].sub_v[subbest].x2-row_v[rbest].sub_v[subbest].cl_v.back().width){
                row_v[rbest].sub_v[subbest].cl_v.back().x=row_v[rbest].sub_v[subbest].x2-row_v[rbest].sub_v[subbest].cl_v.back().width;
            }

            while(pos>0&&row_v[rbest].sub_v[subbest].cl_v[pos-1].x+row_v[rbest].sub_v[subbest].cl_v[pos-1].width>row_v[rbest].sub_v[subbest].cl_v[pos].x){
                row_v[rbest].sub_v[subbest].cl_v[pos-1].collection.insert(row_v[rbest].sub_v[subbest].cl_v[pos-1].collection.end(),row_v[rbest].sub_v[subbest].cl_v[pos].collection.begin(),row_v[rbest].sub_v[subbest].cl_v[pos].collection.end());
                
                row_v[rbest].sub_v[subbest].cl_v[pos-1].weight+=row_v[rbest].sub_v[subbest].cl_v[pos].weight;
                row_v[rbest].sub_v[subbest].cl_v[pos-1].q+=row_v[rbest].sub_v[subbest].cl_v[pos].q-row_v[rbest].sub_v[subbest].cl_v[pos].weight*row_v[rbest].sub_v[subbest].cl_v[pos-1].width;
                row_v[rbest].sub_v[subbest].cl_v[pos-1].width+=row_v[rbest].sub_v[subbest].cl_v[pos].width;
                row_v[rbest].sub_v[subbest].cl_v.pop_back();
                row_v[rbest].sub_v[subbest].cl_v[pos-1].x=row_v[rbest].sub_v[subbest].cl_v[pos-1].q/row_v[rbest].sub_v[subbest].cl_v[pos-1].weight;
                if(row_v[rbest].sub_v[subbest].cl_v[pos-1].x<row_v[rbest].sub_v[subbest].x1){
                    row_v[rbest].sub_v[subbest].cl_v[pos-1].x=row_v[rbest].sub_v[subbest].x1;
                }
                if(row_v[rbest].sub_v[subbest].cl_v[pos-1].x>row_v[rbest].sub_v[subbest].x2-row_v[rbest].sub_v[subbest].cl_v[pos-1].width){
                    row_v[rbest].sub_v[subbest].cl_v[pos-1].x=row_v[rbest].sub_v[subbest].x2-row_v[rbest].sub_v[subbest].cl_v[pos-1].width;
                }
                pos--;
            }
            
            // double optimal_position=sr->cl_v.back().x;
            // for(int i=0;i<sr->cl_v.back().collection.size();i++){
            //     sr->cl_v.back().collection[i]->x=optimal_position;
            //     optimal_position+=sr->cl_v.back().collection[i]->width;
            // }
        }

    }
    // cout<<"SIZE     "<<sr->cl_v.size()<<endl;
    if(row_v[rbest].sub_v[subbest].cl_v.back().x<row_v[rbest].sub_v[subbest].x1){
        cout<<"ERROR in 222222222222"<<endl;
        exit(0);
    }
    

}

// void placerow2(subrow *sr,cell *c){
//     if(sr->width<c->width){
//         cout <<  "EEEEE" << endl;
//         exit(0);
//     }
//     c->newy=sr->y;
//     // double newx=c->x;
//     if(c->x<sr->x1){
//         c->newx=sr->x1;
//     }else if(c->x+c->width > sr->x2){
//         c->newx=sr->x2-c->width;
//     }

//     if(sr->cl_v.size()==0){
//         cluster cl;
//         cl.collection.emplace_back(c);
//         cl.weight=c->weight;
//         cl.q=c->weight*(c->newx-cl.width);
//         cl.width=c->width;
//         cl.x=c->newx;
//         sr->cl_v.push_back(cl);
//         if(sr->cl_v.size()>0){
//                 cout<<"statement 1111111"<<endl;
//         }
        
//     }else {
//         // if(sr->cl_v.size()>0){
//         //         cout<<"statement 222222"<<endl;
//         // }
//         if(sr->cl_v.back().x+sr->cl_v.back().width<=c->newx){
//             // cout<<"in3333333"<<endl;
//             cluster cl;
//             cl.collection.push_back(c);
//             cl.weight+=c->weight;
//             cl.q+=c->weight*(c->newx-cl.width);
//             cl.width+=c->width;
//             cl.x=c->newx;
//             sr->cl_v.push_back(cl);
            
//             // if(sr->cl_v.size()>0){
//             //     cout<<"statement 33333333"<<endl;
//             // }
//         }else {
//             sr->cl_v.back().collection.push_back(c);
//             sr->cl_v.back().weight+=c->weight;
//             sr->cl_v.back().q+=c->weight*(c->newx-sr->cl_v.back().width);
//             sr->cl_v.back().width+=c->width;

//             int pos=sr->cl_v.size()-1;
//             // collapse
//             sr->cl_v.back().x=sr->cl_v.back().q/sr->cl_v.back().weight;
//             if(sr->cl_v.back().x<sr->x1){
//                 sr->cl_v.back().x=sr->x1;
//             }
//             if(sr->cl_v.back().x>sr->x2-sr->cl_v.back().width){
//                 sr->cl_v.back().x=sr->x2-sr->cl_v.back().width;
//             }

//             while(pos>0&&sr->cl_v[pos-1].x+sr->cl_v[pos-1].width>sr->cl_v[pos].x){
//                 sr->cl_v[pos-1].collection.insert(sr->cl_v[pos-1].collection.end(),sr->cl_v[pos].collection.begin(),sr->cl_v[pos].collection.end());
                
//                 sr->cl_v[pos-1].weight+=sr->cl_v[pos].weight;
//                 sr->cl_v[pos-1].q+=sr->cl_v[pos].q-sr->cl_v[pos].weight*sr->cl_v[pos-1].width;
//                 sr->cl_v[pos-1].width+=sr->cl_v[pos].width;
//                 sr->cl_v.pop_back();
//                 sr->cl_v[pos-1].x=sr->cl_v[pos-1].q/sr->cl_v[pos-1].weight;
//                 if(sr->cl_v[pos-1].x<sr->x1){
//                     sr->cl_v[pos-1].x=sr->x1;
//                 }
//                 if(sr->cl_v[pos-1].x>sr->x2-sr->cl_v[pos-1].width){
//                     sr->cl_v[pos-1].x=sr->x2-sr->cl_v[pos-1].width;
//                 }
//                 pos--;
//             }
            
//             // double optimal_position=sr->cl_v.back().x;
//             // for(int i=0;i<sr->cl_v.back().collection.size();i++){
//             //     sr->cl_v.back().collection[i]->x=optimal_position;
//             //     optimal_position+=sr->cl_v.back().collection[i]->width;
//             // }
//         }

//     }
//     // cout<<"SIZE     "<<sr->cl_v.size()<<endl;
//     if(sr->cl_v.back().x<sr->x1){
//         cout<<"ERROR in 222222222222"<<endl;
//         exit(0);
//     }
    

// }


void setxy(){
    cout<<"in "<<endl;
    cout<<row_num<<endl;
    for(int i=0;i<row_num;i++){
        row r=row_v[i];

        // cout<<"subrow "<<r.sub_v.size()<<endl;

        for(int j=0;j<r.sub_v.size();j++){
            subrow sr=r.sub_v[j];
            // cout<<"cluster number"<<sr.cl_v.size()<<endl;
            for(int k=0;k<sr.cl_v.size();k++){
                int num1=(sr.cl_v[k].x-sr.x1)/r.site_width;
                // cout<<num1<<endl;
                int newx=num1*r.site_width+sr.x1;
                if(newx<sr.x1){
                    cout<<"ERROR in first"<<endl;
                    exit(0);
                }
                for(int m=0;m<sr.cl_v[k].collection.size();m++){
                    cell *c=sr.cl_v[k].collection[m];
                    // cout<<"c name: "<<c->name<<endl;
                    c->newx=newx;
                    c->newy=r.y;
                    newx+=c->width;
                    // if(c->name=="a11188"){
                    //     cout<<c->newx<<"    "<<c->newy<<endl;
                    // }
                    if(newx>sr.x2){
                        cout<<"ERROR"<<endl;
                        exit(0);
                    }
                }
            }
        }
        
    }
    return;
}

void abacus(){
    cout<<"In abacus "<<endl;
    vector<cell*>sort_cell=cell_v;
    int rbest=0,subbest=0,sr_idx=0;
    double bcost=DBL_MAX,cost=DBL_MAX;
    // for(int i=0;i<10;i++){
    //     cout<<cell_v[i]->name<<"    "<<cell_v[i]->x<<endl;
    // }
    sort(sort_cell.begin(),sort_cell.end(),[](const cell *a,const cell *b){
        return a->x < b->x;        
    });
    // cout<<"-------------------"<<endl;
    // for(int i=0;i<10;i++){
    //     cout<<cell_v[i]->name<<"    "<<cell_v[i]->x<<endl;
    // }
    subrow *sr; //bool first = true;
    int num=0;
    for(int i=0;i<cell_v.size();i++){
        cell *c=sort_cell[i];
        bcost=DBL_MAX,cost=DBL_MAX;
        bool row_cnt=false;
        int rowcnt_num=0;
        for(int j=c->rbest;j<row_num ;j++){
            // int mindis=INT_MAX,dis1,dis2;
            // bool notgood=false;
            // int notgood_num=0;
            // int runrow_num=0;
            row_cnt=false;
            if(rowcnt_num==SETROW){
                // cout<<"in nummmm";
                break;
            }
            double prex=DBL_MAX,curx;
            bool sub_cnt=false;
            int sub_num=0;
            for(int k=0;k<row_v[j].sub_v.size();k++){
                if(row_v[j].sub_v[k].width<c->width){
                    continue;
                }
                // if(terminal_num!=0&&row_cnt==false){
                if(row_cnt==false){
                    rowcnt_num++;
                    row_cnt=true;
                }

                cost=placerow1(row_v[j].sub_v[k],c);
                //cout << "cost = " << cost << endl;
                if(cost<bcost){
                    bcost=cost;
                    rbest=j;
                    subbest=k;
                    // sr=&row_v[j].sub_v[k];
                    // cout<<sr.x1<<"  "<<sr.y<<endl;
                }
                // if(sub_cnt){
                //     break;
                // }
                // curx=abs(c->newx-c->x);
                // if(prex>curx){
                //     prex=curx;
                // }else{
                //     // break;
                // }
                if(sub_cnt){
                    sub_num++;
                    // break;
                    if(sub_num==3){
                        break;
                    }
                }else {
                    curx=abs(c->newx-c->x);
                    if(prex>curx){
                        prex=curx;
                    }else{
                        sub_cnt=true;
                        // break;
                    }
                }
            }
        }
        row_cnt=false;
        rowcnt_num=0;
        for(int j=c->rbest-1;j>=0;j--){
            // int mindis=INT_MAX,dis1,dis2;
            // bool notgood=false;
            // int notgood_num=0;
            row_cnt=false;
            if(rowcnt_num==SETROW){
                break;
            }
            double prex=DBL_MAX,curx;
            bool sub_cnt=false;
            int sub_num=0;
            for(int k=0;k<row_v[j].sub_v.size();k++){
                if(row_v[j].sub_v[k].width<c->width){
                    continue;
                }
                if(row_cnt==false){
                    rowcnt_num++;
                    row_cnt=true;
                }
                // if(terminal_num!=0){
                //     dis1=abs(c->x - row_v[j].sub_v[k].x1);
                //     dis2=abs(c->x - row_v[j].sub_v[k].x2);
                //     if(dis1<mindis||dis2<mindis){
                //         mindis=min(dis1,dis2);
                //     }else{
                //         notgood_num++;
                //     }
                //     if(notgood_num==5){
                //         notgood=true;
                //     }
                // }
                
                cost=placerow1(row_v[j].sub_v[k],c);
                //cout << "cost = " << cost << endl;
                if(cost<bcost){
                    bcost=cost;
                    rbest=j;
                    subbest=k;
                    // sr=&row_v[j].sub_v[k];
                    // cout<<sr.x1<<"  "<<sr.y<<endl;
                }
                // if(sub_cnt){
                //     break;
                // }
                // curx=abs(c->newx-c->x);
                // if(prex>curx){
                //     prex=curx;
                // }else{
                //     // break;
                // }
                if(sub_cnt){
                    sub_num++;
                    // break;
                    if(sub_num==2){
                        break;
                    }
                }else {
                    curx=abs(c->newx-c->x);
                    if(prex>curx){
                        prex=curx;
                    }else{
                        sub_cnt=true;
                        // break;
                    }
                }
            }
        }
        // double t_x, t_y;
        // if(first){
        //     first = false;
        //     t_x = sr->x1;
        //     t_y = sr-> y;
        // }
        // if(sr->x1== t_x && sr->y==t_y){
        //     cout << sr->width << "," << c->width << endl;
        // }
        // num++;
        placerow2(rbest,subbest,c);
        // cout<<"placerow "<<num<<endl;
        // cout<<"MAIN   SIZE     "<<sr->cl_v.size()<<endl;
        row_v[rbest].sub_v[subbest].width-=c->width;


        // // find the best row
        // double num=abs(row_v[0].y - c->y);
        // for(int j=1;j<row_num;j++){
        //     if(num>abs(row_v[j].y-c->y)){
        //         num=abs(row_v[j].y-c->y);
        //         rbest=j;
        //     }
        // }
        // // cout<<"rbest "<<rbest<<endl;
        // // cout<<"original subbest "<<subbest<<endl;
        // bcost=cost=placerow1(rbest,c,subbest);
        // // cout<<"subbest: "<<subbest<<endl;
        // // cout<<"==================="<<endl;

        // //find the best row
        // for(int j=rbest-1;j>=0;j--){
        //     cost=placerow1(j,c,sr_idx);
        //     if(bcost>cost){
        //         rbest=j;
        //         subbest=sr_idx;
        //         // cout<<subbest<<" ";
        //         bcost=cost;
        //     }
        // }
        // // cout<<"11       " ;
        // for(int j=rbest+1;j<row_num;j++){
        //     cost=placerow1(j,c,sr_idx);
        //     if(bcost>cost){
        //         rbest=j;
        //         subbest=sr_idx;
        //         bcost=cost;
        //     }
        // }
        // // cout<<"22"<<endl;
        // placerow2(rbest,subbest,c);
        // row_v[rbest].sub_v[subbest].width-=c->width;
        
    }
    
    return;
}

int main(int argc, char* argv[]){
    run_parser(argv[1]);
    for(int i=0;i<row_num;i++){
        if(row_v[i].sub_v.size()!=1){
            cout<<"ERROR"<<endl;
        }
    }
    cout << "Data" << (double)clock()/CLOCKS_PER_SEC << " s" << endl;

    spitrow();
    abacus();
    cout << "algorithm" << (double)clock()/CLOCKS_PER_SEC << " s" << endl;

    setxy();

    writefile(argv[2]);
    cout << "all runtime" << (double)clock()/CLOCKS_PER_SEC << " s" << endl;
    cout<<"MAX Displacement "<<maxdisplace<<endl;
    return 0;
}


