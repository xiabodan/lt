#include <iostream>
#include <typeinfo>

using namespace std;

class Home{
public:
    virtual void open() = 0;
    virtual void close() = 0;
};

class Ele : public Home{
public:
    virtual void open() {}
    virtual void close() {} 
};

class Tel : public Home{
public:
    virtual void open() {}
    virtual void close() {}
  
    virtual void vcd(){}  
};

class Fam : public Tel{
public:
    virtual void open() {}
    virtual void close() {}
  
    virtual void vcd(){}  
};

enum Cmd{OPEN,CLOSE,VCD};

class Dct{
public:
    Dct(int n){
        cmd = n;
    }
    Cmd getCmd(int n){
       cmd = n;
    }
    void Control(Home& device){
        //Cmd c = getCmd(VCD)
        switch(cmd){
        case OPEN:
            device.open();
            break;
        case CLOSE:
            device.close();
            break;
        case VCD:
            try{
                Tel tv = dynamic_cast<Tel&>(device);
                cout<< "this device can  vcd" <<endl;
                tv.vcd();
            }catch(std::bad_cast&){
                cout << "this device can not vcd" << endl;           
            }
           
            //if(typeid(device) == typeid(Tel)){
            //    cout<< "Tel this device can  vcd" <<endl;
            //    Tel* t = static_cast<Tel*>(&device);
            //    t->vcd();
            //}else if(typeid(device) == typeid(Fam)){
            //    cout<< "Fam this device can  vcd" <<endl;
            //    Fam* t = static_cast<Fam*>(&device);
            //    t->vcd();
            //}else{
            //    cout<< "this device can not vcd" <<endl;
            //}
            break;
        }
    }
private:
    int cmd;
};


int main(){
    Dct d(2);
    Tel t;
    d.Control(t);
    Ele e;
    d.getCmd(2);    
    d.Control(e);
    Fam f;
    d.getCmd(2);    
    d.Control(f);
        
    return 0;
}










