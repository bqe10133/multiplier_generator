//------------------------------------------------------------------------------
// version 1.1 July 24,2023
//------------------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <math.h>
using namespace std;
void treecalc(int *din,int *dinnum,int dinlen,int afternum,int x,int z,int carrynum,int *carryin);
void treesort(int *din,int *dinnum,int dinlen,int reverse);

//---------------------------------
//function : main
//---------------------------------
int main(int argc, char * const argv[]) {
  int  mainbuf[2048];//x data
  int  mainbuf2[2048];//carry info
  char c0;
  int  d0;
  int  i,m;
  int  afternum;
  int  din[2048];
  int  dinnum[2048];
  int  dinlen;
  int  dinvalue;
  int  dinflag;
  int  carryvalue;
  int  carryflag;
  int  x;
  int  z;
  int  carrynum;
  int  carryin[2048];
  
  ifstream fv0(argv[1]);//x data
  afternum = atoi(argv[2]);
  x        = atoi(argv[3]);
  z        = atoi(argv[4]);
  ifstream fv1(argv[5]);//carry info
  
  //x data
  for(i=0;i<2048;i++){
    mainbuf[i]=0;
  }
  for(i=0;i<2048;i++){
    fv0.get(c0);  
    d0=(int)c0;
    mainbuf[i]=d0;
    if(d0==90){break;}
  }
  
  //carry info
  for(i=0;i<2048;i++){
    mainbuf2[i]=0;
  }
  for(i=0;i<2048;i++){
    fv1.get(c0);  
    d0=(int)c0;
    mainbuf2[i]=d0;
    if(d0==90){break;}
  }

  //-----------------------------
  // x data store 
  //-----------------------------
  m=0;
  dinvalue=0;
  dinflag=0;
  for(i=0;i<2048;i++){//enough big
    if(mainbuf[i]==90){//"Z" = 90
      break;
    }
    else if(mainbuf[i]==32 | mainbuf[i]==10){//" " = 32 or "CR"=10
      if(dinflag==1){
        if(m%2==0){
          din[m/2]=dinvalue;
        }
        else{
          dinnum[m/2]=dinvalue;
        }
        m++;
        dinvalue=0;
        dinflag=0;
      }
    }
    else{
      dinvalue=dinvalue*10;
      dinvalue=dinvalue+(mainbuf[i]-48);//"0" = 48
      dinflag=1;
    }
  }
  dinlen=m/2;

  //-----------------------------
  // carry info store 
  //-----------------------------
  m=0;
  carryvalue=0;
  carryflag=0;
  for(i=0;i<2048;i++){//enough big
    if(mainbuf2[i]==90){//"Z" = 90
      break;
    }
    else if(mainbuf2[i]==32 | mainbuf2[i]==10){//" " = 32 or "CR"=10
      if(carryflag==1){
        carryin[m]=carryvalue;
        m++;
        carryvalue=0;
        carryflag=0;
      }
    }
    else{
      carryvalue=carryvalue*10;
      carryvalue=carryvalue+(mainbuf2[i]-48);//"0" = 48
      carryflag=1;
    }
  }
  carrynum=m;

  treecalc(din,dinnum,dinlen,afternum,x,z,carrynum,carryin);

  fv0.close();
  fv1.close();
  return 0;
}

//---------------------------------
//function : treecalc
//---------------------------------
void treecalc(int *din,int *dinnum,int dinlen,int afternum,int x,int z,int carrynum,int *carryin){
  int i,j,k,m,n;
  int flag;
  int first_val,second_val,third_val;
  int first_valnum,second_valnum,third_valnum;
  int half_carry,half_sum;
  int carrybuf[2048];
  int sumbuf[2048];
  int carrybufnum[2048];
  int sumbufnum[2048];
  int fanum,hanum,wirenum;
  int first_carry,first_sum;
  int second_carry,second_sum;
  int afternum2;
  int din2[2048];
  int dinnum2[2048];
  int dinlen2;
  int numflag[2048];
  int y_s;
  

  for(i=0;i<afternum;i++){
    numflag[i]=0;
  }

  for(i=0;i<carrynum;i++){
    numflag[carryin[i]]=1;
  }

  afternum2=afternum-carrynum;
  
  //-----------------------------
  // fanum,hanum,wirenum calculation
  //-----------------------------
  if(dinlen <= afternum2){
    fanum  = 0;
    hanum  = 0;
    wirenum = dinlen;
  }
  else{
    fanum  = (dinlen-afternum2)/2;
    hanum  = (dinlen-afternum2)%2;
    wirenum = dinlen-3*fanum-2*hanum;
  }
  printf("-----------------------------\n");
  printf("dinlen:%d fanum:%d hanum:%d wirenum:%d\n",dinlen,fanum,hanum,wirenum);

  //-----------------------------
  // din modification
  //-----------------------------
  printf("=============================\n");
  printf("din modification\n");
  printf("=============================\n");

  printf("----initial\n");

  for(i=0;i<dinlen;i++){
    printf("%d ",din[i]);
  }
  printf("\n");
  
  treesort(din,dinnum,dinlen,0);
  printf("----sorted\n");

  printf("din\n");
  for(i=0;i<dinlen;i++){
    printf("%d ",din[i]);
  }
  printf("\n");

  printf("dinnum\n");
  for(i=0;i<dinlen;i++){
    printf("%d ",dinnum[i]);
  }
  printf("\n");
  
  //-----------------------------
  // wire,ha,fa
  //-----------------------------
  printf("=============================\n");
  printf("wire,ha,fa\n");
  printf("=============================\n");

    m=0;
    n=0;
    dinlen2=fanum*3;
    for(i=0;i<dinlen2;i++){
      din2[i]=1000*din[i];
      dinnum2[i]=dinnum[i];
    }

    //#################
    //full adder
    //#################
    for(i=0;i<fanum;i++){

      treesort(din2,dinnum2,dinlen2,1);

      flag=0;
      for(j=0;j<dinlen2;j++){
        if(j==0){
          din2[j]=din2[j]/1000;
          first_val=din2[j];
          first_valnum=dinnum2[j];
        }
        else if(j>0 & j<(fanum-i)){
          if((din2[j]/1000<=(din2[0]-3)) & flag==0){
            flag=1;
            din2[j]=din2[j]/1000;
            din2[j+1]=din2[j+1]/1000;
            second_val=din2[j];
            third_val=din2[j+1];
            second_valnum=dinnum2[j];
            third_valnum=dinnum2[j+1];
          }
        }
        else if(j==(fanum-i)){
          if(flag==0){
            if(din2[j]==din2[j-1]){
              flag=1;
              din2[j]=din2[j]/1000;
              din2[j-1]=din2[j-1]/1000;
              second_val=din2[j];
              third_val=din2[j-1];
              second_valnum=dinnum2[j];
              third_valnum=dinnum2[j-1];
            }
            else{
              flag=1;
              din2[j]=din2[j]/1000;
              din2[j+1]=din2[j+1]/1000;
              second_val=din2[j];
              third_val=din2[j+1];
              second_valnum=dinnum2[j];
              third_valnum=dinnum2[j+1];
            }
          }
        }
      }
      printf("----full adder %d %d %d    %d %d %d\n",first_val,second_val,third_val,first_valnum,second_valnum,third_valnum);

      first_carry=first_val+2000;
      first_sum=first_val+3000;
      second_carry=second_val+3000;
      second_sum=second_val+6000;

      for(k=0;k<afternum;k++){
        if(numflag[k]==0){
          y_s=k;
          numflag[k]=1;
          break;
        }
      }

      if(first_sum>=second_sum){
        printf("table p%d   x %3d   y_s %3d  d %3d  sim_fa_sout p%d_%d_%d p%d_%d_%d p%d_%d_%d \n",(z+1),x,y_s, first_sum,z,x,first_valnum,z,x,second_valnum,z,x,third_valnum);
        sumbuf[m]=first_sum;   m++;
      }
      else{
        printf("table p%d   x %3d   y_s %3d  d %3d  sim_fa_sout p%d_%d_%d p%d_%d_%d p%d_%d_%d \n",(z+1),x,y_s,second_sum,z,x,first_valnum,z,x,second_valnum,z,x,third_valnum);
        sumbuf[m]=second_sum;   m++;
      }
      if(first_carry>=second_carry){
        printf("table p%d   x %3d   y_c %3d  d %3d  sim_fa_cout p%d_%d_%d p%d_%d_%d p%d_%d_%d \n",(z+1),(x+1),y_s,first_carry,z,x,first_valnum,z,x,second_valnum,z,x,third_valnum);
        carrybuf[n]=first_carry; n++;
      }
      else{
        printf("table p%d   x %3d   y_c %3d  d %3d  sim_fa_cout p%d_%d_%d p%d_%d_%d p%d_%d_%d \n",(z+1),(x+1),y_s,second_carry,z,x,first_valnum,z,x,second_valnum,z,x,third_valnum);
        carrybuf[n]=second_carry; n++;
      }
    }  
    printf("\n");
    //#################
    //half adder
    //#################
    for(i=0;i<hanum;i++){

      for(k=0;k<afternum;k++){
        if(numflag[k]==0){
          y_s=k;
          numflag[k]=1;
          break;
        }
      }

      half_carry=din[fanum*3+1]+1000;
      half_sum  =din[fanum*3+1]+3000;
      sumbuf[m]=half_sum;   m++;
      carrybuf[n]=half_carry; n++;
      printf("----half adder %d %d   %d %d\n",din[fanum*3],din[fanum*3+1],dinnum[fanum*3],dinnum[fanum*3+1]);
      printf("table p%d   x %3d   y_s %3d  d %3d  sim_ha_sout p%d_%d_%d p%d_%d_%d \n",(z+1),x,y_s,half_sum,z,x,dinnum[fanum*3],z,x,dinnum[fanum*3+1]);
      printf("table p%d   x %3d   y_c %3d  d %3d  sim_ha_cout p%d_%d_%d p%d_%d_%d \n",(z+1),(x+1),y_s,half_carry,z,x,dinnum[fanum*3],z,x,dinnum[fanum*3+1]);
    }
    //#################
    //wire
    //#################
    for(i=0;i<wirenum;i++){
       sumbuf[m]=din[fanum*3+hanum*2+i]; m++;
    }

     printf("----wire ");
    for(i=0;i<wirenum;i++){
      printf("%d ",din[fanum*3+hanum*2+i]);
    }
     printf("   ");
    for(i=0;i<wirenum;i++){
      printf("%d ",dinnum[fanum*3+hanum*2+i]);
    }
    printf("\n");
    for(i=0;i<wirenum;i++){

      for(k=0;k<afternum;k++){
        if(numflag[k]==0){
          y_s=k;
          numflag[k]=1;
          break;
        }
      }

      printf("table p%d   x %3d   y_s %3d  d %3d  sim_wire0 p%d_%d_%d \n",(z+1),x,y_s,din[fanum*3+hanum*2+i],z,x,dinnum[fanum*3+hanum*2+i]);
    }
    //#################
    //summary    
    //#################
    printf("=============================\n");
    printf("summary\n");
    printf("=============================\n");

    printf("----summary\n");
    printf("sum_num %d carry_num %d\n",m,n);

    printf("FINAL sumbuf ");
    printf("m:%d   ",m);
    for(i=0;i<m;i++){
      printf("%d ",sumbuf[i]);
    }
    printf("\n");

    printf("FINAL carrybuf ");
    printf("n:%d   ",n);
    if(n==0){
      printf("0");
    }
    else{
      for(i=0;i<n;i++){
         printf("%d ",carrybuf[i]);
      }
    }
    printf("\n\n");
}

//---------------------------------
//function : treesort
//---------------------------------
void treesort(int *din,int *dinnum,int dinlen,int reverse){
  int i,j,k;
  int min,tmp;
  int dintmp[2048];

  //------------------------
  // sort body
  //------------------------
  for(i=0;i<dinlen-1;i++){
    min=din[i];
    k=i;
    for(j=i+1;j<dinlen;j++){
      if(din[j]<min){
        min=din[j];
        k=j;
      }
    }

    tmp=din[i];
    din[i]=din[k];
    din[k]=tmp;
    tmp=dinnum[i];
    dinnum[i]=dinnum[k];
    dinnum[k]=tmp;
  }

  if(reverse==1){
    //sort reverse
    for(i=0;i<dinlen;i++){
      dintmp[dinlen-1-i]=din[i];
    }
    for(i=0;i<dinlen;i++){
      din[i]=dintmp[i];
    }
    for(i=0;i<dinlen;i++){
      dintmp[dinlen-1-i]=dinnum[i];
    }
    for(i=0;i<dinlen;i++){
      dinnum[i]=dintmp[i];
    }
  }
}

