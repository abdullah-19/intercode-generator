/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   EquationTree.h

Purpose:

	RollRat Library

Author:

   12-19-2015:   HyunJun Jeong, Creation

***/

#ifndef _EQUATIONTREE_9bf1541fdf7efd41b7b39543fd870ac4_
#define _EQUATIONTREE_9bf1541fdf7efd41b7b39543fd870ac4_

#include "BinaryTree.h"
#include "OperatorPriority.h"
#include "WStringBuilder.h"

namespace Utility {
	
	template<class priority_type = oprator_priority> class 
		EquationTree
	{
		typedef EquationTree<priority_type> _Myparse;
		typedef BinaryTree<WString> _Mytree;

		priority_type pf;
		_Mytree ps_tree;
		WStringBuilder temp;
		const wchar_t* ptr;
		
		explicit EquationTree(bool safe_node)
			: ps_tree(safe_node)
		{
		}

	public:

		EquationTree()
		{
		}

		_Mytree& start(const WString& wstr)
		{
			return start(wstr.Reference());
		}
		
		_Mytree& start(const wchar_t* wstr)
		{
			ptr = wstr;
			var();
			return ps_tree;
		}

	private:

		bool is_char()
		{
			return (
				isalnum(*ptr)  ||
				*ptr == L'.'   ||
				*ptr == L'\"'  ||
				*ptr == L'\''  ||
				*ptr == L'\\'  ||
				*ptr == L'$'   ||
				*ptr == L'_'   ||
				*ptr == L'@'    );
		}

		bool is_operator()
		{
			if (!*ptr)
				return false;
			return strchr("+-*/&|^%<>!~=", *ptr);
		}

		void skipws()
		{
			while (isspace(*ptr))
				ptr++;
		}

		void var()
		{
			factor();
			skipws();

			//
			//	�����ڰ� ������ Ʈ���� �����ڸ� ����־���ϹǷ� 
			//	rvalue�� lvalue�� ���� ����, ������ �м��մϴ�
			//
			if (is_operator())
			{
				//
				//	�������� ���̴� ��Ȳ�� ���� �þ ��
				//	�ֽ��ϴ�. �̸� ����Ͽ� ������ ��ü�� �����ɴϴ�.
				//
				WStringBuilder builder(5);
				for (; is_operator(); ptr++)
					builder.Append(*ptr);
				skipws();

				WString optr = builder.ToString();

				//
				//	���� ���� ��忡 �����ڰ� ���� ��� �װ��� �����ڸ�
				//	������˴ϴ�. �������� ��� lvalue�� �̹� ä���� 
				//	���¿����մϴ�.
				//
				if (!ps_tree.is_data()) 
				{
					*ps_tree = optr;
				}
				else 
				{
				RETRY:

					/*

					���� ����� �����ڿ� �������� �ϴ� �����ڸ� ���� ���Ͽ�
					�켱������ �����ɴϴ�.

					x ::= ���� ����� ������
					y ::= �������� �ϴ� ������
					��� �����ϸ�,

					(y > x) ->  1
					(y = x) ->  0
					(y < x) -> -1

					�� ���� ����մϴ�.

					*/
					int save = pf(optr, *ps_tree);

					//
					//	���� Ʈ�� ������� �ִ� �����ڿ� ����
					//	�����ڸ� ���Ͽ� ������ ���մϴ�.
					//	�� �� �� ������ ������ �ٲپ� ��Ȱ��
					//	����/����/����ǥ������� ��ȯ�� �� �ֵ��� �մϴ�.
					//

					if (save > 0) {
						
						/*

						�� ������ ���� Ʈ�� ������� �ִ� �����ڰ�
						�������ϴ� �����ں��� ���� �켱 ������ ���� �� ����˴ϴ�.

						����, 3+y*z��� ������ ����� ���ؼ�

						�ϼ��� ����

						     +
						    / \
						   3   y

						����

						         +
						        / \
						       3   *
						          / \
						         y   z

						�� �ٲ���մϴ�. (y�� �����Ͱ� *�� �� �������� �̵���)

						�� ����� ������ ���� ������ ����˴ϴ�.

						�����ʿ� �����Ͱ� ���� ���
						1. ���� ��尡 y�� ����Ű�� �մϴ�.
						2. ���� ����� �����ʿ� �ش� �����ڸ� �ֽ��ϴ�.
						3. ���� ����� ���� ������ ����� ���� ���� �ٲߴϴ�.

						�����ʿ� �����Ͱ� �ִ� ���(���� �����͵� ���Ե�)
						1. ������ ��带 �ӽ������մϴ�.
						2. ������ ��忡 �ش� �����ڸ� �ֽ��ϴ�.
						3. ���� ��带 ������ ���� �̵���ŵ�ϴ�.
						4. ���� ����� ���ʿ� �ӽ������� ��带 �ֽ��ϴ�.

						*/
						if (!ps_tree.is_right())  {
							ps_tree.into_right();
							ps_tree.push_left(optr);

							//
							//	���� Ʈ�� ����� �����Ϳ� ���� ����� �����͸� �ٲߴϴ�.
							//
							ps_tree.swap_data_left();
						} else {
							ps_tree.detach_right();
							ps_tree.push_right(optr);
							ps_tree.into_right();
							ps_tree.attach_left();
						}
					}
					else if (save < 0) {
						
						/*

						�� ������ ���� Ʈ�� ������� �ִ� �����ڰ�
						�������ϴ� �����ں��� ���� �켱 ������ ���� �� ����˴ϴ�.

						����, ���� ��尡 ground�� �ִٸ�, 
						������ ���� ������ ����ϴ�.

						����, x*y+i�� ��� + < * �̹Ƿ�,

						    *
						   / \
						  x   y

						����

						        +
						       / \
						      *   i
						     / \
						    x   y

						���� ��ȯ�մϴ�.

						���� ground�� ���� ������
						���� �� ������ ���¸� �����Ƿ�,

						     ...    ...
						    /          \
						   *            *
						  / \          / \
						 x   y        x   y

						�������� ������ ���� escape�� �����մϴ�.

						*/
						if (ps_tree.is_ground())  {
							ps_tree.make_head_left();
							*ps_tree = optr;
						} else {
							ps_tree.escape();
							goto RETRY;
						}

					} else  {
						
						/*

						�� ������ ���� Ʈ�� ������� �ִ� �����ڿ�
						�������ϴ� �����ڰ� ���� �켱 ������ ���� �� ����˴ϴ�.

						����, x+y-i����� �������� ǥ���� (x+y)-i�� �ǹǷ�

						     +
						    / \
						   x   y

						����

						       -
						      / \
						     +   i
						    / \
						   x   y

						�� �Ǿ�� �մϴ�. (-�� �� ���� �����Ƿ� i-(x+y)��
						������ �� ������ �̴� �߸��� ǥ���Դϴ�.)

						�� ����� �Ʒ��� ���� ������� ����˴ϴ�.

						escape���� ���� �� 

						i) ���� ��尡 ground�� ���
						1. ��带 �ӽ������ϰ� �� ��带 ����ϴ�.
						2. ��忡 �����ڸ� �ֽ��ϴ�.
						3. ����� ���ʿ� �ӽ������� ��带 �����մϴ�.

						ii) escape�� �������� �����Ͽ��ٸ�(�ڼ��� ������ escape�Լ� ����)
						1. ���� ��带 �ӽ������մϴ�.
						2. ���� ��忡 �����ڸ� �ֽ��ϴ�.
						3. ���� ��带 ���� ��带 ����Ű�� �մϴ�.
						4. ���ʿ� �ӽ������� ��带 �ֽ��ϴ�.

						iii) escape�� ���������� �����Ͽ��ٸ�
						1. ������ ��带 �ӽ������մϴ�.
						2. ������ ��忡 �����ڸ� �ֽ��ϴ�.
						3. ���� ��带 ������ ��带 ����Ű�� �մϴ�.
						4. ���ʿ� �ӽ������� ��带 �ֽ��ϴ�.

						*/

						ps_tree.detach_this();
						if (ps_tree.last_detach() == 0) {
							*ps_tree = optr;
							ps_tree.attach_left();
						} else if (ps_tree.last_detach() == -1) {
							ps_tree.push_left(optr);
							ps_tree.into_left();
							ps_tree.attach_left();
						} else if (ps_tree.last_detach() == 1) {
							ps_tree.push_right(optr);
							ps_tree.into_right();
							ps_tree.attach_left();
						}

					}
				}
				var();
			}

		}

		void factor()
		{
			skipws();
			if (*ptr == '(')
			{
				ptr++;
				_Myparse rvtemp(true);

				/*

				�� ������ ���� �� ���� ������ �����ϴ�.

				1. ���� ��忡 �����Ͱ� �����ϰ�, �����ʿ� �������� ���� ���.
				2. ���� ��忡 �����Ͱ� �������� ���� ���

				*/
				if (ps_tree.is_left() && !ps_tree.is_right())

					//
					//	���� ���� Ʈ�� ��带 �����ʿ��� ���Դϴ�.
					//
					ps_tree.attach_right_tree(rvtemp.start(ptr).travel());
				else if (!ps_tree.is_left())

					//
					//	���� ���� Ʈ�� ��带 ���ʿ��� ���Դϴ�.
					//
					ps_tree.attach_left_tree(rvtemp.start(ptr).travel());
				else
					;	// error

				//
				//	���� ���� Ʈ���ȿ��� ������ �����͸� �����ɴϴ�.
				//
				ptr = rvtemp.ptr;

				//
				//	��ȣ�� ���������� �����ִ��� Ȯ���մϴ�.
				//	�� �۾��� ���������� �̷������ �ʾҴٸ� ���α׷��� ������ ����
				//	���ɼ��� �ſ� Ů�ϴ�.
				//
				if (*ptr == ')')
					ptr++;
				else
					;	// error
			}
			else if (is_char())
			{
				//
				//  �� ������ ��ǻ� �������� �������� �� ���� ���� �ٸ� ������ ����
				//  �� ���� ��� ó����ŵ�ϴ�. �̷� ������ �������� �������� �������� �˻�
				//  �Ͽ����մϴ�.
				//
				variable();
				factor();
			}
		}

		void variable()
		{
			for (; is_char();)
				temp.Append(*ptr++);

			//
			//  Reference, Function �κ��� Ȯ���մϴ�.
			//
		RETRY:
			skipws();
			if (*ptr == '(' || *ptr == '[') 
			{
				get_inside_string();
				goto RETRY;
			}
			
			//
			//	�� ���� ���ʿ� �־�� �̹� �����Ͱ� �����ϸ�
			//	�����ʿ� �ֽ��ϴ�. 
			//	���� ǥ������� ��ȯ�ϱ� ���� ����->������ ������� �����մϴ�.
			//
			if (!ps_tree.is_left())
				ps_tree.push_left(temp.ToString());
			else if (!ps_tree.is_right())
				ps_tree.push_right(temp.ToString());
			temp.Dispose();
		}

		bool get_inside_string()
		{
			//
			// ����� ���Ǹ� ���� ��ȣ�� ������ �������� ����
			//
			int indicate_inside = 0;

			for (; *ptr; (ptr)++)
			{
				temp.Append(*ptr);

				switch (*ptr)
				{
				case '[': case '(': case '{':
					indicate_inside++;
					break;

				case ']': case ')': case '}':
					indicate_inside--;
					break;

				default:
					continue;
				}

				if (indicate_inside == 0) {
					ptr++;
					return true;
				}
			}

			return false;
		}

	};

}

#endif