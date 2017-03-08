// json parser
#include "json.hpp"

std::vector<Value*> nodeVector;

std::string typeOf(Value* val)
{
  Visitor vis;
  val->accept(vis);
  return vis.type;
}

Value* parseJsonValue(const char*& front, const char* end);

// removes whitespace
void parseWhiteSpace(const char*& front, const char* end) {
    while (front != end && std::isspace(*front))
        ++front;
}


std::pair<std::string, Value*> parseJsonPair(const char*& first, const char* last) {
    Value *key = parseJsonValue(first, last);

    parseWhiteSpace(first, last);
    parseWhiteSpace(first, last);
    ++first;
    
    Value *value = parseJsonValue(first, last);
    return {key->gValue(), value};
}

// Parse object
Value* parseJsonObject(const char*& front, const char* end){
    ++front; 
    Object *object = new Object();
    while(true) {
        if(*front != '}') 
            object->m.insert(parseJsonPair(front, end));
        parseWhiteSpace(front, end);
            
        if (*front == ',') {
            ++front;
            continue;
        }
            
        if (*front == '}') {
            ++front;
            break;
        }
            

    }
    nodeVector.push_back(object);
    return object;
}

// Parse array
Value* parseJsonArray(const char*& front, const char* end) {
    front++; 
        
    Array *a = new Array();
    while(true) {
        if(*front != ']')
            a->vec.push_back(parseJsonValue(front, end));
        
        parseWhiteSpace(front, end);
            
        if (*front == ',') {
            ++front;
            continue;
        }
            
        if (*front == ']') {
            ++front;
            break;
        }
    }
    nodeVector.push_back(a);
    return a;
}

// Parse String
Value* parseJsonString(const char*& front, const char* end) {
    const char* init = front++; 
    String* str;
        
    while (front != end) {
        if (*front == '\\') { 
            front += 2;
        }
            
        if (*front == '"') {
            ++front;
            break;
        }
        ++front;
    }
    str = new String(init, front);
    nodeVector.push_back(str);
    return str;
}

// Parse numbers
Value* parseJsonNumber(const char*& front, const char* end) {
    const char* i = front++;
    Number * num; 
    while (front != end && (std::isdigit(*front) || *front == '.' || *front == '-'))
        ++front;
    num = new Number(i, front);
    nodeVector.push_back(num);
    return num;
}

// Parse null
Value* parseJsonNull(const char*& front, const char* end) {
    Null *n = new Null(); 
    int i = 0;
    while(i < 4){
        ++front;
    	++i;
    }
    nodeVector.push_back(n);
    return n;
}

// Parse bool
Value* parseJsonBoolean(const char*& front, const char* end) {
    Bool *b = new Bool();
    parseWhiteSpace(front, end);
    if(*front == 't') 
        b->val = true;
    else 
        b->val = false;
    do
        ++front;
    while(*front != 'e'); {
    	++front;
    }
    nodeVector.push_back(b);
    return b;
}


Value* parseJsonValue(const char*& front, const char* end) {
    parseWhiteSpace(front, end);
    switch(*front) {
        case '{':
            return parseJsonObject(front, end);
        case '[':
            return parseJsonArray(front, end);
        case '"':
            return parseJsonString(front, end);
        case 't':
            return parseJsonBoolean(front, end);
        case 'f':
            return parseJsonBoolean(front, end);
        case 'n':
            return parseJsonNull(front, end);
        default:
            return parseJsonNumber(front, end);
    }
}

// main parse function
Value* parseJson(const std::string &str) {
    const char* json_str = str.c_str(); 
    return parseJsonValue(json_str, json_str + str.size());
}

void countAllTypes(std::vector<Value *> vec)
{
    int objects = 0,
        arrays = 0,
        strings = 0,
        numbers = 0,
        bools = 0,
        nulls = 0;

    for(auto a: vec)
    {
        std::string type = typeOf(a);

        if(type == "Object")
        {
            ++objects;
        }
        else if(type == "Array")
        {
            ++arrays;
        }
        else if(type == "String")
        {
            ++strings;
        }
        else if(type == "Bool")
        {
            ++bools;
        }
        else if(type == "Number")
        {
            ++numbers;
        }
        else if(type == "Null")
        {
            ++nulls;
        }
    }
    std::cout << "Number of " << objects << std::endl;
    std::cout << "Number of " << arrays << std::endl;
    std::cout << "Number of " << strings << std::endl;
    std::cout << "Number of " << numbers << std::endl;
    std::cout << "Number of " << bools << std::endl;
    std::cout << "Number of " << nulls << std::endl;
}

void getWeight(std::vector<Value*> vec)
{
    int weight = 0;
    for(auto a: vec)
    {
        weight += a->getWeight();
    }
    std::cout << "Weight: " << weight << std::endl;
}

int main(){

    std::string json;
    std::string line;
    while(getline(std::cin, line)) { json.append(line); }
    parseJson(json);

    countAllTypes(nodeVector);
    getWeight(nodeVector);

}
