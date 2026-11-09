#include <Python.h>
#include <longintrepr.h>
#include <stdio.h>

/**
 * print_python_int - prints a Python int as a C unsigned long
 * @p: pointer to the PyObject
 */
void print_python_int(PyObject *p)
{
	PyLongObject *num;
	Py_ssize_t size, i;
	unsigned long value = 0;
	int negative;

	if (!PyLong_Check(p))
	{
		printf("Invalid Int Object\n");
		return;
	}
	num = (PyLongObject *)p;
	size = ((PyVarObject *)p)->ob_size;
	negative = size < 0;
	if (negative)
		size = -size;
	if (size > 3 || (size == 3 && num->ob_digit[2] > 15))
	{
		printf("C unsigned long int overflow\n");
		return;
	}
	for (i = 0; i < size; i++)
		value += (unsigned long)num->ob_digit[i] << (PyLong_SHIFT * i);
	if (negative)
		printf("-");
	printf("%lu\n", value);
}
