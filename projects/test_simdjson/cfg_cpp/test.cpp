#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stack>
#include <map>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <sstream>
using namespace std;

typedef struct {
    string s;
    int type;  // 0:operatorChar;1:number
} expNode;
float calPoland(stack<expNode> t);
int operatorPriority(char c) {
    switch (c) {
        case '#':
            return 1;
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 3;
        default:
            return 1;
    }
}
float calculate(float a, float b, char c) {
    switch (c) {
        case '+':
            return a + b;
        case '-':
            return a - b;
        case '*':
            return a * b;
        case '/':
            return a / b;
        default:
            return 0;
    }
}
stack<expNode> makePoland(string t) {
    int len = t.length();
    char c;
    stack<expNode> expStack;
    stack<char> charStack;
    charStack.push('#');
    for (int i = 0; i < len; i++) {
        c = t[i];
        if (c == '(') {
            charStack.push(c);
        } else if (c == ')') {
            while (1) {
                c = charStack.top();
                charStack.pop();
                if (c == '(') break;
                expNode node;
                node.s.append(1, c);
                node.type = 0;
                expStack.push(node);
            }
        } else if (c >= '0' && c <= '9') {
            expNode node;
            node.s.append(1, c);
            node.type = 1;
            while (1) {
                if (t[i + 1] >= '0' && t[i + 1] <= '9' && i < len) {
                    node.s.append(1, t[i + 1]);
                    i++;
                } else
                    break;
            }
            expStack.push(node);
        } else if (operatorPriority(c) > operatorPriority(charStack.top())) {
            charStack.push(c);
        } else if (operatorPriority(c) <= operatorPriority(charStack.top())) {
            while (1) {
                char topc = charStack.top();
                if (operatorPriority(c) > operatorPriority(topc)) {
                    charStack.push(c);
                    break;
                }
                expNode node;
                node.s.append(1, topc);
                node.type = 0;
                expStack.push(node);
                charStack.pop();
            }
        }
        printf("%d :", i);
        stack<expNode> x = expStack;
        while (!x.empty()) {
            expNode node = x.top();
            cout << node.s << ' ';
            x.pop();
        }
        printf("\n");
    }
    while (!charStack.empty()) {
        c = charStack.top();
        charStack.pop();
        if (c == '#') break;
        expNode node;
        node.s.append(1, c);
        node.type = 0;
        expStack.push(node);
    }
    stack<expNode> x = expStack;
    while (!expStack.empty()) expStack.pop();
    while (!x.empty()) {
        expNode node = x.top();
        x.pop();
        expStack.push(node);
    }
    /*while(!expStack.empty()){
        expNode node=expStack.top();
        cout<<node.s;
        expStack.pop();
    }
    printf("\n"); */
    // printf("%f\n",calPoland(expStack));
    return expStack;
}
float calPoland(stack<expNode> t) {
    stack<float> numStack;
    int x = t.size();
    while (!t.empty()) {
        expNode node = t.top();
        t.pop();
        if (node.type == 1) {
            float num;
            istringstream iss(node.s);
            iss >> num;
            numStack.push(num);
        } else if (node.type == 0) {
            float a = numStack.top();
            numStack.pop();
            float b = numStack.top();
            numStack.pop();
            char c = node.s[0];
            float ans = calculate(b, a, c);
            numStack.push(ans);
        } else {
            printf("error:expStack type error!\n");
        }
    }
    float ans = numStack.top();
    return ans;
}
int main() {
    // string s="((44+55)*66-5)/22+3*2" ;
    string s = "((4+5)*6-5)/2+3*2";
    stack<expNode> expStack = makePoland(s);
    float ans = calPoland(expStack);
    printf("%f\n", ans);
    return 0;
}
