#include<bits/stdc++.h>
using namespace std;

int N,M,k1,k2;
string param;
// struct  edge{
//     int a,b;
//     edge(int _a,int _b) : a(_a), b(_b){}
//     bool operator<(const edge& other) const {
//         if (a != other.a) {
//             return a < other.a;
//         }
//         return b < other.b;
//     }
// };

// return id of the following fresh variable
inline int g1i(int g1){return g1;}
inline int g2i(int g2){return g2+N;}

inline int s1ij(int i,int j){return (2*N+1)+i*(k1+2)+j;}
inline int s2ij(int i,int j){return (2*N+1)+(N+1)*(k1+2)+(k2+2)*i+j;}
// inline int eij(int i,int j){return (i-1)*N+j;}


// map<int,int> Gdegree;
// map<edge,int> edge_id;

vector<vector<int>> totalclauses;

void AddConstraint(int a,int b,int c,int d){
    totalclauses.push_back({-b,a});
    totalclauses.push_back({-c,-d,a});
    totalclauses.push_back({c,b,-a});
    totalclauses.push_back({d,b,-a});
}
void MakeInput(){
    ofstream inpFile(param+".satinput");
    if (totalclauses==vector<vector<int>>{{}}) {
        inpFile<<"p cnf 0 1\n0\n";
        return;
    }
    inpFile<<"p cnf "<<2*N+(N+1)*(k1+2)+(N+1)*(k2+2)<<" "<<totalclauses.size()<<endl;
    for(int i=0;i<totalclauses.size();i++){
        for(int x : totalclauses[i]){
            inpFile<<x<<" ";
        }
        inpFile<<"0\n";
    }
}
void solve(){
    ifstream graph(param+".graph");
    graph>>N>>M>>k1>>k2;
    int Gdegree[N] = {0};
    bool edge_check[N][N] = {{false}};
    for(int i=0;i<M;i++){
        int a,b,a1,b1;graph>>a1>>b1;
        a=min(a1,b1),b=max(a1,b1);
        edge_check[a-1][b-1]=true;
        Gdegree[a-1]++;
        Gdegree[b-1]++;
    }
    
    if (k1>N or k2>N) {
        totalclauses.push_back({});
        MakeInput();
        return;
    }
    // degree constraint for completeness
    for(int i=1;i<=N;i++){
        if(Gdegree[i-1]<k1-1)totalclauses.push_back({-g1i(i)});
    }
    for(int i=1;i<=N;i++){
        if(Gdegree[i-1]<k2-1)totalclauses.push_back({-g2i(i)});
    }

    //  ith nodes can't be in both graph g1 and g2, N no of clauses
    for(int i=1;i<=N;i++){
        totalclauses.push_back({-g1i(i),-g2i(i)});
    }

    // clause for having edge or not  2*(N*(N-1)/2-M clauses)
    for(int i=1;i<=N;i++){
        for(int j=i+1;j<=N;j++){
            if(!edge_check[i-1][j-1]){
                totalclauses.push_back({-g1i(i),-g1i(j)});
                totalclauses.push_back({-g2i(i),-g2i(j)});
            }
        }
    }

    // for g1
    for(int j=1;j<=k1+1;j++){
        totalclauses.push_back({-s1ij(0,j)});
    }

    for(int i=0;i<=N;i++){
        totalclauses.push_back({s1ij(i,0)});
    }

    for(int i=1;i<=N;i++){
        for(int j=1;j<=k1+1;j++){
            AddConstraint(s1ij(i,j),s1ij(i-1,j),g1i(i),s1ij(i-1,j-1));
        }
    }
    // enforcing for k1 to be true
    totalclauses.push_back({s1ij(N,k1)});
    totalclauses.push_back({-s1ij(N,k1+1)});


    // for g2
    for(int j=1;j<=k2+1;j++){
        totalclauses.push_back({-s2ij(0,j)});
    }

    for(int i=0;i<=N;i++){
        totalclauses.push_back({s2ij(i,0)});
    }

    for(int i=1;i<=N;i++){
        for(int j=1;j<=k2+1;j++){
            AddConstraint(s2ij(i,j),s2ij(i-1,j),g2i(i),s2ij(i-1,j-1));
        }
    }

    // enforcing for k2 to be true
    totalclauses.push_back({s2ij(N,k2)});
    totalclauses.push_back({-s2ij(N,k2+1)});


    MakeInput();
}
int main(int argc, char** argv){
    // auto start_time = std::chrono::high_resolution_clock::now();
    param = (string)argv[1];
    solve();
    // auto end_time = std::chrono::high_resolution_clock::now();

    // // Calculate the duration in milliseconds
    // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // // Print the execution time
    // std::cout << "Execution Time: " << duration.count() << " milliseconds" << std::endl;
}