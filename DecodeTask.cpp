#include <bitset>
#include <fstream>
#include <iostream>
#include <ios>
#include <vector>
#include <string>
#include <sstream>
using namespace std;


uint8_t a; 
uint32_t b; 
uint32_t c;
struct position
{
   double lattitude;
   double longitude;
   double height;
   float coordinate[3];
   uint8_t A;
   uint8_t B;
};
struct configuration
{ int16_t id;
  uint8_t bit;
  uint8_t antenna[4];
  uint8_t A;
   uint8_t B;
};

struct measurement
{int32_t count;
char text[5];
uint8_t width[2];
uint8_t length[8];
uint8_t A;
   uint8_t B;
};

int main(){
	position position1;
configuration configuration1;
measurement measurement1;
int checksum_A=0,checksum_B=0;
   ifstream infile("d:/data.dat",ios::binary);
   ofstream outfile1;
outfile1.open("d:/position.dat", ios::out | ios::trunc );
ofstream outfile2;
outfile2.open("d:/configuration.dat", ios::out | ios::trunc );
ofstream outfile3;
outfile3.open("d:/measurement.dat", ios::out | ios::trunc );
   while(infile.good()){
while(int(a)!=170)    infile.read((char*)&a,sizeof(a));	

infile.read((char*)&a,sizeof(a));
infile.read((char*)&b,sizeof(b));

c=b >>16;

if(int(c)==36) {	

infile.read((char*)&position1,sizeof(position1));

 uint8_t a=0;
 uint8_t b=0;
bitset<64> bits1(*(reinterpret_cast<unsigned long long*>(&position1.lattitude)));

for(int i=56; i>=0; i-=8)
{bitset<8> bitvec1(bits1.to_string(),i,8);
b+=(a+=bitvec1.to_ulong()); }

bitset<64> bits2(*(reinterpret_cast<unsigned long long*>(&position1.longitude)));
for(int i=56; i>=0; i-=8)
{bitset<8> bitvec1(bits2.to_string(),i,8);
b+=(a+=bitvec1.to_ulong()); }

bitset<64> bits3(*(reinterpret_cast<unsigned long long*>(&position1.height)));
for(int i=56; i>=0; i-=8)
{bitset<8> bitvec1(bits3.to_string(),i,8);
b+=(a+=bitvec1.to_ulong()); }

bitset<32> bits4(*(reinterpret_cast<unsigned long long*>(&position1.coordinate[0])));
for(int i=24; i>=0; i-=8)
{bitset<8> bitvec1(bits4.to_string(),i,8);
b+=(a+=bitvec1.to_ulong()); }

bitset<32> bits5(*(reinterpret_cast<unsigned long long*>(&position1.coordinate[1])));
for(int i=24; i>=0; i-=8)
{bitset<8> bitvec1(bits5.to_string(),i,8);
b+=(a+=bitvec1.to_ulong()); }

bitset<32> bits6(*(reinterpret_cast<unsigned long long*>(&position1.coordinate[2])));
for(int i=24; i>=0; i-=8)
{bitset<8> bitvec1(bits6.to_string(),i,8);
b+=(a+=bitvec1.to_ulong()); }


if(a==position1.A&&b==position1.B){

 outfile1<<"The lattitude is "<<position1.lattitude<<" radians"<<endl;
 outfile1<<"The longitude is "<<position1.longitude<<" radians"<<endl;
 outfile1<<"The height is "<<position1.height<<" m"<<endl;
 outfile1<<"The ECEF Coordinate is ("<<position1.coordinate[0]<<"km, ";
 outfile1<<position1.coordinate[1]<<"km, ";
 outfile1<<position1.coordinate[2]<<"km)"<<endl<<endl;}
}

else if(int(c)==7) {

infile.read((char*)&configuration1,sizeof(configuration1)); 

uint8_t a=0;
uint8_t b=0;
 
uint8_t partA = (uint8_t)((configuration1.id & 0xFF00) >> 8);
uint8_t partB = (uint8_t)(configuration1.id & 0x00FF);
uint8_t A[7]={partB,partA,configuration1.bit,configuration1.antenna[0],configuration1.antenna[1],configuration1.antenna[2],configuration1.antenna[3]};
for(int i=0; i<7; i++) b+=(a+=A[i]);

if(a==configuration1.A&&b==configuration1.B){
uint32_t antennafin=configuration1.antenna[0]|configuration1.antenna[1]<<8|configuration1.antenna[2]<<16|configuration1.antenna[3]<<24;
uint8_t output=3,device=28,GNSS=32,Satellite=192;
float antenna_float = *((float*)&antennafin);
	
outfile2<<"The id is "<<configuration1.id<<endl;
output=configuration1.bit & output;
device=(configuration1.bit & device)>>2;
GNSS=(configuration1.bit & GNSS)>>5;
Satellite=(configuration1.bit & Satellite)>>6;
outfile2<<"The output type is "<<(int)output<<"; ";
outfile2<<"The device type is "<<(int)device<<"; ";
outfile2<<"The GNSS Flag is "<<(int)GNSS<<"; ";
outfile2<<"The Satellite type is "<<(int)Satellite<<";"<<endl;
outfile2<<"The antenna seperation is "<<antenna_float<<" m"<<endl<<endl;}

}

else if(int(c)==19) {
uint8_t a=0;
uint8_t b=0;	
infile.read((char*)&measurement1,sizeof(measurement1)); 
uint8_t A[4] = {0};

for (int i=0; i<4 ;++i)  {
A[i] = ((uint8_t*)&measurement1.count)[i];
b+=(a+=A[i]);
}

char* str=measurement1.text;
string text(str);
string text1=text+'\0';
vector<uint8_t> myVector(text1.begin(), text1.end());
for (int i=0; i<myVector.size() ;++i) b+=(a+=myVector[i]);
b+=(a+=measurement1.width[0]);
b+=(a+=measurement1.width[1]);
for(int i=0; i<8; i++) b+=(a+=measurement1.length[i]);

if(a==measurement1.A&&b==measurement1.B){
bitset<16> bsTest1 { } ;
 for ( unsigned i = 0 ; i <2 ; ++i ) {
bsTest1 <<= 8;
bitset<16> bsTmp { (unsigned long long) measurement1.width[1-i] };
bsTest1 |= bsTmp;}

short width=stol(bsTest1.to_string(),nullptr,2);
bitset<64> bsTest { } ;
 for ( unsigned i = 0 ; i <8 ; ++i ) {
bsTest <<= 8;
bitset<64> bsTmp { (unsigned long long) measurement1.length[7-i] };
bsTest |= bsTmp;}

double length=0;
unsigned long long bbb=bsTest.to_ullong();
memcpy(&length, &bbb, 8);

outfile3<<"The count is "<<measurement1.count<<endl;
outfile3<<"The text content is "<<text<<endl;
outfile3<<"The width is "<<width<<" mm"<<endl;
outfile3<<"The length is "<<length<<" m"<<endl<<endl;
}
}

}  	
 
   infile.close( );
   outfile1.close();
   outfile2.close();
   outfile3.close();
   return 0;

}


