#include <iostream>
#include <vector>
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <sys/time.h>
#include <unistd.h>
//using namespace std;
# define m 397// nombre de clauses
#define  n 87//nombre de variables 
# define k 10 //nombre d'abeilles
# define IMAX 3// maximum d'iteration 
int nb_regle=0;
float Minconf=0.8;
int instance[m][n]; // instance SAT qui contient m clauses et n vairables 
int clusters[m]; // contient les cluster des m clauses 
int represents[n][m];// mettre les représentative de chaque cluster 
int dance[k][n]; // la table dance de BSO 
int Sref[n]; // la solution référence 
int neighbor[n]; // le voisin d'une solution
//int regles[(n*k*IMAX)][n]; // l'ensemble de régles pertinentes 
int regles[1000][n]; // l'ensemble de régles pertinentes 
void creer_instance(); // créer l'instance SAT  de m clauses et n variables
void creer_Sref(); // creer la solution référence   
void search_area(); // search area 
void neighborhoods(); // calcul de voisinage
float confiance(int s[]); // calcule la confiance d'une solution
int best_dance(); // best dance  
void ARM_clustering(); //appliquer clusteing using ARM process
void display_clusters(); // afficher les clusters 
void display_solution(int s[]); // afficher la solution s
void insertion(int s[]); // inserer s dans la table des régles
void display_regles(); // afficher les régles
void clustering();//groupement des clusters
int MADP();// Minimize average distance between points in cluster
int table_nbr[m];// contains the number of clauses on each cluster
int clusters_ord[m][2]; // clusters ordonée selon le nombre de clusters
void creer_instance_new(); // créer l'instance SAT  de m clauses et n variables
void nbr_clauses();
void reorganiser();
/**************variables and methods for SAT solver************/
#define a 2 // number of bees
float seuil=0.7;
int cost_globale=0;
int idx=0; // current index in tables of clusters
int dance2[a][n]; // dance table of BSO 
int cost[a]; // cost of dance table 
int Sref2[n]; // reference solution of BSO
void create_clusters();
void search_area1(int s[], int flip, int taille);
void ng(int s[], int idx, int taille);
void neighborhood(int indice, int taille);
int fitness(int s[], int idx, int taille);
int find_max_clusters();
int best_dance2();
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
     //creer_instance();
      creer_instance_new();
     //creer_Sref();
     //display_solution(Sref);
     //printf("\n%f", confiance(Sref));
     clock_t t1,t2;
     t1=clock();
     ARM_clustering();
     //printf("%d\n", nb_regle);
     clustering();
     t2=clock();
     float diff =((float)t2-(float)t1);
     float seconds = diff / CLOCKS_PER_SEC;
     //printf ("\n execution time: %f", seconds); 
     //MADP();
     //printf("\n MADP:%d", MADP());
    // display_clusters();
     //display_regles();
     //printf("\n%d", nb_regle);
     //display_clusters();
           nbr_clauses();
      reorganiser();
      //BSO_packet();
      BSO_entire();
      //Local_Search();
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
       //printf("\n");
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
	   int t=5;
		 for (int l=0;l<t;l++) 
		 {
	         int r=rand()%n;
	         instance[i][r]=(rand()%2)+1;
		 }
	   }
}
/**********creer une solution de référence*********/
void creer_Sref()
{
for (int i=0;i<n; i++)
{
Sref[i]=rand()%2+1;    
}
}
/*********determination de search area*************************/
void search_area()
{
   for (int i=0; i<k;i++)
     {
       for (int j=0; j<n;j++)
          {
           if (Sref[j]==0){dance[i][j]=0;}
             else {
                  if (i==j){
                       dance[i][j]=0;
                       }
                   else {
                        dance[i][j]=Sref[j];
                        }
                  }
           }    
     }     
  
}
/**************calcul de voisinage*******/
void neighborhoods()
{
  for (int i=0; i<k;i++)
     {
           // verfification de l'abeille
       for (int j=0; j<n;j++)
        {
            for (int s=0; s<n;s++)
             {
              neighbor[s]=dance[i][s];    
             }
            //if (i==j){ 
                     if (dance[i][j]==1)neighbor[j]=2; 
                     else  {if( dance[i][j]==2) neighbor[j]=1;}
                     //}
             if (confiance(neighbor)>=Minconf) { 
                                          //display_solution(neighbor);
                                          //printf ("  :%f\n",confiance(neighbor));
                                            insertion(neighbor);
                                             //  nb_regle++;
                                          }
            if (confiance(neighbor)>=confiance(dance[i])){
                                                           for (int h=0; h<n;h++)
                                                           {
                                                          dance[i][h]=neighbor[h];
                                                          }
                                                          }
        // verification de neighbor 
        //display_solution(neighbor);
        //printf ("\n%f",confiance(neighbor));
        }       
             
     }
     
}
/**********************calcul de confidence**********/
float confiance(int s[])
{
      float a1=0,b=0;
      for (int i=0; i<m;i++)
      {
       bool existe1=true, existe2=true;
        for (int j=0; j<n;j++)
        {
         if (s[j]==1 && instance[i][j]==0)
           {
            existe1=false;          
           } 
         if ((s[j]==1 || s[j]==2) && instance[i][j]==0)
           {
            existe2=false;          
           } 
        }    
        if (existe1!=false){a1++;}
        if (existe2!=false){b++;}
      }
      //printf ("\na=%f, b=%f", a, b);
      return (b/a1);
}
/**********appliquer clusteing using ARM process*****/
void ARM_clustering()
{
 creer_Sref();
 //display_solution(Sref);
 //int nb=0;
 int iter=0;
 while (iter  < IMAX )//&& nb <(n-1))
   {
       iter++;
       //nb=0;
   //for (int i=0;i < k;i++)
   //{
   search_area();
   neighborhoods();
  //}   
  //int x=rand()%k;
  int x=best_dance();
  for (int j=0;j < n;j++)
     {
     Sref[j]=dance[x][j];
     //if (Sref[j]==0) {nb++;}
     }
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
/***********************afficher solution*********/
void display_solution(int s[])
{
     printf("\n");
for (int i=0;i < n; i++)
  {
  printf("%d",s[i]);       
  }  
}
/***************best dance***********/
int best_dance()
{
    float max=confiance(dance[0]);
    int i_max=0;
for (int i=0; i < k; i++)
{
    float x=confiance(dance[i]);
     if (max <x)
       {
       max= x;
       i_max=i;
       }
}
return i_max;
}
/*************inserer s dans la table des régles*******************/
void insertion(int s[])
 {  bool existe=false;
  int i=0;
  while (i < nb_regle && existe==false)
  {
        int j=0;
        existe=true;
  while (j< n && existe==true)
  {
        if (s[j]!=regles[i][j]){existe=false;}
        j++;
  }
  i++;
  }
  if (existe==false)
  {
   for (int h=0; h<n;h++)
    {
    regles[nb_regle][h]=neighbor[h];
    }
    nb_regle++;
 }
}
/**************afficher la table des régles*****/
void display_regles()
{
     printf("\n");
for (int i=0;i < nb_regle; i++)
  {
  display_solution(regles[i]);  
  }  
}
/***************groupement des clusters*******************/
void clustering()
{

   for (int j=1;j<m;j++)
    {
        int max_v=0;
        int max_i=0;
       for (int l=0; l<nb_regle;l++)
          {
            int v=0;
            for (int s=0;s<n;s++) 
             {
              if (instance[j][s]==regles[l][s])
               {
                v++;                                   
               }        
             }
             if (v>max_v)
             {
             max_v=v;
             max_i=l;
             }
          }
               clusters[j]=max_i;
               
    
    }
}
/****************************************************************/
int MADP()// Minimize average distance between points in cluster
{
    int Distance[nb_regle];
    for (int d=0;d<nb_regle;d++)
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
                   if (instance[j][s]==instance[i][s])
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
     for (int d=1;d<nb_regle;d++)
     {
         if (max < Distance[d]) max=Distance[d];
     }
return (max/m);
	/*float measure=0;

    for (int i=0;i<nb_regle;i++)
       {
    	float sum=0;
	    for (int j=0;j<m;j++)
	    {
	     if (clusters[j]==i)
	     {	 
	       for (int l=0;l<n;l++)
	        {
	    	 if (instance[j][l]==)
	    	 {
	    	  float d=Manhattan(T_objet[j],T_objet[l]);
	    	  sum=sum+d;
	         }
	       }
        }
      }
	    if (sum>measure){measure=sum;}
     }   
return measure;*/
}
/**********Methods for SAT solver********/
/*******************************/
void create_sref()
{
	 for (int i=0; i < n; i++)
	   {
	       Sref2[i]=rand()%2;
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
	    Sref2[j]=best_neighbor[j];
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
	    dance2[indice][j]=best_neighbor[j];
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
int best_dance2()
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
	
int IMAX2=5;
int flip=2;
create_sref();
int indice=find_max_clusters();
int taille=table_nbr[indice];
int cost_sref=fitness(Sref2,0,taille);
//display_solution(Sref);
search_area1(Sref2, flip, taille);
int iter=0;
  while(iter<IMAX)
  {
   	for (int i=0;i<a;i++)
   	{
   	 neighborhood(i, taille);
   	}
   	int indice=best_dance2();
   	if (cost_sref<cost[indice])
   	{
   		
   		for (int l=0;l<n; l++)
   	   	{
   	   	 Sref[l]=dance2[indice][l];
   	   	}
   		cost_sref=cost[indice];
   	}
   	iter++;
   	search_area1(Sref2, flip, taille);
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
int cost=fitness(Sref2,idx,taille);
double x=cost/taille;
if (x<seuil)
{
ng(Sref2, idx, taille);
cost=fitness(Sref2,idx,taille);
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
	    printf("%d ",dance2[i][j]);	    
	   }		
	printf("\n");
	   }
}
/***********************/
void display_solution2(int s[])
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
int IMAX2=5;
int flip=2;
create_sref();
int cost_sref=fitness(Sref2,0,m);
//display_solution(Sref);
search_area1(Sref2, flip, m);
int iter=0;
  while(iter<IMAX)
  {
   	for (int i=0;i<a;i++)
   	{
   	 neighborhood(i,m);
   	}
   	int indice=best_dance2();
   	if (cost_sref<cost[indice])
   	{
   		
   		for (int l=0;l<n; l++)
   	   	{
   	   	 Sref[l]=dance2[indice][l];
   	   	}
   		cost_sref=cost[indice];
   	}
   	iter++;
   	search_area1(Sref2, flip, m);
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
for (int i=1;i<nb_regle;i++)
{
 if (max<table_nbr[i])
 {
 max=table_nbr[i];
 indice=i;                        
 }    
}    
return indice;
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
int table2_nbr[m];
for (int l=0;l<nb_regle;l++)
{table2_nbr[l]=table_nbr[l];}

      while(cpt<m)
      {
        int indice=0;
        int max_i=table2_nbr[indice];
        for (int i=1;i<nb_regle;i++)
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
/***************************/
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
