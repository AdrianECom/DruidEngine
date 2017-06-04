#ifndef LIST_H_
#define LIST_H_

#include "BaseList.h"
#include "BasicTypes.h"
#include "Allocator.h"
#include "Assert.h"
#include "Debug.h"


namespace DE {

template <class T>
class List : public BaseList {

public:

  // template <class T>
  class Iterator : public BaseIterator {
  public:

    Iterator () : BaseIterator(){ };

    Iterator(const BaseIterator& other) : BaseIterator(other){ };

    virtual ~Iterator (){ };
    virtual T& get(){
      return *(static_cast<T*>(BaseIterator::get()));
    };

  };

  List() : BaseList(){

  };

  virtual ~List(){};

  void init(const List& other){
    // TODO copy constructor
  };

  void init() {
    BaseList::init(sizeof(T));
  };

  Iterator getIterator() const {
    return Iterator(BaseList::getIterator());
  };

  Iterator getRevIterator() const {
    return Iterator(BaseList::getRevIterator());
  };

  void pushFront(T element){
    BaseList::pushFront(&element);
  };

  void pushBack(T element){
    BaseList::pushBack(&element);
  };

  T& popFront(){
    return *(static_cast<T*>(BaseList::popFront()));
  };

  T& popBack(){
    return *(static_cast<T*>(BaseList::popBack()));
  };

  T& get(u32 index) const{
    return *(static_cast<T*>(BaseList::get(index)));
  };

  void remove(u32 index){
    BaseList::remove(index);
  };

  void remove(Iterator& it){
    BaseList::remove(it);
  };

  void insert(u32 index, T element){
    BaseList::insert(index, &element);
  };

  void insert(Iterator& it, T element){
    BaseList::insert(it, &element);
  };



};

} /* namespace DE */

#endif /* LIST_H_ */
