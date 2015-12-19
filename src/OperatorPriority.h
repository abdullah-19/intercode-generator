/***

   RollRat Software Project.
   Copyright (C) 2015. rollrat. All Rights Reserved.

File name:
   
   OperatorPriority.h

Purpose:

	RollRat Library

Author:

   12-19-2015:   HyunJun Jeong, Creation

***/

#ifndef _OPERATORPRIORITY_9bf1541fdf7efd41b7b39543fd870ac4_
#define _OPERATORPRIORITY_9bf1541fdf7efd41b7b39543fd870ac4_

#include "WString.h"

#define priority_skip -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
    -1,-1,-1,-1,-1,-1,

#define priority_skip_number \
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,

#define prioirty_skip_alpha  -1,-1,-1,-1,-1,-1,-1,-1,-1,\
    -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,

namespace Utility {
	
	/*
	    Operator Priority 정보
	
	    1순위: *, /
	    2순위: +, -
	    3순위: %
	    4순위: <<, >>
	    5순위: <=, >=, <, >, ==, !=
	    6순위: &, ^, |
	    7순위: &&, ||
	*/

	const char priority_this_1[0x7f] = {
		priority_skip
		-1, /* '!' */
		-1, /* '"' */
		-1, /* '#' */
		-1, /* '$' */
			4, /* '%' */
			1, /* '&' */
		-1, /* ''' */
		-1, /* '(' */
		-1, /* ')' */
			6, /* '*' */
			5, /* '+' */
		-1, /* ',' */
			5, /* '-' */
		-1, /* '.' */
			6, /* '/' */
		priority_skip_number
		-1, /* ':' */
		-1, /* ';' */
			2, /* '<' */
		-1, /* '=' */
			2, /* '>' */
		-1, /* '?' */
		-1, /* '@' */
		prioirty_skip_alpha
		-1, /* '[' */
		-1, /* '\' */
		-1, /* ']' */
			1, /* '^' */
		-1, /* '_' */
		-1, /* '`' */
		prioirty_skip_alpha
		-1, /* '{' */
			1, /* '|' */
		-1, /* '}' */
		-1, /* '~' */};

	const char priority_this_2[0x7f] = {
		priority_skip
		-1, /* '!' */
		-1, /* '"' */
		-1, /* '#' */
		-1, /* '$' */
		-1, /* '%' */
			0, /* '&' */
		-1, /* ''' */
		-1, /* '(' */
		-1, /* ')' */
		-1, /* '*' */
		-1, /* '+' */
		-1, /* ',' */
		-1, /* '-' */
		-1, /* '.' */
		-1, /* '/' */
		priority_skip_number
		-1, /* ':' */
		-1, /* ';' */
			3, /* '<' */
			2, /* '=' */
			3, /* '>' */
		-1, /* '?' */
		-1, /* '@' */
		prioirty_skip_alpha
		-1, /* '[' */
		-1, /* '\' */
		-1, /* ']' */
		-1, /* '^' */
		-1, /* '_' */
		-1, /* '`' */
		prioirty_skip_alpha
		-1, /* '{' */
			0, /* '|' */
		-1, /* '}' */
		-1, /* '~' */};

	class oprator_priority final
	{
	public:

		// left < right -> -1
		// left > right -> 1
		// left == right -> 0
		int operator()(const WString& left, const WString& right)
		{
	        int lpri, rpri;
	        if (left.Length() == 1)
	            lpri = priority_this_1[left[0]];
	        else if (check_enable_operator(left[0], left[1]))
	            lpri = priority_this_2[left[1]];
	        if (right.Length() == 1)
	            rpri = priority_this_1[right[0]];
	        else if (check_enable_operator(right[0], right[1]))
	            rpri = priority_this_2[right[1]];

	        // 없는 연산자는 여기서 오류가 생김
	        if (lpri > rpri)
	            return 1;
	        else if (lpri < rpri)
	            return -1;
	        else
	            return 0;
		}

	    bool check_enable_operator(char first, char second)
	    {
	        if(first == '&') {
	            if(second == '&')
	                return true;
	        } else if (first == '|') {
	            if(second == '|')
	                return true;
	        } else if (first == '<') {
	            if (second == '<' || second == '=')
	                return true;
	        } else if (first == '>') {
	            if (second == '>' || second == '=')
	                return true;
	        } else if (first == '=') {
	            if(second == '=')
	                return true;
	        } else if (first == '!') {
	            if(second == '=')
	                return true;
	        }
	        return false;
	    }
		
	};

}

#endif