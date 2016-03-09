#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
#include <algorithm>

using namespace std;

const int PDBR = 0x220;

int a[0x80*0x20];

void solve(int x)
{
	int pde_index = x>>10;
	int pde_contents = a[PDBR+pde_index];
	int pde_valid = pde_contents>>7;
	int pde_pfn = pde_contents&((1<<7)-1);
	printf("Virtual Address %04x:\n", x);
	printf("  --> pde index:0x%02x  pde contents:(valid %d, pfn 0x%02x)\n", pde_index, pde_valid, pde_pfn);
	if (!pde_valid)
	{
		printf("    --> Fault (page directory entry not valid)\n");
		return;
	}
	int pte_index = (x>>5)&((1<<5)-1);
	int pte_contents = a[pde_pfn<<5|pte_index];
	int pte_valid = pte_contents>>7;
	int pte_pfn = pte_contents&((1<<7)-1);
	printf("    --> pte index:0x%02x  pte contents:(valid %d, pfn 0x%02x)\n", pte_index, pte_valid, pte_pfn);
	if (!pte_valid)
	{
		printf("      --> Fault (page table entry not valid)\n");
		return;
	}
	int address = (pte_pfn<<5)|(x&((1<<5)-1));
	printf("      --> Translates to Physical Address 0x%x --> Value: 0x%x\n", address, a[address]);
}

int main()
{
	freopen("data.txt", "r", stdin);
	freopen("result.txt", "w", stdout);
	for (int i=0; i<0x80; ++i)
	{
		char s[11],t[11];
		scanf("%s %s:", s, t);
		for (int j=0; j<0x20; ++j)
			scanf("%x", &a[i*0x20+j]);
	}
	solve(0x6c74);
	solve(0x6b22);
	return 0;
}
