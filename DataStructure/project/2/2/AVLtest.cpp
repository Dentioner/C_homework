#include "AVLtreeTemplates.h"
using namespace std;
int main()
{
	AVLTree<int> s, t;
	for (int i = 0; i < 100; ++i)
		s.insert(i);

	for (int i = 0; i < 20; ++i)
		t.insert(i);

	s.merge(t);
	s.show();
	s.deleteNode(s.top());
	s.show();
	return 0;
}