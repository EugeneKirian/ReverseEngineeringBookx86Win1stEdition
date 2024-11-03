/*
Copyright (c) 2024 Eugene Kirian

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

int single_variable_do_while_loop_example()
{
	int result = 0;

	do
	{
		result = result + 2;
	} while (result < 100);

	return result;
}

int array_variable_do_while_loop_example()
{
	int i = 0;
	int result[50];

	do
	{
		result[i] = i++;
	} while (i < 50);

	return result[49];
}

int single_variable_do_while_loop_break_example(int start, int count)
{
	int i = 0;
	int result = start;

	do
	{
		result = result + i++;
	} while (i < count);

	return result;
}

int single_variable_for_loop_example()
{
	int result = 0;

	for (int i = 0; i < 100; i++)
	{
		result = i;
	}

	return result;
}

int array_variable_for_loop_example()
{
	int arr[100];

	for (int i = 0; i < 100; i++)
	{
		arr[i] = i;
	}

	return arr[99];
}

int single_variable_parameterized_for_loop_example(int count)
{
	int result = 0;

	for (int i = 0; i < count; i++)
	{
		result = i + count;
	}

	return result;
}

int array_variable_for_loop_function_callee(int value)
{
	return value * 2;
}

int array_variable_for_loop_function_call_example()
{
	int arr[100];

	for (int i = 0; i < 100; i++)
	{
		arr[i] = array_variable_for_loop_function_callee(i);
	}

	return arr[99];
}

int single_variable_min_max_for_loop_example(int min, int max)
{
	int result = 0;

	for (int i = min; i < max; i++)
	{
		result = i;
	}

	return result;
}

int single_variable_for_loop_break_example(int value, int max)
{
	int result = 0;

	for (int i = 0; i < max; i++)
	{
		result = i;

		if (result == value) { break; }
	}

	return result;
}

int multiple_variables_for_loop_example()
{
	int result = 0;

	for (int i = 0, j = 1; i < 100; i++, j += 2)
	{
		result += i + j;
	}

	return result;
}

int multiple_variables_conditional_for_loop_example()
{
	int i;
	int j = 1;
	int result = 0;

	for (i = 0; i < 100;)
	{
		if (result % 2 != 0)
		{
			j = j + 1;
		}

		result = i % 2 == 0 ? (i + j) : (j + 1);

		i++;
	}

	return result;
}

int endless_for_loop_example(int count)
{
	int i = 0;

	for (;;)
	{
		if (i < count)
		{
			i++;
		}
		else
		{
			i = 1;
			return -1;
		}
	}

	return i;
}

int single_variable_while_loop_example()
{
	int result = 0;

	while (result < 100)
	{
		result = (result + 1) * 2;
	}

	return result;
}

int array_variable_while_loop_example()
{
	int i = 0;
	int arr[100];

	while (i < 100)
	{
		arr[i] = i;

		i = i + 2;
	}

	return arr[99];
}

int single_variable_parameterized_while_loop_example(int count)
{
	int result = 0;

	while (result < count)
	{
		result = result + 1;
	}

	return result;
}

int single_variable_while_loop_break_example(int value, int max)
{
	int i = 0;
	int result = 0;

	while (i < max)
	{
		result = i;

		if (result == value) { break; }

		i++;
	}

	return result;
}

int endless_while_loop_example(int count)
{
	int i = 0;

	while (1)
	{
		if (i < count)
		{
			i++;
		}
		else
		{
			return -1;
		}
	}

	return i;
}

int main(int argc, char** argv)
{
	// Do while loop examples.
	single_variable_do_while_loop_example();
	array_variable_do_while_loop_example();
	single_variable_do_while_loop_break_example(15, 50);

	// For loop examples.
	single_variable_for_loop_example();
	array_variable_for_loop_example();
	single_variable_parameterized_for_loop_example(100);
	array_variable_for_loop_function_call_example();
	single_variable_min_max_for_loop_example(50, 75);
	single_variable_for_loop_break_example(45, 99);
	multiple_variables_for_loop_example();
	multiple_variables_conditional_for_loop_example();
	endless_for_loop_example(5);

	// While loop examples.
	single_variable_while_loop_example();
	array_variable_while_loop_example();
	single_variable_parameterized_while_loop_example(3);
	single_variable_while_loop_break_example(17, 27);
	endless_while_loop_example(13);

	return 0;
}