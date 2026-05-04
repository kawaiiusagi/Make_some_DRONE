#include "drone.h"

int main()
{
	input_sys();

	read_list();
	show_nodes(head);
	read_node();
	build_dist_list();
	print_dist_list();
	return 0;
}
