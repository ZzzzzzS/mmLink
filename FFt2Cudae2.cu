#include"FFT2CUDA2.h"
#include"math.h"

texture<float,2,cudaReadModeElementType>texRefR;//实部纹理参照系声明
texture<float,2,cudaReadModeElementType>texRefI;//虚部纹理参照系声明
__global__ void FFT2Cudaa(float*DataInR,float*DataInI,float*DataOutR,float*DataOutI,float*WnR,float*WnI,int N)
{
__shared__ float sdataR[64],sdataI[64];
int ttid=threadIdx.x;
if(ttid<N)
{
sdataR[ttid]=DataInR[ttid];
sdataI[ttid]=0;
sdataR[ttid+N/2]=DataInR[ttid+N/2];
sdataI[ttid+N/2]=0;
__syncthreads();
if(ttid<N/2)
    {
    int p,q;
    float WR,WI,XpR,XpI,XqWnR,XqWnI;
    int stage1=0;float stage2=0;
    for(int Nd=1;Nd<N;Nd=Nd*2)
        {
        p=rintf((ttid/Nd)-0.5)*Nd*2+ttid%Nd;
        q=p+Nd;
        WR=(float)(WnR[ttid+32*(stage1++)]);
        WI=(float)(WnI[ttid+32*(stage2++)]);
        XqWnR=sdataR[q]*WR-sdataI[q]*WI;//复数计算
        XqWnI=sdataR[q]*WI+sdataI[q]*WR;
        XpR=sdataR[p];
        XpI=sdataI[p];
        sdataR[p]=XpR+XqWnR;
        sdataI[p]=XpI+XqWnI;
        sdataR[q]=XpR-XqWnR;
        sdataI[q]=XpI-XqWnI;
        __syncthreads();
        }
    DataOutR[p]=sdataR[p];
    DataOutI[p]=sdataI[p];
    DataOutR[q]=sdataR[q];
    DataOutI[q]=sdataI[q];
    }
}
}
void FFt2Cudae2(float*DataInR,float*DataInI,float*DataOutR,float*DataOutI,float*wnR,float*wnI,int Nn) 
{
   
    float*DeviceSrcR,*DeviceSrcI,*DeviceDstR,*DeviceDstI,*DeviceWnR,*DeviceWnI;

    cudaMalloc(&DeviceSrcR,sizeof(float)*Nn);
    cudaMalloc(&DeviceSrcI,sizeof(float)*Nn);
    cudaMalloc(&DeviceDstR,sizeof(float)*Nn);
    cudaMalloc(&DeviceDstI,sizeof(float)*Nn);
    cudaMalloc(&DeviceWnR,sizeof(float)*Nn/2);
    cudaMalloc(&DeviceWnI,sizeof(float)*Nn/2);
    
    cudaMemcpy(DeviceSrcR,DataInR,sizeof(float)*64,cudaMemcpyHostToDevice);//数据传入Device
    cudaMemcpy(DeviceSrcI,DataInI,sizeof(float)*64,cudaMemcpyHostToDevice);
    cudaMemcpy(DeviceWnR,wnR,sizeof(float)*32,cudaMemcpyHostToDevice);//WN传入Device
    cudaMemcpy(DeviceWnI,wnI,sizeof(float)*32,cudaMemcpyHostToDevice);

    cudaMemset(DeviceDstR,0,sizeof(float)*Nn);
    cudaMemset(DeviceDstI,0,sizeof(float)*Nn);

    dim3 blockSize(32,1,1);
    dim3 gridSize(1,1,1);

    const size_t smemSize=128*sizeof(float);
    FFT2Cudaa<<<gridSize,blockSize,smemSize>>>(DeviceSrcR,DeviceSrcI,DeviceDstR,DeviceDstI,DeviceWnR,DeviceWnI,Nn);
    
    cudaMemcpy(DataOutR,DeviceDstR,sizeof(float)*Nn,cudaMemcpyDeviceToHost);
    cudaMemcpy(DataOutI,DeviceDstI,sizeof(float)*Nn,cudaMemcpyDeviceToHost);


    cudaFree(DeviceSrcR);
    cudaFree(DeviceDstR);
    cudaFree(DeviceSrcI);
    cudaFree(DeviceDstI);
    cudaFree(DeviceWnR);
    cudaFree(DeviceWnI);
}