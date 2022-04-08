#include <iostream>

using namespace std;

typedef struct apex
{
	apex* parent = nullptr;
	apex* brother = nullptr;
	apex* son = nullptr;
	int val;
};

int wczytaj_rozgalezienie(apex* parent, char* buf, int nr_znaku)
{
	apex* previous_child = nullptr;

	while (true)
	{
		if (buf[nr_znaku] == '(')
		{
			apex* child = (apex*)calloc(1, sizeof(apex));

			child->parent = parent;
			if (previous_child != nullptr)
			{
				previous_child->brother = child;
			}
			else
			{
				parent->son = child;
			}

			previous_child = child;
			nr_znaku = wczytaj_rozgalezienie(child, buf, nr_znaku + 1);
		}
		else if (buf[nr_znaku] == ',')
		{
			//nothing to do
		}
		else if (buf[nr_znaku] == ')')
		{
			break;
		}
		else if (buf[nr_znaku] >= '0' && buf[nr_znaku] <= '9')
		{
			int wczytywana = 0;

			do {
				wczytywana *= 10;
				wczytywana += buf[nr_znaku] - '0';
				nr_znaku++;
			} while (buf[nr_znaku] >= '0' && buf[nr_znaku] <= '9');
			nr_znaku--;
			apex* child = (apex*)calloc(1, sizeof(apex));
			child->parent = parent;
			if (previous_child != nullptr)
			{
				previous_child->brother = child;
			}
			else
			{
				parent->son = child;
			}
			previous_child = child;
			child->val = wczytywana;
		}
		nr_znaku++;
	}
	return nr_znaku;
}

int nazwij_wewnetrzne_i_podaj_rozmiar(apex* drzewo, int* y, int wierzch)
{
	apex* x = drzewo;

	if (x->val == 0)
	{
		(*y)--;
		x->val = *y;
	}

	//	cout << x->val << " ";

	while (true)
	{
		if (x->son != nullptr)
		{
			x = x->son;
			wierzch = nazwij_wewnetrzne_i_podaj_rozmiar(x, y, wierzch);
			x = x->parent;
		}
		if (x->brother != nullptr)
		{
			x = x->brother;
			wierzch = nazwij_wewnetrzne_i_podaj_rozmiar(x, y, wierzch);
			x = x->parent;
		}
		wierzch++;
		break;
	}
	return wierzch;
}

int wydziel_liscie(apex* drzewo, int* tab, int roz, int index)
{
	apex* x = drzewo;
	if (x->val > 0)
	{
		tab[index] = x->val;
		index++;
	}
	while (true)
	{
		if (x->son != nullptr)
		{
			x = x->son;
			index = wydziel_liscie(x, tab, roz, index);
			x = x->parent;
		}
		if (x->brother != nullptr)
		{
			x = x->brother;
			index = wydziel_liscie(x, tab, roz, index);
			x = x->parent;
		}
		break;
	}
	return index;
}

void uzupelnij(int* tab, int roz)
{
	int val = 0;
	for (int i = 0; i < roz; i++)
	{
		if (tab[i] == 0)
		{
			val--;
			tab[i] = val;
		}
	}
}

int stworz_naglowek(apex* drzewo, int* tab, int roz, int index)
{
	wydziel_liscie(drzewo, tab, roz, index);
	uzupelnij(tab, roz);
	return index;
}

apex* find_apex(apex* drzewo, int val, apex* found)
{
	apex* x = drzewo;
	if (x->val == val)
	{
		return x;
	}


	while (true)
	{
		if (x->son != nullptr)
		{
			x = x->son;
			found = find_apex(x, val, found);
			x = x->parent;
		}
		if (x->brother != nullptr)
		{
			x = x->brother;
			found = find_apex(x, val, found);
			x = x->parent;
		}
		break;
	}
	if (found != nullptr)
	{
		return found;
	}
	return nullptr;
}

int* get_children(apex* tree, int val, apex* found, int* header)
{
	apex* node = find_apex(tree, val, found);
	int childs = 0;
	node = node->son;
	childs++;
	do
	{
		node = node->brother;
		childs++;

	} while (node->brother != nullptr);

	node = node->parent;
	header = (int*)calloc(childs, sizeof(int));
	node = node->son;

	for (int i = 1; i <= childs; i++)
	{
		header[i] = node->val;
		node = node->brother;
	}

	header[0] = childs;

	return header;
}

int oblicz_tabelke(int** tab)
{
	int result = 0;
	int size = tab[0][0];
	//	cout << size;

	if (size == 3)
	{
		int x = tab[1][1] + tab[2][2];
		int y = tab[1][2] + tab[2][1];
		if (y > x) result = y;
		else result = x;
	}
	else
	{
		//tab[0][0]--;
		int** newtab = (int**)calloc(size - 1, sizeof(int*));
		for (int i = 0; i < size - 1; i++)
		{
			newtab[i] = (int*)calloc(size - 1, sizeof(int));
		}


		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size - 1; j++) {
				newtab[i][j] = tab[i][j];
			}
		}
		newtab[0][0] = tab[0][0] - 1;
		//cout << newtab[0][0];
		int v = oblicz_tabelke(newtab) + tab[size - 1][size - 1];
		for (int i = 1; i < size - 1; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (j > 1)
				{
					newtab[i - 1][j - 1] = tab[i - 1][j - 1];
				}
				if (j != size - 1)
				{
					newtab[i][j] = tab[size - 1][j];
				}

			}

			int tmp = oblicz_tabelke(newtab) + tab[i][size - 1];
			if (tmp > v) v = tmp;
		}

		result = v;

		for (int i = 0; i < size - 1; i++)
		{
			free(newtab[i]);
		}
		free(newtab);

	}

	return result;

}


void stworz_tabele(apex* treeA, apex* treeB, int** tab, int* headA, int* headB, int sizeA, int sizeB)
{
	for (int a = 0; a < sizeA; a++)
	{
		tab[a + 1][0] = headA[a];
	}
	for (int b = 0; b < sizeB; b++)
	{
		tab[0][b + 1] = headB[b];
	}

	int inA = tab[sizeA][0] * (-1);
	int inB = tab[0][sizeB] * (-1);

	int ile_lisci = sizeA - inA;


	for (int a = 1; a <= sizeA; a++) {
		for (int b = 1; b <= sizeB; b++) {
			if (tab[a][0] > 0 && tab[0][b] && tab[a][0] == tab[0][b])
			{
				tab[a][b] = 1;
			}
			else if (tab[a][0] < 0 && tab[0][b]>0)
			{
				apex* found = find_apex(treeA, tab[a][0], nullptr);
				apex* found2 = find_apex(found->son, tab[0][b], nullptr);
				if (found2 != nullptr)
				{
					tab[a][b] = 1;
				}
			}
			else if (tab[a][0] > 0 && tab[0][b] < 0)
			{
				apex* found = find_apex(treeB, tab[0][b], nullptr);
				apex* found2 = find_apex(found->son, tab[a][0], nullptr);
				if (found2 != nullptr)
				{
					tab[a][b] = 1;
				}
			}

		}
	}

	int** headersA = (int**)calloc(inA + 1, sizeof(int*));
	int** headersB = (int**)calloc(inB + 1, sizeof(int*));

	for (int a = sizeA, i = inA; a > ile_lisci; a--, i--)
	{
		headersA[i] = get_children(treeA, tab[a][0], nullptr, headersA[i]);
	}
	for (int b = sizeB, i = inB; b > ile_lisci; b--, i--)
	{
		headersB[i] = get_children(treeB, tab[0][b], nullptr, headersB[i]);
	}


	for (int i = inA; i >= 1; i--) {
		for (int j = inB; j >= 1; j--)
		{
			int width = headersA[i][0] + 1;
			int length = headersB[j][0] + 1;


			int Xkomorki = ile_lisci + i, Ykomorki = ile_lisci + j;


			int square;
			if (width > length) square = width;
			else square = length;


			int** tabelka;
			tabelka = (int**)calloc(square, sizeof(int*));


			for (int k = 0; k < square; k++)
			{
				tabelka[k] = (int*)calloc(square, sizeof(int));
			}

			tabelka[0][0] = square;

			for (int a = 1; a < width; a++)
			{
				tabelka[a][0] = headersA[i][a];
				for (int x = 1; x <= sizeA; x++)
				{
					if (tabelka[a][0] == tab[x][0])
					{
						tabelka[a][0] = x;
						break;
					}
				}
			}
			for (int b = 1; b < length; b++)
			{
				tabelka[0][b] = headersB[j][b];
				for (int x = 1; x <= sizeB; x++)
				{
					if (tabelka[0][b] == tab[0][x])
					{
						tabelka[0][b] = x;
						break;
					}
				}

			}
			for (int a = 1; a < width; a++)
			{
				for (int b = 1; b < length; b++)
				{
					int x = tabelka[a][0];
					int y = tabelka[0][b];

					tabelka[a][b] = tab[x][y];
				}
			}

			/*for (int x = 0; x < square; x++)
			{
				for (int y = 0; y < square; y++)
				{
					cout << tabelka[x][y] << "\t|";
				}
				cout << endl;
			}*/

			tab[Xkomorki][Ykomorki] = oblicz_tabelke(tabelka);

			int najwyzsza = 0;
			for (int a = 1; a < width; a++)
			{
				int wartosc = 0;
				int p = tabelka[a][0];
				//	cout << p << " ";
				wartosc = tab[p][Ykomorki];

				if (wartosc > najwyzsza) najwyzsza = wartosc;

				//	cout << xx << " ";
			}
			for (int a = 1; a < length; a++)
			{
				int wartosc = 0;
				int p = tabelka[0][a];
				//	cout << p << " ";
				wartosc = tab[Xkomorki][p];
				//	cout << wartosc << " ";
				if (wartosc > najwyzsza) najwyzsza = wartosc;
			}

			if (najwyzsza > tab[Xkomorki][Ykomorki]) tab[Xkomorki][Ykomorki] = najwyzsza;



			//cout << endl;
			for (int z = 0; z < square; z++)
			{
				free(tabelka[z]);
			}
			free(tabelka);
		}
	}

	int result = ile_lisci - tab[ile_lisci + 1][ile_lisci + 1];
	printf("%d \n", result);
	free(headersA);
	free(headersB);

}

int main()
{

	int liczba_drzew;
	cin >> liczba_drzew;


	apex* trees = (apex*)calloc(liczba_drzew, sizeof(apex));
	int** naglowki = (int**)calloc(liczba_drzew, sizeof(int*));
	int* size = (int*)calloc(liczba_drzew, sizeof(int));
	char buffer[2000];

	for (int i = 0; i < liczba_drzew; i++)
	{
		cin >> buffer;
		wczytaj_rozgalezienie(&trees[i], buffer, 1);

		int wewnetrzne = 0;

		size[i] = nazwij_wewnetrzne_i_podaj_rozmiar(&trees[i], &wewnetrzne, 0);

		naglowki[i] = (int*)calloc(size[i], sizeof(int));
		stworz_naglowek(&trees[i], naglowki[i], size[i], 0);
		//	cout << endl;

		/*	for (int j = 0; j < size[i]; j++)
			{
				cout << naglowki[i][j] << " ";
			}*/

	}

	//	cout << endl;

	for (int a = 0; a < liczba_drzew - 1; a++)
	{
		for (int b = a + 1; b < liczba_drzew; b++) {

			int** tabela = (int**)calloc(size[a] + 1, sizeof(int));
			for (int x = 0; x <= size[a]; x++)
			{
				tabela[x] = (int*)calloc(size[b] + 1, sizeof(int));
			}

			stworz_tabele(&trees[a], &trees[b], tabela, naglowki[a], naglowki[b], size[a], size[b]);

			/*for (int x = 0; x <= size[a]; x++)
			{
				for (int y = 0; y <= size[b]; y++)
				{
					cout << tabela[x][y] << "\t|";
				}
				cout << endl;
			}*/

			cout << endl;
			for (int i = 0; i <= size[a]; i++)
			{
				free(tabela[i]);
			}
			free(tabela); // + zwolnienie kazdej po kolei
		}
	}


	/*for (int i = 0; i < liczba_drzew; i++)
	{

	}*/

	free(size);
	for (int i = 0; i < liczba_drzew; i++)
	{
		free(naglowki[i]);
	}
	free(naglowki); // + zwolnienie kazdej po kolei
	free(trees); // zwalnianie lisci i wierzcholkow

	return 0;

}