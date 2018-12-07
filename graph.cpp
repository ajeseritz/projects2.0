#include <iostream>
#include "Graph.hpp"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

Graph::Graph(){
}

Graph::~Graph(){
    while(!vertices.empty()){
        vertices.at(1).Edges.clear();
        vertices.clear();
    }
}
void Graph::addEdge(string v1, string v2, int distance){
    vertex * ver1 = findVertex(v1);
    vertex * ver2 = findVertex(v2);
    Edge * a = new Edge; 
    //Edge * b = new Edge;
    a->v = ver2;
    a->distance = distance;
    // b->v = ver1;
    //if(distance > 0)
    //cout << " ... Reading in " << v1 << " -- " << v2 << " -- " << distance << endl;
    ver1->Edges.push_back(*a);
    //ver2->Edges.push_back(*b);
    return;
}

void Graph::addVertex(std::string name){
    vertex * ver = new vertex;
    ver->name = name;
    ver->district = 0;
    ver->visited = false;
    vertices.push_back(*ver);
}

void Graph::displayEdges(){
    for(int i = 0; i < vertices.size(); i++){
        cout<< vertices.at(i).district << ":" << vertices.at(i).name << "-->";
        for(int j = 0; j < vertices.at(i).Edges.size(); j++){
            if(j != 0)
                cout << "***";
            cout << vertices.at(i).Edges.at(j).v->name << " (" << vertices.at(i).Edges.at(j).distance << " miles)";
        }
        cout << endl;
    }
}

void Graph::assignDistricts(){
    // BFTraversalLabel(vertices.front().name, 1);
    int count = 0;
    for(int i = 0; i < vertices.size(); i++){
        if(vertices.at(i).district == 0){
            
            count++;
            BFTraversalLabel(vertices.at(i).name, count);
        }
        //cout << i << endl;
    }
    return;
}

void printdown(vertex * v){
    if(v->visited == true)
    return;
    //cout << "Visited: " << v->visited;
    v->visited = true;
    //cout << "I am hanging on\n";
    cout << v->district <<": " << v->name << endl;
    if(v->Edges.size() != 0){
        for(int i = 0; i < v->Edges.size(); i++){
            printdown(v->Edges.at(i).v);
        }
    }
}

void Graph::printDFS(){
    vertex * a;
    for(int i = 0; i < vertices.size(); i++){
        a = &vertices.at(i);
        printdown(a);
    }
    DFTraversal(a);
}

void Graph::setAllVerticesUnvisited(){
    for(int i = 0; i < vertices.size(); i++){
        vertices.at(i).visited = false;
    }
}

vertex *Graph::findVertex(std::string name){
    for(int i = 0; i < vertices.size(); i++){
        if(vertices.at(i).name == name)
            return &vertices.at(i);
    }
    return NULL;
}

void Graph::BFTraversalLabel(std::string startingCity, int distID){
    vertex * a = findVertex(startingCity);
    vertex * b;
    //cout << "The starting city is: " << startingCity<< endl;
    if(a->visited == true)
    return;
    a->visited = true;
    a->district = distID;
    for(int i = 0; i < a->Edges.size(); i++){
        b = a->Edges.at(i).v;
        b->district = distID;
        b->visited = true;
    }
    
    for(int i = 0; i < a->Edges.size(); i++){
        b = a->Edges.at(i).v;
        if(b->Edges.empty() != 1){
            for(int j = 0; j < b->Edges.size(); j++){
                BFTraversalLabel(b->Edges.at(j).v->name, distID);
            }
        }
    }
    return;
}

void Graph::DFTraversal(vertex *v){
    if(v->visited == true)
    return;
    
    if(v->Edges.size() != 0){
        for(int i = 0; i < v->Edges.size(); i++){
            DFTraversal(v->Edges.at(i).v);
        }
        v->visited = true;      
    }
    else{
        v->visited = true;
    }
}

int main(int argc, char ** argv){
    if(argc != 2){
        cout<<"There are not enough arguments to run this program.\n./main <number of patients>"<<endl;
        return 0;
    }
    Graph a = Graph();
    int i = 1, total = 0;
    int pos =0;
    vector <string> names;
    string item;
    string delim = ",";
    ifstream myfile(argv[1]);
    if(myfile.is_open()){
        string line;
        getline(myfile, line);
        stringstream ss;
        ss<<line;
        string item, city;
        getline(ss, item, ',');
        while(getline(ss, item, ',')){
            a.addVertex(item);
            total++;
            names.push_back(item);
        }
        
        while(getline(myfile, line)){
            i = 0;
            ss<<line;

            while((pos = line.find(delim)) != (signed)string::npos){
                item = line.substr(0, pos);
                
                if(i == 0){
                    city = item;
                }
                else{
                    
                    if(stoi(item) > 0){
                        a.addEdge(city, names.at(i -1), stoi(item));
                        cout << " ... Reading in " << city << " -- " << names.at(i -1) << " -- " << stoi(item) << endl;
                        
                    }
                    
                }
                i++;
                line.erase(0, pos + delim.length());
            }
            if(stoi(line) > 0){
                a.addEdge(city, names.at(total -1), stoi(line));
                cout << " ... Reading in " << city << " -- " << names.at(total -1) << " -- " << stoi(line) << endl;
            }
        }
    }
    else{
        cout<<"error"<<endl;
        return 0;
    }
    a.assignDistricts();
    a.setAllVerticesUnvisited();
    a.displayEdges();
}