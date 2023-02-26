#include<iostream>
#include<pthread.h>
#include<stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include <vector>
#include <string>
// #include "boost/tuple/tuple.hpp"
#include <tuple>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>

using namespace std;
// using boost::tuple;

typedef vector< tuple<string,string> > tuple_list;

int wh, we, ah, ae, NoH, NoE;
int in_boat=0;
int waiting_lions=0;
int waiting_rats=0;
int lions=0;
int rats=0;
pthread_cond_t cv_lion = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_rat = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock1 = PTHREAD_MUTEX_INITIALIZER;

void BoardBoat(int p,int t_id)
{
  if(p==0){
    cout<<"A lion has boarded the boat\n";
  }
  else{
    cout<<"A rat has boarded the boat\n";
  }

}

void RowBoat()
{
  cout<<"One boat left the shore with 4 animals\n";
}


void *LionArrives(void *in){
  pthread_mutex_lock(&lock1);
  int *id = (int *)in;
  int p=0;
  //cout<<"lion\n";

  lions++;
  if(lions ==1)
  { //cout<<"lion=1\n";
    pthread_cond_wait(&cv_lion,&lock1);
    //waiting_lions++;
  }
  else if(lions==2)
  { //cout<<"lion=2\n";
    for(int i=0;i<lions;i++){
      pthread_cond_signal(&cv_lion);
      //waiting_lions--;
      BoardBoat(p,*id);
      in_boat++;
    }
    lions=0;
  }
  //cout<<"inboat"<<in_boat<<"\n";
  if(in_boat==4){
    cout<<"boat full\n";
    in_boat=0;
    RowBoat();
  }

  //return 0;
  pthread_mutex_unlock(&lock1);
  return 0;
}

void *RatArrives(void *in){
  pthread_mutex_lock(&lock1);
  int *id = (int *)in;
  int p=1;
  rats++;
  if(rats ==1)
  { //cout<<"rat=1\n";
    pthread_cond_wait(&cv_rat,&lock1);
    //waiting_rats++;
  }
  else if(rats==2)
  { //cout<<"rat=2\n";
    for(int i=0;i<rats;i++)
    {
      pthread_cond_signal(&cv_rat);
      //waiting_rats--;
      BoardBoat(p,*id);
      in_boat++;
    }
    rats=0;
  }
  //cout<<"inboat"<<in_boat<<"\n";
  if(in_boat==4){
    cout<<"boat full\n";
    in_boat=0;
    RowBoat();
  }
  pthread_mutex_unlock(&lock1);
  return 0;
}

void tokenize(std::string const &str, const char delim, vector<string> &out)
{
    // construct a stream from the string
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}
 

int main(int argc, char *argv[]) // ['./a.out','No_of_Lions','No_of_Rats']
{
  int threads;
  int p_id[40];
  pthread_t t_id[40];


  const char delim = ',';
  string delay;
  string ani;
  tuple_list tl;
  //cout<<argc;

  int animals=abs(atoi(argv[1]));
  //cout<<animals<<"\n";

  //cout<<argv[2]<<"\n";
  
  cout<<"\n-----------------------------------------------\n";
  cout<<"\t\t  *** START ***\n";
  cout<<"-----------------------------------------------\n";

  for(int i=2;i<animals+2;i++)
  {
    //cout<<argv[i]<<"\n";
    vector<string> out;
    tokenize(argv[i],delim,out);
    //cout << "Type of x : " << typeid(x).name() << endl;
    delay=out[0];
    ani=out[1];
    //cout<<delay<<"\n";
    //cout<<ani<<"\n";
    tl.push_back( tuple<string, string>(delay,ani) );
  }

  // for(int i=0;i<tl.size();i++) {
  //       cout <<  get<0>(tl[i]) << " " << get<1>(tl[i]) <<endl;
  //   }

  int counter=0;
  int i=0,j=0;
  while(counter!=animals){
    printf("time:%d\n",i);
    for(j=0;j<tl.size();j++) {
        if (i==stoi(get<0>(tl[j]))){
          //printf("%d\n",i);
          counter++;
          if(get<1>(tl[j])=="l")
          { printf("creating thread for lion\n");
            pthread_create(&t_id[j], NULL, LionArrives, (void *)&p_id[i]);
          }else{
            printf("creating thread for rat\n");
            pthread_create(&t_id[j], NULL, RatArrives, (void *)&p_id[i]);
          }
        }
    }
    i++;
  }
  // printf("%d",j);
  for(int k=0;k<j;k++)
  {
    pthread_join(t_id[k], NULL);
  }
  
  cout<<"-----------------------------------------------\n";
  cout<<"\t\t   *** END ***\n";
  cout<<"-----------------------------------------------\n";

  return 0;
}