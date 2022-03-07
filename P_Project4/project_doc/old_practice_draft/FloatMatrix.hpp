#include <memory>
#include <iostream>
namespace SUSTech{
    namespace CSE{
    class FloatMatrix
    {
    public:
        FloatMatrix(int rows, int cols, float value);
        FloatMatrix(const FloatMatrix &);
        FloatMatrix &operator=(const FloatMatrix &);
        ~FloatMatrix();
        // FloatMatrix &operator+=(const FloatMatrix &);
        FloatMatrix operator+(const FloatMatrix &);
        FloatMatrix operator*(const float&);
        void print();
    private:
        FloatMatrix (std::shared_ptr<float> data, int rows, int cols):data(data), rows(rows), cols(cols){}
        std::shared_ptr<float> data;
        int rows, cols;
    };
    void FloatMatrix::print(){
        for (size_t i = 0; i < rows; i++)
        {
            for (size_t j = 0; j < cols; j++)
            {
                std::cout<<data.get()[i*cols+j]<<" ";
            }
            std::cout<<std::endl;
            
        }
        
    }
    
    FloatMatrix::FloatMatrix(int rows, int cols, float value):rows(rows),cols(cols)
    {
        std::cout<<"FloatMatrix(int rows, int cols, float value) construtor"<<std::endl;
        data = std::shared_ptr<float>(new float[rows*cols]{value});//建议用make_shared
        // std::fill(data.get()[0], data.get()[rows*cols], value);
    }
    FloatMatrix::FloatMatrix(const FloatMatrix &other):data(other.data){}
    FloatMatrix&  FloatMatrix::operator=(const FloatMatrix &other){
        std::cout<<"FloatMatrix::operator="<<std::endl;
        // if(this != &other)
            this->data = other.data;
        return *this;
    }

    FloatMatrix FloatMatrix::operator*(const float& multiple){
        std::cout<<"operator*(const float& multiple)"<<std::endl;

        auto newData = std::shared_ptr<float>(new float[rows*cols]);
        for (size_t i = 0; i < rows*cols; i++)
        {
            newData.get()[i] = (this->data.get()[i]) * multiple;
        }
        return FloatMatrix(newData, rows, cols);
    }

    FloatMatrix FloatMatrix::operator+(const FloatMatrix & other){
        std::cout<<"operator+(const FloatMatrix & other)"<<std::endl;
        auto newData = std::shared_ptr<float>(new float[rows*cols]);
        for (size_t i = 0; i < rows*cols; i++)
        {
            newData.get()[i] = (this->data.get()[i]) + (other.data.get()[i]);
        }
        return FloatMatrix(newData, rows, cols);
    }
    
    FloatMatrix::~FloatMatrix()
    {
        std::cout<<"FloatMatrix header destroyed, but data is not."<<std::endl;
    }
}
}