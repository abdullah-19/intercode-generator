/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   EquationIntercode.h

Purpose:

	RollRat Library

Author:

   12-19-2015:   HyunJun Jeong, Creation

***/

#ifndef _EQUATIONINTERCODE_9bf1541fdf7efd41b7b39543fd870ac4_
#define _EQUATIONINTERCODE_9bf1541fdf7efd41b7b39543fd870ac4_

#include <vector>
#include <stack>
#include <Windows.h>

#include "EquationTree.h"

namespace Utility {
	
	std::vector<WString> _intercode_generator2(BinaryTree<WString>& tree, int count_start = 0)
	{
		std::stack<WString> var;
		std::vector<WString> tdup1, tdup2, code;

		// 변수의 개수를 세는 변수
		int count = count_start;

		/*

		소숫점이 포함된 모든 숫자와 일치합니다.

		*/
		auto is_number=[](const WString& cptr)->bool
			{
			for (const wchar_t *ptr = cptr.Reference(); 
				('0' <= *ptr && *ptr <= '9') || *ptr == '.'; ptr++)
				if(!*(ptr+1))
					return true;
			return false;
			};
		
		tree.postorder_travel([&](const WString& str)
			{
		        
		    /*

		    중간코드 번역의 첫 번째 단계로 현재 처리중인 노드의 값이
		    연산자일 경우와 숫자, 변수일 경우로 나누어 처리합니다.

		    */
			if(::strchr("+-*/&^|%<>!~=", str[0]))
				{

		        /*

		        연산될 값이 들어갈 새로운 변수를 만듭니다.
		        이 변수역시 새로운 연산을 위해 스택에 저장됩니다.

		        */
				WString rvalue(WString(L"$t") + WString(count) + L"_=");

		        /*

		        스택에 아무 것도 없으면 연산자에서 lvalue가 존재하지 않는 것 이므로 
		        트리 생성과정에서 문제가 생긴 것 입니다.
		        이 경우 사용자가 수식을 잘못입력하였을 가능성이 높습니다.

		        1. 스택에서 2개의 변수를 가져옵니다.
		        2. 연산자의 오른에, 왼쪽에 각각 가져온 순서대로 넣습니다.
		        3. 만든 구문에서 결과값이 담기는 변수를 스택에 넣습니다.
		        4. 만일, 최적화 가능 요소가 존재한다면, 해당 요소를 최적화하고,
		           최적화한 값을 스택에 넣습니다.

		        */
				if (!var.empty())
					{
					bool second_number = is_number(var.top());
					WString temp = var.top();
					var.pop();
					if (!var.empty()) {
		                
		                /*

		                number <operator> number의 형태일 경우 연산이 가능하므로,
		                직접 연산하여 중간코드를 최적화합니다.

		                */
						if(is_number(var.top()) && second_number) {
							long long v1 = std::stoll(temp.Reference()), v2 = std::stoll(var.top().Reference()), v3=0;
							switch(*str.Reference()) {
							case '+': v3 = v2 + v1; break;
							case '-': v3 = v2 - v1; break;
							case '*': v3 = v2 * v1; break;
							case '/': v3 = v2 / v1; break;
							case '&': v3 = v2 & v1; break;
							case '^': v3 = v2 ^ v1; break;
							case '|': v3 = v2 | v1; break;
							case '%': v3 = v2 % v1; break;
							default:

		                        //
		                        //  알려지지 않은 연산자일 경우
		                        //
								goto JUST_EXCUATE;
							}
		                    var.pop();
							var.push(std::to_string(v3));
						} else {
				JUST_EXCUATE:
							code.push_back(rvalue+var.top()+str+temp);
							var.pop();
							var.push(WString(L"$t") + WString(count) + L"_");
							count += 1;
							}
					} else 
						goto _LABEL_ERROR;
					}
				else 
		            {
			_LABEL_ERROR:
					MessageBox(0, TEXT("Input value is incorrect. Please enter a valid expression."), 
		                TEXT("_intercode_generator2"), MB_OK | MB_ICONERROR);
		            exit(0);
		            }
				}
			else

		        /*

		        처리 중인 값이 숫자일경우 최적화 요소가 있을 수 있으므로
		        일단, 스택에 저장해 둡니다.

		        만일 숫자가 아닐 경우 변수로 취급하므로 해당 변수를 선언하는 구문을 만들고
		        해당 변수를 스택에 넣습니다.

		        */
				if(is_number(str))
					var.push(str);
				else {
					bool t = false;

		            /*

		            중복되는 변수가 있는지 확인합니다.

		            만일, 중복되는 변수가 있을 경우 해당 변수를 스택에 넣고, 이 과정을 끝마치며,
		            없을 경우 새로운 변수를 선언하는 구문을 삽입합니다.

		            */
					for(int i = 0; i < tdup1.size(); i++)
						if(tdup2[i] == str) {

		                    //
		                    //  벡터를 뒤지면서 하나라도 발견된다면 해당 변수를 넣을 필요가 없으므로
		                    //  현재 변수를 스택에 넣습니다.
		                    //
							var.push(tdup1[i]);
							t = true;
							break;
						}

					if(!t) {

		                /*

		                $t~~_의 변수 형태는 첫 글자의 $는 변수 맨 처음에 자주 사용되지 않는 글자라 써넣은 것이며,
		                마지막 글자인 _는 $t의 변수를 숫자만을 가질 경우 strcmp가 true를 내놓아 버리는 경우가 생겨서 입니다.
		                또한, 연산자와 변수를 구분하지 않았기 때문에 쓸데없는 자원 낭비를 줄이려
		                해당 방법을 사용하였습니다.

		                */
						WString rvalue(WString(L"$t") + WString(count) + L"_=" + str);

		                //
		                //  숫자와 마찬가지로 변수도 스택에 넣습니다.
		                //
						var.push(WString(L"$t") + WString(count) + L"_");

		                //
		                //  변수의 중복을 확인하기 위해 tdup1에 넣음
		                //
						tdup1.push_back(var.top());
						tdup2.push_back(str);

						code.push_back(rvalue);
						count += 1;
					}
		        }
		    });
		if (code.empty())
			code.push_back(var.top());
		return code;
	}

}

#endif