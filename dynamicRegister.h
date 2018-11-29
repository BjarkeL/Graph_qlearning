#ifndef DYNAMICREGISTER_H
#define DYNAMICREGISTER_H

class DynReg {
public:
    DynReg();
    ~DynReg();

    void add(bool val);
    void set(int index);
    void clear(int index);
    bool read(int index);
    bool operator == (DynReg& d);
    int getSize();

private:
    int size = 0;
    int blockwidth = sizeof(int) * 8;
    int* data;
};

#endif