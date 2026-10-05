#include <iostream>
#include <stack>
#include <string>

class Calculator {
private:
    std::string infixToPostfix(const std::string& expression) {
        std::stack<char> Stack;
        std::string postfix;
        for(char c : expression) {
            if(c >= '0' && c <= '9') {
                postfix += c;
            }
            else if(c=='+' || c=='-') {
                while(!Stack.empty() && (Stack.top() == '+' || Stack.top() == '-'
                                         || Stack.top() == '*' || Stack.top() == '/')) {
                    postfix += Stack.top();
                    Stack.pop();
                }
                Stack.push(c);
            }
            else if (c=='*' || c=='/') {
                while(!Stack.empty() && (Stack.top() == '*' || Stack.top() == '/')) {
                    postfix += Stack.top();
                    Stack.pop();
                }
                Stack.push(c);
            }
            else if (c=='(') {
                Stack.push(c);
            }
            else if (c==')') {
                while(!Stack.empty() && (Stack.top() != '(')) {
                    postfix += Stack.top();
                    Stack.pop();
                }
                Stack.pop();
            }
        }
        while(!Stack.empty()) {
            if(Stack.top() == '(') {
                return "";
            }
            postfix += Stack.top();
            Stack.pop();
        }
        return postfix;
    }
public:
    double evaluate(const std::string& expression) {
        std::string postfix = infixToPostfix(expression);
        std::stack<double> Stack;
        for(char c : postfix) {
            if(c >= '0' && c <= '9') {
                Stack.push(c - '0');
            }
            else {
                double a = Stack.top();
                Stack.pop();
                double b = Stack.top();
                Stack.pop();
                Stack.push((c=='+' ? a + b :
                            (c=='-' ? b - a : (c=='*' ? b * a : b / a))));
            }
        }
        return Stack.top();
    }
};

int main() {
    std::cout << "Input expression: ";
    std::string expression;
    std::getline(std::cin, expression);
    Calculator calc;
    std::cout << "Result: " << calc.evaluate(expression) << std::endl;
    return 0;
}
