#ifndef VECTOR_VECTOR_H
#define VECTOR_VECTOR_H


#include <initializer_list>
#include <memory>
#include <algorithm>




template<typename _Tp>
class vector{
public:
    typedef _Tp             value_type;
    
    vector():_M_start(nullptr), _M_finish(nullptr), _M_end_of_storage(nullptr){}

    vector(const std::initializer_list<value_type> & __lst){
        _M_create_storage(__lst.size());
        this->_M_finish = std::uninitialized_copy(__lst.begin(), __lst.end(), this->_M_start);
    }

    vector(const vector &__vec){
        _M_create_storage(__vec.size());
        this->_M_finish = std::uninitialized_copy(__vec.begin(), __vec.end(), this->_M_start);
    }

    vector&
    operator=(const vector &__vec) {
        if(this != &__vec){
            free();
            _M_create_storage(__vec.size());
            this->_M_finish = std::uninitialized_copy(__vec.begin(), __vec.end(), this->_M_start);
        }
        return *this;
    }

    vector(vector &&__vec) noexcept {
        _M_swap_data(__vec);
    }

    vector&
    operator=(vector &&__vec) noexcept {
        if(this != &__vec){
            free();
            _M_swap_data(__vec);
        }
        return *this;
    }


    ~vector(){free();}
    value_type* begin() const {return _M_start;}
    value_type* end() const {return _M_finish;}
    std::size_t capacity() const {return _M_end_of_storage - _M_start;}
    std::size_t size() const {return _M_finish - _M_start;}

    void push_back(const value_type &item){
        check_n_alloc();
        _M_alloc.construct(_M_finish++, item);
    }
    void push_back(value_type &&__x){
        check_n_alloc();
//        emplace_back(std::move(__x));
        _M_alloc.construct(_M_finish++, std::move(__x));
    }


private:
    void free(){
        if(_M_start){
            std::for_each(_M_start, _M_finish, [this](value_type &item){_M_alloc.destroy(&item);});
            _M_alloc.deallocate(_M_start, _M_end_of_storage-_M_start);
            _M_start = _M_end_of_storage = _M_finish = nullptr;
        }
    }

    void check_n_alloc(){
        if(size() == capacity())
            reallocate();
    }

    void
    reallocate(){
        std::size_t __new_capacity = capacity()? 2*capacity(): 1;
        value_type* __new_start = _M_alloc.allocate(__new_capacity);
        value_type* __new_finish = std::uninitialized_copy(
                std::make_move_iterator(begin()), std::make_move_iterator(end()), __new_start);

        free();
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage = __new_start + __new_capacity;
    }



    void
    _M_create_storage(size_t __n)
    {
        this->_M_start = this->_M_alloc.allocate(__n);
        this->_M_finish = this->_M_start;
        this->_M_end_of_storage = this->_M_start + __n;
    }

    void
    _M_swap_data(vector<value_type>& __x) _GLIBCXX_NOEXCEPT
    {
        std::swap(_M_start, __x._M_start);
        std::swap(_M_finish, __x._M_finish);
        std::swap(_M_end_of_storage, __x._M_end_of_storage);
    }


    std::allocator<value_type> _M_alloc;
    value_type* _M_start;
    value_type* _M_finish;
    value_type* _M_end_of_storage;
};






















#endif //VECTOR_VECTOR_H
