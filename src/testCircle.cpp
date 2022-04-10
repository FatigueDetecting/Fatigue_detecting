#include<iostream>
#include <stdio.h>
#include <time.h>
#include "CRingBuffer.h"

using namespace std;
int main(int argc, char** argv){
    CRingBuffer<int> buff(8);
    int j;int k;
    for(int i = 0;i<6;i++)
    {
        // cout<< i <<endl;
        buff.WriteData(&i,1);
    }
    for (int h =0;h<4;h++){
        buff.ReadData(&j,1);
        cout<<j<<endl;

    }
    cout<<"insert New"<<endl;
    for(int l = 0;l<5;l++)
    {
        cout<< l <<endl;
        buff.WriteData(&l,1);
    }
    cout<<"continue Read"<<endl;
    for (int h =0;h<8;h++){
        buff.ReadData(&k,1);
         cout<<k<<endl;
    }

}