#include <cstdio>
#include <cstdlib>
#include <ctime>
#define MAXPA 4
#define MAXVA 6
#define ROUND 10
using namespace std;

struct page
{
	int num;
	bool access;
	bool modify;
}mem[MAXPA];

int head;

void printStates()
{
	for (int i = 0; i < MAXPA; i ++)
	{
		printf("%d: Page %d (A:%d  M:%d)\n", i, mem[i].num, mem[i].access, mem[i].modify);
	}
	printf("\n");
}

void accessPage(int va, int opType)
{
	printf("Access page: %d ", va);
	if (opType == 0)
	{
		printf("(read)\n");
	}
	else if (opType == 1)
	{
		printf("(write)\n");
	}

	// Find target
	for (int i = 0; i < MAXPA; i ++)
	{
		if (va == mem[i].num)
		{
			printf("--> Hit at %d\n", i);
			mem[i].access = true;
			if (opType == 1)
			{
				mem[i].modify = true;
			}
			printStates();
			return;
		}
	}

	// If miss
	printf("--> Miss\n");
	while (mem[head].access || mem[head].modify)
	{
		if (mem[head].access && mem[head].modify)
		{
			mem[head].modify = true;
		}
		else
		{
			if (mem[head].modify)
			{
				printf("--> modify page %d in disk\n", head);
			}
			mem[head].modify = false;
		}
		mem[head].access = false;
		head = (head + 1) % MAXPA;
	}
	printf("--> Replace at address %d\n", head);
	mem[head].num = va;
	head = (head + 1) % MAXPA;
	printStates();
}

int main()
{
	//initialize
	srand((int)time(NULL));
	head = 0;
	for (int i = 0; i < MAXPA; i ++)
	{
		mem[i].num = i;
		mem[i].access = false;
		mem[i].modify = false;
	}
	printf("Initial states:\n");
	printStates();

	//randomly access
	for (int i = 0; i < ROUND; i ++)
	{
		int vp = rand() % MAXVA;
		int op = rand() % 2; // 1 - write, 0 - read
		accessPage(vp, op);
	}

	return 0;
}
