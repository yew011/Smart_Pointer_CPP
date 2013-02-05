#include <iostream>
#include <cassert>
using namespace std;

/* NOTE: the current implementation can only be used to single-pointer dereferencing,
 * cannot dereference array pointer. (that require overloading the [] and + function)
 */
template<class T>
class Smart_Pointer_{
private:
  T * target_;
  unsigned int * reference_;

public:
  // the constructor ordinary constructor.
  // **** should Allow NULL
  Smart_Pointer_( T * object_=NULL ){
    this->target_ = object_;
    if( object_ == NULL )
      this->reference_ = NULL;
    else
      this->reference_ = new unsigned int(1);
  }

  // overload the copy constructor.
  // **** when can only pass by reference, should increment the counter,
  // **** also consider the case when the same pointer is assigned twice, if they are the same pointers => ignore the passed in reference
  // **** 
  Smart_Pointer_( Smart_Pointer_< T >  & object_ ){
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

  // overload the operator ++.
  // overload the operator --.

  // overload the assignment operator =.
  // **** test if the current pointer is this to-be-assigned pointer
  // **** there should be test for NULL, case, => equivalent to disabling the pointer
  // **** 
  void operator=( Smart_Pointer_< T > & object_ ){
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
    if( this->reference_ != 0 )
      this->reset();
  }

  // reset function reset the 
  void reset(){
    if( this->reference_ != NULL )
      *(this->reference_) = *(this->reference_) - 1;
    if( this->reference_ == 0 && this->reference_ != NULL){
      delete this->target_;
      delete this->reference_;
    }
    this->reference_ = NULL;
    this->target_ = NULL;
  }

  // define the getter for target_
  T* get_target_(){ return this->target_; }

  // define the getter for target_
  unsigned int * get_reference_(){ return this->reference_; }
};

int main( int argc, char ** argv ){
  
  typedef struct test_struct_{
    int int_;
    string str_;
  }struct_;

  class test_class_{
  public:
    double double_;
    string str_;
  };

  // the passed in pointer must be created by 'new operator'.
  // the copy constructor argument must be a Smart_Pointer_ object of same type.

  /* design of test cases:
  * 1. Test the creation of smart pointer
  * 2. Test the copy constructor of the smart pointer
  * 3. Test the indirection of primitive type, string, struct, and class object
  * 4. Test the dereference of the primitive type, string, struct and class object
  * 5. Test the = sign operator, check the update of reference_
  * 6. Test the reset function, check the set of reference_ to null
  * 7. Test the NULL pointer
  */

  // 1. test creation of smart pointer,
  Smart_Pointer_<int>    ptr_test1_( new int(5) );
  Smart_Pointer_<double> ptr_test2_( new double(1.0) );
  Smart_Pointer_<char>   ptr_test3_( new char('a') );
  Smart_Pointer_<string> ptr_test4_( new string("abcdefg") );
  Smart_Pointer_<test_class_ > ptr_test5_( new test_class_() );
  Smart_Pointer_<struct_> ptr_test6_( new struct_ );

  assert( *ptr_test1_==5 );
  assert( *ptr_test2_==1.0 );
  assert( *ptr_test3_=='a' );
  assert( *ptr_test4_=="abcdefg" );
  (*ptr_test5_).double_ = 1.0;
  (*ptr_test5_).str_ = "abcdefg";
  assert( (*ptr_test5_).double_ == 1.0 && 
	  (*ptr_test5_).str_ == "abcdefg" );
  (*ptr_test6_).int_ = 5;
  (*ptr_test6_).str_ = "abcdefg";
  assert( (*ptr_test6_).int_ == 5 && 
	  (*ptr_test6_).str_ == "abcdefg" );

  cout << "PASSED: [1] Constructor for the primitive types, struct and class objects." << endl;

  // 2. test the copy constructor of the smart pointer
  Smart_Pointer_<int>    ptr_test1_dup_( ptr_test1_ );
  Smart_Pointer_<double> ptr_test2_dup_( ptr_test2_ );
  Smart_Pointer_<char>   ptr_test3_dup_( ptr_test3_ );
  Smart_Pointer_<string> ptr_test4_dup_( ptr_test4_ );
  Smart_Pointer_<test_class_ > ptr_test5_dup_( ptr_test5_ );
  Smart_Pointer_<struct_> ptr_test6_dup_( ptr_test6_ );
  
  assert( *ptr_test1_dup_==5 );
  assert( *(ptr_test1_dup_.get_reference_())==2 );
  assert( *ptr_test2_dup_==1.0 );
  assert( *(ptr_test2_dup_.get_reference_())==2 );
  assert( *ptr_test3_dup_=='a' );
  assert( *(ptr_test3_dup_.get_reference_())==2 );
  assert( *ptr_test4_dup_=="abcdefg" );
  assert( *(ptr_test4_dup_.get_reference_())==2 );
  assert( (*ptr_test5_dup_).double_ == 1.0 && 
	  (*ptr_test5_dup_).str_ == "abcdefg" );
  assert( *(ptr_test5_dup_.get_reference_())==2 );
  assert( (*ptr_test6_dup_).int_ == 5 && 
	  (*ptr_test6_dup_).str_ == "abcdefg" );
  assert( *(ptr_test6_dup_.get_reference_())==2 );

  cout << "PASSED: [2] Copy constructor for the primitive types, struct and class object." << endl;

  // 3. Test the indirection of primitive type, string, struct, and class object
  // done in the previous tests
  cout << "PASSED: [3] Indirection of the primitive types, struct and class object." << endl;

  // 4. Test the dereference of the struct and class object
  assert( ptr_test5_dup_->double_ == 1.0 && 
	  ptr_test5_dup_->str_ == "abcdefg" );
  assert( ptr_test6_dup_->int_ == 5 && 
	  ptr_test6_dup_->str_ == "abcdefg" );
  cout << "PASSED: [4] Dereference of the struct and class object." << endl;


  return 0;
}
