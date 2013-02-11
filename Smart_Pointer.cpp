#include <iostream>
#include <cassert>
#include <exception>
using namespace std;

/*
 * Define a NULL pointer exception for testing purpose.
*/
class NULL_Pointer_Exception_ : public exception{
  virtual const char * what() const throw(){
    return "NULL Pointer Exception";
  }
} null_ptr_exception_;

/* NOTE: the current implementation can only be used to single-pointer dereferencing,
 * cannot dereference array pointer. (that require overloading the [] and ++ function)
 */
template<class T>
class Smart_Pointer_{
private:
  T * target_;
  unsigned int * reference_;

public:
  // the constructor ordinary constructor.
  Smart_Pointer_( T * object_=NULL ){
    this->target_ = object_;
    if( object_ == NULL )
      this->reference_ = NULL;
    else
      this->reference_ = new unsigned int(1);
  }

  // overload the copy constructor.
  Smart_Pointer_( Smart_Pointer_< T >  & object_ ){
    if ( object_.get_target_() == this->target_ ) return;
    this->target_ = object_.get_target_();
    this->reference_ = object_.get_reference_();
    (*this->reference_)++;
  }  

  // overload the indirection operator *
  T& operator*(){
    if( this->target_ == NULL ){
      throw null_ptr_exception_;
    }
    return *(this->target_);
  }

  // overload the dereference operator ->
  T* operator->(){
    if( this->target_ == NULL ){
      throw null_ptr_exception_;
    }
    return this->target_;
  }

  // for array pointer
  // overload the operator ++.
  // overload the operator --.

  // overload the assignment operator =.
  Smart_Pointer_< T >& operator=( Smart_Pointer_< T > & object_ ){
    if ( object_.get_target_() == this->target_ ) return *this;
    // disable the current pointer.
    this->reset();
    // update the new pointer.
    this->target_ = object_.get_target_();
    this->reference_ = object_.get_reference_();
    *(this->reference_) += 1;
    return *this;
  }

  // overload the desctructor.
  ~Smart_Pointer_(){
    if( this->reference_ != NULL )
      this->reset();
  }

  // reset function reset the 
  void reset(){
    if( this->reference_ == NULL && this->target_ == NULL ) return;
    *(this->reference_) = *(this->reference_) - 1;
    if( *this->reference_ == 0 && this->reference_ != NULL){
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

  typedef struct test_linkedlist_{
    Smart_Pointer_<test_linkedlist_> next_;
  }linkedlist_;

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
  * 5. Test the reset function, check the set of reference_ to null
  * 6. Test the = sign operator, check the update of reference_
  * 7. Test the NULL pointer
  */

  // 1. test creation of smart pointer,
  Smart_Pointer_<int>    ptr_test1_( new int(5) );
  Smart_Pointer_<double> ptr_test2_( new double(1.0) );
  Smart_Pointer_<char>   ptr_test3_( new char('a') );
  Smart_Pointer_<string> ptr_test4_( new string("abcdefg") );
  Smart_Pointer_<test_class_> ptr_test5_( new test_class_() );
  Smart_Pointer_<struct_> ptr_test6_( new struct_() );

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
  Smart_Pointer_<test_class_> ptr_test5_dup_( ptr_test5_ );
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

  // 5. Test the reset function, check the set of reference_ to null
  ptr_test1_dup_.reset();
  assert( ptr_test1_dup_.get_reference_() == NULL );
  assert( ptr_test1_dup_.get_target_() == NULL );
  assert( *(ptr_test1_.get_reference_()) == 1);
  ptr_test1_.reset();
  assert( ptr_test1_.get_reference_() == NULL );
  assert( ptr_test1_.get_target_() == NULL );
  ptr_test2_dup_.reset();
  assert( ptr_test2_dup_.get_reference_() == NULL );
  assert( ptr_test2_dup_.get_target_() == NULL );
  assert( *(ptr_test2_.get_reference_()) == 1);
  ptr_test2_.reset();
  assert( ptr_test2_.get_reference_() == NULL );
  assert( ptr_test2_.get_target_() == NULL );
  ptr_test3_dup_.reset();
  assert( *(ptr_test3_.get_reference_()) == 1);
  ptr_test3_.reset();
  assert( ptr_test3_dup_.get_reference_() == NULL );
  assert( ptr_test3_dup_.get_target_() == NULL );
  assert( ptr_test3_.get_reference_() == NULL );
  assert( ptr_test3_.get_target_() == NULL );
  ptr_test4_dup_.reset();
  assert( *(ptr_test4_.get_reference_()) == 1);
  assert( ptr_test4_dup_.get_reference_() == NULL );
  assert( ptr_test4_dup_.get_target_() == NULL );
  ptr_test4_.reset();
  assert( ptr_test4_.get_reference_() == NULL );
  assert( ptr_test4_.get_target_() == NULL );
  ptr_test5_dup_.reset();
  assert( *(ptr_test5_.get_reference_()) == 1);
  assert( ptr_test5_dup_.get_reference_() == NULL );
  assert( ptr_test5_dup_.get_target_() == NULL );
  ptr_test5_.reset();
  assert( ptr_test5_.get_reference_() == NULL );
  assert( ptr_test5_.get_target_() == NULL );
  ptr_test6_dup_.reset();
  assert( *(ptr_test6_.get_reference_()) == 1);
  assert( ptr_test6_dup_.get_reference_() == NULL );
  assert( ptr_test6_dup_.get_target_() == NULL );
  ptr_test6_.reset();
  assert( ptr_test6_.get_reference_() == NULL );
  assert( ptr_test6_.get_target_() == NULL );

  cout << "PASSED: [5] reset function and reset reference_, target_ to NULL" << endl;
  
  // 6. Test the = sign operator, check the update of reference_
  Smart_Pointer_<int>    ptr_test1_dup_dup_( new int(5) );
  Smart_Pointer_<double> ptr_test2_dup_dup_( new double(1.0) );
  Smart_Pointer_<char>   ptr_test3_dup_dup_( new char('a') );
  Smart_Pointer_<string> ptr_test4_dup_dup_( new string("abcdefg") );
  Smart_Pointer_<test_class_> ptr_test5_dup_dup_( new test_class_() );
  Smart_Pointer_<struct_> ptr_test6_dup_dup_( new struct_() );

  (*ptr_test5_dup_dup_).double_ = 1.0;
  (*ptr_test5_dup_dup_).str_ = "abcdefg";
  (*ptr_test6_dup_dup_).int_ = 5;
  (*ptr_test6_dup_dup_).str_ = "abcdefg";

  ptr_test1_ = ptr_test1_dup_ = ptr_test1_dup_dup_;
  ptr_test2_ = ptr_test2_dup_ = ptr_test2_dup_dup_;
  ptr_test3_ = ptr_test3_dup_ = ptr_test3_dup_dup_;
  ptr_test4_ = ptr_test4_dup_ = ptr_test4_dup_dup_;
  ptr_test5_ = ptr_test5_dup_ = ptr_test5_dup_dup_;
  ptr_test6_ = ptr_test6_dup_ = ptr_test6_dup_dup_;

  assert( *ptr_test1_==5 );
  assert( *ptr_test2_==1.0 );
  assert( *ptr_test3_=='a' );
  assert( *ptr_test4_=="abcdefg" );
  assert( (*ptr_test5_).double_ == 1.0 && 
	  (*ptr_test5_).str_ == "abcdefg" );
  assert( (*ptr_test6_).int_ == 5 && 
	  (*ptr_test6_).str_ == "abcdefg" );

  assert( *(ptr_test1_.get_reference_())==3 );
  assert( *(ptr_test2_.get_reference_())==3 );
  assert( *(ptr_test3_.get_reference_())==3 );
  assert( *(ptr_test4_.get_reference_())==3 );
  assert( *(ptr_test5_.get_reference_())==3 );
  assert( *(ptr_test6_.get_reference_())==3 );

  assert( *ptr_test1_dup_==5 );
  assert( *(ptr_test1_dup_.get_reference_())==3 );
  assert( *ptr_test2_dup_==1.0 );
  assert( *(ptr_test2_dup_.get_reference_())==3 );
  assert( *ptr_test3_dup_=='a' );
  assert( *(ptr_test3_dup_.get_reference_())==3 );
  assert( *ptr_test4_dup_=="abcdefg" );
  assert( *(ptr_test4_dup_.get_reference_())==3 );
  assert( (*ptr_test5_dup_).double_ == 1.0 && 
	  (*ptr_test5_dup_).str_ == "abcdefg" );
  assert( *(ptr_test5_dup_.get_reference_())==3 );
  assert( (*ptr_test6_dup_).int_ == 5 && 
	  (*ptr_test6_dup_).str_ == "abcdefg" );
  assert( *(ptr_test6_dup_.get_reference_())==3 );

  ptr_test1_ = ptr_test1_dup_ = ptr_test1_dup_dup_;
  ptr_test2_ = ptr_test2_dup_ = ptr_test2_dup_dup_;
  ptr_test3_ = ptr_test3_dup_ = ptr_test3_dup_dup_;
  ptr_test4_ = ptr_test4_dup_ = ptr_test4_dup_dup_;
  ptr_test5_ = ptr_test5_dup_ = ptr_test5_dup_dup_;
  ptr_test6_ = ptr_test6_dup_ = ptr_test6_dup_dup_;

  assert( *(ptr_test1_.get_reference_())==3 );
  assert( *(ptr_test2_.get_reference_())==3 );
  assert( *(ptr_test3_.get_reference_())==3 );
  assert( *(ptr_test4_.get_reference_())==3 );
  assert( *(ptr_test5_.get_reference_())==3 );
  assert( *(ptr_test6_.get_reference_())==3 );
  assert( *(ptr_test1_dup_.get_reference_())==3 );
  assert( *(ptr_test2_dup_.get_reference_())==3 );
  assert( *(ptr_test3_dup_.get_reference_())==3 );
  assert( *(ptr_test4_dup_.get_reference_())==3 );
  assert( *(ptr_test5_dup_.get_reference_())==3 );
  assert( *(ptr_test6_dup_.get_reference_())==3 );
  assert( *(ptr_test1_dup_dup_.get_reference_())==3 );
  assert( *(ptr_test2_dup_dup_.get_reference_())==3 );
  assert( *(ptr_test3_dup_dup_.get_reference_())==3 );
  assert( *(ptr_test4_dup_dup_.get_reference_())==3 );
  assert( *(ptr_test5_dup_dup_.get_reference_())==3 );
  assert( *(ptr_test6_dup_dup_.get_reference_())==3 );

  Smart_Pointer_<int>    ptr_test1_new_( new int(5) );
  Smart_Pointer_<double> ptr_test2_new_( new double(1.0) );
  Smart_Pointer_<char>   ptr_test3_new_( new char('a') );
  Smart_Pointer_<string> ptr_test4_new_( new string("abcdefg") );
  Smart_Pointer_<test_class_> ptr_test5_new_( new test_class_() );
  Smart_Pointer_<struct_> ptr_test6_new_( new struct_() );

  ptr_test1_ = ptr_test1_new_;
  ptr_test2_ = ptr_test2_new_;
  ptr_test3_ = ptr_test3_new_;
  ptr_test4_ = ptr_test4_new_;
  ptr_test5_ = ptr_test5_new_;
  ptr_test6_ = ptr_test6_new_;

  assert( *(ptr_test1_.get_reference_())==2 );
  assert( *(ptr_test2_.get_reference_())==2 );
  assert( *(ptr_test3_.get_reference_())==2 );
  assert( *(ptr_test4_.get_reference_())==2 );
  assert( *(ptr_test5_.get_reference_())==2 );
  assert( *(ptr_test6_.get_reference_())==2 );
  assert( *(ptr_test1_new_.get_reference_())==2 );
  assert( *(ptr_test2_new_.get_reference_())==2 );
  assert( *(ptr_test3_new_.get_reference_())==2 );
  assert( *(ptr_test4_new_.get_reference_())==2 );
  assert( *(ptr_test5_new_.get_reference_())==2 );
  assert( *(ptr_test6_new_.get_reference_())==2 );
  assert( *(ptr_test1_dup_.get_reference_())==2 );
  assert( *(ptr_test2_dup_.get_reference_())==2 );
  assert( *(ptr_test3_dup_.get_reference_())==2 );
  assert( *(ptr_test4_dup_.get_reference_())==2 );
  assert( *(ptr_test5_dup_.get_reference_())==2 );
  assert( *(ptr_test6_dup_.get_reference_())==2 );
  assert( *(ptr_test1_dup_dup_.get_reference_())==2 );
  assert( *(ptr_test2_dup_dup_.get_reference_())==2 );
  assert( *(ptr_test3_dup_dup_.get_reference_())==2 );
  assert( *(ptr_test4_dup_dup_.get_reference_())==2 );
  assert( *(ptr_test5_dup_dup_.get_reference_())==2 );
  assert( *(ptr_test6_dup_dup_.get_reference_())==2 );

  ptr_test1_.reset();
  ptr_test1_new_.reset();
  ptr_test2_.reset();
  ptr_test2_new_.reset();
  ptr_test3_.reset();
  ptr_test3_new_.reset();
  ptr_test4_.reset();
  ptr_test4_new_.reset();
  ptr_test5_.reset();
  ptr_test5_new_.reset();
  ptr_test6_.reset();
  ptr_test6_new_.reset();
  ptr_test1_dup_.reset();
  ptr_test1_dup_dup_.reset();
  ptr_test2_dup_.reset();
  ptr_test2_dup_dup_.reset();
  ptr_test3_dup_.reset();
  ptr_test3_dup_dup_.reset();
  ptr_test4_dup_.reset();
  ptr_test4_dup_dup_.reset();
  ptr_test5_dup_.reset();
  ptr_test5_dup_dup_.reset();
  ptr_test6_dup_.reset();
  ptr_test6_dup_dup_.reset();

  cout << "PASSED: [6] copy assignment operation and chain assignment" << endl;

  // 7. Test NULL smart pointer case, dereference/indirection will cause Exception thrown.
  Smart_Pointer_<int>    ptr_test1_null_;
  Smart_Pointer_<test_class_> ptr_test2_null_;
  Smart_Pointer_<struct_> ptr_test3_null_;

  assert( ptr_test1_null_.get_reference_() == NULL );
  assert( ptr_test1_null_.get_target_() == NULL );
  assert( ptr_test2_null_.get_reference_() == NULL );
  assert( ptr_test2_null_.get_target_() == NULL );
  assert( ptr_test3_null_.get_reference_() == NULL );
  assert( ptr_test3_null_.get_target_() == NULL );

  try{
    (*ptr_test1_null_);
    cout << "Error: [7] test the NULL pointer case : Didn't throw exception" << endl;
  }catch( NULL_Pointer_Exception_ &e ){
  }
  try{
    (*ptr_test2_null_).str_;
    cout << "Error: [7] test the NULL pointer case : Didn't throw exception" << endl;
  }catch( NULL_Pointer_Exception_ &e ){
  }
  try{
    (*ptr_test3_null_).str_;
    cout << "Error: [7] test the NULL pointer case : Didn't throw exception" << endl;
  }catch( NULL_Pointer_Exception_ &e ){
  }

  try{
    ptr_test2_null_->str_;
    cout << "Error: [7] test the NULL pointer case : Didn't throw exception" << endl;
  }catch( NULL_Pointer_Exception_ &e ){
  }
  try{
    ptr_test3_null_->str_;
    cout << "Error: [7] test the NULL pointer case : Didn't throw exception" << endl;
  }catch( NULL_Pointer_Exception_ &e ){
  }

  cout << "PASSED: [7] test the NULL pointer case" << endl;

  // 8. Test the linkedlist chained dereference/indirection
  Smart_Pointer_<linkedlist_> ptr_test_linkedlist1_( new linkedlist_() );
  Smart_Pointer_<linkedlist_> ptr_test_linkedlist2_( new linkedlist_() );

  ptr_test_linkedlist1_->next_ = ptr_test_linkedlist2_;

  assert( ptr_test_linkedlist1_->next_->next_.get_target_() == NULL );

  cout << "PASSED: [8] test the linkedlist dereference/indirection case" << endl;
  return 0;
}
