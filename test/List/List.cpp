#include "Druid.h"

using namespace DE;

int main() {


	test(DE::List<u32>);

	DE::LinearAllocator linear;
	linear.init(1024);

	DE::List<u32>* list = DE::allocate<List<u32>>(linear);
	list->init();

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	DE::List<u32>* list2 = DE::allocate<List<u32>>(linear);
	list2->init(*list);

	expected_uint(list2->getLength(),3);

	expected_uint(list2->popBack(),2);
	expected_uint(list2->popBack(),1);
	expected_uint(list2->popBack(),0);

	expected_uint(list2->getLength(),0);

	expected_uint(list->popBack(),2);
	expected_uint(list->popBack(),1);
	expected_uint(list->popBack(),0);

	expected_uint(list->getLength(),0);

	list->pushFront(0);
	list->pushFront(1);
	list->pushFront(2);

	expected_uint(list->getLength(),3);

	expected_uint(list->get(2),0);
	expected_uint(list->get(1),1);
	expected_uint(list->get(0),2);

	expected_uint(list->popFront(),2);
	expected_uint(list->popFront(),1);
	expected_uint(list->popFront(),0);

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	list->remove(1);

	expected_uint(list->getLength(),2);

	list->remove(0);
	list->remove(0);

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	list->insert(0,3);
	list->insert(3,4);

	expected_uint(list->getLength(),5);

	expected_uint(list->get(0),3);
	expected_uint(list->get(1),0);
	expected_uint(list->get(2),1);
	expected_uint(list->get(3),4);
	expected_uint(list->get(4),2);

	list->clear();

	expected_uint(list->getLength(),0);

	list->pushBack(0);
	list->pushBack(1);
	list->pushBack(2);

	expected_uint(list->getLength(),3);

	List<u32>::Iterator it = list->getIterator();

	expected_uint(it.get(),0);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),1);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),2);
	expected_bool(it.hasNext(),false);

	it.setReverse(true);

	expected_uint(it.get(),2);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),1);
	expected_bool(it.hasNext(),true);
	it.next();
	expected_uint(it.get(),0);
	expected_bool(it.hasNext(),false);


	list->clear();

	expected_uint(list->getLength(),0);



	linear.reset();

	summary();

	return 0;
}
