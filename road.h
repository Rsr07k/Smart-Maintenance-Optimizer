#ifndef ROAD_H
#define ROAD_H

#include <iostream>
#include <string>
extern int N;
extern int E;

struct Road {
    int id;
    int u, v, cost, traffic, damage, priority, complaints;
    bool rejected;
    Road() : id(0), u(0), v(0), cost(0), traffic(0), damage(0), priority(0), complaints(0), rejected(false) {}

    Road(int i, int u_, int v_, int c, int t, int d, int comp)
        : id(i), u(u_), v(v_), cost(c), traffic(t), damage(d),
          priority(0), complaints(comp), rejected(false) {}

    void display() const {
        std::cout << "ID: " << id << std::endl;
        std::cout << "U: " << u << std::endl;
        std::cout << "V: " << v << std::endl;
        std::cout << "Cost: " << cost << std::endl;
        std::cout << "Traffic: " << traffic << std::endl;
        std::cout << "Damage: " << damage << std::endl;
        std::cout << "Priority: " << priority << std::endl;
        std::cout << "Complaints: " << complaints << std::endl;
        std::cout << "Rejected: " << std::boolalpha << rejected << std::endl;
    }

    int getu()  { return u; }
    int getv()  { return v; }
    int getu() const { return u; }
    int getv() const { return v; }
};

struct Node {
    std::string type;
    int id;

    Node() : type(""), id(0) {}
    Node(const std::string &t, int i) : type(t), id(i) {}

    void display() const {
        std::cout << "Type: " << type << std::endl;
        std::cout << "ID: " << id << std::endl;
    }
};
extern Node nodes[100];
extern Road Roads[100];

#endif
