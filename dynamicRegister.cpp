#include "dynamicRegister.h"
#include <iostream>

DynReg::DynReg() {
    data = new int;
}

DynReg::~DynReg() {}

void DynReg::add(bool val) {
    if (size++ % blockwidth == 0) {
        int* _data = new int[((size - 1) / blockwidth) + 1];
        for (int i = 0; i < (size - 1) / blockwidth; i++) {
            _data[i] = data[i];
        }
        _data[((size - 1) / blockwidth)] = 0;
        delete data;
        data = _data;
    }
    if (val)
        set(size - 1);
}

void DynReg::set(int index) {
    data[index / blockwidth] |= 1 << index % blockwidth;
}

void DynReg::clear(int index) {
    data[index / blockwidth] &= ~(1 << index % blockwidth);
}

bool DynReg::read(int index) {
    return data[index / blockwidth] & 1 << index % blockwidth;
}

bool DynReg::operator == (DynReg& d) {
    if (size != d.getSize())
        return false;
    for (int i = 0; i < size; i++) {
        if (read(i) != d.read(i))
            return false;
    }
    return true;
}

int DynReg::getSize() {
    return size;
}