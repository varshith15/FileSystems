#include <bits/stdc++.h>
using namespace std;

typedef struct inode{			//structure of inode
	string name;				//file name
	bool df;					//directory or file
	int size;
	time_t create,modify; 
	int db1,db2,db3,nb;
	int ib;
	int read,write;
	char det[5000];				
	map<int,int> index;			//index map
	map<int,string> bs;			//block string map
}inode; 

int main(int argc,char* argv[]){
	string df;
	int d;
	df = argv[1];
	d = stoi(argv[3]);
	vector<int> fv(d*8);
	map<string,inode> hdirec;	//directoris in the home directory
	map<string,vector<string> > direc;	//file names in each directory
	map<string,inode> fnode;	//all files correspoinding inodes
	ifstream fa(df);
	if(fa.good()){				//if the disk file is already present populating all the maps
		int l;
		fa>>l;
		vector<int> e(l);
		fv=e;
		d=l/8;
		for(int i=0;i<l;i++){
			fa>>fv[i];
		}
		//hdirec
		fa>>l;
		for(int i=0;i<l;i++){
			string g;
			fa>>d;
			inode n;
			fa>>n.name;
			fa>>n.df;
			fa>>n.size;
			fa>>n.create;
			fa>>n.modify;
			fa>>n.db1;
			fa>>n.db2;
			fa>>n.db3;
			fa>>n.nb;
			fa>>n.ib;
			fa>>n.read;
			fa>>n.write;
			for(int i=0;i<n.size;i++){
				fa>>n.det[i];
			}
			int m;
			fa>>m;
			for(int i =0;i<m;i++){
				int y,u;
				fa>>y>>u;
				n.index[y]=u;;
			}
			fa>>m;
			for(int i=0;i<m;i++){
				int y;
				string p;
				fa>>y>>p;
				n.bs[y]=p;
			}
			hdirec[g]=n;
		}
		//direct
		fa>>l;
		for(int i=0;i<l;i++){
			vector<string> v;
			string g;
			int m;
			fa>>d>>m;
			for(int i=0;i<m;i++){
				string h;
				fa>>h;
				v.push_back(h);	
			}
			direc[g]=v;
		}
		//fnode
		fa>>l;
		for(int i=0;i<l;i++){
			string g;
			fa>>d;
			inode n;
			fa>>n.name;
			fa>>n.df;
			fa>>n.size;
			fa>>n.create;
			fa>>n.modify;
			fa>>n.db1;
			fa>>n.db2;
			fa>>n.db3;
			fa>>n.nb;
			fa>>n.ib;
			fa>>n.read;
			fa>>n.write;
			for(int i=0;i<n.size;i++){
				fa>>n.det[i];
			}
			int m;
			fa>>m;
			for(int i =0;i<m;i++){
				int y,u;
				fa>>y>>u;
				n.index[y]=u;
			}
			fa>>m;
			for(int i=0;i<m;i++){
				int y;
				string p;
				fa>>y>>p;
				n.bs[y]=p;
			}
			fnode[g]=n;
		}	
	}
	string str;
	cout<<"command>>";
	while(cin>>str){
		if(str=="load"){		//loading a file
			string fn;
			cin>>fn;
			ifstream f1(fn);
			if(f1.good()){
				string cm;
				while(f1>>cm){
					if(cm=="open"){		//opening a file
						string fn1;
						f1>>fn1;
						cout<<"COMMAND: open "<<fn1<<" RESULT: ";
						int i=0;
						string s1="",s2="";
						while(i!=fn1.size()){
							if(i!=0 && fn1[i]=='/'){
								s2=s1;
								s1="";
								i++;
							}
							s1+=fn1[i++];
						}
						if(fnode.find(fn1)!=fnode.end()){
							inode n=fnode[fn1];
							n.read=0;
							n.write=0;
							n.modify=time(NULL);
							fnode[fn1]=n;
							cout<<"File is already present"<<endl;
						}			
						else if(s2=="" || hdirec.find(s2)!=hdirec.end()){
							inode n;
							n.name = fn1;
							n.df=0;
							n.create=n.modify=time(NULL);
							n.db1=n.db2=n.db3=-1,n.nb=-1;
							n.ib=-1;
							for(int i=0;i<fv.size();i++){
								if(!fv[i]){
									n.nb=i;
									fv[i]=true;
									break;
								}
							}
							n.read=n.write=0;
							n.size=0;
							if(n.nb!=-1){
								fnode[fn1]=n;
								if(s2!=""){
									direc[s2].push_back(fn1);
								}
								cout<<"File is created"<<endl;
							}
							else{
								cout<<"Node couldnot be created as there are no blocks available"<<endl;
							}
						}
						else if(s2!="" && hdirec.find(s2)==hdirec.end()){
							cout<<"Directory in the given file name is not present"<<endl;
						}			
					}
					else if(cm=="createdir"){		//creating a directory
						string dir;
						f1>>dir;
						cout<<"COMMAND: createdir "<<dir<<" RESULT: ";
						if(hdirec.find(dir)!=hdirec.end()){
							cout<<"Directory name already present"<<endl;	
						}
						else{
							inode n;
							n.name = dir;
							n.df=1;
							for(int i=0;i<fv.size();i++){
								if(!fv[i]){
									n.nb=i;
									fv[i]=true;
									break;
								}
							}
							n.create=n.modify=time(NULL);
							if(n.nb!=-1){
								hdirec[dir]=n;
								vector<string> p;
								direc[dir]=p;
								cout<<"Directory is created"<<endl;
							}
							else{
								cout<<"Node couldnot be created as there are no blocks available"<<endl;
							}
						}
					}
					else if(cm=="seekread"){		//changing read position
						string s;
						int pos;
						f1>>s>>pos;
						cout<<"COMMAND: seekread "<<s<<" "<<pos<<" RESULT: ";
						if(fnode.find(s)!=fnode.end()){
							inode n = fnode[s];
							if(pos<n.size){
								n.read=pos;
								n.modify=time(NULL);
								cout<<"Read position is changed"<<endl;
							}
							else{
								cout<<"Given pos is out of bound"<<endl;
							}
							fnode[s]=n;
						}
						else{
							cout<<"filename is wrong"<<endl;
						}
					}
					else if(cm=="seekwrite"){		//changing write position
						string s;
						int pos;
						f1>>s>>pos;
						cout<<"COMMAND: seekwrite "<<s<<" "<<pos<<" RESULT: ";
						if(fnode.find(s)!=fnode.end()){
							inode n = fnode[s];
							if(pos<n.size){
								n.write=pos;
								n.modify=time(NULL);
								cout<<"Write position is changed"<<endl;
							}
							else{
								cout<<"Given pos is out of bound"<<endl;
							}
							fnode[s]=n;
						}
						else{
							cout<<"filename is wrong"<<endl;
						}
					}
					else if(cm=="seekwriteend"){	//changing write position
						string s;
						f1>>s;
						cout<<"COMMAND: seekwriteend "<<s<<" RESULT: ";
						if(fnode.find(s)!=fnode.end()){
							inode n = fnode[s];
							n.read=n.size;
							n.modify=time(NULL);
							fnode[s]=n;
							cout<<"Write position is changed"<<endl;
						}
						else{
							cout<<"filename is wrong"<<endl;
						}
					}
					else if(cm=="read"){			//reading from the file
						string s;
						int len;
						f1>>s>>len;
						cout<<"COMMAND: read "<<s<<" "<<len<<" RESULT: ";
						if(fnode.find(s)!=fnode.end()){
							inode n = fnode[s];
							/*for(int i=n.read;i<min(n.read+len,n.size);i++){
								cout<<n.det[i];
							}
							cout<<endl;*/
							cout<<n.read<<" - "<<n.read+len-1<<" ";
							for(int i=n.read;i<min(n.read+len,n.size);i++){
								int l=i/128;
								int m=i%128;
								if(l==0){
									cout<<n.bs[n.db1][m];
								}
								else if(l==1){
									cout<<n.bs[n.db2][m];
								}
								else if(l==2){
									cout<<n.bs[n.db3][m];
								}
								else{
									cout<<n.bs[n.index[l]][m];
								}
							}
							cout<<endl;
							
						}
						else{
							cout<<"filename doesnt exists"<<endl;
						}
					}
					else if(cm=="write"){		//writing from the file
						string s,s1;
						f1>>s>>s1;
						cout<<"COMMAND: write "<<s<<" "<<s1<<" RESULT: ";
						if(fnode.find(s)!=fnode.end()){
							inode n=fnode[s];
							int ne,pr=n.size;
							bool flag=true;
							int sum=0;
							if(n.size==0){
								if(s1.size()/128>=0){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.db1=i;
											break;
										}
									}
									if(n.db1!=-1){
										n.bs[n.db1]=s1.substr(0,128);
										if(s1.size()/128==0){
											sum=s1.size();											
										}
										else{
											sum+=128;										
										}
									}
									else{
										flag=false;
									}
								}
								if(s1.size()/128>=1){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.db2=i;
											break;
										}
									}
									if(n.db2!=-1){
										n.bs[n.db2]=s1.substr(128,128);
										if(s1.size()/128==1){
											sum=s1.size();											
										}
										else{
											sum+=128;										
										}
									}
									else{
										flag=false;
									}
								}
								if(s1.size()/128>=2){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.db3=i;
											break;
										}
									}
									if(n.db3!=-1){
										n.bs[n.db3]=s1.substr(256,128);
										if(s1.size()/128==2){
											sum=s1.size();											
										}
										else{
											sum+=128;										
										}
									}
									else{
										flag=false;
									}
								}
								if(s1.size()/128>=3){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.ib=i;
											break;
										}
									}
									if(n.ib!=-1){
										map<int,int> ind;
										for(int i=3;i<=s1.size()/128;i++){
											for(int j=0;j<fv.size();j++){
												if(!fv[j]){
													fv[j]=true;
													ind[i]=j;
													break;
												}
											}
											if(ind.find(i)!=ind.end()){
												n.bs[ind[i]]=s1.substr(i*128,128);
												if(s1.size()/128==i){
													sum=s1.size();											
												}
												else{
													sum+=128;										
												}
											}
											else{
												flag=false;
											}
										}
										n.index=ind;
									}
									else{
										flag=false;
									}
								}
								//n.size=sum;
								if(n.size<n.write+sum){
									n.size=n.write+sum;
								}
							}
							ne=n.size;
							sum=0;
							if(ne<n.write+s1.size()){
								ne=n.write+s1.size();
							}
							if((ne/128)!=(pr/128)){
								if(ne/128>=1 && n.db2==-1){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.db2=i;
											break;
										}
									}
									if(n.db2!=-1){
										n.bs[n.db2]=s1.substr(128,128);
										if(ne/128==1){
											sum=s1.size();
										}
										else{
											sum+=128;
										}
									}
									else{
										flag=false;
									}
								}
								if(ne/128>=2 && n.db3==-1){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.db3=i;
											break;
										}
									}
									if(n.db3!=-1){
										n.bs[n.db3]=s1.substr(256,128);
										if(ne/128==2){
											sum=s1.size();
										}
										else{
											sum+=128;
										}
									}
									else{
										flag=false;
									}
								}
								if(ne/128>=3 && n.ib==-1){
									for(int i=0;i<fv.size();i++){
										if(!fv[i]){
											fv[i]=true;
											n.ib=i;
											break;
										}
									}
									map<int,int> ind;
									for(int i=3;i<=ne/128;i++){
										for(int j=0;j<fv.size();j++){
											if(!fv[j]){
												fv[j]=true;
												ind[i]=j;
												break;
											}
										}
										if(ind.find(i)!=ind.end()){
											n.bs[ind[i]]=s1.substr(i*128,128);
											if(ne/128==i){
												sum=s1.size();
											}
											else{
												sum+=128;
											}
										}
										else{
											flag=false;
										}	
									}
									n.index=ind;
								}
								else if(ne/128>=3 && n.ib!=-1){
									map<int,int> ind=n.index;
									for(int i=ind.size()+3;i<=ne/128;i++){
										for(int j=0;j<fv.size();j++){
											if(!fv[j]){
												fv[j]=true;
												ind[i]=j;
												break;
											}
										}
										if(ind.find(i)!=ind.end()){
											n.bs[ind[i]]=s1.substr(i*128,128);
											if(ne/128==i){
												sum=s1.size();
											}
											else{
												sum+=128;
											}
										}
										else{
											flag=false;
										}	
									}
									n.index=ind;
								}
							}
							if(n.size<n.write+sum){
								n.size=n.write+sum;
							}
							for(int i=n.write;i<n.write+sum;i++){
								n.det[i]=s1[i-n.write];
							}
							n.modify=time(NULL);
							fnode[s]=n;
							if(!flag){
								cout<<"The string is partially written to file as blocks are insuffiecient"<<endl;	
							}
							else{
								cout<<"The string is written to the file"<<endl;
							}
						}
						else{
							cout<<"filename doesnt exists"<<endl;
						}
					}
					else if(cm=="delete"){		//deleting a file
						string s;
						f1>>s;
						cout<<"COMMAND: delete "<<s<<" RESULT: ";
						if(fnode.find(s)!=fnode.end()){
							int i=0;
							string s1="",s2="";
							while(i!=s.size()){
								if(i!=0 && s[i]=='/'){
									s2=s1;
									s1="";
									i++;
								}
								s1+=s[i++];
							}
							if(s2==""){
								inode n=fnode[s];
								fv[n.nb]=0;
								if(n.db1!=-1){
									fv[n.db1]=0;
								}
								if(n.db2!=-1){
									fv[n.db2]=0;
								}
								if(n.db3!=-1){
									fv[n.db3]=0;
								}
								if(n.ib!=-1){
									fv[n.ib]=0;
									map<int,int> ind=n.index;
									for(auto i=ind.begin();i!=ind.end();i++){
										fv[i->second]=0;
									}
								}
								fnode.erase(fnode.find(s));
								cout<<"file is deleted"<<endl;	
							}
							else{
								inode n=fnode[s];
								fv[n.nb]=0;
								if(n.db1!=-1){
									fv[n.db1]=0;
								}
								if(n.db2!=-1){
									fv[n.db2]=0;
								}
								if(n.db3!=-1){
									fv[n.db3]=0;
								}
								if(n.ib!=-1){
									fv[n.ib]=0;
									map<int,int> ind=n.index;
									for(auto i=ind.begin();i!=ind.end();i++){
										fv[i->second]=0;
									}
								}
								fnode.erase(fnode.find(s));
								direc[s2].erase(find(direc[s2].begin(),direc[s2].end(),s));
								cout<<"file is deleted"<<endl;
							}
						}
						else{
							cout<<"filename doesnt exists"<<endl;
						}
					}
					else if(cm=="deletedir"){			//deleting a directory
						string s;
						f1>>s;
						cout<<"COMMAND: deletedir "<<s<<" RESULT: ";
						if(hdirec.find(s)!=hdirec.end()){
							if(direc[s].size()!=0){
								cout<<"Directory is not deleted as files are present in it"<<endl;
							}
							else{
								inode n = hdirec[s];
								fv[n.nb]=0;
								direc.erase(direc.find(s));
								hdirec.erase(hdirec.find(s));
								cout<<"Directory is delted"<<endl;
							}
						}
						else{
							cout<<"Directory name doesnt exists"<<endl;
						}
					}
					else if(cm=="ls"){					//printing the files and directories present
						string s;
						f1>>s;
						cout<<"COMMAND: ls "<<s<<" RESULT:"<<endl;
						if(s=="/"){
							cout<<"Directories:"<<endl;
							for(auto i=hdirec.begin();i!=hdirec.end();i++){
								cout<<i->first<<endl;
							}
							cout<<"Files:"<<endl;
							for(auto i=fnode.begin();i!=fnode.end();i++){
								inode n = i->second;
								cout<<i->first<<" "<<n.size<<endl;
							}
						}
						else{
							if(direc.find(s)!=direc.end()){
								cout<<"Files:"<<endl;
								for(int i=0;i!=direc[s].size();i++){
									inode n = fnode[direc[s][i]];
									cout<<direc[s][i]<<" "<<n.size<<endl;
								}
							}
							else{
								cout<<"Directory name doesnt exists"<<endl;
							}
						}
					}
				}
			}
			else{
				cout<<"File name is given wrong in system call"<<endl;
			}
		}
		else if(str=="printinode"){					//printing inode
			string fn;
			cin>>fn;
			if(fnode.find(fn)!=fnode.end()){
				inode n = fnode[fn];
				cout<<"Filename: "<<fn<<endl;
				cout<<"Size: "<<n.size<<endl;
				char* c=ctime(&n.create);
				char* c1=ctime(&n.modify);
				cout<<"Date Created: "<<c;
				cout<<"Date Modified: "<<c1;
				cout<<"Direct Block values: "<<n.db1<<" "<<n.db2<<" "<<n.db3<<endl;
				cout<<"Index Block: "<<n.ib<<endl;
				cout<<"Index block contents: ";
				for(auto i = n.index.begin();i!=n.index.end();i++){
					cout<<i->second<<" ";
				}
				cout<<endl;
			}
			else{
				cout<<"Filename is wrong"<<endl;
			}
			
		}
		else if(str=="exit"){					//writing all the info back to the disk file
			//freevector
			ofstream fa(df);
			fa<<d*8<<endl;
			for(int i=0;i<d*8;i++){
				fa<<fv[i]<<" ";
			}
			fa<<endl;
			//hdirec
			fa<<hdirec.size()<<endl;
			for(auto i=hdirec.begin();i!=hdirec.end();i++){
				fa<<i->first<<endl;
				inode n=i->second;
				fa<<n.name<<endl;
				fa<<n.df<<endl;
				fa<<n.size<<endl;
				fa<<n.create<<endl;
				fa<<n.modify<<endl;
				fa<<n.db1<<endl;
				fa<<n.db2<<endl;
				fa<<n.db3<<endl;
				fa<<n.nb<<endl;
				fa<<n.ib<<endl;
				fa<<n.read<<endl;
				fa<<n.write<<endl;
				for(int i=0;i<n.size;i++){
					fa<<n.det[i];
				}
				fa<<endl;
				fa<<n.index.size()<<endl;
				for(auto i =n.index.begin();i!=n.index.end();i++){
					fa<<i->first<<" "<<i->second<<endl;
				}
				fa<<n.bs.size()<<endl;
				for(auto i=n.bs.begin();i!=n.bs.end();i++){
					fa<<i->first<<" "<<i->second<<endl;
				}
			}
			//direc
			fa<<direc.size()<<endl;
			for(auto i=direc.begin();i!=direc.end();i++){
				vector<string> v = i->second;
				fa<<i->first<<" "<<v.size()<<endl;
				for(int i=0;i<v.size();i++){
					fa<<v[i]<<" ";	
				}
				fa<<endl;
			}
			//fnode
			fa<<fnode.size()<<endl;
			for(auto i=fnode.begin();i!=fnode.end();i++){
				fa<<i->first<<endl;
				inode n=i->second;
				fa<<n.name<<endl;
				fa<<n.df<<endl;
				fa<<n.size<<endl;
				fa<<n.create<<endl;
				fa<<n.modify<<endl;
				fa<<n.db1<<endl;
				fa<<n.db2<<endl;
				fa<<n.db3<<endl;
				fa<<n.nb<<endl;
				fa<<n.ib<<endl;
				fa<<n.read<<endl;
				fa<<n.write<<endl;
				for(int i=0;i<n.size;i++){
					fa<<n.det[i];
				}
				fa<<endl;
				fa<<n.index.size()<<endl;
				for(auto i =n.index.begin();i!=n.index.end();i++){
					fa<<i->first<<" "<<i->second<<endl;
				}
				fa<<n.bs.size()<<endl;
				for(auto i=n.bs.begin();i!=n.bs.end();i++){
					fa<<i->first<<" "<<i->second<<endl;
				}
			}
			cout<<"exiting the shell"<<endl;
			break;
		}
		cout<<"command>>";
	}
}
