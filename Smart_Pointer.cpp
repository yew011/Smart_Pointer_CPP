#include <iostream>

using namespace std;


/* NOTE: the current implementation can only be used to single-pointer dereferencing,
 * cannot dereference array pointer. (that require overloading the [] and + function)
 */
template<class T>
class Smart_Pointer_{
private:
  T * target_;
  unsigned int * reference_ = 0;

public:
  // the constructor ordinary constructor.
  // **** should Allow NULL
  Smart_Pointer_( T * object_=NULL ){
    this->target_ = object_;
    this->reference_ = new int(1);
  }

  // overload the copy constructor.
  // **** when can only pass by reference, should increment the counter,
  // **** also consider the case when the same pointer is assigned twice, if they are the same pointers => ignore the passed in reference
  // **** 
  Smart_Pointer_( T & object_ ){
    if ( object_.get_target_() == this->target_ ) return;
    this->target_ = object_.get_target_();
    this->reference_ = object_.get_reference_();
    (*this->reference_)++;
  }  

  // overload the indirection operator *.
  T& operator*(){
    if( this->target_ == NULL ){
      cout << "indirection from NULL pointer." << endl;
      exit( -1 );
    }
    return *(this->target_);
  }

  // overload the dereference operator ->.
  T* operator->(){
    if( this->target_ == NULL ){
      cout << "indirection from NULL pointer." << endl;
      exit( -1 );
    }
    return this->target_;
  }

  // overload the assignment operator =.
  // **** test if the current pointer is this to-be-assigned pointer
  // **** there should be test for NULL, case, => equivalent to disabling the pointer
  // **** 
  void operator=( T & object_ ){
    if ( object_.get_target_() == this->target_ ) return;
    // disable the current pointer.
    this->target_.reset();
    // update the new pointer.
    this->target_ = object_.get_target_();
    this->reference_ = object_.get_reference_();
    *(this->reference_)++;
  }

  // overload the desctructor.
  // **** decrement the reference count, => free the object if count=0
  // ****
  ~Smart_Pointer_(){
    if( this->reference != 0 )
      this->reset();
    if( this->reference == 0 )
      delete this->reference_;
  }

  // reset function reset the 
  void reset(){
    this->target_ = NULL;
    if( this->reference_ != NULL )
      *(this->reference_)--;
  }

  // define the getter for target_
  T* get_target_(){ return this->target_; }

  // define the getter for target_
  T* get_reference_(){ return this->reference_; }
};