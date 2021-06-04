#include <bits/stdc++.h>
#define NMAX 102

using namespace std;

ifstream fin("date.in");

string gram[NMAX][NMAX];	//aici retinem gramatica
string dpr[NMAX];
int p,np;		//np = numarul de productii

 string concat( string a, string b)	//concatenam non-terminalele unice
{
	int i;
	string r=a;
	for(i = 0; i < b.length() ; ++i)
		if(r.find(b[i]) > r.length())
			r+=b[i];
	return r;
}

 void break_gram(string a)	//separam partea dreapta a gramaticii
{
	int i;
	p=0;
	while(a.length())
	{
		i=a.find("|");
		if(i>a.length())
		{
			dpr[p++] = a;
			a="";
		}
		else
		{
			dpr[p++] = a.substr(0,i);
			a=a.substr(i+1,a.length());
		}
	}
}

 int lchomsky(string a)
{
	if(a.length()==1 && a[0]>='A' && a[0]<='Z')
		return 1;
	return 0;
}

 int rchomsky(string a)
{
	if (a.length() == 1 && a[0]>='a' && a[0] <='z')
		return 1;
	if (a.length()==2 && a[0]>='A' && a[0]<='Z' && a[1]>='A' && a[1]<='Z' )
		return 1;
	return 0;
}

 string search_prod(string p)	//returneaza un string concatenat din simboluri care pot produce stringul p
{
	int j,k;
	string r="";
	for(j = 0 ; j < np ; ++j)
	{
		k=1;
		while(gram[j][k] != "")
		{
			if(gram[j][k] == p)
			{
				r=concat(r,gram[j][0]);
			}
			k++;
		}
	}
	return r;
}

 string gen_comb(string a, string b)	//generam toate combinatiile posibile din a si b . For eg: BA * AB = {BA, BB, AA, AB}
{
	int i,j;
	string pri=a,re="";
	for(i = 0 ; i < a.length() ; ++i)
		for(j = 0 ; j < b.length() ; ++j)
		{
			pri="";
			pri=pri+a[i]+b[j];
			re=re+search_prod(pri);		//cautam sa vedem daca productiile generate pot fi create
		}
	return re;
}

int main()
{
	int i,pt,j,l,k;
	string a,str,r,pr,start;
	cout<<"\nIntroduceti simbolul de start: ";
	cin >> start;
	cout<<"\nDati numarul de productii: ";
	cin >> np;
	for(i = 0 ; i < np ; ++i)
	{
		fin >> a;
		pt=a.find("->");
		gram[i][0] = a.substr(0,pt);
		if (lchomsky(gram[i][0]) == 0)
		{
			cout<<"\nGramatica nu este in Forma Normala Chomsky!";
			abort();
		}
		a = a.substr(pt+2, a.length());
		break_gram(a);
		for(j = 0 ; j < p ; ++j)
		{
			gram[i][j+1]=dpr[j];
			if (rchomsky(dpr[j]) == 0)
			{
				cout<<"\nGramatica nu este in Forma Normala Chomsky!";
				abort();
			}
		}
	}
	string matrix[NMAX][NMAX],st;
	cout<<"\nDati cuvantul care sa fie verificat : ";
	cin >> str;
	for(i = 0 ; i < str.length() ; ++i)		//dam valori pe diagonala principala a matricii
	{
		r="";
		st = "";
		st+=str[i];
		for(j = 0 ; j < np ; ++j)
		{
			k=1;
			while(gram[j][k] != "")
			{
				if(gram[j][k] == st)
				{
					r=concat(r,gram[j][0]);
				}
				k++;
			}
		}
		matrix[i][i]=r;
	}
	int ii,kk;
	for(k = 1 ; k < str.length() ; ++k)		//dam valori pe jumatatea superioara a matricii
	{
		for(j = k ; j < str.length() ; ++j)
		{
			r="";
			for(l = j-k ; l < j ; ++l)
			{
				pr = gen_comb(matrix[j-k][l],matrix[l+1][j]);
				r = concat(r,pr);
			}
			matrix[j-k][j] = r;
		}
	}

	for(i = 0 ; i < str.length() ; ++i)	//Afisam matricea
	{
		k=0;
		l=str.length()-i-1;
		for(j = l ; j < str.length() ; ++j)
		{
			cout<<setw(5)<<matrix[k++][j]<<" ";
		}
		cout<<endl;
	}

	int f=0;
	for(i = 0 ; i < start.length() ; ++i)
		if(matrix[0][str.length()-1].find(start[i]) <= matrix[0][str.length()-1].length())	//Verificam daca ultimul element de pe primul rand contine simbolul de start
		{
			cout<<"Cuvantul poate fi generat\n";
			return 0;
		}
	cout<<"Cuvantul nu poate fi generat\n";
	return 0;
}
