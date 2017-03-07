#ifndef JSON_HPP
#define JSON_HPP

#include <string>
#include <iostream>
#include <map>
#include <vector>

struct Visitor;
struct Base;
struct Object;
struct Array;
struct String;
struct Number;
struct Bool;
struct Null;

struct Visitor
{
  std::string type;
  void visit(Object *) { type =  "Object"; }
  void visit(Array *) { type =  "Array"; }
  void visit(String*) { type =  "String"; }
  void visit(Number *) { type =  "Number"; }
  void visit(Bool*) { type =  "Bool"; }
  void visit(Null *) { type =  "Null"; }
};

struct Value {
	virtual void accept(Visitor&) = 0;
	virtual int getWeight(){ return 1; }
	virtual std::string gValue(){
		return "";
	}
};

struct Object: Value {
	std::map<std::string, Value*> m;
	int getWeight(){ return 1 + m.size(); }
	void accept(Visitor & v) { v.visit(this); }
};

struct Array: Value {
	std::vector<Value*> vec;
	int getWeight(){ return 1 + vec.size(); }
	void accept(Visitor & v) { v.visit(this); }
};

struct String:Value {
	std::string val;

	String(std::string start, std::string end) {
        std::string::size_type pos = start.find(end);
        if(pos != std::string::npos)
            val = start.substr(1, pos - 2);
        else
            val = start.substr(1, pos - 2);
    }

	virtual std::string gValue() {
		return val;
	}

	void accept(Visitor & v) { v.visit(this); }
};

struct Number:Value {
	std::string val;

	Number(std::string start, std::string end) {
        std::string::size_type pos = start.find(end);
        val = start.substr(0, pos);
    }

    virtual std::string gValue() {
        return val;
    }

    void accept(Visitor & v) { v.visit(this); }
};

struct Bool:Value {
	bool val;

	Bool() {}
	Bool(bool *a) {
		val = a;
	}

	virtual std::string gValue() {
		return val ? "true" : "false";
	}

	void accept(Visitor & v) { v.visit(this); }
};

struct Null:Value {

	virtual std::string gValue() {
		return "null";
	}

	void accept(Visitor & v) { v.visit(this); }
};

#endif