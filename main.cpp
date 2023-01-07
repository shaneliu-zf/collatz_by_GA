#include <iostream>
#include <vector>
#include <functional>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

int collatz(int n){
    int ans = 0;
    while(n!=1){
        if(n&1)n = 3*n+1;
        else n = n/2;
        ans++;
    }
    return ans;
}


class Gene:public vector<bool>{
public:
    int getAns()const{
        int n = 0,bi = 1;
        for(auto i = rbegin();i!=rend();i++){
            n += (*i)*bi;
            bi*=2;
        }
        return n;
    }
    bool operator<(const Gene& o)const{
        return getAns() > o.getAns();
    }
};

double rand0to1(){
    return ((double)rand()/(double)RAND_MAX);
}
int rand0and1(){
    return rand() %2;
}

class GA{
private:
    function<int(Gene)>_fitness;
    vector<Gene>genes;
    int iter_count;
    int _gene_length;
    double _mutation_rate;
    Gene best_gene;
    void generateGenes(){
        for(Gene& gene:genes){
            gene.resize(_gene_length);
            for(int i=0;i<gene.size();i++){
                gene[i] = rand0and1();
            }
        }
    }
    void sortGene(){
        sort(genes.begin(),genes.end(),[this](Gene gene1,Gene gene2){
            return _fitness(gene1) < _fitness(gene2);
        });
        genes.erase(unique(genes.begin(),genes.end()),genes.end());
    }
    void reproduction(){
        sortGene();
        if(genes[0].getAns() >best_gene.getAns()){
            best_gene = genes[0];
        }
        int size = genes.size() - ceil(genes.size()*0.3);
        genes.erase(genes.begin()+ceil(genes.size()*0.5), genes.end());
        while(genes.size() < size){
            Gene gene;
            gene.resize(_gene_length);
            for(int i=0;i<gene.size();i++){
                gene[i] = rand0and1();
            }
            genes.push_back(gene);
        }
    }

    void crossover(){
        int rand_gene1 = rand()%genes.size();
        int rand_gene2 = rand()%genes.size();
        int rand_pos = rand() % _gene_length;
        int rand_length = rand()%(_gene_length-rand_pos);
        for(int i=0;i<rand_pos;i++){
            genes[rand_gene2][rand_pos+i] = genes[rand_gene1][rand_pos+i];
        }
    }

    void mutation(){
        for(int i=0;i<genes.size();i++){
            double mutation_if = rand0to1();
            if(mutation_if < _mutation_rate){
                int rand_pos = rand() % _gene_length;
                genes[i][rand_pos] = !genes[i][rand_pos];
            }
        }
    }
public:
    GA(function<int(Gene)>fitness,int gene_length,int genes_size,double mutation_rate){
        _fitness = std::move(fitness);
        genes.resize(genes_size);
        iter_count = 0;
        _gene_length = gene_length;
        _mutation_rate = mutation_rate;
    }
    void run(int generation){
        this->generateGenes();
        while(iter_count<=generation && genes.size()>1){
            cout<<"generations:"<<iter_count<<endl;
            cout<<"============"<<endl;
            print();
            cout<<"============"<<endl;
            reproduction();
            crossover();
            mutation();
            iter_count++;
        }
        cout<<"best ans:"<<endl;
        cout<<"============"<<endl;
        cout<<best_gene.getAns()<<endl;
        cout<<"============"<<endl;
    }
    void print(){
        for(const Gene& g:genes){
            cout<<g.getAns()<<endl;
        }
    }
};


#define X 4

int fitness(Gene g){
    return abs(g.getAns() - collatz(X));
}

int main(){
    srand(time(NULL));
    GA ga(fitness,15,100000,0.01);
    ga.run(1000);
    cout<<"true answer:"<<collatz(X)<<endl;
    return 0;
}
