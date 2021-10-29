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
        if (random <= p) {
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
    
    if (last_line == true){
        vec.push_back(1);
    }else{
    	vec.push_back(0);}
    	
    vec.push_back(tmin);
    vec.push_back(t);
    
    return vec;
    
}

vector<float> statistics(int L, double p, int nrSamples = 1000){
    double avg_totalTime = 0.0;
    double avg_percTime = 0.0;
    double frac_perc = 0.0;
    
    for (int i = 0; i < nrSamples; i++) {
        vector<int> config = burning_method(L,p); //[perc?, tmin, t]
        int t = config[2];
        int tmin = config[1];
        int perc = config[0];
        frac_perc += (double) perc;
        
        if (perc > 0.00001){   //checks if there is a percolation cluster
        	avg_percTime += tmin;
        	avg_totalTime += t;
        }
    }
    
    if (frac_perc > 0.00001){
    	avg_percTime = avg_percTime/(frac_perc*1000);
    	avg_totalTime = avg_totalTime/(frac_perc*1000);
    }
    
    frac_perc = frac_perc;
    
    vector<float> vec;
    vec.push_back(frac_perc);
    vec.push_back(avg_totalTime);
    vec.push_back(avg_percTime);
    return vec;
}

int main() {
    int L[5]= {16, 32, 64, 128, 256};
    ofstream StatFile("Statistic.txt");
    for (int k = 0; k < 5; k++) {
        for (double p = 0.1; p < 1; p = p + 0.01) { 

            vector<float> sta = statistics(L[k], p, 1000);
            StatFile << L[k] <<"\t"<< p << "\t" << sta[0] << "\t" << sta[1] << "\t" << sta[2] << endl;


       }
  }  
    StatFile.close();

    return 0;
    }
