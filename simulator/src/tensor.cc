//
// Created by hyl on 14/05/24.
//

#include "src/tensor.h"
template<typename T>
struct Tensor<T>::Content {
    T* data = nullptr;
    Shape shape;
    Device device = Device::kCPU;
};





