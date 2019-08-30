#include <iostream>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <unistd.h>
//using namespace std;
# define m 10000// number of constraints
#define  n 50// number of variables 
#define k 10 // le nombre de clusters
int nb=1;
int instance[m][n]; // list of Constraints
int clusters[m]; // list of clusters 
int clusters_ord[m][2]; // clusters sorted according to the number of clusters
typedef struct bbb{
int centre[n];
}center;
typedef struct aaa{
	center obj;
    int cluster;
}objet;
objet centers[k];
int table_nbr[k];// contains the number of clauses on each cluster
void creer_instance(); // créer l'instance SAT  de m clauses et n variables  
void creer_instance_new(); // créer l'instance SAT  de m clauses et n variables 
void display_clusters(); // afficher les clusters 
int MADP();// Minimize average distance between points in cluster
int distance(int t[], int v[]); // calculate distance between clauses
void initialiser_centers();
void FC(int cluster);
void assigned_clusters();
void Kmeans_function();
void nbr_clauses();
void reorganiser();

/**************variables and methods for SAT solver************/
#define a 2 // number of bees
float seuil=0.7;
int cost_globale=0;
int idx=0; // current index in tables of clusters
int dance[a][n]; // dance table of BSO 
int cost[a]; // cost of dance table 
int Sref[n]; // reference solution of BSO
void create_clusters();
void search_area1(int s[], int flip, int taille);
void ng(int s[], int idx, int taille);
void neighborhood(int indice, int taille);
int fitness(int s[], int idx, int taille);
int find_max_clusters();
int best_dance();
void BSO_packet();
void BSO_entire();
void display_dance();
void display_solution(int s[]);
void display_instance();
void Local_Search();
/***************************/
/*************************le programme principale**********/
int  main()
{
     creer_instance_new();
     clock_t t1,t2;
     t1=clock();
     // clustering methods
     Kmeans_function();
          t2=clock();
     float diff =((float)t2-(float)t1);
     float seconds = diff / CLOCKS_PER_SEC;
     //printf ("\n execution time: %f", seconds); 
      //display_clusters();
      nbr_clauses();
      reorganiser();
      BSO_packet();
      //BSO_entire();
     //Local_Search();
     //printf("\n MADP:%d", MADP());
int x; 
scanf("%d", &x);
return 0;
}
/**************creer l'instance SAT**********/ 
void creer_instance()
{
   for (int i=0; i < m; i++)
   {
        for (int j=0; j < n; j++)
        {
         instance[i][j]=rand()%3;
         //printf("%d  ", instance[i][j]);     
        }
       // printf("\n");
  }
}
void creer_instance_new()
{
	for (int i=0; i < m; i++)
	   {  
		 for (int j=0; j < n; j++)
	        {
	      instance[i][j]=0;
	        }
	   int t=(n/10);
		 for (int l=0;l<t;l++) 
		 {
	         int r=rand()%n;
	         instance[i][r]=(rand()%2)+1;
		 }
	   }
}
/**********************calculate distance between tow clauses***********************/
int distance(int t[], int v[])
{
int nvc=0;
  for (int i=0;i<n; i++)
   {
    if ((t[i]==v[i]) || (t[i]== 1 && v[i]==2) || (t[i]==2 && v[i]==1)) 
      {
      nvc++;
      }
   }
   return (n-nvc);
}
/****************calculate center computation of clauses*******/
void FC(int cluster)
{
	int nbr=0;
	int s=0;
	int card_center=0;
	objet obj_temp;
	for (int j=0;j<n;j++)
    {
		obj_temp.obj.centre[j]=0;
		centers[cluster].obj.centre[j]=0;
    }
  for (int i=0;i<m; i++)
{
	
    if (clusters[i]==cluster)
    {
    	for (int j=0;j<n;j++)
        {
    		if (instance[i][j]==1 || instance[i][j]==2)
    		{
    			s++;
    			obj_temp.obj.centre[j]++;
    		}
        }
    	nbr++;
    }
}
  if (nbr!=0)
  {
   card_center=s/nbr;
  }
  
for (int i=0;i<card_center;i++)
{	
	int max=obj_temp.obj.centre[0];
    int indice_max=0;
	  for (int j=1;j<n;j++)	
	  {
		  if (max<obj_temp.obj.centre[j])
		  {
			  max=obj_temp.obj.centre[j];
			  indice_max=j;
		  }
		  
	  }	  
   centers[cluster].obj.centre[indice_max]=1;
   obj_temp.obj.centre[indice_max]=0;
}
}
/***************initaliser_ centers**********************/
void initialiser_centers()
{
for (int i=0;i<k;i++)
  {
      for (int j=0;j<n;j++)
      {
	   centers[i].obj.centre[j]=instance[i][j];
      }      
   clusters[i]=i;
  }
}
/*****************************affectation des clusters************/
void assigned_clusters()
{
for (int i=0;i<m;i++)
{
    //float min=Manhattan(T_objet[i], centers[0]);
	//float min=Chebychev(T_objet[i], centers[0]);
//	printf("%f ",min);
    //float min=Euclidian(T_objet[i], centers[0]);
	//float min=Euclidian_squared(T_objet[i], centers[0]);
	//float min=Dist_aicha(T_objet[i], centers[0]);
	float min=distance(instance[i], centers[0].obj.centre);
    int indice=0;
	for (int j=1;j<k;j++)
    {
	//float d=Manhattan(T_objet[i], centers[j]);
	//float d=Chebychev(T_objet[i], centers[j]);
	//printf("%f ",d);
	//float d=Euclidian(T_objet[i], centers[j]);
	//float d=Euclidian_squared(T_objet[i], centers[j]);
	//float d=Dist_aicha(T_objet[i], centers[j]);
	float d=distance(instance[i], centers[j].obj.centre);
		if (d<min){indice=j;}
    }
    //printf("\n");
clusters[i]=indice;
}
}
/**********************************Main algorithm of kmeans***********/
void Kmeans_function()
{
int iteration=0;
//download_ruleset();
//gen_rules();
initialiser_centers();
assigned_clusters();
while (iteration<2)
{
for (int i=0;i<k;i++)
{
  //classical_center_computation(i);
 FC(i);
 //Uoi(i);
  //Iuc(i);
 //Uic(i);
}
assigned_clusters();
iteration++;
//printf("\n\t%d",iteration);
}
}
/****************afficher clustering********/
void display_clusters()
{
for (int i=0;i <m; i++)
{
printf("%d\n", clusters[i]);    
}
}
/***********MADP evaluation**************************/
int MADP()// Minimize average distance between points in cluster
{
    int Distance[nb];
    for (int d=0;d<nb;d++)
     {
         Distance[d]=0;
     }
      
     for (int i=0; i<m;i++)
       {
         for (int j=i+1; j<m;j++)
             {
              if (clusters[j]==clusters[i])
                 {
                // printf ("\n%d %d", i,j);
                 int v=0;
                 for (int s=0;s<n;s++) 
                  {
                   if ((instance[j][s]==instance[i][s]))
                   {
                    v++;                                   
                   }        
                  }
                  int indice=clusters[j];                             
           Distance[indice]=Distance[indice]+n-v;                          
                                           
                 }
             } 
              
       }
      int max=Distance[0];
     for (int d=1;d<nb;d++)
     {
         if (max < Distance[d]) max=Distance[d];
     }
return (max/m);
}
/***************************/
void nbr_clauses()
{
  for (int i=0;i<k;i++)     
   {
   table_nbr[i]=0;        
   }
  for (int i=0;i<m;i++)
   {
   int j=clusters[i];
   table_nbr[j]++;        
   }
   /*for (int i=0;i<k;i++)
   {
   printf("\n%d",table_nbr[i]);        
   }*/
}
/***************/
void reorganiser()
{
int cpt= 0;
int table2_nbr[k];
for (int l=0;l<k;l++)
{table2_nbr[l]=table_nbr[l];}

      while(cpt<m)
      {
        int indice=0;
        int max_i=table2_nbr[indice];
        for (int i=1;i<k;i++)
         {
          if (max_i<table2_nbr[i]) 
           {
            max_i=table2_nbr[i]; 
            indice=i;                                                    
           }       
         }
         table2_nbr[indice]=0;
        for (int i=0;i<m;i++)
         {
          if (clusters[i]==indice) 
           {
           clusters_ord[cpt][0]=i;
           clusters_ord[cpt][1]=indice; 
           cpt++;
           }    
         }
      } 
 /*     printf("\n**************organiser**********\n");
      for (int j=0;j<m;j++)
      {
        printf("%d  %d\n", clusters_ord[j][0], clusters_ord[j][1]);    
      }     
      */
}
/**********Methods for SAT solver********/
/*******************************/
void create_sref()
{
	 for (int i=0; i < n; i++)
	   {
	       Sref[i]=rand()%2;
	   }	
}
/*********************************/
void search_area1(int s[], int flip, int taille)
{
    int i=0;
	   while (i<a)
	   {
		  
		   for (int j=0;j<n;j++)
		   {   
		    dance[i][j]=s[j];	    
		   }
		   for (int indice=i;indice<n;indice=indice+flip)
		  {
			 	
          if (dance[i][indice]==0) 
          {
        	dance[i][indice]=1;   
          }
		
          else{
		  dance[i][indice]=0;		 
              }
	   }
     cost[i]=fitness(dance[i],0,taille);
	 //T_Dance[i].cost=fitness1(T_Dance[i].solution);//evaluer solution  
  i++;
	   }
}
/********************/
void ng(int s[], int idx, int taille)
{

	int neighbor[n];
	int best_neighbor[n];
	for (int j=0;j<n;j++)
	   {   
	    best_neighbor[j]=s[j];	    
	   }
	int cost_best=fitness(best_neighbor,idx,taille);
for(int i=0;i<n;i++)
{

	   for (int j=0;j<n;j++)
	   {   
	    neighbor[j]=s[j];	    
	   }
	   if (neighbor[i]==1){neighbor[i]=0;}
	   else {neighbor[i]=1;}
	   //display_solution(neighbor);
	   int cost_neighbor=fitness(neighbor,idx,taille);
	   if (cost_neighbor > cost_best)
	   {
		   for (int j=0;j<n;j++)
		   {   
		    best_neighbor[j]=neighbor[j];	    
		   }  
		   cost_best=cost_neighbor;
	   }
}

	  for (int j=0;j<n;j++)
	   { 
	    Sref[j]=best_neighbor[j];
	   }
}
/*************************/
void neighborhood(int indice, int taille)
{
	int neighbor[n];
	int best_neighbor[n];
	for (int j=0;j<n;j++)
	   {   
	    best_neighbor[j]=dance[indice][j];	    
	   }
	int cost_best=fitness(best_neighbor,0,taille);
for(int i=0;i<n;i++)
{

	   for (int j=0;j<n;j++)
	   {   
	    neighbor[j]=dance[indice][j];	    
	   }
	   if (neighbor[i]==1){neighbor[i]=0;}
	   else {neighbor[i]=1;}
	   //display_solution(neighbor);
	   int cost_neighbor=fitness(neighbor,0,taille);
	   if (cost_neighbor > cost_best)
	   {
		   for (int j=0;j<n;j++)
		   {   
		    best_neighbor[j]=neighbor[j];	    
		   }  
		   cost_best=cost_neighbor;
	   }
}

	cost[indice]=cost_best;
	  for (int j=0;j<n;j++)
	   { 
	    dance[indice][j]=best_neighbor[j];
	   }
}
/***************/
int fitness(int s[], int idx, int taille)
{
int nb=0;
int i=idx;
 while(i<idx+taille && i<m)
 {
	int j=clusters_ord[i][0];
	bool satisfied=false;
	int l=0;
	  while((satisfied==false) && (l<n))
		 {
		  if(((s[l]==0) && (instance[j][l]==2))||((s[l]==1) && (instance[j][l]==1)))
		  {satisfied=true;}
		  l++;
		 }
	  if (satisfied==true) {nb++;}
  i++;	  
 }
 //System.out.println(nb);
return nb;	
}
/*********************/
int best_dance()
{
	float max=cost[0];
	int indice=0;
	for (int i=1;i<a;i++)	
	{
     	if (cost[i]>max)
	     {     
           max=cost[i];
		   indice=i;
         }
	}
return indice;
}
/******************/
void BSO_packet()
{
	
int IMAX=5;
int flip=2;
create_sref();
int indice=find_max_clusters();
int taille=table_nbr[indice];
int cost_sref=fitness(Sref,0,taille);
//display_solution(Sref);
search_area1(Sref, flip, taille);
int iter=0;
  while(iter<IMAX)
  {
   	for (int i=0;i<a;i++)
   	{
   	 neighborhood(i, taille);
   	}
   	int indice=best_dance();
   	if (cost_sref<cost[indice])
   	{
   		
   		for (int l=0;l<n; l++)
   	   	{
   	   	 Sref[l]=dance[indice][l];
   	   	}
   		cost_sref=cost[indice];
   	}
   	iter++;
   	search_area1(Sref, flip, taille);
  }
 cost_globale=cost_sref;
 table_nbr[indice]=0;
idx=idx+taille;
 /***********handle the others packets******************/
do
{
int indice=find_max_clusters();
int taille=table_nbr[indice];
 table_nbr[indice]=0;
if(idx<m)
{
int cost=fitness(Sref,idx,taille);
double x=cost/taille;
if (x<seuil)
{
ng(Sref, idx, taille);
cost=fitness(Sref,idx,taille);
}
cost_globale=cost_globale+cost;
}
idx=idx+taille;

}
while(idx<m);
printf("\n%d", cost_globale);
float xx=cost_globale;
float taux=(float)(xx/m)*100;
printf("\ntaux:%f",taux);
}
/**************/
void display_dance()
{
  for (int i=0;i<a;i++)
	   {
	for (int j=0;j<n;j++)
	   {   
	    printf("%d ",dance[i][j]);	    
	   }		
	printf("\n");
	   }
}
/***********************/
void display_solution(int s[])
{
	for (int i=0;i<n;i++)
	   {   
	    printf("%d ",s[i]);	    
	   }		
	printf("\n");
}
/*****************************/
void BSO_entire()
{	
int IMAX=5;
int flip=2;
create_sref();
int cost_sref=fitness(Sref,0,m);
//display_solution(Sref);
search_area1(Sref, flip, m);
int iter=0;
  while(iter<IMAX)
  {
   	for (int i=0;i<a;i++)
   	{
   	 neighborhood(i,m);
   	}
   	int indice=best_dance();
   	if (cost_sref<cost[indice])
   	{
   		
   		for (int l=0;l<n; l++)
   	   	{
   	   	 Sref[l]=dance[indice][l];
   	   	}
   		cost_sref=cost[indice];
   	}
   	iter++;
   	search_area1(Sref, flip, m);
  }
 cost_globale=cost_sref;
printf("\n%d", cost_globale);
float xx=cost_globale;
float taux=(float)(xx/m)*100;
printf("\ntaux:%f",taux);
}
/************************************/
int find_max_clusters()
{
int indice=0;
int max=table_nbr[0];
for (int i=1;i<k;i++)
{
 if (max<table_nbr[i])
 {
 max=table_nbr[i];
 indice=i;                        
 }    
}    
return indice;
}
/***********************************/
void Local_Search()
{
int tab_sol[m][n];
int i=0;
do
{
int indice=find_max_clusters();
int taille=table_nbr[indice];
table_nbr[indice]=0;
if(idx<m)
{
create_sref();
ng(Sref, idx, taille);
for (int j=0;j<n;j++)
{
tab_sol[i][j]=Sref[j];
}
i++;
}
idx=idx+taille;
}
while(idx<m);
int cost_sol[m];
for (int ind=0; ind<i; ind++)
{
cost_sol[ind]=fitness(tab_sol[ind],0,m);    
}
cost_globale=cost_sol[0];
for (int ind=0;ind<i; ind++)
{
if (cost_globale<cost_sol[ind])
{
cost_globale=cost_sol[ind];     
}    
}

printf("\n%d", cost_globale);
float xx=cost_globale;
float taux=(float)(xx/m)*100;
printf("\ntaux:%f",taux);    
}
