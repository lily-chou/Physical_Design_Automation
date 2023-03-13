#include<bits/stdc++.h>
using namespace std;

int cell_n=0,net_n=0,p_max,total_cell,pmax=0,gmaxa=0,gmaxb=0,a_num,b_num;
long long int area_a=0,area_b=0;
stringstream ss;

struct cell{
    string name;
    int sizea;
    int sizeb;
    int gain=0;
    int pin=0;
    bool isA=false;
    bool lock=false;
    vector<int>net_list;
    cell *pre=NULL;
    cell *next=NULL;
};
vector<cell*>cell_v(100000,NULL);
vector<cell*>lista,listb;

struct net{
    string name;
    int seta=0;
    int setb=0;
    vector<int>cell_list;
};
vector<net*>net_v(100000,NULL);

void runparser(string cell_f,string net_f){
    //cout<<"runparser "<<endl;
    string line,tmp;

    /**cell file**/
    ifstream ifs(cell_f);  
    while(getline(ifs,line)){
        ss.str("");
        ss.clear();
        ss<<line;
        cell *tmpc=new cell();
        ss>>tmpc->name;
        ss>>tmpc->sizea;
        ss>>tmpc->sizeb;
        cell_n++;
        int pos=stoi(&(tmpc->name[1]));
        while(pos>cell_v.size()-1){
            cell_v.resize(2*cell_v.size());
        }
        cell_v[pos]=tmpc;
        total_cell=total_cell+tmpc->sizea+tmpc->sizeb;
    }
    ifs.close();
    //cout<<"cell"<<endl;
    // for(int i=1;i<cell_n;i++){
    //     cout<<cell_v[i]->name<<" ";
    // }

    /**NET file**/
    ifs.open(net_f);    
    while(getline(ifs,line)){
        //ss.str("");
        ss.clear();
        ss<<line;
        ss>>tmp;    //NET
        net *tmpn=new net();
        ss>>tmpn->name;
        
        while(ss>>tmp){
            if(tmp=="{"){
                continue;
            }
            if(tmp=="}"){
                break;
            }
            int pos=stoi(&(tmp[1]));
            tmpn->cell_list.push_back(pos);
            cell_v[pos]->pin++;                  //add the number of pin in cell
            cell_v[pos]->net_list.push_back(stoi(&(tmpn->name[1])));      //add the net from the cell
        }
        int pos=stoi(&(tmpn->name[1]));
        while(pos>net_v.size()-1){
            net_v.resize(2*net_v.size());
        }
        net_v[pos]=tmpn;
        net_n++;
    }
    ifs.close();

    //cout<<"net"<<endl;
    // for(int i=1;i<=net_n;i++){
    //     cout<<net_v[i]->name<<" "<<net_v[i]->cell_list.size()<<endl;
    // }

    return;
}

void init_partition(){
    // cout<<"initial partition"<<endl;
    // cout<<cell_v.size()<<" "<<net_v.size()<<endl;
    vector<cell*>partb=cell_v;
    sort(partb.begin()+1,partb.begin()+cell_n+1,[](const cell* a,const cell*b)
    {return abs(a->sizea-a->sizeb)<abs(b->sizea-b->sizeb);
    });
    // cout<<"sort"<<endl;
    // for(int i=1;i<=cell_n;i++){
    //     cout<<partb[i]->name<<" ";
    // }
    int st=1;
    b_num=cell_n;
    for(int i=1;i<=cell_n;i++){
        area_b+=partb[i]->sizeb;
    }
    while(!(10*abs(area_a-area_b)<abs(area_a+area_b))){
        area_b-=partb[st]->sizeb;
        area_a+=partb[st]->sizea;
        partb[st]->isA=true;
        st++;
        b_num--;
        a_num++;
        // for(int i=1;i<cell_n;i++){
        //     if(partb[i]->isA){
        //         area_a+=partb[i]->sizea;
        //     }else{
        //         area_b+=partb[i]->sizeb;
        //     }
        // }
    }
    // cout<<area_a<<"      "<<area_b<<endl;
    return;
}

void output_list(){
    cout<<"AAAAAAAAAAAA"<<endl;
    for(int i=0;i<2*pmax+1;i++){
        cout<<"i: "<<i<<"( "<< i-pmax<<" )"<<endl;
        if(lista[i]!=NULL){
            cout<<lista[i]->name<<" ";
            cell *c=lista[i];
            while(c->next!=NULL){
                c=c->next;
                cout<<c->name<<" ";
            }
        }
        cout<<endl;
    }
    cout<<endl;

    cout<<"BBBBBBBBBBBBBB"<<endl;
    for(int i=0;i<2*pmax+1;i++){
        cout<<"i: "<<i<<"( "<< i-pmax<<" )"<<endl;
        if(listb[i]!=NULL){
            cout<<listb[i]->name<<" ";
            cell *c=listb[i];
            while(c->next!=NULL){
                c=c->next;
                cout<<c->name<<" ";
            }
        }
        cout<<endl;
    }
    cout<<endl;
    return;
}

void init_gain(){
    // cout<<"Gain  "<<endl;
    //get the max pin
    for(int i=1;i<=cell_n;i++){
        pmax=max(pmax,cell_v[i]->pin);
    }
    // cout<<"Pin Max  "<<pmax<<endl;
    lista.resize(2*pmax+1);
    listb.resize(2*pmax+1);

    //get the number of set A and set B of the net
    for(int i=1;i<=net_n;i++){
        for(int j=0;j<net_v[i]->cell_list.size();j++){
            int pos=net_v[i]->cell_list[j];
            //cell *tmp=cell_v[pos];
            if(cell_v[pos]->isA){
                (net_v[i]->seta)++;
            }else{
                (net_v[i]->setb)++;
            }
        }
    }
    //cout<<"finish net counting"<<endl;

    // compute the gain
    for(int i=1;i<=cell_n;i++){
        for(int j=0;j<cell_v[i]->net_list.size();j++){
            int netidx=cell_v[i]->net_list[j];
            if(cell_v[i]->isA){
                if(net_v[netidx]->seta==1){
                    cell_v[i]->gain++;
                }
                if(net_v[netidx]->setb==0){
                    cell_v[i]->gain--;
                }
            }else{
                if(net_v[netidx]->setb==1){
                    cell_v[i]->gain++;
                }
                if(net_v[netidx]->seta==0){
                    cell_v[i]->gain--;
                }
            }
        }
    }

    //create bucket list of a & b

    for(int i=1;i<=cell_n;i++){
        int pos=cell_v[i]->gain+pmax;
        if(cell_v[i]->isA){
            gmaxa=max(cell_v[i]->gain,gmaxa);
            if(lista[pos]==NULL){
                lista[pos]=cell_v[i];
            }else{
                //insert_cell(lista,pos,cell_v[i]);
                lista[pos]->pre=cell_v[i];
                cell_v[i]->next=lista[pos];
                lista[pos]=cell_v[i];
            }
        }else{
            gmaxb=max(cell_v[i]->gain,gmaxb);
            if(listb[pos]==NULL){
                listb[pos]=cell_v[i];
            }else{
                //insert_cell(listb,pos,cell_v[i]);
                listb[pos]->pre=cell_v[i];
                cell_v[i]->next=listb[pos];
                listb[pos]=cell_v[i];
            }
        }
    }

    // cout<<"Pin Max  "<<pmax<<endl;
    // cout<<"Gain Maxa  "<<gmaxa<<endl;
    // cout<<"Gain Maxb  "<<gmaxb<<endl;  
    return;
}

void get_maxgain(){
    gmaxa=INT_MIN;
    gmaxb=INT_MIN;
   
    //cout<<"cccccccccccc "<<endl;
    
    //cout<<"get ";
    for(int i=2*pmax;i>=0;i--){
        if(lista[i]!=NULL){
            gmaxa=i-pmax;
            break;
        }
    }
    for(int i=2*pmax;i>=0;i--){
        if(listb[i]!=NULL){
            gmaxb=i-pmax;
            break;
        }
    }
    //cout<<"gain "<<gmaxa<<" "<<gmaxb<<endl;
    //output_list();
    
    
}

void remove_cell(cell *tmpc2){
    if(tmpc2==NULL){
        cout<<"error"<<endl;
    }
    if(tmpc2->gain<-pmax||tmpc2->gain>pmax){
        cout<<"error in gain"<<endl;
    }
    if(tmpc2->isA){
        if(tmpc2->pre==NULL&&tmpc2->next==NULL){
            lista[tmpc2->gain+pmax]=NULL;
        }else if(tmpc2->pre==NULL){
            lista[tmpc2->gain+pmax]=tmpc2->next;
            tmpc2->next->pre=NULL;
        }else if(tmpc2->next==NULL){
            tmpc2->pre->next=NULL;
        }else{
            tmpc2->pre->next=tmpc2->next;
            tmpc2->next->pre=tmpc2->pre;
        }
    }else{
        if(tmpc2->pre==NULL&&tmpc2->next==NULL){
            listb[tmpc2->gain+pmax]=NULL;
        }else if(tmpc2->pre==NULL){
            listb[tmpc2->gain+pmax]=tmpc2->next;
            tmpc2->next->pre=NULL;
        }else if(tmpc2->next==NULL){
            tmpc2->pre->next=NULL;
        }else{
            tmpc2->pre->next=tmpc2->next;
            tmpc2->next->pre=tmpc2->pre;
        }
    }
    tmpc2->pre=NULL;
    tmpc2->next=NULL;
    return ;
}

void add_cell(cell *tmpc2){
    if(tmpc2->gain<-pmax||tmpc2->gain>pmax){
        cout<<"error in gain"<<endl;
    }
    if(tmpc2->isA){
        //gmaxa=max(gmaxa,tmpc2->gain);
        if(lista[tmpc2->gain+pmax]==NULL){
            lista[tmpc2->gain+pmax]=tmpc2;
        }else{
            //insertnode(lista,tmpc2->gain+pmax,tmpc2);
            lista[tmpc2->gain+pmax]->pre=tmpc2;
            tmpc2->next=lista[tmpc2->gain+pmax];
            lista[tmpc2->gain+pmax]=tmpc2;
            //tmpc2->pre=NULL;
        }
    }else{
        //gmaxb=max(gmaxb,tmpc2->gain);
        if(listb[tmpc2->gain+pmax]==NULL){
            listb[tmpc2->gain+pmax]=tmpc2;
        }else{
            //insertnode(lista,tmpc2->gain+pmax,tmpc2);
            listb[tmpc2->gain+pmax]->pre=tmpc2;
            tmpc2->next=listb[tmpc2->gain+pmax];
            listb[tmpc2->gain+pmax]=tmpc2;
            //tmpc2->pre=NULL;
        }
    }
    return ;
}

void update_gain(cell *tmpc){
    //cout<<"update gain ";
    //When the updated cell is is A
    remove_cell(tmpc);
    tmpc->lock=true;
    if(tmpc->isA){
        //the net from the cell that needed to be updated
        for(int i=0;i<tmpc->net_list.size();i++){    
            net* tmpn=net_v[tmpc->net_list[i]];
            //erase the original gain in lista
            if(tmpn->setb==0){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if(tmpc2->lock==false){
                        remove_cell(tmpc2);
                        tmpc2->gain++;
                        add_cell(tmpc2);
                    }
                }
            }else if(tmpn->setb==1){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if((tmpc2->isA==false)&&(tmpc2->lock==false)){
                        remove_cell(tmpc2);
                        tmpc2->gain--;
                        add_cell(tmpc2);
                    }
                }
            }

            
            tmpc->isA=false;
            //tmpc->lock=true;
            //the cell's nets which contain the cell that connected to 
            
            tmpn->seta--;
            tmpn->setb++;

            if(tmpn->seta==0){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if(tmpc2->lock==false){
                        remove_cell(tmpc2);
                        tmpc2->gain--;
                        add_cell(tmpc2);
                    }
                }
            }else if(tmpn->seta==1){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if((tmpc2->isA)&&(tmpc2->lock==false)){
                        remove_cell(tmpc2);
                        tmpc2->gain++;
                        add_cell(tmpc2);
                    }
                }
            }
        }
    }//when the updated cell is in B
    else{
        for(int i=0;i<tmpc->net_list.size();i++){    
            net* tmpn=net_v[tmpc->net_list[i]];
            //erase the original gain in lista
            if(tmpn->seta==0){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if(tmpc2->lock==false){
                        remove_cell(tmpc2);
                        tmpc2->gain++;
                        add_cell(tmpc2);
                    }
                }
            }else if(tmpn->seta==1){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if((tmpc2->isA==true)&&(tmpc2->lock==false)){
                        remove_cell(tmpc2);
                        tmpc2->gain--;
                        add_cell(tmpc2);
                    }
                }
            }

            //cout<<"remove the cell "<<endl;
            tmpc->isA=true;

            tmpn->seta++;
            tmpn->setb--;

            if(tmpn->setb==0){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if(tmpc2->lock==false){
                        remove_cell(tmpc2);
                        tmpc2->gain--;
                        add_cell(tmpc2);
                    }
                }
            }else if(tmpn->setb==1){
                for(int j=0;j<tmpn->cell_list.size();j++){
                    cell* tmpc2=cell_v[tmpn->cell_list[j]];
                    if((tmpc2->isA==false)&&(tmpc2->lock==false)){
                        remove_cell(tmpc2);
                        tmpc2->gain++;
                        add_cell(tmpc2);
                    }
                }
            }
            
            // cout<<"rebuild list "<<endl;
            // cout<<"----------------"<<endl;
        }
    }
    get_maxgain();
    
    //cout<<"cccccccccccc "<<endl;
    //output_list();
    //cout<<"a b : "<<gmaxa<<" "<<gmaxb<<endl;

}

bool check_balance(long long int a, long long int b){
    return (10*abs(a-b)<abs(a+b));
}

cell* select_cell(){
    //cout<<"select   ";
    // cout<<gmaxa<<" "<<gmaxb<<endl;
    int cur_gaina=gmaxa,cur_gainb=gmaxb;
    while(cur_gaina>=-pmax||cur_gainb>=-pmax){
        // if(!(gmaxa<=pmax&&gmaxa>=-pmax)){
        //     cout<<"error A"<<endl;
        // }
        // if(!(gmaxb<=pmax&&gmaxb>=-pmax)){
        //     cout<<"error B"<<endl;
        // }
        //cout<<gmaxa+pmax<<endl;
        if(cur_gaina>=cur_gainb){
            //cout<<"inAAAA "<<endl;
            cell *tmpc=lista[gmaxa+pmax];
            if(check_balance(area_a-tmpc->sizea,area_b+tmpc->sizeb)){
                //cout<<"return ";
                return tmpc;
            }
            while(tmpc->next!=NULL){
                tmpc=tmpc->next;
                if(check_balance(area_a-tmpc->sizea,area_b+tmpc->sizeb)){
                    //cout<<"return ";
                    return tmpc;
                }
            }
            cur_gaina--;
            while(lista[cur_gaina+pmax]==NULL){
                cur_gaina--;
                if(cur_gaina<-pmax){
                    break;
                }
            }
            continue;
        }else{
            //cout<<"inBBBB "<<endl;
            cell *tmpc=listb[cur_gainb+pmax];
            if(check_balance(area_a+tmpc->sizea,area_b-tmpc->sizeb)){
                //cout<<"return ";
                return tmpc;
            }
            while(tmpc->next!=NULL){
                tmpc=tmpc->next;
                if(check_balance(area_a+tmpc->sizea,area_b-tmpc->sizeb)){
                    //cout<<"return ";
                    return tmpc;
                }
            }
            cur_gainb--;
            while(listb[cur_gainb+pmax]==NULL){
                cur_gainb--;
                if(cur_gainb<-pmax){
                    break;
                }
            }
            continue;
        }
    }
    //cout<<"return ";
    return NULL;
}

int fm(){
    int partial=0,sum=0, par_pos=0;
    int par_numa,par_numb;
    vector<cell*>update_cell;
    //cout<<"initial num "<<a_num<<" "<<b_num<<endl;
    //output_list();
    //cout<<"a b "<<gmaxa<<" "<<gmaxb<<endl;
    for(int i=1;i<=cell_n;i++){
        cell *tmpc=select_cell();
        //cout<<"return";
        if(tmpc==NULL){
            //cout<<"NULL "<<endl;
            break;
        }
        //cout<<"name "<<tmpc->name<<endl;
        if(tmpc->isA){
            sum+=tmpc->gain;
            update_gain(tmpc);
            a_num--;
            b_num++;
            area_a-=tmpc->sizea;
            area_b+=tmpc->sizeb;
        }else{
            sum+=tmpc->gain;
            update_gain(tmpc);
            a_num++;
            b_num--;
            area_a+=tmpc->sizea;
            area_b-=tmpc->sizeb;
        }
        update_cell.push_back(tmpc);
        if(partial<=sum){
            partial=sum;
            par_numa=a_num;
            par_numb=b_num;
            //store_partial.push_back(make_pair(partial,update_cell.size()));
            par_pos=update_cell.size()-1;
        }

    }

    //cout<<"last num "<<a_num<<" "<<b_num<<endl;

    for(int i=update_cell.size()-1;i>=par_pos;i--){
        update_gain(update_cell[i]);
        if(update_cell[i]->isA){
            area_a-=update_cell[i]->sizea;
            area_b+=update_cell[i]->sizeb;
        }else{
            area_a+=update_cell[i]->sizea;
            area_b-=update_cell[i]->sizeb;
        }
        update_cell[i]->lock=false;
        add_cell(update_cell[i]);
    }
    //cout<<"66666666";
    for(int i=0;i<par_pos;i++){
        update_cell[i]->lock=false;
        update_cell[i]->gain=0;
        for(int j=0;j<update_cell[i]->net_list.size();j++){
            if(update_cell[i]->isA){
                if(net_v[update_cell[i]->net_list[j]]->seta==1){
                    update_cell[i]->gain++;
                }else if(net_v[update_cell[i]->net_list[j]]->setb==0){
                    update_cell[i]->gain--;
                }

            }else if(!update_cell[i]->isA){
                if(net_v[update_cell[i]->net_list[j]]->setb=1){
                    update_cell[i]->gain++;
                }else if(net_v[update_cell[i]->net_list[j]]->seta==0){
                    update_cell[i]->gain--;
                }
            }
            
        }
        add_cell(update_cell[i]);
    }
    get_maxgain();

    return partial;
}

void writefile(string outfile){
    //cout<<"inwrite file"<<endl;
    ofstream ofs(outfile);
    int cutsize=0;
    vector<string>ansa,ansb;
    for(int i=1;i<=cell_n;i++){
        if(cell_v[i]->isA){
            //ansa=ansa+"\n"+cell_v[i]->name;
            ansa.push_back(cell_v[i]->name);
        }else{
            //ansb=ansb+"\n"+cell_v[i]->name;
            ansb.push_back(cell_v[i]->name);
        }
    }
    for(int i=1;i<=net_n;i++){
        if(net_v[i]->seta!=0&&net_v[i]->setb!=0){
            cutsize++;
        }
    }
    //cout<<cutsize<<endl;
    ofs<<"cut_size "<< cutsize<<"\n";
    ofs<<"A "<<ansa.size()<<"\n";
    for(int i=0;i<ansa.size();i++){
        ofs<<ansa[i]<<"\n";
    }
    ofs<<"B "<<ansb.size()<<"\n";
    for(int i=0;i<ansb.size();i++){
        ofs<<ansb[i]<<"\n";
    }
    //ofs<<ansa<<ansb;
    ofs.close();
}

int main(int argc, char* argv[]){
    double st_time,end_time;
    //st_time=clock();
    runparser(argv[1],argv[2]);
    init_partition();
    init_gain();
    int iter=0;
    int sum=0;
    //st_time=clock();
    while(true){
        iter++;

        sum=fm();
        end_time=clock();
        if(sum<=0){
            break;
        }
        if((clock()/CLOCKS_PER_SEC)>298.0){
            break;
        }
    }
    
    //cout<<"iter "<<iter<<endl;
    writefile(argv[3]);
    //cout<<(t/CLOCKS_PER_SEC)<<"sec."<<endl;
    return 0;
}

