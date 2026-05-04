

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

//o-o
// A---------B-------....
//
//			o-o
//A	---------B-----.......
//*/
//
//

void printDistance(Node* head)
{
	if (head == NULL)
		return;
	Node* temp = head;
	printf("연결리스트\n");
	printf("%s(%d,%d)", temp->content, temp->x, temp->y);
	do
	{
		printf(" - ");
		printf("%s(%d,%d)", temp->rlink->content, temp->rlink->x, temp->rlink->y);
		temp = temp->rlink;
	}while(temp->rlink != NULL);

	printf("\n\n");

	temp = head;

	while (temp->rlink != NULL)
	{
		printf("%s(%d,%d)", temp->content, temp->x, temp->y);
		printf(" ---(x->%d, y->%d)---> ", temp->rlink->x - temp->x, temp->rlink->y - temp->y);
		temp = temp->rlink;
		printf("%s(%d,%d)\n", temp->content, temp->x, temp->y);
	}
} 
/*printDistance(Node* ls);	
연결리스트 A(0, 0) - B(100, 40) - .....

A(0,0)		-----(x-> 100, y -> 40) ----> B(100, 40)

B(100, 40)  -----(x -> 30, y -> 30)-----> C(130, 70)

....*/
