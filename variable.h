#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <vector>
#include "atom.h"
using std::string;


class Variable : public Term{
public:
  Variable(string s):_symbol(s), _value(s){}
  string symbol() const{return _symbol;}
  string value() const{
    if(_tvalue==0)
      return _value;
    else
      return _tvalue->value();
  }

  void assignValue(string val, std::vector<string>*symbols){
    _value = val;
    for(int i = 0; i<v.size(); i++){
      if(!find(v[i]->symbol(),symbols)){
        symbols->push_back(v[i]->symbol());
        dynamic_cast<Variable*>(v[i])->assignValue(val, symbols);
      }
    }
  }

  bool find(string data, std::vector<string>*symbols){
    
    for(std::vector<string>::iterator it = symbols->begin(); it!=symbols->end(); ++it){
      if(*it==data)
        return true;
    }
    return false;
  }

  bool match(Term &term ){
    if(_value == _symbol){
      if(term.type()=="Variable" && this!=&term){
        Variable *var = dynamic_cast<Variable*>(&term);
        v.push_back(&term);
        var->v.push_back(this);

        if(term.value()!=term.symbol()){
          assignValue(term.value(), new std::vector<string>);
        }
      }
      else if(term.type()=="Struct"){
        _tvalue = &term;

      }
      else{
        assignValue(term.value(), new std::vector<string>);
      }
      return true;
    }
    else if(!term.value().empty()){
      return term.value()==value();
    }
    else{
      return term.symbol()==symbol();
    }
  }
  bool setValue(Term *t) {
    _value = (t)->value();
    return true;
  }
  string type() const{return "Variable";}

private:
  string _symbol;
  string _value;
  Term *_tvalue = 0;
  std::vector<Term*> v;
};

#endif