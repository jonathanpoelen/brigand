#include <brigand/sequences/filled_list.hpp>

void filled_test() {

brigand::filled_list<int, 0> l0 = brigand::list<>{};
brigand::filled_list<int, 1> l1 = brigand::list<int>{};
brigand::filled_list<int, 2> l2 = brigand::list<int, int>{};
brigand::filled_list<int, 3> l3 = brigand::list<int, int, int>{};
brigand::filled_list<int, 4> l4 = brigand::list<int, int, int, int>{};
brigand::filled_list<int, 5> l5 = brigand::list<int, int, int, int, int>{};
brigand::filled_list<int, 6> l6 = brigand::list<int, int, int, int, int, int>{};
brigand::filled_list<int, 7> l7 = brigand::list<int, int, int, int, int, int, int>{};

}
