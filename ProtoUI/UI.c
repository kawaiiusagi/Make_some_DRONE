

void droneMove(Node* head, Node*dest)
{
	if (head == NULL)
		return;
	if (dest == NULL)
		return;
	printf("o-o \n");

	Node* temp = head;
	Node* val = temp;
	printf(" %s",val->content);
	do
	{
		
		printf("-------");
		val = val->rlink;
		printf("%s", val->content);

	} while (val->rlink != NULL);

	printf("\n\n");
	val = temp;
  
	int spaces = 0;
	val = head;
	while (val != dest)
	{
		spaces += NODE_WIDTH;
		val = val->rlink;
	}
	printf("%*so-o \n", spaces, "");
	val = temp;
	printf(" %s", val->content);
	do
	{

		printf("-------");
		val = val->rlink;
		printf("%s", val->content);

	} while (val->rlink != NULL);

}
