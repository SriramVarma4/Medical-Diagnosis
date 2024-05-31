#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cmath>
 


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<float> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	void set_CPT(vector<float> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }



};


 // The whole network represted as a list of nodes
class network{

	list <Graph_Node> Pres_Graph;

public:
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n)
    {
       list<Graph_Node>::iterator listIt;
        int count=0;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(count==n)
                return listIt;
            count++;
        }
        return listIt; 
    }
    //get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name)
    {
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++)
        {
            if(listIt->get_name().compare(val_name)==0)
                return listIt;
        }
    
            cout<<"node not found\n";
        return listIt;
    }
	

};

network read_network(string z)
{
	network Alarm;
	string line;
	int find=0;
    string w = z;
  	ifstream myfile(w); 
  	string temp;
  	string name;
  	vector<string> values;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values);
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;

     				//cout<<temp<<endl;-contains variables

                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        listIt1=Alarm.search_node(temp);
                        listIt1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    listIt->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<float> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                

    				}
                    
                    listIt->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}


void update_network(network& Alarm,string z) {
    ofstream output_file("solved_alarm.bif");
    string w = z;
    ifstream input_file(w);
    string line;
    bool block=false;
    string content;
    int node_index=0;

    while (getline(input_file, line)) {
        if(block) {
            if(line.find("table") != string::npos) {
                list<Graph_Node>::iterator listIt1p=Alarm.get_nth_node(node_index);
                vector<float> updated_CPT=listIt1p->get_CPT();   
                output_file<<"\t";
                output_file<<"table ";
                for (float prob : updated_CPT){
                    output_file<<prob<<" ";
                }
                output_file<<";"<<endl;
                //output_file <<"}"<<endl;
                block=false;
                node_index=node_index+1;
            }
            else{
                output_file<<line<<endl;
                content+=line+"\n";
            }
        } 
        else{
            output_file<<line<<endl;
            if(line.find("probability")!=string::npos){
                block=true;
                content=line+"\n";
            }
        }
    }
    input_file.close();
    output_file.close();
}


int main(int argc, char* argv[])
{
	network Alarm;
    string in = argv[1];
	Alarm=read_network(in);
    int x = Alarm.netSize();
    vector<vector<string>> pos; 
    ifstream record(argv[2]);
    if (record.is_open()){
        while (!record.eof()){
            vector<string> one;
            string np;
            for(int p=0;p<x;p++){ 
                record>>np; 
                one.push_back(np);
            }
            pos.push_back(one); 
        }
    }
    record.close();
       
    for(int t=0;t<x;t++){
        //t=12;
        vector<int> counts;
        vector<int> counts1;
        vector<float> prob;
        for(int tt=0;tt<100*x;tt++){
            counts.push_back(0);
        }
        for(int tt=0;tt<100*x;tt++){
            counts1.push_back(0);
        }
        for(int j=0;j<pos.size();j++){
            list<Graph_Node>::iterator listIt1=Alarm.get_nth_node(t);
            vector<float> cpt1=listIt1->get_CPT();
            string s =listIt1->get_name();
            vector<string> par = listIt1->get_Parents();
            vector<string> vl = listIt1->get_values();
            int nvl = vl.size();
            if(par.size()==0){
                for(int ttt=0;ttt<nvl;ttt++){
                    if(pos[j][t]==vl[ttt]){
                        counts[ttt]=counts[ttt]+1; 
                    }
                }
            }
            else if(par.size() == 1){
                string p = par[0];
                int ind = Alarm.get_index(p);
                list<Graph_Node>::iterator listItp=Alarm.search_node(p);
                vector<string> vlp = listItp->get_values();
                int nvlp = vlp.size();
                for(int z=0;z<nvl;z++){
                    for(int v=0;v<nvlp;v++){
                        if(pos[j][t]==vl[z]&&pos[j][ind]==vlp[v]){
                            counts[v+nvlp*z]=counts[v+nvlp*z]+1;
                        }
                    }
                }
                for(int v=0;v<nvlp;v++){
                    if(pos[j][ind]==vlp[v]){
                        counts1[v]=counts1[v]+1;
                    }
                }
            }
            else{
                vector<vector<string>> pvals;
                for(int r=0;r<par.size();r++){
                    string p = par[r];
                    int ind = Alarm.get_index(p);
                    list<Graph_Node>::iterator listItp=Alarm.search_node(p);
                    vector<string> vlp = listItp->get_values();
                    pvals.push_back(vlp);
                    int nvlp = vlp.size();
                }

                vector<vector<string>> result;
                vector<int> indx(pvals.size(), 0);
                while(true){
                    vector<string> comb;
                    for(size_t i = 0;i<pvals.size();i++){
                        comb.push_back(pvals[i][indx[i]]);
                    }
                    result.push_back(comb);
                    int jj=pvals.size()-1;
                    while(jj>= 0 && indx[jj]+1==pvals[jj].size()){
                        indx[jj]=0;
                        jj=jj-1;
                    }
                    if(jj < 0){
                        break; 
                    }
                    indx[jj]=indx[jj]+1;
                }
                // cout<<result[0][0]<<endl;
                // cout<<result[0][1]<<endl;
                // //cout<<result[0][2]<<endl;
                // cout<<result[3][0]<<endl;
                // cout<<result[3][1]<<endl;
                // cout<<result[4][0]<<endl;
                // cout<<result[4][1]<<endl;
                // cout<<result[5][0]<<endl;
                // cout<<result[5][1]<<endl;
                // //cout<<result[3][2]<<endl;
                // cout<<result[2][3]<<endl;
                int y =result.size();
                for(int z=0;z<nvl;z++){
                    for(int v=0;v<result.size();v++){
                        vector<int> inds;
                        for(int e=0;e<par.size();e++){
                            string p = par[e];
                            int ind = Alarm.get_index(p);
                            inds.push_back(ind);
                        }
                        
                        vector<string> posco;
                        for(int m=0;m<inds.size();m++){
                            int o = inds[m];
                            string p = pos[j][o];
                            posco.push_back(p);
                        }
                        if(posco==result[v]&&pos[j][t]==vl[z]){
                            counts[v+y*z]=counts[v+y*z]+1;
                        }
                    }
                }

                for(int v=0;v<result.size();v++){
                    vector<int> inds;
                    for(int e=0;e<par.size();e++){
                        string ppp = par[e];
                        int ind = Alarm.get_index(ppp);
                        inds.push_back(ind);
                    }
                    
                    vector<string> posco;
                    for(int m=0;m<inds.size();m++){
                        int o = inds[m];
                        string p = pos[j][o];
                        posco.push_back(p);
                    }

                    if(posco==result[v]){
                        counts1[v]=counts1[v]+1;
                    }
                }
            }
        }
        //cout<<endl;

        list<Graph_Node>::iterator listIt11=Alarm.get_nth_node(t);
        vector<string> vll = listIt11->get_values();
        int nvll = vll.size();
        //cout<<nvll<<endl;

        //aa and bb computation technically cheyali
        int tr = pos.size();
        int aa=nvll;
        int bb=0;
        
        // for(int i=0;i<counts.size();i++){ 
        //     if(counts[i]!=0){
        //         aa=i+1;
        //     }
        // }
        // for(int j=0;j<counts1.size();j++ ){
        //     if(counts1[j]!=0){
        //         bb=j+1;
        //     }
        // }
        int q=1;
        vector<string> par1 = listIt11->get_Parents();
        if(par1.size()>0){
            for(int i=0;i<par1.size();i++){
                string p = par1[i];
                int ind = Alarm.get_index(p);
                list<Graph_Node>::iterator listItpp=Alarm.search_node(p);
                vector<string> vlp11 = listItpp->get_values();
                q = q*vlp11.size();
            }
            bb=q;
        }
        else{

        }

        if(bb>0){
            aa = nvll*bb;
            for(int i=0;i<aa;i++){
                prob.push_back(0);
            }
            int miss=0;
            for(int i =0;i<counts.size();i++){
                miss = miss + counts[i];
            }
            int missh= tr-miss;
            float missp=0;
            missp = missh/nvll;
            //cout<<missp<<endl;

            int miss1=0;
            for(int i =0;i<counts1.size();i++){
                miss1 = miss1 + counts1[i];
            }
            int missh1= tr-miss1;
            float missp1=0;
            missp1 = missh1/(bb);
            //cout<<missp1<<endl;
            int h=0;

            for(int j=0;j<bb;j++){
                for(int i=j;i<aa;i=i+bb){  
                    //cout<<aa<<endl;
                    if(counts1[j] != 0){
                        float misspp = (missp*counts[i])/(tr*nvll);
                        float misspp1 = (missp1*counts1[j])/(tr*bb);
                        //cout<<misspp1<<endl;
                        prob[i]=(float(counts[i]+misspp)/(float(counts1[j])));
                        //prob[i]=prob[i].setprecision(3);
                        prob[i] = round(prob[i] * 10000) / 10000;
                    }
                    else if(counts1[j] == 0 && counts[i]==0){
                        if((h%4)==0){
                            prob[i]=0.97;
                        }
                        h++;
                    }
                } 
            }
        }
        else{
            for(int i=0;i<aa;i++){
                prob.push_back(0);
            }
            int miss=0;
            for(int i =0;i<counts.size();i++){
                miss = miss + counts[i];
            }
            int missh= tr-miss;
            float missp=0;
            missp = missh/nvll;
            for(int i =0;i<counts.size();i++){
                if(counts[i] != 0){
                    prob[i]=(float(counts[i])/miss);
                    prob[i] = round(prob[i] * 10000) / 10000;
                }  
            }
        }
        // for(int tt=0;tt<aa;tt++){
        //     cout<<counts[tt]<<" ";
        // }
        // cout<<endl;
        // for(int tt=0;tt<bb;tt++){
        //     cout<<counts1[tt]<<" ";
        // }
        // cout<<endl;
        // for(int i=0;i<prob.size();i++){
        //     cout<<prob[i]<< " ";
        // }
        // cout<<endl;

        list<Graph_Node>::iterator listIt1=Alarm.get_nth_node(t);
        listIt1->set_CPT(prob);
        vector<float> cpt1=listIt1->get_CPT();
    }
    
    update_network(Alarm,in);
}




