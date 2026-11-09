#include <Python.h>
#include <stdio.h>

/**
 * print_python_list - prints basic info about a Python list
 * @p: pointer to the PyObject
 */
void print_python_list(PyObject *p)
{
	PyListObject *list = (PyListObject *)p;
	Py_ssize_t size, i;

	size = ((PyVarObject *)p)->ob_size;
	printf("[*] Python list info\n");
	printf("[*] Size of the Python List = %ld\n", size);
	printf("[*] Allocated = %ld\n", list->allocated);
	for (i = 0; i < size; i++)
		printf("Element %ld: %s\n", i,
		       list->ob_item[i]->ob_type->tp_name);
}
