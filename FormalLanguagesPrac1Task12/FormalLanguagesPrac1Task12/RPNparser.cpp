//
//  RPNparser.cpp
//  FormalLanguagesPrac1Task12
//
//  Created by Сергей Миллер on 19.10.15.
//  Copyright © 2015 Сергей Миллер. All rights reserved.
//

#include <stdio.h>
#include "RPNparser.h"
#include <stack>

using std::pair;
using std::make_pair;

GraphNode::GraphNode(char symb, size_t vertID) {
    this->symb = symb;
    this->vertID = vertID;
    delFlag = false;
}

NFA::~NFA() {
    finish->delFlag = true;
    recursiveDelete(finish);
}

void NFA::recursiveDelete(GraphNode* vert) {
    GraphNode* nextVert;
    for(size_t i = 0;i < vert->next.size(); ++i) {
        nextVert = vert->next[i];
        if(nextVert && !nextVert->delFlag) {
            nextVert->delFlag = true;
            recursiveDelete(nextVert);
        }
    }
    delete vert;
}

NFA::NFA(string& regExpInRPN) {
    std::stack <pair<GraphNode*, GraphNode*> > parsStack;
    pair<GraphNode*, GraphNode*> leftOperand, rightOperand, operand;
    GraphNode* leftEmptyBuff;
    GraphNode* rightEmptyBuff;
    GraphNode* emptyBuff;
    GraphNode* letterVert;
    char letter;
    sizeVert = 0;
    
    for(size_t i = 0;i < regExpInRPN.size(); ++i) {
        letter = regExpInRPN[i];
        switch (letter) {
            case '*':
                if(parsStack.empty()) {
                    throw BadRegExpInRPNException("");
                }
                operand = parsStack.top();
                parsStack.pop();
                leftEmptyBuff = new GraphNode('-', sizeVert++);
                rightEmptyBuff = new GraphNode('-', sizeVert++);
                operand.first->next.push_back(leftEmptyBuff);
                rightEmptyBuff->next.push_back(operand.second);
                
                operand.first->next.push_back(operand.second);
                operand.second->next.push_back(operand.first);
                parsStack.push(make_pair(leftEmptyBuff, rightEmptyBuff));
                break;
            case '.':
                if(parsStack.size() < 2) {
                    throw BadRegExpInRPNException("");
                }
                rightOperand = parsStack.top();
                parsStack.pop();
                leftOperand = parsStack.top();
                parsStack.pop();
                rightOperand.first->next.push_back(leftOperand.second);
                parsStack.push(make_pair(leftOperand.first, rightOperand.second));
                break;
            case '+':
                if(parsStack.size() < 2) {
                    throw BadRegExpInRPNException("");
                }
                rightOperand = parsStack.top();
                parsStack.pop();
                leftOperand = parsStack.top();
                parsStack.pop();
                leftEmptyBuff = new GraphNode('-', sizeVert++);
                rightEmptyBuff = new GraphNode('-', sizeVert++);
                rightOperand.first->next.push_back(leftEmptyBuff);
                leftOperand.first->next.push_back(leftEmptyBuff);
                rightEmptyBuff->next.push_back(leftOperand.second);
                rightEmptyBuff->next.push_back(rightOperand.second);
                parsStack.push(make_pair(leftEmptyBuff, rightEmptyBuff));
                break;
            default:
                leftEmptyBuff = new GraphNode('-', sizeVert++);
                rightEmptyBuff = new GraphNode('-', sizeVert++);
                if(letter == '1') {
                    letter = '-';
                }
                letterVert = new GraphNode(letter, sizeVert++);
                rightEmptyBuff->next.push_back(letterVert);
                letterVert->next.push_back(leftEmptyBuff);
                parsStack.push(make_pair(leftEmptyBuff, rightEmptyBuff));
                break;
        }
    }
    
    if(parsStack.size() != 1) {
        throw BadRegExpInRPNException("");
    } else {
        operand = parsStack.top();
        parsStack.pop();
        start = operand.first;
        finish = operand.second;
    }
}

int NFA::findMaxPathInDFS(string& word) {
    vector <vector <bool> > dfsStatus(sizeVert, vector <bool> (word.size() + 1, false));
    std::stack <pair <GraphNode*, size_t> > dfsStack;
    int maxSuffixSize = -1;
    pair <GraphNode*, size_t> curStatus;
    GraphNode* nextVert;
    
    dfsStack.push(make_pair(finish, word.size()));
    while(!dfsStack.empty()) {
        curStatus = dfsStack.top();
        dfsStack.pop();
        
        if(curStatus.first == start) {
                maxSuffixSize = std::max(maxSuffixSize, (int)(word.size() - curStatus.second));
        }
    
        for(size_t i = 0;i < curStatus.first->next.size(); ++i) {
            nextVert = curStatus.first->next[i];
            if(curStatus.second != 0) {
                if(nextVert->symb == word[curStatus.second - 1] && !dfsStatus[nextVert->vertID][curStatus.second - 1]) {
                    dfsStatus[nextVert->vertID][curStatus.second - 1] = true;
                    dfsStack.push(make_pair(nextVert, curStatus.second - 1));
                }
            }
            
            if(nextVert->symb == '-' && !dfsStatus[nextVert->vertID][curStatus.second]) {
                dfsStatus[nextVert->vertID][curStatus.second] = true;
                dfsStack.push(make_pair(nextVert, curStatus.second));
            }
        }
    }
    return maxSuffixSize;
}

int findSizeOfMaxWordSuffixInRegExp(string& regExp, string& word) {
        NFA nfa(regExp);
        return nfa.findMaxPathInDFS(word);
}
