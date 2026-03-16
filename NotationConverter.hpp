
#ifndef NOTATION_CONVERTER_HPP
#define NOTATION_CONVERTER_HPP

#include <string>
#include <stdexcept>
#include <cstddef>

//our deque
template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* prev;
        Node* next;
        Node(const T& d): data(d), prev(nullptr), next(nullptr) {}
    };
    Node* m_head{nullptr};
    Node* m_tail{nullptr};
    std::size_t m_size{0};

public:
    Deque() = default;
    ~Deque() { clear(); }
    Deque(const Deque& other) { Node* c = other.m_head; while (c) { push_back(c->data); c = c->next; } }
    Deque& operator=(const Deque& other) {
        if (this == &other) return *this;
        clear(); Node* c = other.m_head; while (c) { push_back(c->data); c = c->next; } return *this;
    }

    //queries
    bool empty() const { return m_size == 0; }
    std::size_t size() const { return m_size; }

    //clear all
    void clear() {
        Node* c = m_head; while (c) { Node* n=c->next; delete c; c=n; }
        m_head = m_tail = nullptr; m_size = 0;
    }

    //new node become new head
    void push_front(const T& v) {
        Node* n = new Node(v); n->next = m_head; if (m_head) m_head->prev = n; else m_tail = n; m_head = n; ++m_size;
    }

    //new node becomes new tail
    void push_back(const T& v) {
        Node* n = new Node(v); n->prev = m_tail; if (m_tail) m_tail->next = n; else m_head = n; m_tail = n; ++m_size;
    }
    
    //reference to first element
    T& front() {
        if (empty()) throw std::out_of_range("Deque::front on empty"); return m_head->data;
    }
    const T& front() const {
        if (empty()) throw std::out_of_range("Deque::front on empty"); return m_head->data;
    }

    //reference to last element
    T& back() {
        if (empty()) throw std::out_of_range("Deque::back on empty"); return m_tail->data;
    }
    const T& back() const {
        if (empty()) throw std::out_of_range("Deque::back on empty"); return m_tail->data;
    }

    //pop first node
    void pop_front() {
        if (empty()) throw std::out_of_range("Deque::pop_front on empty");
        Node* n = m_head; m_head = m_head->next; if (m_head) m_head->prev = nullptr; else m_tail = nullptr; delete n; --m_size;
    }

    //pop last node
    void pop_back() {
        if (empty()) throw std::out_of_range("Deque::pop_back on empty");
        Node* n = m_tail; m_tail = m_tail->prev; if (m_tail) m_tail->next = nullptr; else m_head = nullptr; delete n; --m_size;
    }
};


class NotationConverter {
public:
    std::string postfixToInfix(std::string inStr);
    std::string postfixToPrefix(std::string inStr);

    std::string infixToPostfix(std::string inStr);
    std::string infixToPrefix(std::string inStr);

    std::string prefixToInfix(std::string inStr);
    std::string prefixToPostfix(std::string inStr);

private:
   
    static bool isSpace(char c);
    static bool isLetter(char c);
    static bool isOperator(char c);
    static int precedence(char op);

    //splits into tokens
    static Deque<std::string> tokenize(const std::string& s); 

    template <typename T> static void push(Deque<T>& st, const T& v) { st.push_back(v); }
    template <typename T> static T pop(Deque<T>& st) { if (st.empty()) throw std::runtime_error("pop on empty"); T v = st.back(); st.pop_back(); return v; }
    template <typename T> static T& top(Deque<T>& st) { if (st.empty()) throw std::runtime_error("top on empty"); return st.back(); }

    //output joiner
    static std::string joinWithSingleSpaces(Deque<std::string>& q); 

    std::string infixTokensToPostfix(const Deque<std::string>& tokens);

    std::string postfixTokensToInfix(const Deque<std::string>& tokens);

    std::string postfixTokensToPrefix(const Deque<std::string>& tokens);

    std::string prefixTokensToInfix(const Deque<std::string>& tokens);

    std::string prefixTokensToPostfix(const Deque<std::string>& tokens);
};

#endif 
