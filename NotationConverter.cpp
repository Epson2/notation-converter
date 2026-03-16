
#include "NotationConverter.hpp"

//checks for white space
bool NotationConverter::isSpace(char c) { return c==' ' || c=='\t' || c=='\n' || c=='\r'; 
}

//checks if character is an operand
bool NotationConverter::isLetter(char c) { return (c>='A'&&c<='Z') || (c>='a'&&c<='z'); 
}

//checks if its an operater
bool NotationConverter::isOperator(char c) { return c=='+'||c=='-'||c=='*'||c=='/'; 
}

//gives precendent for operators
int  NotationConverter::precedence(char op) { return (op=='*'||op=='/')?2: (op=='+'||op=='-')?1:0; }

// organizing our characters and putting in our deque
Deque<std::string> NotationConverter::tokenize(const std::string& s) {
    Deque<std::string> out;
    for (std::size_t i=0;i<s.size();) {
        char c = s[i];
        if (isSpace(c)) { ++i; continue; }
        if (c=='(' || c==')' || isOperator(c)) { out.push_back(std::string(1,c)); ++i; continue; }
        if (isLetter(c)) { out.push_back(std::string(1,c)); ++i; continue; }
        throw std::runtime_error(std::string("Invalid character: ")+c);
    }
    return out;
}

// concatenate strings with only one space in between
std::string NotationConverter::joinWithSingleSpaces(Deque<std::string>& q) {
    std::string out; while (!q.empty()) { out += q.front(); q.pop_front(); if (!q.empty()) out += ' '; } return out;
}

//tokenize string, convert and output
std::string NotationConverter::postfixToInfix(std::string inStr) { return postfixTokensToInfix(tokenize(inStr)); 
}

std::string NotationConverter::postfixToPrefix(std::string inStr) { return postfixTokensToPrefix(tokenize(inStr)); 
}

std::string NotationConverter::infixToPostfix(std::string inStr) { return infixTokensToPostfix(tokenize(inStr)); 
}

//infix to prefix caller
std::string NotationConverter::infixToPrefix(std::string inStr) {
    std::string post = infixTokensToPostfix(tokenize(inStr));
    return postfixTokensToPrefix(tokenize(post));
}
std::string NotationConverter::prefixToInfix(std::string inStr) { return prefixTokensToInfix(tokenize(inStr)); 
}

std::string NotationConverter::prefixToPostfix(std::string inStr) { return prefixTokensToPostfix(tokenize(inStr)); 
}

//takes string of infix notation and return sting in the postfix notation
std::string NotationConverter::infixTokensToPostfix(const Deque<std::string>& tokensIn) {
    Deque<std::string> tokens = tokensIn;
    //queue of postfix tokens
    Deque<std::string> output; 
    //operator stack
    Deque<std::string> ops;    

    while (!tokens.empty()) {
        std::string t = tokens.front(); tokens.pop_front();
        if (t.size()==1 && isLetter(t[0])) {
            output.push_back(t);
        } else if (t=="(") {
            push(ops, t);
        } else if (t==")") {
            while (!ops.empty() && top(ops)!="(") { output.push_back(pop(ops)); }
            if (ops.empty()) throw std::runtime_error("Mismatched parentheses");
            ops.pop_back(); 
        } else if (t.size()==1 && isOperator(t[0])) {
            char op = t[0];
            while (!ops.empty()) {
                std::string s2 = top(ops);
                if (s2.size()==1 && isOperator(s2[0]) && precedence(s2[0]) >= precedence(op)) {
                    output.push_back(pop(ops));
                } else break;
            }
            push(ops, t);
        } else {
            throw std::runtime_error("Invalid infix token");
        }
    }
    while (!ops.empty()) {
        if (top(ops)=="(") throw std::runtime_error("Mismatched parentheses");
        output.push_back(pop(ops));
    }
    return joinWithSingleSpaces(output);
}

//takes string of postfix and returns infix
std::string NotationConverter::postfixTokensToInfix(const Deque<std::string>& tokensIn) {
    Deque<std::string> tokens = tokensIn; Deque<std::string> st;
    while (!tokens.empty()) {
        std::string t = tokens.front(); tokens.pop_front();
        if (t.size()==1 && isLetter(t[0])) {
            push(st, t);
        } else if (t.size()==1 && isOperator(t[0])) {
            std::string b = pop(st); std::string a = pop(st);
            std::string expr = "(" + a + " " + t + " " + b + ")"; 
            push(st, expr);
        } else throw std::runtime_error("Invalid postfix token");
    }
    if (st.size()!=1) throw std::runtime_error("Invalid postfix expression");
    return st.back();
}

//takes string of postifix and returns prefix
std::string NotationConverter::postfixTokensToPrefix(const Deque<std::string>& tokensIn) {
    Deque<std::string> tokens = tokensIn; Deque<std::string> st;
    while (!tokens.empty()) {
        std::string t = tokens.front(); tokens.pop_front();
        if (t.size()==1 && isLetter(t[0])) { push(st, t); }
        else if (t.size()==1 && isOperator(t[0])) {
            std::string b = pop(st); std::string a = pop(st);
            push(st, t + std::string(" ") + a + std::string(" ") + b);
        } else throw std::runtime_error("Invalid postfix token");
    }
    if (st.size()!=1) throw std::runtime_error("Invalid postfix expression");
    return st.back();
}

//take string of string of prefix and return infix
std::string NotationConverter::prefixTokensToInfix(const Deque<std::string>& tokensIn) {
    Deque<std::string> rev; { Deque<std::string> t = tokensIn; while (!t.empty()) { rev.push_front(t.front()); t.pop_front(); } }
    Deque<std::string> st;
    while (!rev.empty()) {
        std::string tok = rev.front(); rev.pop_front();
        if (tok.size()==1 && isLetter(tok[0])) { push(st, tok); }
        else if (tok.size()==1 && isOperator(tok[0])) {
            std::string a = pop(st); std::string b = pop(st);
            std::string expr = "(" + a + " " + tok + " " + b + ")"; 
        } else throw std::runtime_error("Invalid prefix token");
    }
    if (st.size()!=1) throw std::runtime_error("Invalid prefix expression");
    return st.back();
}

//take string of prefix and return postfix
std::string NotationConverter::prefixTokensToPostfix(const Deque<std::string>& tokensIn) {
    Deque<std::string> rev; { Deque<std::string> t = tokensIn; while (!t.empty()) { rev.push_front(t.front()); t.pop_front(); } }
    Deque<std::string> st;
    while (!rev.empty()) {
        std::string tok = rev.front(); rev.pop_front();
        if (tok.size()==1 && isLetter(tok[0])) { push(st, tok); }
        else if (tok.size()==1 && isOperator(tok[0])) {
            std::string a = pop(st); std::string b = pop(st);
            push(st, a + std::string(" ") + b + std::string(" ") + tok);
        } else throw std::runtime_error("Invalid prefix token");
    }
    if (st.size()!=1) throw std::runtime_error("Invalid prefix expression");
    return st.back();
}
