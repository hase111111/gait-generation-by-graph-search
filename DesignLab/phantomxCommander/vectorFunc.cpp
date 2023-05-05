#include"vectorFunc.h"
void myvector::VectorOutPut(myvector::VECTOR v1, std::ofstream& fout){
	fout<<"(\t"<<v1.x<<"\t"<<v1.y<<"\t"<<v1.z<<"\t)\n";
}

void myvector::VectorOutPut(myvector::VECTOR v1){
	std::cout<<"( "<<v1.x<<","<<v1.y<<","<<v1.z<<" )\n";
}