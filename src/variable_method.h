/*************************************************************************
  
   Copyright (C) 2015. rollrat. All Rights Reserved.

------
FILE NAME:
   
   variable_method.h - [Private]

Project:

   Compiler Method Second

------
   AUTHOR: HyunJun Jeong  2015-02-20

***************************************************************************/

#ifndef _VARIABLE_METHOD_
#define _VARIABLE_METHOD_

#include <string>
#include <hash_map>
#include <functional>
#include <vector>

class parse_variable 
{
    int rcount = 0;
    int fcount = 0;
    int tcount = 0;
    std::vector<std::string> inside_vector, variable_vector;
public:

    bool get_inside_string(char const const* *ptr, std::string& str);
    void skip_whitespace(char const const* *ptr);
    std::vector<std::string> parse_replacement(const char *variable, std::string& replace_argument);
    std::vector<std::string> parse_replacement2(const char *variable, std::string& replace_argument);

};

bool parse_variable
    ::get_inside_string(char const const* *ptr, std::string& str)
{
    //
    // ��ȣ�� ������ �������� ����
    //

    int indicate_inside = 0;

    for(; **ptr; (*ptr)++)
    {
        str.push_back(**ptr);

        switch(**ptr)
        {
        case '[':
        case '(':
        case '{':
            indicate_inside++;
            break;

        case ']':
        case ')':
        case '}':
            indicate_inside--;
            break;

        default:
            continue;
        }

        if (indicate_inside == 0) {
            return true;
        }
    }

    return false;
}

void parse_variable
    ::skip_whitespace(char const const* *ptr)
{
    while (**ptr)
        if (!((char *)isspace((int)(void *)**ptr)))
            return;
}

std::vector<std::string> parse_variable
    ::parse_replacement(const char *variable, std::string& replace_argument)
{
    std::string _var_prefix = "@t";
    std::string _fun_prefix = "@f";
    std::string _ref_prefix = "@r";
    std::string _postfix = "_";
    std::string _substitue;
    std::vector<std::string> return_vector;
    const char *ptr = variable;

    auto is_char = [&]() -> bool {
        return (
	    	isalnum(*ptr) ||
	    	*ptr == '.'  ||   // �Ҽ���
	    	*ptr == '\"' ||   // ���ڿ�
	    	*ptr == '\'' ||   // ����
	    	*ptr == '\\' ||   // �ּ�
	    	*ptr == '$' ||    // �������� ��밡��
	    	*ptr == '_' ||    // �������� ��밡��
	    	*ptr == '@'       // ������ ���Ұ���, ������� ġȯ �뵵�� ����� ��
	    );
    };

    auto is_chara = [](char ch) -> bool {
        return (
	    	isalnum(ch) ||
	    	ch == '.'  ||   // �Ҽ���
	    	ch == '\"' ||   // ���ڿ�
	    	ch == '\'' ||   // ����
	    	ch == '\\' ||   // �ּ�
	    	ch == '$' ||    // �������� ��밡��
	    	ch == '_' ||    // �������� ��밡��
	    	ch == '@'       // ������ ���Ұ���, ������� ġȯ �뵵�� ����� ��
	    );
    };

    replace_argument = variable;

    //std::vector<std::string> replace_preserved_vector, inside_vector, variable_vector;
    std::vector<std::string> replace_preserved_vector;

    //
    //  ġȯ�Ǿ���ϴ� ��� �κ� ���� ���¸� ã�� vector�� �����մϴ�.
    //
    for(; *ptr; ) {

        // ������ �ʴ� �κ��� �ǳ� ��
        for (; !is_char() && *ptr;)
            ptr++;

        std::string variable_string;
	    for (; is_char();)
		    variable_string.push_back(*ptr++);

        skip_whitespace(&ptr);

        if (*ptr == '[' || *ptr == '(') {
            std::string inside_string;
            get_inside_string(&ptr, inside_string);
            replace_preserved_vector.push_back(variable_string + inside_string);
            inside_vector.push_back(inside_string);
            variable_vector.push_back(variable_string);
        }
    }

    //
    //  ��� ������ ġȯ�մϴ�.
    //
    for(std::string str : replace_preserved_vector) {
        std::string variable_replacement_name = 
            _var_prefix + std::to_string(fcount++) + _postfix;

        int s_pos = 0;
        while((s_pos = replace_argument.find(str, s_pos)) != std::string::npos) {
            replace_argument.replace(s_pos, str.length(), variable_replacement_name);
            s_pos += variable_replacement_name.length();
        }

        //
        //  reference & function ó��
        //
        const std::string varin_string = inside_vector[tcount];
        const std::string var_string = variable_vector[tcount];

        tcount++;

        if(varin_string[0] == '[') {
            std::string variable_pure;
            std::string var_varname = _ref_prefix + std::to_string(rcount++) + _postfix;
            std::string var_refname = _ref_prefix + std::to_string(rcount++) + _postfix;

            for (int i = 1; i < varin_string.length() - 1; i++)
                variable_pure.push_back(varin_string[i]);
            
            {
                int j = 0;
                for (j = 0; is_chara(variable_pure[j]); j++)
                    ;
                if (variable_pure[j] == '[' || variable_pure[j] == '(') {
                    //parse_variable pv_temp;
                    std::string revolve;
                    //std::vector<std::string> revolve_vector = pv_temp.parse_replacement(variable_pure.c_str(), revolve);
                    std::vector<std::string> revolve_vector = parse_replacement(variable_pure.c_str(), revolve);
                    return_vector.insert(return_vector.end(), revolve_vector.begin(), revolve_vector.end());
                    return_vector.push_back(variable_replacement_name + "=" + revolve);
                } else {
                    return_vector.push_back(variable_replacement_name + "=" + variable_pure);
                }
            }

            return_vector.push_back(var_varname + "=" + var_string);
            return_vector.push_back(var_refname + "=" + var_varname + "[" + variable_replacement_name + "]");

        } else if (varin_string[0] == '(') {
            std::string variable_pure;
            std::string var_varname = _ref_prefix + std::to_string(fcount++) + _postfix;
            std::string var_refname = _ref_prefix + std::to_string(fcount++) + _postfix;

            for (int i = 1; i < varin_string.length() - 1; i++)
                variable_pure.push_back(varin_string[i]);
            
            {
                int j = 0;
                for (j = 0; is_chara(variable_pure[j]); j++)
                    ;
                if (variable_pure[j] == '[' || variable_pure[j] == '(') {
                    //parse_variable pv_temp;
                    std::string revolve;
                    //std::vector<std::string> revolve_vector = pv_temp.parse_replacement(variable_pure.c_str(), revolve);
                    std::vector<std::string> revolve_vector = parse_replacement(variable_pure.c_str(), revolve);
                    return_vector.insert(return_vector.end(), revolve_vector.begin(), revolve_vector.end());
                    return_vector.push_back(variable_replacement_name + "=" + revolve);
                } else {
                    return_vector.push_back(variable_replacement_name + "=" + variable_pure);
                }
            }

            return_vector.push_back(var_varname + "=" + var_string);
            return_vector.push_back(var_refname + "=" + var_varname + "(" + variable_replacement_name + ")");
        }
    }

    return return_vector;
}

std::vector<std::string> parse_variable
    ::parse_replacement2(const char *variable, std::string& replace_argument)
{
    std::string _var_prefix = "@t";
    std::string _fun_prefix = "@f";
    std::string _ref_prefix = "@r";
    std::string _postfix = "_";
    std::string _substitue;
    std::vector<std::string> return_vector;
    const char *ptr = variable;

    auto is_char = [&]() -> bool {
        return (
	    	isalnum(*ptr) ||
	    	*ptr == '.'  ||   // �Ҽ���
	    	*ptr == '\"' ||   // ���ڿ�
	    	*ptr == '\'' ||   // ����
	    	*ptr == '\\' ||   // �ּ�
	    	*ptr == '$' ||    // �������� ��밡��
	    	*ptr == '_' ||    // �������� ��밡��
	    	*ptr == '@'       // ������ ���Ұ���, ������� ġȯ �뵵�� ����� ��
	    );
    };

    auto is_chara = [](char ch) -> bool {
        return (
	    	isalnum(ch) ||
	    	ch == '.'  ||   // �Ҽ���
	    	ch == '\"' ||   // ���ڿ�
	    	ch == '\'' ||   // ����
	    	ch == '\\' ||   // �ּ�
	    	ch == '$' ||    // �������� ��밡��
	    	ch == '_' ||    // �������� ��밡��
	    	ch == '@'       // ������ ���Ұ���, ������� ġȯ �뵵�� ����� ��
	    );
    };

    replace_argument = variable;

    std::vector<std::string> replace_preserved_vector, inside_vector, variable_vector;
    //std::vector<std::string> replace_preserved_vector;

    //
    //  ġȯ�Ǿ���ϴ� ��� �κ� ���� ���¸� ã�� vector�� �����մϴ�.
    //
    for(; *ptr; ) {

        // ������ �ʴ� �κ��� �ǳ� ��
        for (; !is_char() && *ptr;)
            ptr++;

        std::string variable_string;
	    for (; is_char();)
		    variable_string.push_back(*ptr++);

        skip_whitespace(&ptr);

        if (*ptr == '[' || *ptr == '(') {
            std::string inside_string;
            get_inside_string(&ptr, inside_string);
            replace_preserved_vector.push_back(variable_string + inside_string);
            inside_vector.push_back(inside_string);
            variable_vector.push_back(variable_string);
        }
    }

    //
    //  ��� ������ ġȯ�մϴ�.
    //
    for(std::string str : replace_preserved_vector) {
        std::string variable_replacement_name = 
            _var_prefix + std::to_string(fcount++) + _postfix;

        //
        //  reference & function ó��
        //
        const std::string varin_string = inside_vector[tcount];
        const std::string var_string = variable_vector[tcount];

        tcount++;

        if(varin_string[0] == '[') {
            std::string variable_pure;
            std::string var_varname = _ref_prefix + std::to_string(rcount++) + _postfix;
            std::string var_refname = _ref_prefix + std::to_string(rcount++) + _postfix;

            for (int i = 1; i < varin_string.length() - 1; i++)
                variable_pure.push_back(varin_string[i]);
            
            {
                int j = 0;
                for (j = 0; variable_pure[j] != '[' && variable_pure[j] != '(' && j < variable_pure.length(); j++)
                    ;
                if (variable_pure[j] == '[' || variable_pure[j] == '(') {
                    parse_variable pv_temp;
                    std::string revolve;
                    std::vector<std::string> revolve_vector = pv_temp.parse_replacement2(variable_pure.c_str(), revolve);
                    //std::vector<std::string> revolve_vector = parse_replacement(variable_pure.c_str(), revolve);
                    return_vector.insert(return_vector.end(), revolve_vector.begin(), revolve_vector.end());
                    return_vector.push_back(variable_replacement_name + "=" + revolve);
                } else {
                    return_vector.push_back(variable_replacement_name + "=" + variable_pure);
                }
            }

            return_vector.push_back(var_varname + "=" + var_string);
            return_vector.push_back(var_refname + "=" + var_varname + "[" + variable_replacement_name + "]");
            
            int s_pos = 0;
            while((s_pos = replace_argument.find(str, s_pos)) != std::string::npos) {
                replace_argument.replace(s_pos, str.length(), var_refname);
                s_pos += var_refname.length();
            }

        } else if (varin_string[0] == '(') {
            std::string variable_pure;
            std::string var_varname = _ref_prefix + std::to_string(fcount++) + _postfix;
            std::string var_refname = _ref_prefix + std::to_string(fcount++) + _postfix;

            for (int i = 1; i < varin_string.length() - 1; i++)
                variable_pure.push_back(varin_string[i]);
            
            {
                int j = 0;
                for (j = 0; is_chara(variable_pure[j]); j++)
                    ;
                if (variable_pure[j] == '[' || variable_pure[j] == '(') {
                    parse_variable pv_temp;
                    std::string revolve;
                    std::vector<std::string> revolve_vector = pv_temp.parse_replacement2(variable_pure.c_str(), revolve);
                    //std::vector<std::string> revolve_vector = parse_replacement(variable_pure.c_str(), revolve);
                    return_vector.insert(return_vector.end(), revolve_vector.begin(), revolve_vector.end());
                    return_vector.push_back(variable_replacement_name + "=" + revolve);
                } else {
                    return_vector.push_back(variable_replacement_name + "=" + variable_pure);
                }
            }

            return_vector.push_back(var_varname + "=" + var_string);
            return_vector.push_back(var_refname + "=" + var_varname + "(" + variable_replacement_name + ")");
            
            int s_pos = 0;
            while((s_pos = replace_argument.find(str, s_pos)) != std::string::npos) {
                replace_argument.replace(s_pos, str.length(), var_refname);
                s_pos += var_refname.length();
            }

        } else {
            int s_pos = 0;
            while((s_pos = replace_argument.find(str, s_pos)) != std::string::npos) {
                replace_argument.replace(s_pos, str.length(), variable_replacement_name);
                s_pos += variable_replacement_name.length();
            }
        }
    }

    return return_vector;
}

#endif