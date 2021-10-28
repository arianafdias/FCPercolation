#include <iostream>
#include "latticeview.h"
#include <fstream>
#include<vector>
#include <list>
#define ImageWidth 1000  //image width
#define ImageHeight 1000 //image height
using namespace std;

double random_nr(){
	double x = (double) drand48();
	//cout << drand48() << endl;
    //double x = (double) rand()/RAND_MAX;
    return x;

}

vector<int> burning_method( int L, double p, unsigned seed = 2147483647){
    int arr[L * L];
    //------------ array definition ------------//
    for (int i = 0; i < L * L; ++i) {
        double random = random_nr();
        if (random > p) {
            arr[i] = 1;
        } else {
            arr[i] = 0;
        }
    }
    //Print_lattice(arr, L, L, ImageWidth, ImageHeight, "configInitial.ppm");

    //------------ burning method ------------//
    // 0: empty -> branco
    // 1: occupied -> verde
    // 2: burning -> vermelho
    // 3: burnt -> preto
    // 4: burn next round -> azul


    // first row in flames

    for (int i = 0; i < L; i++) { //first row on fire
        if (arr[i] == 1) {
            arr[i] = 2;
        }
    }
    //Print_lattice(arr, L, L, ImageWidth, ImageHeight, "1stRow.ppm");

    // spreading fire

    bool found = true;
    int t = 0;  // total time to burn all the system

    bool last_line = false;
    int tmin = 0; // time it takes to get to the other side of the system
    int idx;


    while (found) {
        if (last_line == false){
            tmin++;
        }
        t++;
        for (int nlinha = 0; nlinha < L; nlinha++) {
            for (int ncoluna = 0; ncoluna < L; ncoluna++) {
                idx = ncoluna + nlinha *L;
                if (arr[idx] == 2) {  //if burning, verify neighbors, if occupied->burn next
                    if (nlinha != 0) {
                        if (arr[ncoluna + (nlinha -1) *L] == 1) { //down neighbor inflammable?
                            arr[ncoluna + (nlinha -1) *L] = 4; //burning next
                        }
                    }
                    if (nlinha != L - 1) {
                        if (arr[ncoluna + (nlinha +1)*L] == 1) { // upper neighbor inflammable?
                            arr[ncoluna + (nlinha +1)*L] = 4; //burning next
                        }
                    }
                    if (ncoluna != 0) {
                        if (arr[ncoluna-1+nlinha*L] == 1) { //left neighbor inflammable?
                            arr[ncoluna-1+nlinha*L] = 4; //burning next
                        }
                    }
                    if (ncoluna != L - 1) {
                        if (arr[ncoluna+1+nlinha*L] == 1) { //right neighbor inflammable?
                            arr[ncoluna+1+nlinha*L] = 4; //burning next
                        }
                    }
                    if (nlinha == L-1 && (idx + L >(L*L-1))  ){
                        //cout << idx << "\t" << idx + L<< " > " <<L*L-1 << endl;
                        last_line = true;
                    }
                }
            }
        }
        for (int k = 0; k < L * L; k++) {
            if (arr[k] == 2) {
                arr[k] = 3;
            }
            if (arr[k] == 4) {
                arr[k] = 2;
            }
        }
        found = false;
        for (int k = 0; k < L * L; k++) {
            if (arr[k] == 2) {
                found = true;
            }
        }

    }
    //Print_lattice(arr, L, L, ImageWidth, ImageHeight, "configF.ppm");
 
	//cout << t << "   número total de passos de tempo até o fogo parar completamente" << endl;
    //cout << tmin << "   número de passos de tempo necessários para o fogo alcançar o outro lado do sistema" << endl;
    
    vector<int> vec;
    vec.push_back(t);
    if (last_line == true){
    	vec.push_back(tmin);
        vec.push_back(1);
    }else{
    vec.push_back(0);}
    return vec;
    
}

vector<float> statistics(int L, double p, int nrSamples = 1000){
    vector<unsigned int > tmin;
    vector<unsigned int > t;
    for (int i = 0; i < nrSamples; i++) {
        vector<int> config = burning_method(L,p);
        //cout << config[2] <<"\t" << "agregado?"<< endl;
        //cout << config[1] <<"\t" << "tmin"<< endl;
        //cout << config[0] <<"\t" << "t"<< endl;
        if (config[2] == 1){   //checks if there is a percolation cluster
            t.push_back(config[0]); // total burning time
            tmin.push_back(config[1]); // time to go from start to end
        }
    }
    float fraction = (float) tmin.size()/ nrSamples;

    int shrt = 0;
    int lng = 0 ;
    for (int i = 0; i < tmin.size(); i++) {
        shrt += tmin[i];
        lng += t[i];
    }
    float meanTmin = (float) shrt/tmin.size();
    float meanT = (float) lng/t.size();
    vector<float> vec;
    vec.push_back(fraction);
    if (fraction == 0){
    vec.push_back(0);
    vec.push_back(0);
    }else{
    vec.push_back(meanTmin);
    vec.push_back(meanT);
    }
    return vec;
}

int main() {
    int L[5]= {16, 32, 64, 128, 256};
    ofstream StatFile("StatisticPc.txt");
    for (int k = 0; k < 5; k++) {
        for (double p = 0.4; p < 0.43; p = p + 0.005) {

            vector<float> sta = statistics(L[k], p, 1000);
            StatFile << L[k] <<"\t"<< p << "\t" << sta[0] << "\t" << sta[1] << "\t" << sta[2] << endl;


       }
  }  
    StatFile.close();

    return 0;
    }
