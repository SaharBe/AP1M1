//
// Created by sahar on 11/11/2019.
//

#include <iostream>
#include <vector>
#include <map>
#include "ex1.h"


//binaryOperator
BinaryOperator::BinaryOperator(Expression *right, Expression *left) {
    this->left = left;
    this->right = right;
}
BinaryOperator::~BinaryOperator() {
    if(left != NULL){
        delete(left);
    }
    if(right != NULL){
        delete(right);
    }

}
Plus ::Plus(Expression *left, Expression *right): BinaryOperator( right, left) {}
double Plus::calculate() {
    return left->calculate() + right->calculate();
}
Plus::~Plus() {
    if(left != NULL){
       // delete this->left;
    }
    if(right != NULL){
     //   delete( this->right);
    }
}

Minus::Minus(Expression *left, Expression *right) : BinaryOperator( right, left) {}
double Minus::calculate() {
    return left->calculate() - right->calculate();
}
Minus::~Minus() {

}
Mul::Mul(Expression *left, Expression *right):BinaryOperator( right, left) {}
double Mul::calculate() {
    return (left->calculate() *right->calculate());
}
Mul::~Mul() {

}
Div::Div(Expression *left, Expression *right): BinaryOperator( right, left) {}
double Div::calculate() {
        try {
            if (right->calculate() == 0){
                throw "Try to divide by zero";
            }
            return left->calculate() / right->calculate();
        } catch(std::logic_error e) {
            std::cerr << e.what();
            cout <<"Try to divide by zero";
        }
}
Div::~Div() {

}

//value
Value::Value(double d) {
    this->d = d;
}
double Value :: calculate(){
    return d;
};
Value::~Value() {

}

//Variable
double Variable::calculate(){
    return this->value;
}
Variable::Variable(string name, double value) {
    this->name = name;
    this->value = value;
}
Variable& Variable::operator++() {
    value =  value +1;
    return *this;
}
Variable& Variable::operator+=(double d) {
    value = value +d;
    return *this;
}
Variable& Variable::operator++(int i) {
    value++;
    return *this;
}

Variable& Variable::operator--() {
    value = value -1;
    return *this;
}
Variable& Variable::operator--(int i) {
    value = value - i;
    return *this;
}
Variable& Variable::operator-=(double d) {
    value = value - d;
    return  *this;
}
Variable::~Variable() {

}
//UnaryOperator
UnaryOperator::UnaryOperator(Expression *expression1) : expression(expression1){}
UnaryOperator::~UnaryOperator() {
    if(expression != NULL){
        delete this->expression;
    }
}

UPlus::UPlus(Expression *exp): UnaryOperator(exp) {}
double UPlus::calculate() {
    return expression->calculate();
}
UPlus::~UPlus() {

}
UMinus::UMinus(Expression *exp) : UnaryOperator(exp) {}
double UMinus::calculate() {
    return 0-expression->calculate();
}
UMinus::~UMinus() {

}


