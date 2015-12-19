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

		// ������ ������ ���� ����
		int count = count_start;

		/*

		�Ҽ����� ���Ե� ��� ���ڿ� ��ġ�մϴ�.

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

		    �߰��ڵ� ������ ù ��° �ܰ�� ���� ó������ ����� ����
		    �������� ���� ����, ������ ���� ������ ó���մϴ�.

		    */
			if(::strchr("+-*/&^|%<>!~=", str[0]))
				{

		        /*

		        ����� ���� �� ���ο� ������ ����ϴ�.
		        �� �������� ���ο� ������ ���� ���ÿ� ����˴ϴ�.

		        */
				WString rvalue(WString(L"$t") + WString(count) + L"_=");

		        /*

		        ���ÿ� �ƹ� �͵� ������ �����ڿ��� lvalue�� �������� �ʴ� �� �̹Ƿ� 
		        Ʈ�� ������������ ������ ���� �� �Դϴ�.
		        �� ��� ����ڰ� ������ �߸��Է��Ͽ��� ���ɼ��� �����ϴ�.

		        1. ���ÿ��� 2���� ������ �����ɴϴ�.
		        2. �������� ������, ���ʿ� ���� ������ ������� �ֽ��ϴ�.
		        3. ���� �������� ������� ���� ������ ���ÿ� �ֽ��ϴ�.
		        4. ����, ����ȭ ���� ��Ұ� �����Ѵٸ�, �ش� ��Ҹ� ����ȭ�ϰ�,
		           ����ȭ�� ���� ���ÿ� �ֽ��ϴ�.

		        */
				if (!var.empty())
					{
					bool second_number = is_number(var.top());
					WString temp = var.top();
					var.pop();
					if (!var.empty()) {
		                
		                /*

		                number <operator> number�� ������ ��� ������ �����ϹǷ�,
		                ���� �����Ͽ� �߰��ڵ带 ����ȭ�մϴ�.

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
		                        //  �˷����� ���� �������� ���
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

		        ó�� ���� ���� �����ϰ�� ����ȭ ��Ұ� ���� �� �����Ƿ�
		        �ϴ�, ���ÿ� ������ �Ӵϴ�.

		        ���� ���ڰ� �ƴ� ��� ������ ����ϹǷ� �ش� ������ �����ϴ� ������ �����
		        �ش� ������ ���ÿ� �ֽ��ϴ�.

		        */
				if(is_number(str))
					var.push(str);
				else {
					bool t = false;

		            /*

		            �ߺ��Ǵ� ������ �ִ��� Ȯ���մϴ�.

		            ����, �ߺ��Ǵ� ������ ���� ��� �ش� ������ ���ÿ� �ְ�, �� ������ ����ġ��,
		            ���� ��� ���ο� ������ �����ϴ� ������ �����մϴ�.

		            */
					for(int i = 0; i < tdup1.size(); i++)
						if(tdup2[i] == str) {

		                    //
		                    //  ���͸� �����鼭 �ϳ��� �߰ߵȴٸ� �ش� ������ ���� �ʿ䰡 �����Ƿ�
		                    //  ���� ������ ���ÿ� �ֽ��ϴ�.
		                    //
							var.push(tdup1[i]);
							t = true;
							break;
						}

					if(!t) {

		                /*

		                $t~~_�� ���� ���´� ù ������ $�� ���� �� ó���� ���� ������ �ʴ� ���ڶ� ����� ���̸�,
		                ������ ������ _�� $t�� ������ ���ڸ��� ���� ��� strcmp�� true�� ������ ������ ��찡 ���ܼ� �Դϴ�.
		                ����, �����ڿ� ������ �������� �ʾұ� ������ �������� �ڿ� ���� ���̷�
		                �ش� ����� ����Ͽ����ϴ�.

		                */
						WString rvalue(WString(L"$t") + WString(count) + L"_=" + str);

		                //
		                //  ���ڿ� ���������� ������ ���ÿ� �ֽ��ϴ�.
		                //
						var.push(WString(L"$t") + WString(count) + L"_");

		                //
		                //  ������ �ߺ��� Ȯ���ϱ� ���� tdup1�� ����
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