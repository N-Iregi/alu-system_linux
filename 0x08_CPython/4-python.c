#include <Python.h>
#include <stdio.h>

/**
 * print_python_string - prints info about a Python string
 * @p: pointer to the PyObject
 */
void print_python_string(PyObject *p)
{
	setbuf(stdout, NULL);
	printf("[.] string object info\n");
	if (!PyUnicode_Check(p))
	{
		printf("  [ERROR] Invalid String Object\n");
		return;
	}
	if (PyUnicode_READY(p) < 0)
		return;
	if (PyUnicode_IS_COMPACT_ASCII(p))
		printf("  type: compact ascii\n");
	else
		printf("  type: compact unicode object\n");
	printf("  length: %ld\n", PyUnicode_GET_LENGTH(p));
	printf("  value: %s\n", PyUnicode_AsUTF8(p));
}
