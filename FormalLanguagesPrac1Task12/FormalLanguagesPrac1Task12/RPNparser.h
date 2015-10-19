//
//  RPNparser.h
//  FormalLanguagesPrac1Task12
//
//  Created by Сергей Миллер on 19.10.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#ifndef RPNparser_h
#define RPNparser_h

#include <string>
#include <vector>


using std::vector;
using std::string;

int findSizeOfMaxWordSuffixInRegExp(string& regExp, string& word);

class GraphNode {
public:
    GraphNode(){};
    GraphNode(char symb, size_t vertID);
    vector <GraphNode*> next;
    char symb;
    bool delFlag;
    size_t vertID;
};


class NFA {
public:
    NFA(string& regExpInRPN);
    ~NFA();
    GraphNode* start;
    GraphNode* finish;
    size_t sizeVert;
    vector <size_t> vertID;
    int findMaxPathInDFS(string& word);
    void recursiveDelete(GraphNode* vert);
};

class BadRegExpInRPNException {
public:
    string message;
    BadRegExpInRPNException(string message){this->message = message;}
};
#endif /* RPNparser_h */
