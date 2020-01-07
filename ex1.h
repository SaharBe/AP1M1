//
// Created by sahar on 11/11/2019.
//

#ifndef UNTITLED2_EX1_H
#define UNTITLED2_EX1_H


#include "Expression.h"
#include <string>
#include "interpreter.h"

using namespace std;

class ex1 {
};

class Value: public Expression{
private:
     double d;
public:
    Value(double d );
    double calculate();
    virtual ~Value();
};

class Variable: public Expression{
private:
    string name;
    double value;
public:
    Variable(string name, double value);
    double calculate();
    Variable& operator++();
    Variable& operator--();
    Variable& operator+=(double d);
    Variable& operator-=(double d);
    Variable& operator++(int i);
    Variable& operator--(int i);
    virtual ~Variable();


};

class BinaryOperator: public Expression{
protected:
    Expression* right;
    Expression* left;
public:
    BinaryOperator(Expression* right, Expression* left);
   virtual ~BinaryOperator();
};

class Plus: public BinaryOperator{
public:
    Plus(Expression* left, Expression* right);
    double calculate();
    virtual ~Plus();
};

class Minus: public BinaryOperator{
public:
    Minus(Expression* left, Expression* right);
    double calculate();
    virtual ~Minus();

};

class Mul: public BinaryOperator{
public:
    Mul(Expression* left, Expression* right);
    double calculate();
    virtual ~Mul();
};
class Div: public BinaryOperator{
public:
    Div(Expression* left, Expression* right);
    double calculate();
    virtual ~Div();
};
#endif //UNTITLED2_EX1_H

class UnaryOperator: public Expression{
protected:
    Expression* expression;
public:
    UnaryOperator(Expression* expression1);
    virtual ~UnaryOperator();
};

class UPlus: public UnaryOperator{
public:
    UPlus(Expression* exp);
    double calculate();
    virtual  ~UPlus();
};

class UMinus: public UnaryOperator{
public:
    UMinus(Expression* exp);
    double calculate();
    virtual ~UMinus();
};