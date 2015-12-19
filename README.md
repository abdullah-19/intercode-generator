# intercode-generator

## Usage EquationTree
``` c++
int _tmain()
{
	_wsetlocale(LC_ALL, L"korean");

	std::locale::global(std::locale("kor"));
	std::wcout.imbue(std::locale("kor"));
	std::wcin.imbue(std::locale("kor"));

	auto cout_element = [] (const WString& str) {
		std::wcout << str;
	};

	auto tree_test = [&] (const wchar_t* fomula) {
		EquationTree<oprator_priority> pr;
		BinaryTree<WString> ttree = pr.start(fomula);
		
		ttree.postorder_travel(cout_element); std::wcout << std::endl;
		//ttree.preorder_travel([](const WString& str) {std::wcout << str; }); std::wcout << std::endl;
		//ttree.inorder_travel([](const WString& str) {std::wcout << str; }); std::wcout << std::endl;

		std::vector<WString> a = _intercode_generator2(ttree);
		for (auto st : a)
			std::wcout << st << std::endl;
		
		std::wcout << std::endl;
	};
	
	tree_test(L"y+y*(y-z)*z+z*sin(x-y)");
	tree_test(L"(1-y)*(6-z)/(7-x)");
	tree_test(L"1+x&&y+2");
	tree_test(L"a+(a&&b)+(a&&b&c)+(a&&b&c<d)+(a&&b&c<d<<e)+(a&&b&c<d<<e%f)"
              L"+(a&&b&c<d<<e%f+g)+(a&&b&c<d<<e%f+g*h)");
	tree_test(L"a*l&b%z==k+i&&z>>j");
	tree_test(L"(A >= 0 && A < 9) || (A >= '0' && A < '9')");
	tree_test(L"3+4*(5+6)");
	tree_test(L"a+a+a");
	tree_test(L"x+y");
}
```
```
yyyz-*z*+zsin(x-y)*+
1y-6z-*7x-/
1x+y2+&&
aab&&+abc&&&+abcd<&&&+abcde<<<&&&+abcdef%<<<&&&+abcdefg+%<<<&&&+abcdefgh*+%<<<&&&+
al*bz%ki+==&zj>>&&
A0>=A9<&&A'0'>=A'9'<&&||
3456+*+
aa+a+
xy+
```
```
yyyz-*z*+zsin(x-y)*+
$t0_=y
$t1_=z
$t2_=$t0_-$t1_
$t3_=$t0_*$t2_
$t4_=$t3_*$t1_
$t5_=$t0_+$t4_
$t6_=sin(x-y)
$t7_=$t1_*$t6_
$t8_=$t5_+$t7_

...
```

## Usage variable_method
``` c++
int main()
{
using namespace std;

    parse_variable pv;
    std::string parsinglot;
    for (std::string str : pv.parse_replacement2("p[a+b[a-b]+c[d-z[k]+a]]+k(a)", parsinglot))
        cout << str << endl;
    cout << endl;
    cout << parsinglot << endl;
	
    return 0;
}
```
```
@t0_=a-b
@r0_=b
@r1_=@r0_[@t0_]
@t0_=k
@r0_=z
@r1_=@r0_[@t0_]
@t1_=d-@r1_+a
@r2_=c
@r3_=@r2_[@t1_]
@t0_=a+@r1_+@r3_
@r0_=p
@r1_=@r0_[@t0_]
@t1_=a
@r2_=k
@r3_=@r2_(@t1_)

@r1_+@r3_
```