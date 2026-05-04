#include "drone.h"

int main()
{
	start_UI();

	input_sys();

	read_list();

	read_node();

	build_dist_list();
	
	print_dist_list();

	fread03();
	reporting();
	return 0;
}
