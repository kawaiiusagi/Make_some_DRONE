//fr : file read
//fw : file write
//fa : file append
//파일 면저 헤더 다음!
void fprint01(FILE* fw, Node* head) //Node_h는 헤더
{
	Node* temp;
	temp = head;
	while (temp->rlink != NULL)
	{
		temp=temp->rlink;
	}
	if(temp!=NULL)
	{
		fprintf(fw,"%d %d\n",temp->x,temp->y);
		fprintf(stdout,"%d %d\n",temp->x,temp->y);
	}
}

void read01()
{
	FILE* fr,* fw;
	int x,y;
	fr=fopen("01.txt","r");
	fw=fopen("02.txt","w");
	while(fscanf)
}


void fprint02(Node_h* Hnode, Node* pre) //pre는 선행 노드로 이미 써둔 노드를 가리킨다.
{
	FILE* fa;
	Node* tmp;
	tmp = Node->head;
	
	while (tmp != pre)
	{
		tmp = tmp->rlink;
	}
	
	if (tmp == pre)
	{
		while (tmp != NULL)
		{
			fprintf(fa, "%d %d\n", point->x, point->y);
		}
	}
}

void print03(FILE* fw, Calc_h* cacl); //Calc_h는 거리 및 배터리소모가 계산되어 저장된 노드의 헤더
void print04(FILE* fw, Last_h* last);
//Last_h는 Calc_h에 누적 소모 배터리, 상태, 이벤트, 안전 경로까지 있는 노드의 헤더

void read02(); //02.txt를 읽어 노드 만들기
void read03(); //03.txt를 읽어 새 노드 만들기
